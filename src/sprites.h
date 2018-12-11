#ifndef GPROJ_SPRITES_H_
#define GPROJ_SPRITES_H_
#include "types.h"


extern void sprite_register_static(int id, const struct vec2i* ssinfo);
extern void sprite_register_animated(int id, const struct vec2i* ssinfos, int n);


#endif
