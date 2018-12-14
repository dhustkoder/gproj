#include <tmx.h>
#include "log.h"
#include "types.h"
#include "render.h"
#include "map.h"

// 2 layers
static int32_t map_layers[GPROJ_MAP_NLAYERS][GPROJ_Y_TILES][GPROJ_X_TILES] = { 0 };
static int map_layer_cnt = 0;
static struct animated_tile animated_tiles[16];
static int animated_tiles_cnt = 0;
static tmx_map* map = NULL;


void map_load(const char* path)
{
	if (map != NULL)
		tmx_map_free(map);

	map = tmx_load(path);

	if (map == NULL) {
		LOG_ERR("Couldn't load map: %s", tmx_strerr());
		return;
	}

	const tmx_layer* layp = map->ly_head;
	map_layer_cnt = 0;
	animated_tiles_cnt = 0;

	while (layp != NULL && map_layer_cnt < GPROJ_MAP_NLAYERS) {
		for (int y = 0; y < GPROJ_Y_TILES; ++y) {
			for (int x = 0; x < GPROJ_X_TILES; ++x) {
				const int32_t gidfull = layp->content.gids[y * 32 + x];
				const int32_t gid = gidfull & TMX_FLIP_BITS_REMOVAL;
				map_layers[map_layer_cnt][y][x] = gidfull;
				if (map->tiles != NULL && map->tiles[gid] != NULL && map->tiles[gid]->animation != NULL) {
					animated_tiles[animated_tiles_cnt++] = (struct animated_tile) {
						.tmx_tile = map->tiles[gid],
						.gid_ptr = &map_layers[map_layer_cnt][y][x],
						.current_frame_idx = 0,
						.frame_clk = 0
					};
				}
			}
		}
		++map_layer_cnt;
		layp = layp->next;
	}

	render_tile_layers((int32_t*)map_layers);
}


void map_update(void)
{
	int update_len = 0;
	int32_t* ids_to_update[32];

	uint32_t clk = timer_now();
	for (int i = 0; i < animated_tiles_cnt; ++i) {
		struct animated_tile* const at = &animated_tiles[i];
		if ((clk - at->frame_clk) >= at->tmx_tile->animation[at->current_frame_idx].duration) {
			at->frame_clk = clk;
			at->current_frame_idx = (at->current_frame_idx + 1) % at->tmx_tile->animation_len;
			*at->gid_ptr = (*at->gid_ptr&(~TMX_FLIP_BITS_REMOVAL)) | (at->tmx_tile->animation[at->current_frame_idx].tile_id + 1);
			const uintptr_t layer_idx = (at->gid_ptr - &map_layers[0][0][0]) / (GPROJ_X_TILES * GPROJ_Y_TILES);
			if (layer_idx == GPROJ_MAP_LAYER_BG2 || layer_idx == GPROJ_MAP_LAYER_FG2) {
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

