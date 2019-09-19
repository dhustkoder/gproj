#include "types.h"
#include "render.h"
#include "log.h"
#include "actors.h"
#include "map.h"
#include "timer.h"
#include "characters.h"


input_button_t input_buttons_states;


static const struct actor_frame idle_frames[] = {
	{ 196, { .size = { 26, 46 }, .pos = { 90, 56 } } },
	{ 196, { .size = { 26, 46 }, .pos = { 124, 56 } } },
	{ 196, { .size = { 26, 46 }, .pos = { 160, 56 } } },
};

static const struct actor_frame walking_frames[] = {
	{ 128, { .size = { 26, 46 }, .pos = { 350, 56 } } },
	{ 128, { .size = { 26, 46 }, .pos = { 385, 56 } } },
	{ 128, { .size = { 26, 46 }, .pos = { 412, 56 } } },
	{ 128, { .size = { 26, 46 }, .pos = { 441, 56 } } },
	{ 128, { .size = { 26, 46 }, .pos = { 473, 56 } } },
	{ 128, { .size = { 26, 46 }, .pos = { 505, 56 } } },
	{ 128, { .size = { 26, 46 }, .pos = { 531, 56 } } },
	{ 128, { .size = { 26, 46 }, .pos = { 560, 56 } } },
};



enum {
	CHR_STATE_IDLE    = 0x01,
};

static int actor_id = 0;
static int anim_flags = ANIM_FLAG_LOOP;
static input_button_t prev_buttons_states;


void characters_init(void)
{
	render_set_ss("richter-ss.png");

	actor_id = actors_create(
		&(struct rectf) {
			.size = { 32, 64 },
			.pos  = { 0, GPROJ_SCR_HEIGHT - (68 + GPROJ_TILE_HEIGHT) }
		}
	);

	actors_anim_set(
		actor_id,
		timer_now(),
		idle_frames,
		ARRSZ(idle_frames),
		anim_flags
	);

}


void characters_update(const uint32_t now, const float dt)
{
	((void)dt);

	if (prev_buttons_states == input_buttons_states)
		return;

	if (input_buttons_states&(INPUT_BUTTON_LEFT |
	                          INPUT_BUTTON_RIGHT|
	                          INPUT_BUTTON_UP   |
	                          INPUT_BUTTON_DOWN)) {

		float velx = GPROJ_X_TILES * 4;
		anim_flags &= ~ANIM_FLAG_BIDIR;
		if (input_buttons_states&INPUT_BUTTON_RIGHT) {
			velx = +velx;
			anim_flags &= ~ANIM_FLAG_FLIPH;
		} else {
			velx = -velx;
			anim_flags |= ANIM_FLAG_FLIPH;
		}

		actors_mov_set(actor_id, velx, 0);
		actors_anim_set(actor_id, now, walking_frames, ARRSZ(walking_frames), anim_flags);

	} else {
		anim_flags &= ~ANIM_FLAG_BIDIR;
		actors_mov_set(actor_id, 0, 0);
		actors_anim_set(actor_id, now, idle_frames, ARRSZ(idle_frames), anim_flags);
	}

	prev_buttons_states = input_buttons_states;

}

