#ifndef GPROJ_LOG_H_
#define GPROJ_LOG_H_
#include <SDL2/SDL.h>

#define LOG(...) SDL_Log(__VA_ARGS__)

#define LOG_ERR(...) SDL_LogError(SDL_LOG_CATEGORY_ERROR, __VA_ARGS__)

#if defined(DEBUG) && defined(GPROJ_DEBUG)
#define LOG_DEBUG(...) SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, __VA_ARGS__)
#else
#define LOG_DEBUG(...) (void)
#endif



#endif
