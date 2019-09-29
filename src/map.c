#include <assert.h>
#include <tmx.h>
#include "logger.h"
#include "types.h"
#include "render.h"
#include "map.h"


static struct vec2i ts_size;
static struct vec2i tile_size;
static struct vec2i map_size;
static struct vec2i world_size;

static int32_t* map_layers = NULL;
static int map_layer_cnt = 0;






void map_load(const char* const path)
{
	LOG_DEBUG("LOADING MAP %s", path);

	assert(map_layers == NULL);

	tmx_map* const map = tmx_load(path);

	assert(map != NULL && map->ts_head != NULL
	       && map->ts_head->tileset != NULL
	       && map->ts_head->tileset->image != NULL
	       && map->ts_head->tileset->image->source != NULL);

	map_size.x = map->width;
	map_size.y = map->height;

	tile_size.x = map->ts_head->tileset->tile_width;
	tile_size.y = map->ts_head->tileset->tile_height;

	ts_size.x = map->ts_head->tileset->image->width;
	ts_size.y = map->ts_head->tileset->image->height;

	map_layers = calloc(map_size.x * map_size.y * MAP_NLAYERS, sizeof(*map_layers));

	assert(map_layers != NULL);

	world_size = (struct vec2i) {
		map_size.x * tile_size.x,
		map_size.y * tile_size.y
	};

	render_load_ts(map->ts_head->tileset->image->source);
	render_fb_setup(&world_size);

	LOG_DEBUG("MAP WIDTH: %d", map_size.x);
	LOG_DEBUG("MAP HEIGHT: %d", map_size.y);
	LOG_DEBUG("MAP TILE WIDTH: %d", tile_size.x);
	LOG_DEBUG("MAP TILE HEIGHT: %d", tile_size.y);
	LOG_DEBUG("MAP TILESHEET WIDTH: %d", ts_size.x);
	LOG_DEBUG("MAP TILESHEET HEIGHT : %d", ts_size.y);

	const tmx_layer* layp = map->ly_head;
	map_layer_cnt = 0;

	while (layp != NULL && map_layer_cnt < MAP_NLAYERS) {
		int32_t* const layer = &map_layers[map_layer_cnt * map_size.x * map_size.y];
		for (int y = 0; y < map_size.y; ++y) {
			for (int x = 0; x < map_size.x; ++x) {
				const int32_t gidfull = layp->content.gids[y * map_size.x + x];
				layer[y * map_size.x + x] = gidfull;
			}
		}
		++map_layer_cnt;
		layp = layp->next;
	}

	tmx_map_free(map);

	LOG_DEBUG("MAP LAYER CNT: %d", map_layer_cnt);
}

void map_free(void)
{
	free(map_layers);
	map_layers = NULL;
}

void map_update(const timer_clk_t now, const float dt)
{
	((void)now);
	((void)dt);
}


void map_send_render(void)
{
	render_map(map_layers, &map_size, &tile_size);
}
