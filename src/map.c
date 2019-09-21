#include <assert.h>
#include <tmx.h>
#include "log.h"
#include "types.h"
#include "render.h"
#include "map.h"


struct vec2i map_ts_size;
struct vec2i map_tile_size;
struct vec2i map_map_size;
struct vec2f map_scrl_pos = { 0, 0 };
static struct vec2f map_vel = { 0, 0 };

static int32_t* map_layers = NULL;
static int map_layer_cnt = 0;
static tmx_map* map = NULL;

void map_load(const char* const path)
{
	LOG_DEBUG("LOADING MAP %s", path);

	if (map != NULL)
		map_free();

	map = tmx_load(path);

	assert(map != NULL && map->ts_head != NULL
	       && map->ts_head->tileset != NULL
		   && map->ts_head->tileset->image != NULL
		   && map->ts_head->tileset->image->source != NULL);

	map_map_size.x = map->width;
	map_map_size.y = map->height;

	map_tile_size.x = map->ts_head->tileset->tile_width;
	map_tile_size.y = map->ts_head->tileset->tile_height;

	map_ts_size.x = map->ts_head->tileset->image->width;
	map_ts_size.y = map->ts_head->tileset->image->height;

	map_layers = malloc(sizeof(*map_layers) * map_map_size.x * map_map_size.y * MAP_NLAYERS);

	assert(map_layers != NULL);

	render_load_ts(map->ts_head->tileset->image->source);


	LOG_DEBUG("MAP WIDTH: %d", map_map_size.x);
	LOG_DEBUG("MAP HEIGHT: %d", map_map_size.y);
	LOG_DEBUG("MAP TILE WIDTH: %d", map_tile_size.x);
	LOG_DEBUG("MAP TILE HEIGHT: %d", map_tile_size.y);
	LOG_DEBUG("MAP TILESHEET WIDTH: %d", map_ts_size.x);
	LOG_DEBUG("MAP TILESHEET HEIGHT : %d", map_ts_size.y);



	const tmx_layer* layp = map->ly_head;
	map_layer_cnt = 0;

	while (layp != NULL && map_layer_cnt < MAP_NLAYERS) {
		for (int y = 0; y < map_map_size.y; ++y) {
			for (int x = 0; x < map_map_size.x; ++x) {
				const int32_t gidfull = layp->content.gids[y * map_map_size.x + x];
				int32_t* const layer =
					&map_layers[map_layer_cnt * map_map_size.x * map_map_size.y];

				layer[(y * map_map_size.x) + x] = gidfull;

			}
		}
		++map_layer_cnt;
		layp = layp->next;
	}

	LOG_DEBUG("MAP LAYER CNT: %d", map_layer_cnt);

	render_map(map_layers);
}

void map_free(void)
{
	free(map_layers);
	tmx_map_free(map);
}

void map_scrl_vel_set(const float velx, const float vely)
{
	map_vel.x = velx;
	map_vel.y = vely;
}

void map_update(const uint32_t now, const float dt)
{
	((void)now);

	map_scrl_pos.x += map_vel.x * dt;
	map_scrl_pos.y += map_vel.y * dt;

	if (map_scrl_pos.x < 0)
		map_scrl_pos.x = 0;
	else if (map_scrl_pos.x > ((map_map_size.x * map_tile_size.x) - GPROJ_SCR_WIDTH))
		map_scrl_pos.x = ((map_map_size.x * map_tile_size.x) - GPROJ_SCR_WIDTH);

	if (map_scrl_pos.y < 0)
		map_scrl_pos.y = 0;
	else if (map_scrl_pos.y > ((map_map_size.y * map_tile_size.y) - GPROJ_SCR_HEIGHT))
		map_scrl_pos.y = ((map_map_size.y * map_tile_size.y) - GPROJ_SCR_HEIGHT);


}

