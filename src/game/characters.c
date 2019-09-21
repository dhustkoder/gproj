#include "types.h"
#include "render.h"
#include "log.h"
#include "actors.h"
#include "map.h"
#include "timer.h"
#include "characters.h"


input_button_t input_buttons_states;
extern struct vec2f map_scrl_pos;

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
			.pos  = { 256 / 2, 160 / 2}
		}
	);

	actors_anim_set(
		player_id,
		timer_now(),
		idle_frames,
		ARRSZ(idle_frames),
		anim_flags
	);

}

void characters_update(const uint32_t now, const float dt)
{
	actors_update(now, dt);

	const struct vec2f player_pos = actors_get_pos(player_id);
	const float player_x = player_pos.x;

	if (prev_buttons_states != input_buttons_states) {
		if (input_buttons_states&(INPUT_BUTTON_LEFT |
								INPUT_BUTTON_RIGHT|
								INPUT_BUTTON_UP   |
								INPUT_BUTTON_DOWN)) {
			const float defvel = 32;
			if (input_buttons_states&INPUT_BUTTON_RIGHT) {
				velx = +defvel;
				anim_flags &= ~ANIM_FLAG_FLIPH;
			} else {
				velx = -defvel;
				anim_flags |= ANIM_FLAG_FLIPH;
			}

			actors_anim_set(player_id, now, walking_frames, ARRSZ(walking_frames), anim_flags);
		} else {
			velx = 0;
			map_scrl_vel_set(0, 0);
			actors_mov_set(player_id, 0, 0);
			actors_anim_set(player_id, now, idle_frames, ARRSZ(idle_frames), anim_flags);
		}

	}

	enum { MOVE_PLAYER, MOVE_MAP } who_moves;

	if (velx < -0.1|| velx > +0.1) {
		if (velx < -0.1) {
			if (map_scrl_pos.x <= 0 || player_x > (GPROJ_SCR_WIDTH / 2))
				who_moves = MOVE_PLAYER;
			else
				who_moves = MOVE_MAP;
		} else {
			if ((map_scrl_pos.x >= (GPROJ_FB_WIDTH - GPROJ_SCR_WIDTH)) ||
			    (player_x < (GPROJ_SCR_WIDTH / 2)))
				who_moves = MOVE_PLAYER;
			else
				who_moves = MOVE_MAP;
		}

		switch (who_moves) {
		case MOVE_PLAYER:
			actors_mov_set(player_id, velx, 0);
			map_scrl_vel_set(0, 0);
			break;
		case MOVE_MAP:
			actors_mov_set(player_id, 0, 0);
			map_scrl_vel_set(velx, 0);
			break;
		}

	}

	prev_buttons_states = input_buttons_states;
}

