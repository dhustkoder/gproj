#include <SDL.h>
#include "events.h"


static SDL_Scancode sdl_keys[] = {
	SDL_SCANCODE_W,
	SDL_SCANCODE_S,
	SDL_SCANCODE_A,
	SDL_SCANCODE_D,
	SDL_SCANCODE_RSHIFT,
	SDL_SCANCODE_SPACE

#ifdef GPROJ_DEBUG
	,
	SDL_SCANCODE_K,
	SDL_SCANCODE_J,
	SDL_SCANCODE_H,
	SDL_SCANCODE_L,
	SDL_SCANCODE_M,
	SDL_SCANCODE_N,
	SDL_SCANCODE_B
#endif
};

static input_button_t gproj_buttons[] = {
	INPUT_BUTTON_UP,
	INPUT_BUTTON_DOWN,
	INPUT_BUTTON_LEFT,
	INPUT_BUTTON_RIGHT,
	INPUT_BUTTON_RUN,
	INPUT_BUTTON_ACTION

#ifdef GPROJ_DEBUG
	,
	INPUT_BUTTON_CAM_UP,
	INPUT_BUTTON_CAM_DOWN,
	INPUT_BUTTON_CAM_LEFT,
	INPUT_BUTTON_CAM_RIGHT,
	INPUT_BUTTON_WORLD_SCALE_UP,
	INPUT_BUTTON_WORLD_SCALE_DOWN,
	INPUT_BUTTON_WORLD_FWD
#endif
};


static inline void update_input_event(const SDL_Event* const ev,
                                      struct events* const ge)
{
	input_button_t buttons = ge->input.buttons;
	for (int idx = 0; idx < INPUT_BUTTON_NBUTTONS; ++idx) {
		if (sdl_keys[idx] == ev->key.keysym.scancode) {
			input_button_t bt = gproj_buttons[idx];
			if (ev->type == SDL_KEYDOWN)
				buttons |= bt;
			else
				buttons &= ~bt;
			break;
		}
	}
	if (buttons != ge->input.buttons) {
		ge->input.buttons = buttons;
		ge->flags |= EVENT_FLAG_NEW_INPUT;
	}
}

void events_update(struct events* const gproj_ev)
{
	SDL_Event ev;

	gproj_ev->flags = 0x00;

	while (SDL_PollEvent(&ev)) {
		switch (ev.type) {
			case SDL_QUIT:
				gproj_ev->flags |= EVENT_FLAG_QUIT;
				return;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				update_input_event(&ev, gproj_ev);
				break;
			case SDL_WINDOWEVENT:
				// implement
				break;
		}
	}
}










