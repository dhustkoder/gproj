#ifndef GPROJ_GMATH_H_
#define GPROJ_GMATH_H_

#include "platform_defs.h"


struct vec2i {
	s16 x, y;
};

struct vec2f {
	f32 x, y;
};

struct recti {
	struct vec2i pos;
	struct vec2i size;
};

struct rectf {
	struct vec2f pos;
	struct vec2f size;
};




#endif

