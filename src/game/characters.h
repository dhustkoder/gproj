#ifndef GPROJ_PLAYER_H_
#define GPROJ_PLAYER_H_
#include "types.h"

extern void characters_init(void);
extern void characters_update(timer_clk_t now, float dt);
extern void characters_send_render(void);

#endif
