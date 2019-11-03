#include "logger.h"
#include "workman.h"
#include "render.h"
#include "audio.h"
#include "timer.h"
#include "worldman.h"
#include "game.h"

extern struct world_meta levels[5];

void game_init(int argc, char** argv)
{
	((void)argc);
	((void)argv);
	LOG_DEBUG("#####INITIALIZING GAME#####");

	timer_profiler_init();
	render_init("GProj Testing", RENDER_MODE_OPENGL);
	audio_init();

	worldman_init(levels, STATIC_ARRAY_SIZE(levels));
	worldman_load_world("First");

	LOG_DEBUG("#####GAME INITIALIZED#####");
}

void game_step(const timer_clk_t now, const float dt)
{
	timer_profiler_block_start("frame time", 1);


	worldman_update_world(now, dt);
	worldman_send_render();
	render_finish_frame();
	


	timer_profiler_block_end();
}

void game_term()
{
	LOG_DEBUG("#####TERMINATING GAME#####");

	worldman_term();
	audio_term();
	render_term();
	timer_profiler_term();

	LOG_DEBUG("#####GAME TERMINATED#####");
}


