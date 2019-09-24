#include "types.h"
#include "render.h"
#include "logger.h"
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
	{ 96, { .size = { 26, 46 }, .pos = { 350, 56 } } },
	{ 96, { .size = { 26, 46 }, .pos = { 385, 56 } } },
	{ 96, { .size = { 26, 46 }, .pos = { 412, 56 } } },
	{ 96, { .size = { 26, 46 }, .pos = { 441, 56 } } },
	{ 96, { .size = { 26, 46 }, .pos = { 473, 56 } } },
	{ 96, { .size = { 26, 46 }, .pos = { 505, 56 } } },
	{ 96, { .size = { 26, 46 }, .pos = { 531, 56 } } },
	{ 96, { .size = { 26, 46 }, .pos = { 560, 56 } } },
};



enum {
	CHR_STATE_IDLE    = 0x01,
};

static const float player_defvel = GPROJ_SCR_WIDTH / 4;
static int player_id = 0;
static int anim_flags = ANIM_FLAG_LOOP;
static input_button_t prev_buttons_states;
static float velx = 0;



void characters_init(void)
{
	render_load_ss("richter-ss.png");

	player_id = actors_create(
		&(struct rectf) {
			.size = { 26, 46 },
			.pos  = { 26, GPROJ_SCR_HEIGHT - 46 - 16}
		}
	);

	actors_anim_set(
		player_id,
		idle_frames,
		ARRSZ(idle_frames),
		anim_flags,
		timer_now()
	);

}

void characters_update(const timer_clk_t now, const float dt)
{
	if (prev_buttons_states != input_buttons_states) {
		const struct actor_frame* anim = NULL;
		int anim_sz = 0;
		if (input_buttons_states&(INPUT_BUTTON_LEFT |
		                          INPUT_BUTTON_RIGHT|
		                          INPUT_BUTTON_UP   |
		                          INPUT_BUTTON_DOWN)) {

			if (input_buttons_states&INPUT_BUTTON_RIGHT) {
				velx = +player_defvel;
				anim_flags &= ~ANIM_FLAG_FLIPH;
			} else {
				velx = -player_defvel;
				anim_flags |= ANIM_FLAG_FLIPH;
			}
			anim = walking_frames;
			anim_sz = ARRSZ(walking_frames);
		} else {
			velx = 0;
			anim = idle_frames;
			anim_sz = ARRSZ(idle_frames);
		}

		actors_mov_set(player_id, velx, 0);
		actors_anim_set(player_id, anim, anim_sz, anim_flags, now);
		prev_buttons_states = input_buttons_states;
	}

	actors_update(now, dt);

	const struct vec2f player_pos = actors_get_pos(player_id);

	const int camx = player_pos.x - (GPROJ_SCR_WIDTH / 2);

	render_set_camera(camx, 0);

	render_text("PLAYER POS => (%d, %d)", (int)player_pos.x, (int)player_pos.y);

}

