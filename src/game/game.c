#include "logger.h"
#include "workman.h"
#include "window.h"
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

	window_init("GProj Testing");
	render_init();
	render_layers_setup(GPROJ_SCR_WIDTH, GPROJ_SCR_HEIGHT);
	audio_init();

	worldman_init(levels, STATIC_ARRAY_SIZE(levels));
	worldman_load_world("First");

	LOG_DEBUG("#####GAME INITIALIZED#####");
}

void game_step(const timer_clk_t now, const float dt)
{
	worldman_update_world(now, dt);
	worldman_send_render();
	render_present();
}

void game_term()
{
	LOG_DEBUG("#####TERMINATING GAME#####");

	worldman_term();
	audio_term();
	render_term();
	window_term();

	LOG_DEBUG("#####GAME TERMINATED#####");
}


