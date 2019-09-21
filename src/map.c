#include <assert.h>
#include <tmx.h>
#include "log.h"
#include "types.h"
#include "render.h"
#include "map.h"


struct animated_tile {
	const tmx_anim_frame* frames;
	int32_t* gid_ptr;
	uint32_t clk;
	unsigned int idx;
	int cnt;
};


static uint8_t map_blk[GPROJ_Y_TILES][GPROJ_X_TILES];
static int32_t map_layers[MAP_NLAYERS][GPROJ_Y_TILES][GPROJ_X_TILES] = { 0 };
static int map_layer_cnt = 0;
static struct animated_tile animated_tiles[32];
static int animated_tiles_cnt = 0;
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

	render_set_ts(map->ts_head->tileset->image->source);

	const tmx_layer* layp = map->ly_head;
	map_layer_cnt = 0;
	animated_tiles_cnt = 0;

	while (layp != NULL && map_layer_cnt < MAP_NLAYERS) {
		for (int y = 0; y < GPROJ_Y_TILES; ++y) {
			for (int x = 0; x < GPROJ_X_TILES; ++x) {
				const int32_t gidfull = layp->content.gids[y * GPROJ_X_TILES + x];
				const int32_t gid = gidfull & TMX_FLIP_BITS_REMOVAL;
				map_layers[map_layer_cnt][y][x] = gidfull;
				if (map->tiles[gid] != NULL) {
					if (map->tiles[gid]->animation != NULL) {
						animated_tiles[animated_tiles_cnt++] = (struct animated_tile) {
							.frames = map->tiles[gid]->animation,
							.cnt = map->tiles[gid]->animation_len,
							.gid_ptr = &map_layers[map_layer_cnt][y][x],
							.idx = 0,
							.clk = 0
						};
					}
					if (map->tiles[gid]->properties != NULL) {
						map_blk[y][x] = 1;
					}
				}

			}
		}
		++map_layer_cnt;
		layp = layp->next;
	}

	LOG_DEBUG("MAP ANIMATED TILES CNT: %d", animated_tiles_cnt);
	LOG_DEBUG("MAP LAYER CNT: %d", map_layer_cnt);

	render_tile_layers((int32_t*)map_layers);
}

void map_free(void)
{
	tmx_map_free(map);
}

void map_update(const uint32_t now)
{
	static int32_t* ids_to_update[64];
	int update_len = 0;

	for (int i = 0; i < animated_tiles_cnt; ++i) {
		struct animated_tile* const at = &animated_tiles[i];
		if ((now - at->clk) >= at->frames[at->idx].duration) {
			at->clk = now;
			at->idx = (at->idx + 1) % at->cnt;
			*at->gid_ptr = (*at->gid_ptr&(~TMX_FLIP_BITS_REMOVAL)) | (at->frames[at->idx].tile_id + 1);
			const uintptr_t layer_idx = (at->gid_ptr - &map_layers[0][0][0]) / (GPROJ_X_TILES * GPROJ_Y_TILES);
			if (layer_idx == MAP_LAYER_BG2 || layer_idx == MAP_LAYER_FG2) {
				ids_to_update[update_len++] = (at->gid_ptr - (GPROJ_X_TILES * GPROJ_Y_TILES));
				ids_to_update[update_len++] = at->gid_ptr;
			} else {
				ids_to_update[update_len++] = at->gid_ptr;
				ids_to_update[update_len++] = (at->gid_ptr + (GPROJ_X_TILES * GPROJ_Y_TILES));
			}
		}
	}

	if (update_len > 0) {
		render_update_tile_layers((int32_t*)map_layers,
		                          (const int32_t**)ids_to_update,
		                          update_len);
	}

}

