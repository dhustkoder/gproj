#include "types.h"
#include "render.h"
#include "log.h"
#include "actors.h"
#include "map.h"
#include "timer.h"
#include "player.h"


input_button_t input_buttons_states;


static const struct actor_frame walk_up[] = {
	{ 128, { .size = { 21, 44 }, .pos = { 15, 11 } } },
	{ 128, { .size = { 21, 44 }, .pos = { 15, 11 } } }
};


static const struct actor_frame walk_down[] = {
	{ 128, { .size = { 21, 44 }, .pos = { 15, 11 } } },
	{ 128, { .size = { 21, 44 }, .pos = { 15, 11 } } }
};


static const struct actor_frame walk_left[] = {
	{ 128, { .size = { 21, 44 }, .pos = { 15, 11 } } },
	{ 128, { .size = { 21, 44 }, .pos = { 15, 11 } } }
};


static const struct actor_frame walk_right[] = {
	{ 98, { .size = { 26, 43 }, .pos = { 270, 11 } }},
	{ 98, { .size = { 26, 43 }, .pos = { 308, 11 } }},
	{ 98, { .size = { 26, 43 }, .pos = { 340, 11 } }},
	{ 98, { .size = { 26, 43 }, .pos = { 367, 11 } }},
	{ 98, { .size = { 26, 43 }, .pos = { 394, 11 } }},
	{ 98, { .size = { 26, 43 }, .pos = { 430, 11 } }},
	{ 98, { .size = { 26, 43 }, .pos = { 459, 11 } }},
	{ 98, { .size = { 26, 43 }, .pos = { 484, 11 } }},

};



enum {
	ATTACKING    = 0x01,
	FACING_UP    = 0x02,
	FACING_DOWN  = 0x04,
	FACING_LEFT  = 0x08,
	FACING_RIGHT = 0x10,
	FACING_STATES = (FACING_UP|FACING_DOWN|FACING_LEFT|FACING_RIGHT)
};

static int actor_id = 0;
static int anim_id = 0;
static int mov_id  = 0;
static float velocity = 5 * GPROJ_TILE_WIDTH;
static uint8_t states = 0;
static const struct actor_frame* curr_frames = NULL;
static input_button_t prev_buttons_states;


void player_init(void)
{
	actor_id = actors_create(
		&(struct rectf) {
			.size = { 32, 32 },
			.pos  = {GPROJ_SCR_WIDTH / 2, GPROJ_SCR_HEIGHT / 2 }
		},
		&walk_down[0].ts
	);

	anim_id = actors_anim_create(actor_id, walk_down, 1, ANIM_FLAG_DISABLED);
	states |= FACING_DOWN;
	curr_frames = NULL;
	mov_id = actors_mov_create(actor_id, 0, 0);
}


void player_update(const uint32_t now, const float dt)
{
	((void)dt);

	if (states&ATTACKING) {
		if (actors_anim_flags(anim_id)&ANIM_FLAG_ENDED)
			states &= ~ATTACKING;
		else
			return;
	}

	if (prev_buttons_states == input_buttons_states)
		return;

	if (input_buttons_states&(INPUT_BUTTON_LEFT |
	                                 INPUT_BUTTON_RIGHT|
	                                 INPUT_BUTTON_UP   |
	                                 INPUT_BUTTON_DOWN)) {

		const struct actor_frame* frames = NULL;
		int cnt = 0, facing = 0;
		float vx = 0, vy = 0;

		if (input_buttons_states&INPUT_BUTTON_DOWN && curr_frames != walk_down) {
			facing = FACING_DOWN;
			frames = walk_down;
			vy = velocity;
			cnt = ARRSZ(walk_down);
		} else if (input_buttons_states&INPUT_BUTTON_UP && curr_frames != walk_up) {
			facing = FACING_UP;
			frames = walk_up;
			vy = -velocity;
			cnt = ARRSZ(walk_up);
		} else if (input_buttons_states&INPUT_BUTTON_LEFT && curr_frames != walk_left) {
			facing = FACING_LEFT;
			frames = walk_left;
			vx = -velocity;
			cnt = ARRSZ(walk_left);
		} else if (input_buttons_states&INPUT_BUTTON_RIGHT && curr_frames != walk_right) {
			facing = FACING_RIGHT;
			frames = walk_right;
			vx = velocity;
			cnt = ARRSZ(walk_right);
		}

		if (frames != NULL) {
			states &= ~FACING_STATES;
			states |= facing;
			actors_mov_set(mov_id, vx, vy);
			actors_anim_set(anim_id, now, frames, cnt, ANIM_FLAG_LOOP);
			curr_frames = frames;
		}

	} else {
		actors_mov_set(mov_id, 0, 0);
		actors_anim_set(anim_id, 0, NULL, 0, ANIM_FLAG_DISABLED);
		curr_frames = NULL;
	}

	prev_buttons_states = input_buttons_states;

}

