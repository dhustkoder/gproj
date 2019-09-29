#include <assert.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "logger.h"
#include "workman.h"

#define WORKQUEUE_MAX_ENTRIES (4096)


struct work_queue_entry {
	work_fn_ptr fn;
	void* arg;
};


static struct work_queue_entry queue[WORKQUEUE_MAX_ENTRIES];
static volatile unsigned work_cnt = 0;
static volatile unsigned work_next = 0;
static volatile bool terminated = false;
static SDL_Thread** threads;
static SDL_SpinLock splk;
static SDL_sem* sem;
static int threads_cnt;


static int worker_thr_fn(void* dummy) 
{
	((void)dummy);
	LOG_DEBUG("STARTING WORKER THREAD %ld", SDL_ThreadID());
	SDL_SetThreadPriority(SDL_THREAD_PRIORITY_HIGH);
	workman_work_until_term();
	LOG_DEBUG("TERMINATING WORKER THREAD %ld", SDL_ThreadID());
	return 0;
}

static struct work_queue_entry* try_get_work_splk(void)
{
	if (SDL_AtomicTryLock(&splk) == SDL_TRUE) {
		if (work_cnt == work_next) {
			SDL_AtomicUnlock(&splk);
			return NULL;
		}
		const unsigned entry = work_next;
		work_next = (work_next + 1)&(WORKQUEUE_MAX_ENTRIES - 1);
		SDL_AtomicUnlock(&splk);
		return &queue[entry];
	}
	return NULL;
}

static bool try_work(void)
{
	const struct work_queue_entry* const entry = try_get_work_splk();
	if (entry != NULL) {
		entry->fn(entry->arg);
		return true;
	}
	return false;
}


void workman_init(void)
{
	char namebuff[32];
	threads_cnt = SDL_GetCPUCount() - 1;
	sem = SDL_CreateSemaphore(0);
	if (threads_cnt > 0) {
		threads = malloc(sizeof(SDL_Thread*) * threads_cnt);
		for (int i = 0; i < threads_cnt; ++i) {
			sprintf(namebuff, "WORKER_THREAD %d", i);
			threads[i] = SDL_CreateThread(worker_thr_fn, namebuff, NULL);
			assert(threads[i] != NULL);
			SDL_DetachThread(threads[i]);
		}
	}
}


void workman_term(void)
{
	workman_work_until_empty();
	SDL_CompilerBarrier();
	terminated = true;
	if (threads_cnt > 0) {
		free(threads);
		SDL_DestroySemaphore(sem);
	}
}


void workman_push_work(work_fn_ptr fn, void* arg)
{
	queue[work_cnt] = (struct work_queue_entry) {
		.fn = fn,
		.arg = arg
	};
	work_cnt = (work_cnt + 1)&(WORKQUEUE_MAX_ENTRIES - 1);
	SDL_CompilerBarrier();
	SDL_SemPost(sem);
}


void workman_work_until_empty(void)
{
	while (work_next != work_cnt) {
		try_work();
	}
}

void workman_work_until_term(void)
{
	while (!terminated) {
		if (work_next != work_cnt) {
			while (try_work())
				;
		} else {
			SDL_SemWait(sem);
		}
	}
}








