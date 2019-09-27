#include "render.h"
#include "audio.h"
#include "timer.h"
#include "map.h"
#include "actors.h"
#include "characters.h"
#include "game.h"


static int bgm_id;

void game_init(int argc, char** argv)
{
	timer_profiler_init();
	render_init("GProj Testing");
	audio_init();
	map_load(argc > 1 ? argv[1] : "map00-00.tmx");
	characters_init();
	bgm_id = audio_load_bgm("bloodlines.ogg");
	audio_play_bgm(bgm_id);
}

void game_step(timer_clk_t now, float dt)
{
	map_update(now, dt);
	characters_update(now, dt);
}

void game_term()
{
	map_free();
	audio_term();
	render_term();
}


