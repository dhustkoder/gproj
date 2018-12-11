#ifndef GPROJ_RENDER_H_
#define GPROJ_RENDER_H_
#include <stdbool.h>
#include <stdint.h>
#include "types.h"

#define GPROJ_FB_WIDTH   (64 * 16)
#define GPROJ_FB_HEIGHT  (32 * 16)


#define RENDER_CLEAR_BKG  (0x01)
#define RENDER_CLEAR_SPRS (0x02)


extern void render_clear(uint8_t flags);
extern void render_bkg(const uint32_t* pixels);
extern void render_sprs(const struct sprite* sprs, int count);
extern void render_present(void);


#endif
