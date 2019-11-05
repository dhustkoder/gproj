#ifndef GPROJ_LOGGER_H_
#define GPROJ_LOGGER_H_
#include "platform_defs.h"
#include <stdio.h>

#define LOG(...) fprintf(stdout, __VA_ARGS__)

#define LOG_ERR(...) fprintf(stderr, SDL_LOG_CATEGORY_APPLICATION, __VA_ARGS__)

#if defined(GPROJ_DEBUG)
#define LOG_DEBUG(...) fprintf(stdout, __VA_ARGS__)
#else
#define LOG_DEBUG(...) ((void)0)
#endif


#endif
