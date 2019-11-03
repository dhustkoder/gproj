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

enum event_flag {
	EVENT_FLAG_NEW_INPUT      = 0x01,
	EVENT_FLAG_WINDOW_RESIZED = 0x02,
	EVENT_FLAG_QUIT           = 0x04
};

typedef u16 input_button_t;

struct input_event {
	input_button_t buttons;
};

struct window_event {
	s16 w;
	s16 h;
};

struct events {
	struct input_event input;
	struct window_event window;
	u8 flags;
};

typedef void (*window_resize_callback_fn_t)(int w, int h);

extern void events_update(struct events* gproj_ev);
extern void events_set_window_resize_clbk(window_resize_callback_fn_t clbk);


#endif
