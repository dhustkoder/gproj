#include "logger.h"
#include "workman.h"
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
	LOG_DEBUG("INITIALIZING GAME");
	workman_init();
	timer_profiler_init();
	render_init("GProj Testing");
	audio_init();
	characters_init();
	map_load(argc > 1 ? argv[1] : "map00-00.tmx");
	map_send_render();
	bgm_id = audio_load_bgm("bloodlines.ogg");
	audio_play_bgm(bgm_id);
	LOG_DEBUG("GAME INITIALIZED");
}

void game_step(timer_clk_t now, float dt)
{
	// update
	characters_update(now, dt);
	actors_update(now, dt);
	map_update(now, dt);

	// send render
	actors_send_render();
}

void game_term()
{
	LOG_DEBUG("TERMINATING GAME");
	map_free();
	audio_term();
	render_term();
	workman_term();
	LOG_DEBUG("GAME TERMINATED");
}


