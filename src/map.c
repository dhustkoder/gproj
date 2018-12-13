#include <tmx.h>
#include "log.h"
#include "types.h"
#include "render.h"
#include "map.h"

// 2 layers
static int32_t bkg_lay_gids[2][GPROJ_Y_TILES][GPROJ_X_TILES];
static struct animated_tile animated_tiles[16];
static int animated_tiles_cnt = 0;
static tmx_map* map = NULL;


void map_load(const char* path)
{
	if (map != NULL) {
		animated_tiles_cnt = 0;
		tmx_map_free(map);
	}

	map = tmx_load(path);

	if (map == NULL) {
		LOG_ERR("Couldn't load map: %s", tmx_strerr());
		return;
	}

	const tmx_layer* layp = map->ly_head;
	int layer_idx = 0;

	while (layp != NULL) {
		for (int y = 0; y < GPROJ_Y_TILES; ++y) {
			for (int x = 0; x < GPROJ_X_TILES; ++x) {
				const int32_t gidfull = layp->content.gids[y * 32 + x];
				const int32_t gid = gidfull & TMX_FLIP_BITS_REMOVAL;
				bkg_lay_gids[layer_idx][y][x] = gidfull;
				if (map->tiles != NULL && map->tiles[gid] != NULL && map->tiles[gid]->animation != NULL) {
					animated_tiles[animated_tiles_cnt++] = (struct animated_tile) {
						.tmx_tile = map->tiles[gid],
						.gid_ptr = &bkg_lay_gids[layer_idx][y][x],
						.current_frame_idx = 0,
						.frame_clk = 0
					};
				}
			}
		}
		++layer_idx;
		layp = layp->next;
	}

	render_bkg_tiles((int32_t*)bkg_lay_gids);
}


void map_update(void)
{
	int update_len = 0;
	int32_t* ids_to_update[32];

	const int32_t* const frst_layer = &bkg_lay_gids[0][0][0];
	const int32_t* const scnd_layer = &bkg_lay_gids[1][0][0];

	uint32_t clk = timer_now();
	for (int i = 0; i < animated_tiles_cnt; ++i) {
		struct animated_tile* const at = &animated_tiles[i];
		if ((clk - at->frame_clk) >= at->tmx_tile->animation[at->current_frame_idx].duration) {
			at->frame_clk = clk;
			at->current_frame_idx = (at->current_frame_idx + 1) % at->tmx_tile->animation_len;
			*at->gid_ptr = (*at->gid_ptr&(~TMX_FLIP_BITS_REMOVAL)) | (at->tmx_tile->animation[at->current_frame_idx].tile_id + 1);
			if (at->gid_ptr >= scnd_layer) {
				ids_to_update[update_len++] = at->gid_ptr - (scnd_layer - frst_layer);
				ids_to_update[update_len++] = at->gid_ptr;
			} else {
				ids_to_update[update_len++] = at->gid_ptr;
				ids_to_update[update_len++] = at->gid_ptr + (scnd_layer - frst_layer);
			}
		}
	}

	if (update_len > 0) {
		render_update_bkg_tiles((int32_t*)bkg_lay_gids,
		                        (const int32_t**)ids_to_update,
		                        update_len);
	}

}

