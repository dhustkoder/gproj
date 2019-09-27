#include <SDL2/SDL_thread.h>


typedef SDL_Thread gproj_thread_t;
typedef int(*gproj_thread_fn)(void* args);


static inline gproj_thread_t* thread_start(gproj_thread_fn thr_main, const char* const name, void* args)
{
	SDL_Thread* const thr = SDL_CreateThread(thr_main, name, args);
	return thr;
}


static inline void thread_detach(gproj_thread_t* const thr)
{
	SDL_DetachThread(thr);
}

static inline int thread_join(gproj_thread_t* thr)
{
	int i;
	SDL_WaitThread(thr, &i);
	return i;
}



