#ifndef GPROJ_LOGGER_H_
#define GPROJ_LOGGER_H_
#include <SDL.h>

#define LOG(...) SDL_Log(__VA_ARGS__)

#define LOG_ERR(...) SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, __VA_ARGS__)

#if defined(GPROJ_DEBUG)
#define LOG_DEBUG(...) SDL_Log(__VA_ARGS__)
#else
#define LOG_DEBUG(...) ((void)0)
#endif


#endif
