#ifndef GPROJ_TYPES_H_
#define GPROJ_TYPES_H_
#include <SDL2/SDL_stdinc.h>
#include "utils.h"

typedef Uint8  u8;
typedef Uint16 u16;
typedef Uint32 u32;
#define U8_MAX  (SDL_MAX_UINT8)
#define U16_MAX (SDL_MAX_UINT16)
#define U32_MAX (SDL_MAX_UINT32)

typedef Sint8  s8;
typedef Sint16 s16;
typedef Sint32 s32;
#define S8_MAX  (SDL_MAX_SINT8)
#define S16_MAX (SDL_MAX_SINT16)
#define S32_MAX (SDL_MAX_SINT32)

typedef u8  b8;
typedef u16 b16;
typedef u32 b32;
#define true  1
#define false 0

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


enum input_button {

	INPUT_BUTTON_UP               = 0x001,
	INPUT_BUTTON_DOWN             = 0x002,
	INPUT_BUTTON_LEFT             = 0x004,
	INPUT_BUTTON_RIGHT            = 0x008,
	INPUT_BUTTON_RUN              = 0x010,
	INPUT_BUTTON_ACTION           = 0x020,

#ifdef GPROJ_DEBUG
	INPUT_BUTTON_CAM_UP           = 0x040,
	INPUT_BUTTON_CAM_DOWN         = 0x080,
	INPUT_BUTTON_CAM_LEFT         = 0x100,
	INPUT_BUTTON_CAM_RIGHT        = 0x200,
	INPUT_BUTTON_WORLD_SCALE_UP   = 0x400,
	INPUT_BUTTON_WORLD_SCALE_DOWN = 0x800,
	INPUT_BUTTON_NBUTTONS         = 12
#else
	INPUT_BUTTON_NBUTTONS = 6
#endif
};

typedef u16 input_button_t;

struct input_event {
	b8 new_state;
	input_button_t buttons;
};

struct events {
	struct input_event input;
	b32 quit;
};

#endif
