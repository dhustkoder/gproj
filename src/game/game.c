#include "logger.h"
#include "workman.h"
#include "render.h"
#include "audio.h"
#include "timer.h"
#include "actors.h"
#include "chars.h"
#include "maps.h"
#include "game.h"

static struct vec2f game_camera = { 0, 0 };

void game_init(int argc, char** argv)
{
	((void)argc);
	((void)argv);
	LOG_DEBUG("INITIALIZING GAME");
	
	render_init("GProj Testing");
	render_layers_setup(GPROJ_SCR_WIDTH, GPROJ_SCR_HEIGHT);
	audio_init();
	maps_init();
//	chars_init();

	LOG_DEBUG("GAME INITIALIZED");
}

void game_step(const timer_clk_t now, const float dt)
{
	extern struct events gproj_events;

	const input_button_t buttons = gproj_events.input.buttons;
	if (buttons&INPUT_BUTTON_RIGHT)
		game_camera.x += 64.f * dt;
	else if (buttons&INPUT_BUTTON_LEFT)
		game_camera.x -= 64.f * dt;
	else if (buttons&INPUT_BUTTON_UP)
		game_camera.y -= 64.f * dt;
	else if (buttons&INPUT_BUTTON_DOWN)
		game_camera.y += 64.f * dt;
	render_text("CAMPOS => (%.2f, %.2f)", game_camera.x, game_camera.y);
	// update
	//chars_update(now, dt);
	
	maps_update(&game_camera, now, dt);
	
	// send render
	
	//chars_send_render();
	
	maps_send_render();
	render_present();
}

void game_term()
{
	LOG_DEBUG("TERMINATING GAME");

//	chars_term();
	maps_term();
	audio_term();
	render_term();

	LOG_DEBUG("GAME TERMINATED");
}


