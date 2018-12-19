#include "types.h"
#include "render.h"
#include "log.h"
#include "actors.h"
#include "map.h"
#include "timer.h"
#include "player.h"


input_button_t input_buttons_states;


static const struct actor_frame walk_up[] = {
	{ 128, { .size = { 16, 16 }, .pos = { 511, 3040 } } },
	{ 128, { .size = { 16, 16 }, .pos = { 526, 3040 } } }
};
static const struct actor_frame walk_down[] = {
	{ 128, { .size = { 16, 16 }, .pos = { 512, 3072 } } },
	{ 128, { .size = { 16, 16 }, .pos = { 527, 3072 } } }
};
static const struct actor_frame walk_left[] = {
	{ 128, { .size = { 16, 16 }, .pos = { 510, 3088 } } },
	{ 128, { .size = { 16, 16 }, .pos = { 527, 3088 } } }
};
static const struct actor_frame walk_right[] = {
	{ 128, { .size = { 16, 16 }, .pos = { 512, 3056 } } },
	{ 128, { .size = { 16, 16 }, .pos = { 527, 3056 } } }
};

static const struct actor_frame attack_right[] = {
	{ 155, { .size = { 16, 16 }, .pos = { 544, 3056 } } },
	{ 155, { .size = { 16, 16 },  .pos = { 559, 3056 } } },
	{ 155, { .size = { 16, 16 },  .pos = { 575, 3056 } } }
};

static const struct actor_frame attack_left[] = {
	{ 155, { .size = { 16, 16 }, .pos = { 542, 3088 } } },
	{ 155, { .size = { 16, 16 },  .pos = { 559, 3088 } } },
	{ 155, { .size = { 16, 16 },  .pos = { 575, 3088 } } }
};
static const struct actor_frame attack_up[] = {
	{ 155, { .size = { 16, 16 }, .pos = { 543, 3040 } } },
	{ 155, { .size = { 16, 16 },  .pos = { 560, 3040 } } },
	{ 155, { .size = { 16, 16 },  .pos = { 574, 3040 } } }
};
static const struct actor_frame attack_down[] = {
	{ 155, { .size = { 16, 16 }, .pos = { 543, 3072 } } },
	{ 155, { .size = { 16, 16 },  .pos = { 558, 3072 } } },
	{ 155, { .size = { 16, 16 },  .pos = { 575, 3072 } } }
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
	mov_id = actors_mov_create(actor_id, 0, 0);
}


void player_update(const uint32_t now, const float dt)
{
	((void)dt);

	if (states&ATTACKING) {
		if (actors_anim_flags(anim_id)&ANIM_FLAG_ENDED)
			states &= ~ATTACKING;
		return;
	}


	if (prev_buttons_states == input_buttons_states)
		return;

	if (input_buttons_states&INPUT_BUTTON_ACTION) {
		states |= ATTACKING;
		actors_mov_set(mov_id, 0, 0);
		switch (states&(FACING_STATES)) {
		case FACING_UP:
			actors_anim_set(anim_id, now, attack_up, ARRSZ(attack_up), 0);
			break;
		case FACING_DOWN:
			actors_anim_set(anim_id, now, attack_down, ARRSZ(attack_down), 0);
			break;
		case FACING_LEFT:
			actors_anim_set(anim_id, now, attack_left, ARRSZ(attack_left), 0);
			break;
		case FACING_RIGHT:
			actors_anim_set(anim_id, now, attack_right, ARRSZ(attack_right), 0);
			break;

		}
	} else if (input_buttons_states&(INPUT_BUTTON_LEFT |
	                                 INPUT_BUTTON_RIGHT|
	                                 INPUT_BUTTON_UP   |
	                                 INPUT_BUTTON_DOWN)) {
		states &= ~FACING_STATES;
		if (input_buttons_states&INPUT_BUTTON_DOWN && !(prev_buttons_states&INPUT_BUTTON_DOWN)) {
			states |= FACING_DOWN;
			actors_mov_set(mov_id, 0, velocity);
			actors_anim_set(anim_id, now, walk_down, ARRSZ(walk_down), ANIM_FLAG_LOOP);
		} else if (input_buttons_states&INPUT_BUTTON_UP && !(prev_buttons_states&INPUT_BUTTON_UP)) {
			states |= FACING_UP;
			actors_mov_set(mov_id, 0, -velocity);
			actors_anim_set(anim_id, now, walk_up, ARRSZ(walk_up), ANIM_FLAG_LOOP);
		} else if (input_buttons_states&INPUT_BUTTON_LEFT && !(prev_buttons_states&INPUT_BUTTON_LEFT)) {
			states |= FACING_LEFT;
			actors_mov_set(mov_id, -velocity, 0);
			actors_anim_set(anim_id, now, walk_left, ARRSZ(walk_left), ANIM_FLAG_LOOP);
		} else if (input_buttons_states&INPUT_BUTTON_RIGHT && !(prev_buttons_states&INPUT_BUTTON_RIGHT)) {
			states |= FACING_RIGHT;
			actors_mov_set(mov_id, velocity, 0);
			actors_anim_set(anim_id, now, walk_right, ARRSZ(walk_right), ANIM_FLAG_LOOP);
		}
		
	} else {
		actors_mov_set(mov_id, 0, 0);
		actors_anim_set(anim_id, 0, NULL, 0, ANIM_FLAG_DISABLED);
	}

	prev_buttons_states = input_buttons_states;

}

