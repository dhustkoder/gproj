#include <SDL2/SDL.h>
#include "log.h"
#include "input.h"
#include "render.h"
#include "gproj.h"


extern input_button_t input_buttons_states;

static struct sprite player = {
	.x = GPROJ_FB_WIDTH / 2,
	.y = GPROJ_FB_HEIGHT / 2,
	.w = 20,
	.h = 20,
	.rgba = RGBA(0xFF, 0xFF, 0xFF, 0xFF)
};



int gproj(int argc, char** argv)
{
	((void)argc);
	((void)argv);

	Uint32 clk = SDL_GetTicks();
	int fps = 0;
	while (input_bump_events()) {

		if (input_buttons_states&INPUT_BUTTON_UP) {
			player.y -= 1;
		} else if (input_buttons_states&INPUT_BUTTON_DOWN) {
			player.y += 1;
		}

		if (input_buttons_states&INPUT_BUTTON_LEFT) {
			player.x -= 1;
		} else if (input_buttons_states&INPUT_BUTTON_RIGHT) {
			player.x += 1;
		}

		render_clear(false, true);
		render_draw_sprites(&player, 1);
		render_present();
		++fps;

		if ((SDL_GetTicks() - clk) >= 1000) {
			LOG("FPS: %d\n", fps);
			fps = 0;
			clk = SDL_GetTicks();
		}

	}

	return 0;
}


