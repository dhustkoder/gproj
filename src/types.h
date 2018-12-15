#ifndef GPROJ_TYPES_H_
#define GPROJ_TYPES_H_
#include <stdint.h>
#include <stdbool.h>

#define RGBA32(r, g, b, a) ((r<<24ul)|(g<<16ul)|(b<<8ul)|a)
#define ARRSZ(array) (sizeof(array)/sizeof(array[0]))

struct vec2i {
	int x, y;
};

struct vec2f {
	float x, y;
};

struct recti {
	struct vec2i pos;
	struct vec2i size;
};

struct rectf {
	struct vec2f pos;
	struct vec2f size;
};


enum input_button {
	INPUT_BUTTON_UP       = 0x01,
	INPUT_BUTTON_DOWN     = 0x02,
	INPUT_BUTTON_LEFT     = 0x04,
	INPUT_BUTTON_RIGHT    = 0x08,
	INPUT_BUTTON_RUN      = 0x10,
	INPUT_BUTTON_SHOOT    = 0x20,
	INPUT_BUTTON_NBUTTONS = 6
};

typedef uint8_t input_button_t;



#endif
