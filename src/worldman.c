#include <assert.h>
#include <string.h>
#include "render.h"
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
	assert(cnt <= GPROJ_MAX_WORLDS && metas != NULL);
	memcpy(metas, _metas, cnt * sizeof(struct world_meta)); 
	nworlds = cnt;
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
		world.cam.x += 64.f * dt;
	else if (buttons&INPUT_BUTTON_CAM_LEFT)
		world.cam.x -= 64.f * dt;
	else if (buttons&INPUT_BUTTON_CAM_UP)
		world.cam.y -= 64.f * dt;
	else if (buttons&INPUT_BUTTON_CAM_DOWN)
		world.cam.y += 64.f * dt;

	if (gproj_events.input.new_state) {
		if (buttons&INPUT_BUTTON_WORLD_FWD) {
			const int idx = (loaded_world_idx + 1) % nworlds;
			worldman_load_world(metas[idx].name);
		} else if (buttons&INPUT_BUTTON_WORLD_SCALE_DOWN &&
		           world.map.scale >= 0.20) {
			world.map.scale -= 0.1;
		} else if (buttons&INPUT_BUTTON_WORLD_SCALE_UP &&
		           world.map.scale < 10.0) {
			world.map.scale += 0.1;
		}
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

void worldman_term(void)
{

}



