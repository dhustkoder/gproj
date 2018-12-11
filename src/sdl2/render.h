#ifndef GPROJ_RENDER_H_
#define GPROJ_RENDER_H_
#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include "types.h"

#define GPROJ_FB_WIDTH   (64 * 16)
#define GPROJ_FB_HEIGHT  (32 * 16)


#define RENDER_CLEAR_BKG  (0x01)
#define RENDER_CLEAR_SPRS (0x02)

typedef SDL_Texture sprite_sheet_t;

extern sprite_sheet_t* render_load_spr_sheet(const char* name);
extern void render_free_spr_sheet(sprite_sheet_t* ss);
extern void render_set_spr_sheet(sprite_sheet_t* ss);

extern void render_clear(uint8_t flags);
extern void render_bkg(const uint32_t* pixels);
extern void render_sprs(const struct sprite* sprs, int count);
extern void render_present(void);


#endif
