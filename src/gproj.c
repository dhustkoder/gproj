#include <SDL2/SDL.h>
#include "log.h"
#include "events.h"
#include "render.h"
#include "map.h"
#include "gproj.h"


input_button_t input_buttons_states;

static struct sprite player = {
	.size = { 4, 4 },
	.color = { .raw = 0xFFFFFFFF }
};

static const char* const map0 = 
	"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
	"xxxxxxxx...xxxxxx...xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx...."
	"xxxxxxxx...xxxxxx...xxxxxxxxxxxxxxxxxxxxxxxxx..................."
	"xxxxxxxx............xxxxxxxxxxxxxxxxxxxxxxxxx..................."
	"xxxxxxxxxxxxx..xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx..xxxxxxxxxxxxx...."
	"xxxxxxxxxxxxx..xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx..xxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxx..xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx..xxxxxxxxxxxxxxxxx"
	"xxxxxxxxxx........xxxxxxxxxxxxxxxxxxxxxxxx........xxxxxxxxxxxxxx"
	"xxxxxxxxxxxxx..xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx..xxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxx..xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx..xxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxx..xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx..xxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxx..xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx..xxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxx..xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx..xxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxx..xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx..xxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxx..xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx..xxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxx..xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx..xxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxx..xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx..xxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxx.......xxxxxxxxxxxxxxxxxxxx............xxxxxxxxxxxx"
	"xxxxxxxxxxxxx.......xxxxxxxxxxxxxxxxxxxx.x........x.xxxxxxxxxxxx"
	"xxxxxxxxxxxxx..xxxxxxxxxxxxxxxxxxxxxxxxx............xxxxxxxxxxxx"
	"xxxxxxxxxxxxx..xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx..xxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxx..xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx..xxxxxxxxxxxxxxxxx"
	"xxxxxxxxxxxxx..xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx..xxxxxxxxxxxxxxxxx"
	"xxxxxxxxxx........................................xxxxxxxxxxxxxx"
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
	int fps = 0;

	render_clear(true, true);
	map_load(map0);

	player.pos = map_get_player_init_pos();

	while (events_update()) {

		if (input_buttons_states&INPUT_BUTTON_UP) {
			player.pos.y -= 1;
		} else if (input_buttons_states&INPUT_BUTTON_DOWN) {
			player.pos.y += 1;
		}

		if (input_buttons_states&INPUT_BUTTON_LEFT) {
			player.pos.x -= 1;
		} else if (input_buttons_states&INPUT_BUTTON_RIGHT) {
			player.pos.x += 1;
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


