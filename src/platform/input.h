#ifndef GPROJ_INPUT_H_
#define GPROJ_INPUT_H_
#include "platform_defs.h"


enum input {
	INPUT_BUTTON_UP               = 0x001,
	INPUT_BUTTON_DOWN             = 0x002,
	INPUT_BUTTON_LEFT             = 0x004,
	INPUT_BUTTON_RIGHT            = 0x008,
	INPUT_BUTTONS_GAME =
		INPUT_BUTTON_UP|
		INPUT_BUTTON_DOWN|
		INPUT_BUTTON_LEFT|
		INPUT_BUTTON_RIGHT

#ifdef GPROJ_DEBUG
	,
	INPUT_BUTTON_CAM_UP           = 0x040,
	INPUT_BUTTON_CAM_DOWN         = 0x080,
	INPUT_BUTTON_CAM_LEFT         = 0x100,
	INPUT_BUTTON_CAM_RIGHT        = 0x200,

	INPUT_BUTTON_WORLD_SCALE_UP   = 0x400,
	INPUT_BUTTON_WORLD_SCALE_DOWN = 0x800,
	INPUT_BUTTON_WORLD_FWD        = 0x1000,


	INPUT_BUTTONS_CAM = 
		INPUT_BUTTON_CAM_UP  |
		INPUT_BUTTON_CAM_DOWN|
		INPUT_BUTTON_CAM_LEFT|
		INPUT_BUTTON_CAM_RIGHT,

	INPUT_BUTTONS_WORLD =
		INPUT_BUTTON_WORLD_SCALE_UP|
		INPUT_BUTTON_WORLD_SCALE_DOWN|
		INPUT_BUTTON_WORLD_FWD,


	INPUT_BUTTON_NBUTTONS = 11
#else
	INPUT_BUTTON_NBUTTONS = 4
#endif
};

typedef u32 input_t;

#define OLD_INPUT(input) (input_t)((input&0xFFFF0000)>>16)
#define NEW_INPUT(input) (input_t)((input&0x0000FFFF))

#define INPUT_UPDATE_OLD_VALUE(input) \
	(input = ((input&0x0000FFFF)<<16)|(input&0x0000FFFF))

#define INPUT_SET_NEW_VALUE(input, new_value) \
	(input = (input<<16)|(new_value&0x0000FFFF))

#define IS_HOLDING_INPUT(input, flags) \
	((OLD_INPUT(input)&flags) && (NEW_INPUT(input)&flags))

#define IS_TAPPED_INPUT(input, flags) \
	((NEW_INPUT(input)&flags) && !IS_HOLDING_INPUT(input, flags))

#define IS_PRESSED_INPUT(input, flags) (NEW_INPUT(input)&flags)


#endif
