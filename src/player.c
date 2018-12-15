#include "types.h"
#include "render.h"
#include "log.h"
#include "actors.h"
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

static const struct actor_frame attack_right[] = {
	{ 1250, { .size = { 16, 16 }, .pos = { 544, 3056 } } },
	{ 125, { .size = { 16, 16 }, .pos = { 559, 3056 } } },
	{ 100, { .size = { 16, 16 }, .pos = { 575, 3056 } } }
};
/*
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
	.scr = { .size = { 32, 32 }, .pos = { 64, 64 }    },
	.ts =  { .size = { 16, 16 }, .pos = { 511, 3040 } }
};


static double velocity = 5 * GPROJ_TILE_WIDTH;
static int actor_id = 0;
static struct actor_animation anim = {
	.frames = walk_down,
	.clk = 0,
	.idx = ARRSZ(walk_down),
	.flags = ANIM_FLAG_LOOP,
	.cnt = ARRSZ(walk_down)
};



void player_init(void)
{
	actor_id = actors_add(&actor);
	actors_set_animation(actor_id, &anim);
}


void player_update(const uint32_t now, const float dt)
{
	if (input_buttons_states&(INPUT_BUTTON_LEFT |
	                          INPUT_BUTTON_RIGHT|
	                          INPUT_BUTTON_UP   |
	                          INPUT_BUTTON_DOWN)) {
		anim.flags |= ANIM_FLAG_ENABLED;
		if (input_buttons_states&INPUT_BUTTON_DOWN) {
			actor.scr.pos.y += velocity * dt;
			if (anim.frames != walk_down) {
				anim.frames = walk_down;
				anim.cnt = ARRSZ(walk_down);
				anim.idx = 0;
				anim.clk = now;
			}
		} else if (input_buttons_states&INPUT_BUTTON_UP) {
			actor.scr.pos.y -= velocity * dt;
			if (anim.frames != walk_up) {
				anim.frames = walk_up;
				anim.cnt = ARRSZ(walk_up);
				anim.idx = 0;
				anim.clk = now;
			}
		} else if (input_buttons_states&INPUT_BUTTON_LEFT) {
			actor.scr.pos.x -= velocity * dt;
			if (anim.frames != walk_left) {
				anim.frames = walk_left;
				anim.cnt = ARRSZ(walk_left);
				anim.idx = 0;
				anim.clk = now;
			}
		} else if (input_buttons_states&INPUT_BUTTON_RIGHT) {
			actor.scr.pos.x += velocity * dt;
			if (anim.frames != walk_right) {
				anim.frames = walk_right;
				anim.cnt = ARRSZ(walk_right);
				anim.idx = 0;
				anim.clk = now;
			}
		}
	} else {
		anim.flags &= ~ANIM_FLAG_ENABLED;
	}

}

