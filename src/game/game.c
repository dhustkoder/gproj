#include "logger.h"
#include "workman.h"
#include "render.h"
#include "audio.h"
#include "timer.h"
#include "actors.h"
#include "chars.h"
#include "game.h"


void game_init(int argc, char** argv)
{
	((void)argc);
	((void)argv);
	LOG_DEBUG("INITIALIZING GAME");
	workman_init();
	timer_profiler_init();
	render_init("GProj Testing");
	render_layers_setup((struct vec2i){GPROJ_SCR_WIDTH, GPROJ_SCR_HEIGHT}, 1);
	audio_init();
	chars_init();
	LOG_DEBUG("GAME INITIALIZED");
}

void game_step(timer_clk_t now, float dt)
{
	// update
	chars_update(now, dt);

	// send render
	chars_send_render();
}

void game_term()
{
	LOG_DEBUG("TERMINATING GAME");
	audio_term();
	render_term();
	workman_term();
	LOG_DEBUG("GAME TERMINATED");
}


