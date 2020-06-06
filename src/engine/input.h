#ifndef GPROJ_INPUT_H_
#define GPROJ_INPUT_H_
#include "platform_defs.h"

typedef u8 dinput_t;


enum digital_input {
	DINPUT_UP    = 0x01,
	DINPUT_DOWN  = 0x02,
	DINPUT_LEFT  = 0x04,
	DINPUT_RIGHT = 0x08
};


#endif
