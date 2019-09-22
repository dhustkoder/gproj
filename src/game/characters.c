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

const float player_defvel = GPROJ_SCR_WIDTH / 4;

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
		timer_now(),
		idle_frames,
		ARRSZ(idle_frames),
		anim_flags
	);

	for (int i = 0; i < 255; ++i) {
		const int id = actors_create(
			&(struct rectf) {
				.size = { 26, 46 },
				.pos  = {
					(i * 26) % GPROJ_WORLD_WIDTH,
					(46 * ((i * 26) / GPROJ_WORLD_WIDTH)) % GPROJ_WORLD_HEIGHT
				}
			}
		);

		actors_anim_set(
			id,
			timer_now(),
			idle_frames,
			ARRSZ(idle_frames),
			anim_flags
		);

		actors_mov_set(id, -8, -1);
	}
}

void characters_update(const uint32_t now, const float dt)
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
		actors_anim_set(player_id, now, anim, anim_sz, anim_flags);
		prev_buttons_states = input_buttons_states;
	}


	actors_update(now, dt);

	const struct vec2f player_pos = actors_get_pos(player_id);

	int camx = player_pos.x - (GPROJ_SCR_WIDTH / 2);

	if (camx < 0)
		camx = 0;
	else if (camx >= GPROJ_WORLD_WIDTH - GPROJ_SCR_WIDTH)
		camx = GPROJ_WORLD_WIDTH - GPROJ_SCR_WIDTH;

	render_set_camera(camx, 0);

	render_text("PLAYER POS => (%d, %d)", (int)player_pos.x, (int)player_pos.y);

}

