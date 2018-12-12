#include <SDL2/SDL.h>
#include "log.h"
#include "events.h"
#include "render.h"
#include "map.h"
#include "gproj.h"


input_button_t input_buttons_states;


int gproj(void)
{

	while (events_update()) {
		/*
		if (input_buttons_states&INPUT_BUTTON_UP) {
			player_spr.ss.pos.y = 0;
			if ((SDL_GetTicks() - anim_clk) > 150) {
				player_spr.ss.pos.x = (player_spr.ss.pos.x + 16) % 32;
				anim_clk = SDL_GetTicks();
			}
			player_spr.scr.pos.y -= 3;
		} else if (input_buttons_states&INPUT_BUTTON_DOWN) {
			player_spr.ss.pos.y = 32;
			if ((SDL_GetTicks() - anim_clk) > 150) {
				player_spr.ss.pos.x = (player_spr.ss.pos.x + 16) % 32;
				anim_clk = SDL_GetTicks();
			}
			player_spr.scr.pos.y += 3;
		} else if (input_buttons_states&INPUT_BUTTON_LEFT) {
			player_spr.ss.pos.y = 48;
			if ((SDL_GetTicks() - anim_clk) > 150) {
				player_spr.ss.pos.x = (player_spr.ss.pos.x + 16) % 32;
				anim_clk = SDL_GetTicks();
			}
			player_spr.scr.pos.x -= 3;
		} else if (input_buttons_states&INPUT_BUTTON_RIGHT) {
			player_spr.ss.pos.y = 16;
			if ((SDL_GetTicks() - anim_clk) > 150) {
				player_spr.ss.pos.x = (player_spr.ss.pos.x + 16) % 32;
				anim_clk = SDL_GetTicks();
			}
			player_spr.scr.pos.x += 3;
		}
		*/
		render_clear(RENDER_CLEAR_FG);
		render_present();
	}

	return 0;
}


