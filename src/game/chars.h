#ifndef GPROJ_CHARS_H_
#define GPROJ_CHARS_H_
#include "platform_defs.h"


extern void chars_init(void);
extern void chars_term(void);
extern void chars_update(timer_clk_t now, float dt);
extern void chars_send_render(void);


#endif

