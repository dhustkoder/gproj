#include "logger.h"
#include "render.h"
#include "audio.h"
#include "profiler.h"
#include "worldman.h"
#include "game.h"


extern struct world_meta levels[5];


void game_init(void)
{
	log_dbg("#####INITIALIZING GAME#####");

	profiler_init();
	render_init("GProj Testing");
	audio_init();

	worldman_init(levels, STATIC_ARRAY_SIZE(levels));
	worldman_load_world("First");

	log_dbg("#####GAME INITIALIZED#####");
}

void game_step(const input_t input, const timer_clk_t now, const float dt)
{
	profiler_block_start("frame time", 60);

	worldman_update_world(input, now, dt);
	worldman_send_render();
	render_finish_frame();

	profiler_block_end();
}

void game_term()
{
	log_dbg("#####TERMINATING GAME#####");

	worldman_term();
	audio_term();
	render_term();
	profiler_term();

	log_dbg("#####GAME TERMINATED#####");
}


