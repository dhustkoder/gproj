#include <SDL2/SDL.h>
#include "log.h"
#include "events.h"
#include "render.h"
#include "map.h"
#include "gproj.h"


input_button_t input_buttons_states;

static const char* const map0 = 
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx...."
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx..................."
	"xxxxxxxxxxxxx..xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx..................."
	"xxxxxxxxxxxxx..xxxxxxxxxxxxxxxpxxxxxxxxxxxxxx..xxxxxxxxxxxxx...."
	"xxxxxxxxxxxxx..xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx..xxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxx..xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx..xxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxx......xxxxxxxxxxxxxxxxxxxxxxx........xxxxxxxxxxxxxx"
	"xxxxxxxxxxxxx..xxx..xxxxxxxxxxxxxxxxxxxxxxxxx..xxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxx..xxxx..xxxxxxxxxxxxxxxxxxxxxxxx..xxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxx..xxxx....xxxxxxxxxxxxxxxxxxxxxx..xxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxx..xxxx........xx..xxxxxxxxxxxxxx..xxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxx..xxxx.........x..xxxxxxxxxxxxxx..xxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxx..xxxx...xxxx.....xxxxxxxxxxxxxx..xxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxx..xxxx.x.xxxxxxxxxxxxxxxxxxxxxxx..xxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxx..xxxx.x.xxxxxxxxxxxxxxxxxxxxxxx..xxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxx..xxxx.x.xxxxxxxxxxxxxxxxxxxxxxx..xxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxx.......x........xxxxxxxxxxx............xxxxxxxxxxxx"
	"xxxxxxxxxxxxx.......xxxxxxxxxxxxxxxxxxxx.x........x.xxxxxxxxxxxx"
	"xxxxxxxxxxxxx..xxxxxxxxxxxxxxxxxxxxxxxxx............xxxxxxxxxxxx"
	"xxxxxxxxxxxxx..xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx..xxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxx..xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx..xxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxx..xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx..xxxxxxxxxxxxxxxxx"
	"xx................................................xxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx..xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx..xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx..xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx..xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx..xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx..xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxx......xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxxxxxxxxxxxxxxx......xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";


int gproj(void)
{
	render_clear(RENDER_CLEAR_BKG|RENDER_CLEAR_SPRS);
	map_load(map0);

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
		render_clear(RENDER_CLEAR_SPRS);
		render_present();
	}

	return 0;
}


