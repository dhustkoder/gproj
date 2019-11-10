#ifndef GPROJ_LOGGER_H_
#define GPROJ_LOGGER_H_
#include "platform_defs.h"
#include <stdio.h>

#define LOG(...) fprintf(stdout, __VA_ARGS__); fprintf(stdout, "\n");

#define LOG_ERR(...) fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n");

#if defined(GPROJ_DEBUG)
#define LOG_DEBUG(...) fprintf(stdout, __VA_ARGS__); fprintf(stdout, "\n");
#else
#define LOG_DEBUG(...) ((void)0)
#endif


#endif
