#ifndef GPROJ_LOGGER_H_
#define GPROJ_LOGGER_H_
#include "platform_defs.h"
#include <stdio.h>

#define GPROJ_WIN32_LOG_BUFFER_SIZE (4096)

enum gproj_win32_log_handle {
	GPROJ_WIN32_LOG_HANDLE_STDOUT,
	GPROJ_WIN32_LOG_HANDLE_STDERR,
	GPROJ_WIN32_LOG_HANDLE_NHANDLES
};

extern void gproj_win32_log_write(
	enum gproj_win32_log_handle handle,
	const char* fmt,
	...
);

#define log(...) gproj_win32_log_write(GPROJ_WIN32_LOG_HANDLE_STDOUT, __VA_ARGS__)
#define log_err(...) gproj_win32_log_write(GPROJ_WIN32_LOG_HANDLE_STDERR, __VA_ARGS__)

#if defined(GPROJ_DEBUG)
#define log_dbg(...) gproj_win32_log_write(GPROJ_WIN32_LOG_HANDLE_STDOUT, __VA_ARGS__)
#else
#define log_dbg(...) ((void)0)
#endif




#endif
