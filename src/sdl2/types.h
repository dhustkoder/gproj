#ifndef GPROJ_TYPES_H_
#define GPROJ_TYPES_H_
#include <stdint.h>
#include <limits.h>
#include <SDL_stdinc.h>
#include "utils.h"


#ifdef _MSC_VER
// fucking MS please start supporting C99 properly!
#define restrict __restrict
#endif

typedef Uint8  u8;
typedef Uint16 u16;
typedef Uint32 u32;

#define U8_MAX  (SDL_MAX_UINT8)
#define U16_MAX (SDL_MAX_UINT16)
#define U32_MAX (SDL_MAX_UINT32)

#define U8_MIN  (SDL_MIN_UINT8)
#define U16_MIN (SDL_MIN_UINT16)
#define U32_MIN (SDL_MIN_UINT32)

#define U8_FMT  SDL_PRIu8
#define U16_FMT SDL_PRIu16
#define U32_FMT SDL_PRIu32


typedef Sint8  s8;
typedef Sint16 s16;
typedef Sint32 s32;
#define S8_MAX  (SDL_MAX_SINT8)
#define S16_MAX (SDL_MAX_SINT16)
#define S32_MAX (SDL_MAX_SINT32)

#define S8_MIN  (SDL_MIN_SINT8)
#define S16_MIN (SDL_MIN_SINT16)
#define S32_MIN (SDL_MIN_SINT32)

#define S8_FMT  SDL_PRIs8
#define S16_FMT SDL_PRIs16
#define S32_FMT SDL_PRIs32



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

struct vec2i {
	s16 x, y;
};

struct vec2f {
	f32 x, y;
};

struct recti {
	struct vec2i pos;
	struct vec2i size;
};

struct rectf {
	struct vec2f pos;
	struct vec2f size;
};


#endif
