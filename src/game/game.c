#include "logger.h"
#include "workman.h"
#include "render.h"
#include "audio.h"
#include "timer.h"
#include "actors.h"
#include "chars.h"
#include "maps.h"
#include "game.h"

static struct vec2i game_camera = { 0, 0 };
static float xvel = 5;
static float x = 0;

void game_init(int argc, char** argv)
{
	((void)argc);
	((void)argv);
	LOG_DEBUG("INITIALIZING GAME");
	
	render_init("GProj Testing");
	render_layers_setup(GPROJ_SCR_WIDTH, GPROJ_SCR_HEIGHT);
	audio_init();
	maps_init();
	chars_init();

	LOG_DEBUG("GAME INITIALIZED");
}

void game_step(timer_clk_t now, float dt)
{
	x += xvel * dt;
	game_camera.x = x;	
	// update
	chars_update(now, dt);
	maps_update(&game_camera, now, dt);
	// send render
	render_text("CAMPOS => (%d, %d)", game_camera.x, game_camera.y);
	chars_send_render();
	maps_send_render();
	render_present();
}

void game_term()
{
	LOG_DEBUG("TERMINATING GAME");

	chars_term();
	maps_term();
	audio_term();
	render_term();

	LOG_DEBUG("GAME TERMINATED");
}


