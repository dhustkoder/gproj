#ifndef GPROJ_LOGGER_H_
#define GPROJ_LOGGER_H_
#include <SDL.h>

#define log(...) SDL_Log(__VA_ARGS__)

#define log_err(...) SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, __VA_ARGS__)

#if defined(GPROJ_DEBUG)
#define log_dbg(...) SDL_Log(__VA_ARGS__)
#else
#define log_dbg(...) ((void)0)
#endif


#endif
