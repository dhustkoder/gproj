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
	"xxxxxxxxxxxxx..xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx..xxxxxxxxxxxxx...."
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
	"xxxxxxxxxxxxxxxxxxxxxxxxxx.p....xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";


int gproj(void)
{
	Uint32 clk = SDL_GetTicks();
	Uint32 anim_clk = clk;
	int fps = 0;

	sprite_sheet_t* const player_ss = render_load_spr_sheet("../assets/h_spr.png");
	
	render_set_spr_sheet(player_ss);

	struct sprite player_spr = {
		.ss.pos = { 0, 0 },
		.ss.size = { 16, 16 }
	};

	render_clear(RENDER_CLEAR_BKG|RENDER_CLEAR_SPRS);
	map_load(map0);

	player_spr.scr.pos = map_get_player_init_pos();
	player_spr.scr.size = (struct vec2i) { 64, 64 };

	while (events_update()) {

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

		render_clear(RENDER_CLEAR_SPRS);
		render_sprs(&player_spr, 1);
		render_present();
		++fps;

		if ((SDL_GetTicks() - clk) >= 1000) {
			LOG("FPS: %d\n", fps);
			fps = 0;
			clk = SDL_GetTicks();
		}

	}

	render_free_spr_sheet(player_ss);

	return 0;
}


