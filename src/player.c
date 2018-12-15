#include "types.h"
#include "render.h"
#include "log.h"
#include "actors.h"
#include "timer.h"
#include "player.h"


input_button_t input_buttons_states;


static const struct actor_frame walk_up[] = {
	{ 150, { .size = { 16, 16 }, .pos = { 511, 3040 } } },
	{ 150, { .size = { 16, 16 }, .pos = { 526, 3040 } } }
};
static const struct actor_frame walk_down[] = {
	{ 150, { .size = { 16, 16 }, .pos = { 512, 3072 } } },
	{ 150, { .size = { 16, 16 }, .pos = { 527, 3072 } } }
};
static const struct actor_frame walk_left[] = {
	{ 150, { .size = { 16, 16 }, .pos = { 510, 3088 } } },
	{ 150, { .size = { 16, 16 }, .pos = { 527, 3088 } } }
};
static const struct actor_frame walk_right[] = {
	{ 150, { .size = { 16, 16 }, .pos = { 512, 3056 } } },
	{ 150, { .size = { 16, 16 }, .pos = { 527, 3056 } } }
};
static const struct actor_frame attack_right[] = {
	{ 150, { .size = { 16, 16 }, .pos = { 544, 3056 } } },
	{ 125, { .size = { 16, 16 }, .pos = { 559, 3056 } } },
	{ 100, { .size = { 16, 16 }, .pos = { 575, 3056 } } },
	{ 75, { .size = { 16, 16 }, .pos = { 591, 3056 } } }
};

static struct actor actor = {
	.scr = { .size = { 32, 32 }, .pos = { 64, 64 }    },
	.ts =  { .size = { 16, 16 }, .pos = { 511, 3040 } }
};

static int frame_idx = 0;
static uint32_t frame_clk = 0;
static uint32_t frame_duration = 0;
static int is_attacking = 0;
static double velocity = 5 * GPROJ_TILE_WIDTH;


void player_init(void)
{
	actors_add(&actor);
}


void player_update(const uint32_t now, const float dt)
{
	const struct actor_frame* nextf = NULL;
	static uint32_t lastclk = 0;

	if (lastclk == 0)
		lastclk = now;


	int arrsz = 0;

	if (is_attacking) {
		arrsz = ARRSZ(attack_right);
		
		if (frame_idx + 1 == arrsz)
			is_attacking = 0;

		nextf = attack_right;
		
	} else {
		if (input_buttons_states&INPUT_BUTTON_SHOOT) {
			is_attacking = 1;
			frame_idx = -1;
		} else if (input_buttons_states&INPUT_BUTTON_UP) {
			actor.scr.pos.y -= velocity * dt;
			nextf = walk_up;
			arrsz = ARRSZ(walk_up);
		} else if (input_buttons_states&INPUT_BUTTON_DOWN) {
			actor.scr.pos.y += velocity * dt;
			nextf = walk_down;
			arrsz = ARRSZ(walk_down);
		} else if (input_buttons_states&INPUT_BUTTON_LEFT) {
			actor.scr.pos.x -= velocity * dt;
			nextf = walk_left;
			arrsz = ARRSZ(walk_left);
		} else if (input_buttons_states&INPUT_BUTTON_RIGHT) {
			actor.scr.pos.x += velocity * dt;
			nextf = walk_right;
			arrsz = ARRSZ(walk_right);
		}

	}

	if (nextf != NULL) {
		if ((now - frame_clk) >= frame_duration) {
			frame_clk = now;
			frame_idx = (frame_idx + 1) % arrsz;
			frame_duration = nextf[frame_idx].duration;
			actor.ts = nextf[frame_idx].ts;
		}
	}

	lastclk = now;
}

