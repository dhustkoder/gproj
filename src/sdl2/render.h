#ifndef GPROJ_RENDER_H_
#define GPROJ_RENDER_H_
#include <stdint.h>

struct sprite {
	int x, y;
	int w, h;
	uint8_t r, g, b;
};



extern void render_begin(void);
extern void render_end(void);

extern void render_draw_sprites(const struct sprite* spr, int count);


#endif
