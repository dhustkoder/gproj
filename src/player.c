#include "types.h"
#include "actors.h"
#include "timer.h"
#include "player.h"


input_button_t input_buttons_states;


static const struct actor_frame walkup[] = {
	{ 150, { .size = { 16, 16 }, .pos = { 511, 3040 } } },
	{ 150, { .size = { 16, 16 }, .pos = { 526, 3040 } } }
};
static const struct actor_frame walkdown[] = {
	{ 150, { .size = { 16, 16 }, .pos = { 512, 3072 } } },
	{ 150, { .size = { 16, 16 }, .pos = { 527, 3072 } } }
};
static const struct actor_frame walkleft[] = {
	{ 150, { .size = { 16, 16 }, .pos = { 510, 3088 } } },
	{ 150, { .size = { 16, 16 }, .pos = { 527, 3088 } } }
};
static const struct actor_frame walkright[] = {
	{ 150, { .size = { 16, 16 }, .pos = { 512, 3056 } } },
	{ 150, { .size = { 16, 16 }, .pos = { 527, 3056 } } }
};

static struct actor actor = {
	.scr = { .size = { 32, 32 }, .pos = { 64, 64 }    },
	.ts =  { .size = { 16, 16 }, .pos = { 511, 3040 } }
};

static int frame_idx = 0;
static uint32_t frame_clk = 0;
static uint32_t frame_duration = 0;

static float velocity = 2.5;



void player_init(void)
{
	actors_add(&actor);
}


void player_update(void)
{
	if (input_buttons_states == 0)
		return;


	const uint32_t now = timer_now();
	const struct actor_frame* nextf = NULL;
	int arrsz = 0;

	if (input_buttons_states&INPUT_BUTTON_UP) {
		actor.scr.pos.y -= velocity * 1;
		nextf = walkup;
		arrsz = ARRSZ(walkup);
	} else if (input_buttons_states&INPUT_BUTTON_DOWN) {
		actor.scr.pos.y += velocity * 1;
		nextf = walkdown;
		arrsz = ARRSZ(walkdown);
	} else if (input_buttons_states&INPUT_BUTTON_LEFT) {
		actor.scr.pos.x -= velocity * 1;
		nextf = walkleft;
		arrsz = ARRSZ(walkleft);
	} else if (input_buttons_states&INPUT_BUTTON_RIGHT) {
		actor.scr.pos.x += velocity * 1;
		nextf = walkright;
		arrsz = ARRSZ(walkright);
	}

	if (nextf != NULL) {
		if ((now - frame_clk) >= frame_duration) {
			frame_clk = now;
			frame_idx = (frame_idx + 1) % arrsz;
			frame_duration = nextf[frame_idx].duration;
			actor.ts = nextf[frame_idx].ts;
		}
	}

}

