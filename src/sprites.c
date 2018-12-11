#include <stdlib.h>
#include "sprites.h"


static unsigned char* sprite_pool = NULL;
static struct sprite* addrs[16];
static int registered_count = 0;

void sprite_register_static(int id, const struct vec2i* ssinfo)
{

}

void sprite_register_animated(int id, const struct vec2i* ssinfos, int n)
{

}


