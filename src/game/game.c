#include "logger.h"
#include "workman.h"
#include "render.h"
#include "audio.h"
#include "timer.h"
#include "worldman.h"
#include "game.h"

extern struct world_meta levels[1];

void game_init(int argc, char** argv)
{
	((void)argc);
	((void)argv);
	LOG_DEBUG("INITIALIZING GAME");
	
	render_init("GProj Testing");
	render_layers_setup(GPROJ_SCR_WIDTH, GPROJ_SCR_HEIGHT);
	audio_init();

	worldman_init(levels, 1);
	worldman_load_world("first");

	LOG_DEBUG("GAME INITIALIZED");
}

void game_step(const timer_clk_t now, const float dt)
{
	worldman_update_world(now, dt);
	worldman_send_render();
	render_present();
}

void game_term()
{
	LOG_DEBUG("TERMINATING GAME");
	
	audio_term();
	render_term();

	LOG_DEBUG("GAME TERMINATED");
}


