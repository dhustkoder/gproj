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
static struct actor actor = {
	.scr = { .size = { 32, 32 }, .pos = { GPROJ_SCR_WIDTH / 2, GPROJ_SCR_HEIGHT / 2 } },
	.anim = {
		.frames = walk_down,
		.clk    = 0,
		.cnt    = ARRSZ(walk_down),
		.idx    = 0,
		.flags  = ANIM_FLAG_LOOP|ANIM_FLAG_ENABLED
	}
};


static double velocity = 5 * GPROJ_TILE_WIDTH;
static int actor_id = 0;


void player_init(void)
{
	actor_id = actors_add(&actor);
}


void player_update(const uint32_t now, const float dt)
{
	if (input_buttons_states&(INPUT_BUTTON_LEFT |
	                          INPUT_BUTTON_RIGHT|
	                          INPUT_BUTTON_UP   |
	                          INPUT_BUTTON_DOWN)) {
		struct rectf origin = {
			.pos = { 
				actor.scr.pos.x + actor.scr.size.x / 2.f,
				actor.scr.pos.y + actor.scr.size.y / 2.f
			},
			.size =  { 32, 32 }
		};
		actor.anim.flags |= ANIM_FLAG_ENABLED;
		if (input_buttons_states&INPUT_BUTTON_DOWN) {
			actor.scr.pos.y += velocity * dt;
			if (actor.anim.frames != walk_down) {
				actor.anim.frames = walk_down;
				actor.anim.cnt = ARRSZ(walk_down);
				actor.anim.idx = 0;
				actor.anim.clk = now;
			}
		} else if (input_buttons_states&INPUT_BUTTON_UP) {
			actor.scr.pos.y -= velocity * dt;
			if (actor.anim.frames != walk_up) {
				actor.anim.frames = walk_up;
				actor.anim.cnt = ARRSZ(walk_up);
				actor.anim.idx = 0;
				actor.anim.clk = now;
			}
		} else if (input_buttons_states&INPUT_BUTTON_LEFT) {
			actor.scr.pos.x -= velocity * dt;
			if (actor.anim.frames != walk_left) {
				actor.anim.frames = walk_left;
				actor.anim.cnt = ARRSZ(walk_left);
				actor.anim.idx = 0;
				actor.anim.clk = now;
			}
		} else if (input_buttons_states&INPUT_BUTTON_RIGHT && !map_is_blocking(&origin)) {
			actor.scr.pos.x += velocity * dt;
			if (actor.anim.frames != walk_right) {
				actor.anim.frames = walk_right;
				actor.anim.cnt = ARRSZ(walk_right);
				actor.anim.idx = 0;
				actor.anim.clk = now;
			}
		}
	} else {
		actor.anim.flags &= ~ANIM_FLAG_ENABLED;
	}

}

