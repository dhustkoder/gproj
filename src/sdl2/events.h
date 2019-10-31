#ifndef GPROJ_EVENTS_H_
#define GPROJ_EVENTS_H_
#include "types.h"


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
	INPUT_BUTTON_WORLD_FWD        = 0x1000,
	INPUT_BUTTON_NBUTTONS         = 13
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

extern void events_update(struct events* gproj_ev);


#endif
