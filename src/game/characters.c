#include "workman.h"
#include "audio.h"
#include "types.h"
#include "render.h"
#include "logger.h"
#include "actors.h"
#include "map.h"
#include "timer.h"
#include "characters.h"


input_button_t input_buttons_states;


/* PLAYER ANIMATIONS */
static const struct actor_frame idle_frames[] = {
	{ 196, { .size = { 26, 46 }, .pos = { 90, 56 } } },
	{ 196, { .size = { 26, 46 }, .pos = { 124, 56 } } },
	{ 196, { .size = { 26, 46 }, .pos = { 160, 56 } } },
};

static const struct actor_frame walking_frames[] = {
	{ 96, { .size = { 26, 46 }, .pos = { 350, 56 } } },
	{ 96, { .size = { 26, 46 }, .pos = { 385, 56 } } },
	{ 96, { .size = { 26, 46 }, .pos = { 412, 56 } } },
	{ 96, { .size = { 26, 46 }, .pos = { 441, 56 } } },
	{ 96, { .size = { 26, 46 }, .pos = { 473, 56 } } },
	{ 96, { .size = { 26, 46 }, .pos = { 505, 56 } } },
	{ 96, { .size = { 26, 46 }, .pos = { 531, 56 } } },
	{ 96, { .size = { 26, 46 }, .pos = { 560, 56 } } },
};

static struct vec2f vel = { 0, 0 };
static struct vec2f wpos = { .x = 26, .y = GPROJ_SCR_HEIGHT - 46 - 32 };
static struct vec2i wsize = { .x = 26, .y = 46 };
static struct vec2i spos = idle_frames[0].ss.pos;
static struct vec2i ssize = walking_frames[0].ss.size;
static sprite_flag_t flag = 0;

static input_button_t prev_buttons_states;


void characters_init(void)
{
	render_load_ss("richter-ss.png");
}

void characters_update(const timer_clk_t now, const float dt)
{
	((void)dt);
	((void)now);

	if (prev_buttons_states != input_buttons_states) {
		if (input_buttons_states&(INPUT_BUTTON_LEFT |
		                          INPUT_BUTTON_RIGHT|
		                          INPUT_BUTTON_UP   |
		                          INPUT_BUTTON_DOWN)) {

			if (input_buttons_states&INPUT_BUTTON_RIGHT) {
				vel.x = +128;
				flag &= ~SPRITE_FLAG_FLIPH;
			} else {
				vel.x = -128;
				flag |= SPRITE_FLAG_FLIPH;
			};
		} else {
			vel.x = 0;
		}

		prev_buttons_states = input_buttons_states;
	}

	actors_move(dt, &wpos, &vel, &wpos, 1);

}


void characters_send_render(void)
{
	workman_work_until_all_finished();
	const int camx = wpos.x - (GPROJ_SCR_WIDTH / 2) + (26 / 2);
	render_set_camera(camx, 0);
	render_ss(&wpos, &wsize, &spos, &ssize, &flag, 1);
}

