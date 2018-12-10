#ifndef GPROJ_TYPES_H_
#define GPROJ_TYPES_H_
#include <stdint.h>
#include <stdbool.h>

#define RGBA32(r, g, b, a) ((r<<24ul)|(g<<16ul)|(b<<8ul)|a)


struct vec2i {
	int x, y;
};

struct color {
	union {
		struct {
			uint8_t r, g, b, a;
		};
		uint32_t raw;
	};
};

struct sprite {
	struct vec2i pos;
	struct vec2i size;
	struct color color;
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
