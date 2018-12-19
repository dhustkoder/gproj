#include "types.h"
#include "render.h"
#include "log.h"
#include "actors.h"
#include "map.h"
#include "timer.h"
#include "player.h"


input_button_t input_buttons_states;


static const struct actor_frame walk_up[] = {
	{ 144, { .size = { 16, 16 }, .pos = { 511, 3040 } } },
	{ 144, { .size = { 16, 16 }, .pos = { 526, 3040 } } }
};
static const struct actor_frame walk_down[] = {
	{ 144, { .size = { 16, 16 }, .pos = { 512, 3072 } } },
	{ 144, { .size = { 16, 16 }, .pos = { 527, 3072 } } }
};
static const struct actor_frame walk_left[] = {
	{ 144, { .size = { 16, 16 }, .pos = { 510, 3088 } } },
	{ 144, { .size = { 16, 16 }, .pos = { 527, 3088 } } }
};
static const struct actor_frame walk_right[] = {
	{ 144, { .size = { 16, 16 }, .pos = { 512, 3056 } } },
	{ 144, { .size = { 16, 16 }, .pos = { 527, 3056 } } }
};
/*
static const struct actor_frame attack_right[] = {
	{ 1250, { .size = { 16, 16 }, .pos = { 544, 3056 } } },
	{ 125, { .size = { 16, 16 }, .pos = { 559, 3056 } } },
	{ 100, { .size = { 16, 16 }, .pos = { 575, 3056 } } }
};

static const struct actor_frame attack_left[] = {
	{ 150, { .size = { 16, 16 }, .pos = { 544, 3056 } } },
	{ 125, { .size = { 16, 16 }, .pos = { 559, 3056 } } },
	{ 100, { .size = { 16, 16 }, .pos = { 575, 3056 } } }
};
static const struct actor_frame attack_up[] = {
	{ 150, { .size = { 16, 16 }, .pos = { 544, 3056 } } },
	{ 125, { .size = { 16, 16 }, .pos = { 559, 3056 } } },
	{ 100, { .size = { 16, 16 }, .pos = { 575, 3056 } } }
};
static const struct actor_frame attack_down[] = {
	{ 150, { .size = { 16, 16 }, .pos = { 544, 3056 } } },
	{ 125, { .size = { 16, 16 }, .pos = { 559, 3056 } } },
	{ 100, { .size = { 16, 16 }, .pos = { 575, 3056 } } }
};
*/

static int actor_id = 0;
static int anim_id = 0;
static int mov_id  = 0;
static float velocity = 5 * GPROJ_TILE_WIDTH;
static input_button_t prev_buttons_states;


void player_init(void)
{

	actor_id = actors_create(32, 32, GPROJ_SCR_WIDTH / 2, GPROJ_SCR_HEIGHT / 2);
	anim_id = actors_anim_create(actor_id, walk_down, ARRSZ(walk_down));
	mov_id = actors_mov_create(actor_id, 0, 0);

}


void player_update(const uint32_t now, const float dt)
{
	((void)dt);
	if (prev_buttons_states == input_buttons_states)
		return;

	if (input_buttons_states&(INPUT_BUTTON_LEFT |
	                          INPUT_BUTTON_RIGHT|
	                          INPUT_BUTTON_UP   |
	                          INPUT_BUTTON_DOWN)) {

		if (input_buttons_states&INPUT_BUTTON_DOWN && !(prev_buttons_states&INPUT_BUTTON_DOWN)) {
			actors_mov_set(mov_id, 0, velocity);
			actors_anim_set(anim_id, now, walk_down, ARRSZ(walk_down));
		} else if (input_buttons_states&INPUT_BUTTON_UP && !(prev_buttons_states&INPUT_BUTTON_UP)) {
			actors_mov_set(mov_id, 0, -velocity);
			actors_anim_set(anim_id, now, walk_up, ARRSZ(walk_up));
		} else if (input_buttons_states&INPUT_BUTTON_LEFT && !(prev_buttons_states&INPUT_BUTTON_LEFT)) {
			actors_mov_set(mov_id, -velocity, 0);
			actors_anim_set(anim_id, now, walk_left, ARRSZ(walk_left));
		} else if (input_buttons_states&INPUT_BUTTON_RIGHT && !(prev_buttons_states&INPUT_BUTTON_RIGHT)) {
			actors_mov_set(mov_id, velocity, 0);
			actors_anim_set(anim_id, now, walk_right, ARRSZ(walk_right));
		}
		
	} else {
		actors_mov_set(mov_id, 0, 0);
	}

	prev_buttons_states = input_buttons_states;

}

