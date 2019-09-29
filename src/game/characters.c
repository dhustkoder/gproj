#include "audio.h"
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
static int walking_sfx_id = 0;


void characters_init(void)
{
	render_load_ss("richter-ss.png");

	player_id = actors_create(
		&(struct rectf) {
			.size = { 26, 46 },
			.pos  = { 26, 160 - 46 - 16 }
		}
	);

	actors_anim_set(
		player_id,
		idle_frames,
		ARRSZ(idle_frames),
		anim_flags,
		timer_now()
	);

	int x = 0, y = 0;
	for (int i = 0; i < GPROJ_MAX_ACTORS - 1; ++i) {
		const int id = actors_create(
			&(struct rectf) {
				.size = { 26, 46 },
				.pos  = { x + (i * 0.46), y + (i * 0.06) }
			}
		);

		x += 20;
		if (x > GPROJ_SCR_WIDTH) {
			x = 0;
			y += 40;
			if (y >= GPROJ_SCR_HEIGHT)
				y = 0;
		}

		actors_anim_set(
			id,
			id%2?idle_frames:walking_frames,
			id%2?ARRSZ(idle_frames):ARRSZ(walking_frames),
			anim_flags,
			timer_now()
		);

		actors_mov_set(id, 0.2, 0);
	}

	walking_sfx_id = audio_load_sfx("sounds/stepstone_4.wav");	
}

void characters_update(const timer_clk_t now, const float dt)
{
	((void)dt);
	timer_profiler_block_start("PLAYER_UPDATE", 512);
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
			audio_play_sfx(walking_sfx_id);
		} else {
			audio_stop_sfx(walking_sfx_id);
			velx = 0;
			anim = idle_frames;
			anim_sz = ARRSZ(idle_frames);
		}

		actors_mov_set(player_id, velx, 0);
		actors_anim_set(player_id, anim, anim_sz, anim_flags, now);
		prev_buttons_states = input_buttons_states;
	}
	
	timer_profiler_block_end();


	timer_profiler_block_start("PLAYER_POS", 512);
	const struct vec2f player_pos = actors_get_pos(player_id);
	const int camx = player_pos.x - (GPROJ_SCR_WIDTH / 2);
	render_set_camera(camx, 0);
	timer_profiler_block_end();

	render_text("PLAYER POS => (%d, %d)", (int)player_pos.x, (int)player_pos.y);

}

