#include <assert.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "logger.h"
#include "workman.h"

#define QUEUE_NULL_INDEX (GPROJ_MAX_WORKMAN_QUEUE_SIZE)
#define INC_CNTER(n) (n = ((n + 1)&(GPROJ_MAX_WORKMAN_QUEUE_SIZE - 1)))

struct work_queue_entry {
	work_ptr_t fn;
	union work_arg arg;
};


static struct work_queue_entry queue[GPROJ_MAX_WORKMAN_QUEUE_SIZE];
static volatile int work_cnt;
static volatile int work_next;
static volatile bool terminated = false;
static SDL_Thread** threads;
static SDL_SpinLock splk;
static SDL_sem* sem;
static SDL_atomic_t working_thr_cnt;
static int thread_cnt;

#ifdef GPROJ_DEBUG
static volatile SDL_threadID manager_thr_id;
static volatile int manager_thr_work;
static volatile SDL_threadID* thread_ids;
static volatile const char** thread_names;
static volatile int* thread_curr_work;
#endif



#ifdef GPROJ_DEBUG 

static const char* debug_get_thread_name_by_id(const SDL_threadID id)
{
	if (id != manager_thr_id) {
		for (int i = 0; i < thread_cnt; ++i) {
			if (thread_ids[i] == id) {
				return SDL_GetThreadName(threads[i]);
			}
		}
	}
	return "MANAGER_THREAD";
}

static void debug_work_ownership_monitor_report_duplication(SDL_threadID a, SDL_threadID b)
{
	const char* b_name = debug_get_thread_name_by_id(a);
	const char* a_name = debug_get_thread_name_by_id(b);
	LOG_DEBUG("DUPLICATED WORK BETWEEN THREADS (%s) AND (%s)", a_name, b_name);
	assert(false && "DUPLICATED WORK");
}

static void debug_work_ownership_monitor_init(SDL_Thread** thrs, int cnt)
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
		thread_curr_work[i] = QUEUE_NULL_INDEX;
	}
	manager_thr_id = SDL_ThreadID();
	manager_thr_work = QUEUE_NULL_INDEX;
}

static void debug_work_ownership_monitor_lock(const struct work_queue_entry* const entryp)
{
	const int entry = (int)(entryp - (&queue[0]));
	const SDL_threadID myid = SDL_ThreadID();
	const char* my_name = debug_get_thread_name_by_id(myid);
	LOG_DEBUG("THREAD %-16s STARTING WORK IDX => %4d => (%"UPTR_FMT
	          ") : WORK_CNT => %4d",
	           my_name, entry, (uptr)entryp->fn, work_cnt);

	for (int i = 0; i < thread_cnt; ++i) {
		if (thread_ids[i] == myid) {
			thread_curr_work[i] = entry;
		} else if (thread_curr_work[i] != QUEUE_NULL_INDEX &&
		           thread_curr_work[i] == entry) {
			debug_work_ownership_monitor_report_duplication(myid, thread_ids[i]);
		}
	}

	if (myid == manager_thr_id) {
		manager_thr_work = entry;
	} else if (manager_thr_work == entry) {
		debug_work_ownership_monitor_report_duplication(myid, manager_thr_id);
	}
}

static void debug_work_ownership_monitor_unlock(const struct work_queue_entry* const entryp)
{
	const SDL_threadID myid = SDL_ThreadID();
	const char* my_name = debug_get_thread_name_by_id(myid);
	const int entry = (int)(entryp - (&queue[0]));
	LOG_DEBUG("THREAD %-16s FINISHED WORK IDX => %4d => (%"UPTR_FMT
	          ") : WORK_CNT => %4d",
	          my_name, entry, (uptr)entryp->fn, work_cnt);
	if (myid == manager_thr_id) {
		assert(entry == manager_thr_work);
		manager_thr_work = QUEUE_NULL_INDEX;
	} else {
		for (int i = 0; i < thread_cnt; ++i) {
			if (thread_ids[i] == myid) {
				assert(entry == thread_curr_work[i]);
				thread_curr_work[i] = QUEUE_NULL_INDEX;
			}
		}
	}
}

static void debug_work_ownership_monitor_term(void)
{
	free((void*)thread_curr_work);
	free((void*)thread_names);
	free((void*)thread_ids);
}

#else

#define debug_work_ownership_monitor_init(...)    ((void)0)
#define debug_work_ownership_monitor_lock(...)    ((void)0)
#define debug_work_ownership_monitor_unlock(...)  ((void)0)
#define debug_work_ownership_monitor_term(...)    ((void)0)

#endif /* GPROJ_DEBUG */



static struct work_queue_entry* try_get_work(void)
{
	if (SDL_AtomicTryLock(&splk) == SDL_TRUE) {
		const int next = work_next;
		const int cnt = work_cnt;
		if (next == cnt) {
			SDL_AtomicUnlock(&splk);
			return NULL;
		}
		INC_CNTER(work_next);
		SDL_AtomicUnlock(&splk);
		return &queue[next];
	}
	return NULL;
}

static bool try_work(void)
{
	const struct work_queue_entry* const entry = try_get_work();
	if (entry != NULL) {
		SDL_AtomicIncRef(&working_thr_cnt);
		debug_work_ownership_monitor_lock(entry);
		entry->fn(entry->arg);
		debug_work_ownership_monitor_unlock(entry);
		SDL_AtomicDecRef(&working_thr_cnt);
		return true;
	}
	return false;
}


static void work_until_empty(void)
{
	while (work_next != work_cnt) {
		while (try_work()) {
			// ...
		}
	}
}

static void work_until_term(void)
{
	while (!terminated) {
		SDL_SemWait(sem);
		work_until_empty();
	}
}

static void sleeper(union work_arg arg)
{
	timer_sleep(arg.clk);
}

static int worker_thr_main(void* dummy) 
{
	((void)dummy);
	SDL_SetThreadPriority(SDL_THREAD_PRIORITY_HIGH);

	LOG_DEBUG("STARTING WORKER_THREAD_%lx", SDL_ThreadID());
	
	work_until_term();
	
	LOG_DEBUG("TERMINATING WORKER_THREAD_%lx", SDL_ThreadID());
	
	return 0;
}


void workman_init(void)
{
	LOG_DEBUG("STARTING WORKMAN");
	char namebuff[32];
	work_cnt = 0;
	work_next =  0;
	thread_cnt = SDL_GetCPUCount() - 1;
	sem = SDL_CreateSemaphore(0);
	SDL_AtomicSet(&working_thr_cnt, 0);

	if (thread_cnt > 0) {
		threads = malloc(sizeof(SDL_Thread*) * thread_cnt);
		for (int i = 0; i < thread_cnt; ++i) {
			sprintf(namebuff, "WORKER_THREAD_%d", i);
			threads[i] = SDL_CreateThread(worker_thr_main, namebuff, NULL);
			assert(threads[i] != NULL);
			SDL_DetachThread(threads[i]);
		}
	}

	debug_work_ownership_monitor_init(threads, thread_cnt);
}


void workman_term(void)
{
	/* TODO: check if terminating as this can cause crashes */
	assert(SDL_ThreadID() == manager_thr_id);
	LOG_DEBUG("TERMINATING WORKMAN");
	terminated = true;
	if (thread_cnt > 0) {
		debug_work_ownership_monitor_term();
		free(threads);
		SDL_DestroySemaphore(sem);
	}
}


void workman_push_work(work_ptr_t fn, union work_arg arg)
{
	assert(SDL_ThreadID() == manager_thr_id);
	const int cnt = work_cnt;
	queue[cnt] = (struct work_queue_entry) {
		.fn = fn,
		.arg = arg
	};
	INC_CNTER(work_cnt);
	SDL_SemPost(sem);
}

void workman_push_sleep(timer_clk_t ms)
{
	assert(SDL_ThreadID() == manager_thr_id);
	((void)ms);
	((void)sleeper);
	workman_push_work(sleeper, (union work_arg){.clk = ms});
}

void workman_work_until_all_finished(void)
{
	assert(SDL_ThreadID() == manager_thr_id);
	work_until_empty();
	while (SDL_AtomicGet(&working_thr_cnt) > 0)
		;
}








