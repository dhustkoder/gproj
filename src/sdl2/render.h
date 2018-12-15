#ifndef GPROJ_RENDER_H_
#define GPROJ_RENDER_H_
#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include "types.h"
#include "actors.h"


#define GPROJ_X_TILES        (24)
#define GPROJ_Y_TILES        (16)

#define GPROJ_TILE_WIDTH     (32)
#define GPROJ_TILE_HEIGHT    (32)

#define GPROJ_FB_WIDTH       (GPROJ_TILE_WIDTH * GPROJ_X_TILES)
#define GPROJ_FB_HEIGHT      (GPROJ_TILE_HEIGHT * GPROJ_Y_TILES)

#define GPROJ_TILESET_WIDTH  (2048)
#define GPROJ_TILESET_HEIGHT (5088)

#define RENDER_CLEAR_BKG     (0x01)
#define RENDER_CLEAR_FG      (0x02)
#define RENDER_CLEAR_ACTORS  (0x04)


extern void render_clear(uint8_t flags);

extern void render_tile_layers(const int32_t* gids);

void render_update_tile_layers(const int32_t* gids,
                               const int32_t** gids_to_update,
                               int update_len);

extern void render_actors(const struct actor* const* actors, int count);
extern void render_present(void);


#endif
