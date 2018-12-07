#ifndef GPROJ_LOG_H_
#define GPROJ_LOG_H_
#include <SDL2/SDL.h>

#define LOG_ERR(fmt, ...) SDL_LogError(SDL_LOG_CATEGORY_ERROR, fmt, __VA_ARGS__)
#if defined(DEBUG) && defined(GPROJ_DEBUG)
#define LOG_DEBUG(fmt, ...) SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, fmt, __VA_ARGS__)
#else
#define LOG_DEBUG(fmt, ...) (void)
#endif
#define LOG(fmt, ...) SDL_Log(SDL_LOG_CATEGORY_APPLICATION, fmt, __VA_ARGS__)


#endif
