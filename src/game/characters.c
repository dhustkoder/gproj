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

static const struct actor_animation player_animations[] = {
	{ .frames = idle_frames,    .cnt = ARRSZ(idle_frames)    },
	{ .frames = walking_frames, .cnt = ARRSZ(walking_frames) }
};


static const struct actor_animation_collection anim_collections[] = {
	{ .animations = player_animations, .cnt = ARRSZ(player_animations) }
};

static input_button_t prev_buttons_states;

static struct actors_ctrl actrl;


void characters_init(void)
{
	render_load_ss("richter-ss.png");
	actrl = actors_create(anim_collections, ARRSZ(anim_collections));
	actrl.flags[0] = ACTOR_FLAG_LOOP;
	actrl.wrects[0].size.x = 26;
	actrl.wrects[0].size.y = 46;
	actrl.wrects[0].pos.x = 26;
	actrl.wrects[0].pos.y = GPROJ_SCR_HEIGHT - 46 - 32;
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
				actrl.vels[0].x = +128;
				actrl.flags[0] &= ~ACTOR_FLAG_FLIPH;
			} else {
				actrl.vels[0].x = -128;
				actrl.flags[0] |= ACTOR_FLAG_FLIPH;
			}
			actrl.anim_idxs[0] = 1;
		} else {
			actrl.vels[0].x = 0;
			actrl.anim_idxs[0] = 0;
		}

		prev_buttons_states = input_buttons_states;
	}

	actors_update(now, dt);
}


void characters_send_render(void)
{
	workman_work_until_all_finished();
	const int camx = actrl.wrects[0].pos.x - (GPROJ_SCR_WIDTH / 2) + (26 / 2);
	render_set_camera(camx, 0);

	actors_send_render();
}
