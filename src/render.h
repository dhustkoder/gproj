#ifndef GPROJ_RENDER_H_
#define GPROJ_RENDER_H_
#include <stdbool.h>
#include <stdint.h>
#include "types.h"

#define GPROJ_FB_WIDTH   (1024)
#define GPROJ_FB_HEIGHT  (720)
#define RGBA32(r, g, b, a) ((r<<24ul)|(g<<16ul)|(b<<8ul)|a)



extern void render_clear(bool clear_bkg, bool clear_sprites);
extern void render_draw_sprites(const struct sprite* spr, int count);
extern void render_present(void);


#endif
