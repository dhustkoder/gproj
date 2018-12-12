#include <tmx.h>
#include "log.h"
#include "types.h"
#include "render.h"
#include "map.h"

// 2 layers
static int32_t map_gids[2][GPROJ_Y_TILES][GPROJ_X_TILES];
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
	int layer_idx = 0;

	while (layp != NULL) {
		for (int y = 0; y < GPROJ_Y_TILES; ++y) {
			memcpy(&map_gids[layer_idx][y][0], 
			       &layp->content.gids[y * 32],
			       sizeof(int32_t) * GPROJ_X_TILES);
		}
		++layer_idx;
		layp = layp->next;
	}

	render_bkg((int32_t*)map_gids);
}


