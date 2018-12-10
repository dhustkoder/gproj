#ifndef GPROJ_RENDER_H_
#define GPROJ_RENDER_H_
#include <stdbool.h>
#include <stdint.h>
#include "types.h"

#define GPROJ_FB_WIDTH   (64 * 16)
#define GPROJ_FB_HEIGHT  (32 * 16)



extern void render_clear(bool clear_bkg, bool clear_sprites);
extern void render_draw_sprites(const struct sprite* spr, int count);
extern void render_present(void);


#endif
