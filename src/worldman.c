#include <assert.h>
#include <string.h>
#include "logger.h"
#include "render.h"
#include "events.h"
#include "worldman.h"


static const char* loaded_world_name = NULL;
static int loaded_world_idx = -1;
static int nworlds = 0;
static struct world_meta metas[GPROJ_MAX_WORLDS];
static struct world world;

static int search_meta(const char* const name)
{
	for (int i = 0; i < nworlds; ++i)
		if (strcmp(metas[i].name, name) == 0)
			return i;

	assert(false && "INVALID CODE PATH");
	return 0;
}


void worldman_init(const struct world_meta* const _metas,
                   const int cnt)
{
	LOG_DEBUG("INITIALIZING WORLDMAN");
	assert(cnt <= GPROJ_MAX_WORLDS && metas != NULL);
	memcpy(metas, _metas, cnt * sizeof(struct world_meta));
	nworlds = cnt;
}

void worldman_term(void)
{
	LOG_DEBUG("TERMINATING WORLDMAN");
}

void worldman_load_world(const char* const name)
{
	assert(name != loaded_world_name);
	const int idx = search_meta(name);
	const struct world_meta* const meta = &metas[idx];

	map_init(&meta->map_meta, &world.map);
	render_load_ts(meta->map_meta.ts_path);

	loaded_world_name = name;
	loaded_world_idx = idx;
}

void worldman_update_world(const timer_clk_t now, const float dt)
{
	assert(loaded_world_name != NULL);

#ifdef GPROJ_DEBUG
	extern struct events gproj_events;

	const input_button_t buttons = gproj_events.input.buttons;

	if (buttons&INPUT_BUTTON_CAM_RIGHT)
		world.cam.x += WORLD_DEBUG_CAM_VELOCITY * dt;
	else if (buttons&INPUT_BUTTON_CAM_LEFT)
		world.cam.x -= WORLD_DEBUG_CAM_VELOCITY * dt;
	else if (buttons&INPUT_BUTTON_CAM_UP)
		world.cam.y -= WORLD_DEBUG_CAM_VELOCITY * dt;
	else if (buttons&INPUT_BUTTON_CAM_DOWN)
		world.cam.y += WORLD_DEBUG_CAM_VELOCITY * dt;

	if (gproj_events.flags&EVENT_FLAG_NEW_INPUT) {


		if (buttons&INPUT_BUTTON_WORLD_FWD) {
			const int idx = (loaded_world_idx + 1) % nworlds;
			worldman_load_world(metas[idx].name);
		} else if (buttons&INPUT_BUTTON_WORLD_SCALE_DOWN &&
		           world.map.scale > MAP_MIN_SCALE) {
			world.map.scale -= MAP_SCALE_MOD;
		} else if (buttons&INPUT_BUTTON_WORLD_SCALE_UP &&
		           world.map.scale < MAP_MAX_SCALE) {
			world.map.scale += MAP_SCALE_MOD;
		}
		if (world.map.scale < MAP_MIN_SCALE)
			world.map.scale = MAP_MIN_SCALE;
		else if (world.map.scale > MAP_MAX_SCALE)
			world.map.scale = MAP_MAX_SCALE;
	}
#endif

	map_view_update(&world.cam, &world.map, &world.map_view);

#ifdef GPROJ_DEBUG
	render_text("WORLD: %s", loaded_world_name);
	render_text("WORLD CAM => (%.2f, %.2f)", world.cam.x, world.cam.y);
	render_text("MAP VIEW SCALE => %.2f", world.map_view.scale);
	render_text("MAP VIEW SIZE (%d, %d)",
	            world.map_view.size.x, world.map_view.size.y);
#endif

}

void worldman_send_render(void)
{
	render_map(&world.map_view);
}

