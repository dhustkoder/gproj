#ifndef GPROJ_PLATFORM_H_
#define GPROJ_PLATFORM_H_
#include <windows.h>
#include <stdint.h>
#include <assert.h>
#include <limits.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdarg.h>
#include "utils.h"


typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;

#define U8_MAX  (UINT8_MAX)
#define U16_MAX (UINT16_MAX)
#define U32_MAX (UINT32_MAX)

#define U8_MIN  (UINT8_MAX)
#define U16_MIN (UINT16_MAX)
#define U32_MIN (UINT32_MAX)

#define U8_FMT  PRIu8
#define U16_FMT PRIu16
#define U32_FMT PRIu32


typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
#define S8_MAX  (INT8_MAX)
#define S16_MAX (INT16_MAX)
#define S32_MAX (INT32_MAX)

#define S8_MIN  (INT8_MAX)
#define S16_MIN (INT16_MAX)
#define S32_MIN (INT32_MAX)

#define S8_FMT  PRId8
#define S16_FMT PRId16
#define S32_FMT PRId32



typedef uintptr_t uptr;
typedef intptr_t  sptr;
#define UPTR_MAX (UINTPTR_MAX)
#define SPTR_MAX (INTPTR_MAX)

#define UPTR_MIN (UINTPTR_MIN)
#define SPTR_MIN (INTPTR_MIN)

#if UPTR_MAX > 0xFFFFFFFFu
#define UPTR_FMT "lX"
#define SPTR_FMT "lX"
#else
#define UPTR_FMT "X"
#define SPTR_FMT "X"
#endif


typedef u8  b8;
typedef u16 b16;
typedef u32 b32;
typedef _Bool bool;
#define true  ((bool)1)
#define false ((bool)0)

typedef float f32;


STATIC_ASSERT(u_type_sizes,
	(sizeof(u8) == 1 && sizeof(u16) == 2 && sizeof(u32) == 4));
STATIC_ASSERT(s_type_sizes,
	(sizeof(s8) == 1 && sizeof(s16) == 2 && sizeof(s32) == 4));
STATIC_ASSERT(b_type_sizes,
	(sizeof(b8) == 1 && sizeof(b16) == 2 && sizeof(b32) == 4));
STATIC_ASSERT(f_type_sizes, (sizeof(f32) == 4));





#endif
