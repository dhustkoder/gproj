#ifndef GPROJ_INPUT_H_
#define GPROJ_INPUT_H_
#include <stdbool.h>
#include <stdint.h>


enum input_button {
	INPUT_BUTTON_UP    = 0x01,
	INPUT_BUTTON_DOWN  = 0x02,
	INPUT_BUTTON_LEFT  = 0x04,
	INPUT_BUTTON_RIGHT = 0x08,
	INPUT_BUTTON_RUN   = 0x10,
	INPUT_BUTTON_SHOOT = 0x20,
	INPUT_BUTTON_NBUTTONS = 6
};
typedef uint8_t input_button_t;


extern input_button_t input_buttons_states;
extern bool input_bump_events(void);



#endif
