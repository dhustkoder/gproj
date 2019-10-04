#include <assert.h>
#include "workman.h"
#include "logger.h"
#include "timer.h"
#include "render.h"
#include "actors.h"


static struct recti srects[GPROJ_MAX_ACTORS];
static struct rectf wrects[GPROJ_MAX_ACTORS];
static struct vec2f vels[GPROJ_MAX_ACTORS];

static struct frame_timing_pack {
	timer_clk_t clk;
	int16_t ms;
} frame_timings[GPROJ_MAX_ACTORS];

static struct frame_indexing_pack {
	const struct actor_frame* frames;
	int8_t cnt;
	int8_t idx;
} frame_indexes[GPROJ_MAX_ACTORS];


static int8_t anim_idxs[GPROJ_MAX_ACTORS];
static const struct actor_animation_collection* anim_collections;
static actor_flag_t actor_flags[GPROJ_MAX_ACTORS];


static int nacts = 0;
static volatile bool need_render = false;



struct actors_ctrl actors_create(
	const struct actor_animation_collection* const collections,
	const int cnt)
{
	assert(collections != NULL);
	assert(cnt > 0 && cnt <= GPROJ_MAX_ACTORS);

	
	memset(frame_timings, 0, sizeof(frame_timings[0]) * cnt);
	memset(anim_idxs, 0, sizeof(anim_idxs[0]) * cnt);
	
	for (int aid = 0; aid < cnt; ++aid) {
		assert(
			collections[aid].cnt > 0 &&
			collections[aid].animations != NULL &&
			collections[aid].animations[0].cnt >  0 &&
			collections[aid].animations[0].frames != NULL
		);
		srects[aid] = collections[aid].animations[0].frames[0].ss;
		frame_indexes[aid].frames = collections[aid].animations[0].frames;
		frame_indexes[aid].cnt = collections[aid].animations[0].cnt;
		frame_indexes[aid].idx = 0;
	}

	anim_collections = collections;
	nacts = cnt;

	return (struct actors_ctrl) {
		.wrects = wrects,
		.vels = vels,
		.anim_idxs = anim_idxs,
		.flags = actor_flags
	};

}




static struct workpack {
	timer_clk_t now;
	float dt;
	int start;
	int end;
} wpack;


static void actors_move(union work_arg arg)
{
	const struct workpack pack = *(struct workpack*)arg.ptr;
	const float dt = pack.dt;
	const int start = pack.start;
	const int cnt = pack.end;
	for (int i = start; i < cnt; ++i) {
		if (vels[i].x < -0.01f || vels[i].x > 0.01f ||
		    vels[i].y < -0.01f || vels[i].y > 0.01f) {
			wrects[i].pos.x += vels[i].x * dt;
			wrects[i].pos.y += vels[i].y * dt;
			need_render = true;
		}
	}
}

static void actors_animate(union work_arg arg)
{
	const struct workpack pack = *(struct workpack*)arg.ptr;
	const timer_clk_t now = pack.now;
	const int start = pack.start;
	const int cnt = pack.end;

	for (int i = start; i < cnt; ++i) {
		const int flags = actor_flags[i];
		if (flags&ACTOR_FLAG_DISABLED)
			continue;

		struct frame_timing_pack* const ftp = &frame_timings[i];
		struct frame_indexing_pack* const fip = &frame_indexes[i];

		const struct actor_animation_collection* const coll = &anim_collections[i];
		const int8_t curr_anim_idx = anim_idxs[i];
		assert(curr_anim_idx < coll->cnt);

		const struct actor_animation* const curr_anim = &coll->animations[curr_anim_idx];


		if (((int)(now - ftp->clk)) >= ftp->ms || curr_anim->frames != fip->frames) {
			need_render = true;
			
			ftp->clk = now;

			if (curr_anim->frames == fip->frames) {
				fip->idx += flags&ACTOR_FLAG_BKWD ? -1 : 1;
			} else {
				fip->idx = 0;
				fip->frames = curr_anim->frames;
				fip->cnt = curr_anim->cnt;
			}

			if (fip->idx >= fip->cnt || fip->idx < 0) {
				if (flags&ACTOR_FLAG_LOOP) {
					if (fip->idx > 0 && flags&ACTOR_FLAG_BIDIR) {
						fip->idx = fip->cnt - 2;
						actor_flags[i] |= ACTOR_FLAG_BKWD;
					} else if (fip->idx < 0) {
						fip->idx = 1;
						actor_flags[i] &= ~ACTOR_FLAG_BKWD;
					} else {
						fip->idx = 0;
					}
				} else {
					fip->idx = 0;
					actor_flags[i] |= ACTOR_FLAG_DISABLED;
				}
			}

			ftp->ms = fip->frames[fip->idx].ms;
			srects[i] = fip->frames[fip->idx].ss;
		}
	}
	
}




void actors_update(const timer_clk_t now, const float dt)
{
	wpack = (struct workpack) { .now=now,.dt=dt,.start=0,.end=nacts };
	workman_push_work(actors_move, (union work_arg){.ptr = &wpack});
	workman_push_work(actors_animate, (union work_arg){.ptr = &wpack});
	workman_push_sleep(1);
}


void actors_send_render(void)
{
	workman_work_until_all_finished();
	if (need_render) {
		need_render = false;
		render_ss(srects, wrects, actor_flags, nacts);
	}
}
