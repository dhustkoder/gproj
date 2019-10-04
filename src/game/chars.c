#include "workman.h"
#include "audio.h"
#include "types.h"
#include "render.h"
#include "logger.h"
#include "actors.h"
#include "timer.h"
#include "chars.h"


input_button_t input_buttons_states;


/* PLAYER ANIMATIONS */
static const struct frame idle_frames[] = {
	{ 196, { .size = { 26, 46 }, .pos = { 90, 56 } } },
	{ 196, { .size = { 26, 46 }, .pos = { 124, 56 } } },
	{ 196, { .size = { 26, 46 }, .pos = { 160, 56 } } },
};

static const struct frame walking_frames[] = {
	{ 96, { .size = { 26, 46 }, .pos = { 350, 56 } } },
	{ 96, { .size = { 26, 46 }, .pos = { 385, 56 } } },
	{ 96, { .size = { 26, 46 }, .pos = { 412, 56 } } },
	{ 96, { .size = { 26, 46 }, .pos = { 441, 56 } } },
	{ 96, { .size = { 26, 46 }, .pos = { 473, 56 } } },
	{ 96, { .size = { 26, 46 }, .pos = { 505, 56 } } },
	{ 96, { .size = { 26, 46 }, .pos = { 531, 56 } } },
	{ 96, { .size = { 26, 46 }, .pos = { 560, 56 } } },
};




static struct frame_timing timing = { .clk = 0, .ms = 0 };
static struct animation animation = { .frames = idle_frames, .cnt = ARRSZ(idle_frames), .idx = 0 };

static struct vec2f vel = { 0, 0 };
static struct vec2f wpos = { .x = 0, .y = GPROJ_SCR_HEIGHT - 46 };
static struct vec2i wsize = { .x = 26, .y = 46 };
static struct vec2i spos = {0, 0};
static struct vec2i ssize = {0, 0};
static render_flag_t flag = 0;

static input_button_t prev_buttons_states;


void chars_init(void)
{
	render_load_ss("richter-ss.png");
}

void chars_update(const timer_clk_t now, const float dt)
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
				flag &= ~RENDER_FLAG_FLIPH;
			} else {
				vel.x = -128;
				flag |= RENDER_FLAG_FLIPH;
			};
			animation.frames = walking_frames;
			animation.cnt = ARRSZ(walking_frames);
			animation.idx = 0;
		} else {
			vel.x = 0;
			animation.frames = idle_frames;
			animation.cnt = ARRSZ(idle_frames);
			animation.idx = 0;
		}

		prev_buttons_states = input_buttons_states;
	}

	actors_move(dt, &vel, &wpos, 1);
	actors_animate(now, &timing, &animation, &spos, &ssize, 1);
}


void chars_send_render(void)
{
	render_ss(0, &wpos, &wsize, &spos, &ssize, &flag, 1);
}

