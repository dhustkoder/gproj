#include <assert.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "logger.h"
#include "workman.h"

#define WORKQUEUE_MAX_ENTRIES (4096)


struct work_queue_entry {
	work_ptr_t fn;
	union work_arg arg;
};


static struct work_queue_entry queue[WORKQUEUE_MAX_ENTRIES];
static volatile int work_cnt;
static volatile int work_next;
static volatile bool terminated = false;
static SDL_Thread** threads;
static SDL_SpinLock splk;
static SDL_sem* sem;
static int thread_cnt;

#ifdef GPROJ_DEBUG
static SDL_threadID* thread_ids;
static const char** thread_names;
static int* thread_curr_work;
#endif



#ifdef GPROJ_DEBUG

static const char* get_thread_name_by_id(SDL_threadID id)
{
	for (int i = 0; i < thread_cnt; ++i) {
		if (thread_ids[i] == id) {
			return SDL_GetThreadName(threads[i]);
		}
	}
	return "MANAGER_THREAD";
}

static void work_ownership_monitor_init(SDL_Thread** thrs, int cnt)
{
	thread_ids = malloc(sizeof(*thread_ids) * cnt);
	assert(thread_ids != NULL);
	thread_names = malloc(sizeof(*thread_names) * cnt);
	assert(thread_names != NULL);
	thread_curr_work = malloc(sizeof(int) * cnt);
	assert(thread_curr_work != NULL);
	for (int i = 0; i < cnt; ++i) {
		thread_ids[i] = SDL_GetThreadID(thrs[i]);
		thread_names[i] = SDL_GetThreadName(thrs[i]);
		thread_curr_work[i] = -1;
	}
}

static void work_ownership_monitor_verify_update(const int entry)
{
	const SDL_threadID myid = SDL_ThreadID();
	const char* tname = get_thread_name_by_id(myid);
	LOG_DEBUG("THREAD %-16s GOT WORK IDX => %4d : WORK_CNT => %4d", tname, entry, work_cnt);
	for (int i = 0; i < thread_cnt; ++i) {
		if (thread_ids[i] == myid) {
			thread_curr_work[i] = entry;
		} else if (thread_curr_work[i] != -1 && thread_curr_work[i] == entry) {
			assert(false && "DUPLICATED WORK");
		}
	}
}

static void work_ownership_monitor_term(void)
{
	free(thread_curr_work);
	free(thread_names);
	free(thread_ids);
}


#else
#define work_ownership_monitor_init(...)           ((void)0)
#define work_ownership_monitor_verify_update(...)  ((void)0)
#define work_ownership_monitor_term(...)           ((void)0)
#endif



static int worker_thr_fn(void* dummy) 
{
	((void)dummy);
	LOG_DEBUG("STARTING WORKER_THREAD_%lx", SDL_ThreadID());
	SDL_SetThreadPriority(SDL_THREAD_PRIORITY_HIGH);
	workman_work_until_term();
	LOG_DEBUG("TERMINATING WORKER_THREAD_%lx", SDL_ThreadID());
	return 0;
}

static struct work_queue_entry* try_get_work(void)
{
	if (SDL_AtomicTryLock(&splk) == SDL_TRUE) {
		const int next = work_next;
		const int cnt = work_cnt;
		if (next == cnt) {
			SDL_AtomicUnlock(&splk);
			return NULL;
		}
		work_next = (next + 1)&(WORKQUEUE_MAX_ENTRIES - 1);
		SDL_AtomicUnlock(&splk);
		work_ownership_monitor_verify_update(next);
		return &queue[next];
	}
	return NULL;
}

static bool try_work(void)
{
	const struct work_queue_entry* const entry = try_get_work();
	if (entry != NULL) {
		entry->fn(entry->arg);
		return true;
	}
	return false;
}

static void sleeper(union work_arg arg)
{
	timer_sleep(arg.clk);
}

void workman_init(void)
{
	char namebuff[32];
	work_cnt = 0;
	work_next =  0;
	thread_cnt = SDL_GetCPUCount();
	sem = SDL_CreateSemaphore(0);

	if (thread_cnt > 0) {
		threads = malloc(sizeof(SDL_Thread*) * thread_cnt);
		for (int i = 0; i < thread_cnt; ++i) {
			sprintf(namebuff, "WORKER_THREAD_%d", i);
			threads[i] = SDL_CreateThread(worker_thr_fn, namebuff, NULL);
			assert(threads[i] != NULL);
			SDL_DetachThread(threads[i]);
		}
	}

	work_ownership_monitor_init(threads, thread_cnt);

}


void workman_term(void)
{
	workman_work_until_empty();
	SDL_CompilerBarrier();
	terminated = true;
	if (thread_cnt > 0) {
		work_ownership_monitor_term();
		free(threads);
		SDL_DestroySemaphore(sem);
	}
}


void workman_push_work(work_ptr_t fn, union work_arg arg)
{
	const int cnt = work_cnt;
	queue[cnt] = (struct work_queue_entry) {
		.fn = fn,
		.arg = arg
	};
	work_cnt = (cnt + 1)&(WORKQUEUE_MAX_ENTRIES - 1);
	SDL_SemPost(sem);
}

void workman_push_sleep(timer_clk_t ms)
{
	((void)ms);
	((void)sleeper);
	workman_push_work(sleeper, (union work_arg){.clk = ms});
}

void workman_work_until_empty(void)
{
	while (work_next != work_cnt) {
		while (try_work()) {
			// ...
		}
	}
}

void workman_work_until_term(void)
{
	while (!terminated) {
		SDL_SemWait(sem);
		while (try_work()) {
			// ...
		}
	}
}








