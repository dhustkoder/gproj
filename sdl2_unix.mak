SHELL := /bin/bash

#
#
#

GPROJ_DEFINES=
ifeq ($(OS),Linux)
	GPROJ_DEFINES+=-DGPROJ_OS_LINUX
else
	GPROJ_DEFINES+=-DGPROJ_OS_OSX
endif
PLATFORM_LSTR=sdl2
PLATFORM_USTR=SDL2

PLATFORM_CFLAGS=$(shell sdl2-config --cflags) -DPLATFORM_SDL2
PLATFORM_LDFLAGS=$(shell sdl2-config --libs) -lSDL2_image -lSDL2_mixer -lSDL2_ttf

EXTERNALS_DIR=./externals
SDLFC_DIR=$(EXTERNALS_DIR)/SDL_FontCache

BUILD_DIR=./build
OBJS_DIR=./objs
ASM_DIR=./asm
SRC_DIR=./src

GAME_SRC_DIR=$(SRC_DIR)/game
GAME_OBJS_DIR=$(OBJS_DIR)/game
GAME_ASM_DIR=$(ASM_DIR)/game

OGL_SRC_DIR=$(SRC_DIR)/ogl
OGL_OBJS_DIR=$(OBJS_DIR)/ogl
OGL_ASM_DIR=$(ASM_DIR)/ogl

PLATFORM_OBJS_DIR=$(OBJS_DIR)/$(PLATFORM_LSTR)
PLATFORM_SRC_DIR=$(SRC_DIR)/$(PLATFORM_LSTR)
PLATFORM_ASM_DIR=$(ASM_DIR)/$(PLATFORM_LSTR)

SRC=$(SRC_DIR)/%.c
GAME_SRC=$(GAME_SRC_DIR)/%.c
PLATFORM_SRC=$(PLATFORM_SRC_DIR)/%.c


OBJS=$(patsubst $(SRC_DIR)/%.c, $(OBJS_DIR)/%.o, $(wildcard $(SRC_DIR)/*.c))
ASM=$(patsubst $(SRC_DIR)/%.c, $(ASM_DIR)/%.asm, $(wildcard $(SRC_DIR)/*.c))

GAME_OBJS=$(patsubst $(GAME_SRC_DIR)/%.c, $(GAME_OBJS_DIR)/%.o, $(wildcard $(GAME_SRC_DIR)/*.c))
GAME_ASM=$(patsubst $(GAME_SRC_DIR)/%.c, $(GAME_ASM_DIR)/%.asm, $(wildcard $(GAME_SRC_DIR)/*.c))

OGL_OBJS=$(patsubst $(OGL_SRC_DIR)/%.c, $(OGL_OBJS_DIR)/%.o, $(wildcard $(OGL_SRC_DIR)/*.c))
OGL_ASM=$(patsubst $(OGL_SRC_DIR)/%.c, $(OGL_ASM_DIR)/%.asm, $(wildcard $(OGL_SRC_DIR)/*.c))

PLATFORM_OBJS=$(patsubst $(PLATFORM_SRC_DIR)/%.c, $(PLATFORM_OBJS_DIR)/%.o, $(wildcard $(PLATFORM_SRC_DIR)/*.c))
PLATFORM_ASM=$(patsubst $(PLATFORM_SRC_DIR)/%.c, $(PLATFORM_ASM_DIR)/%.asm, $(wildcard $(PLATFORM_SRC_DIR)/*.c))

CFLAGS=-std=c99 -Wpedantic -Wall -Wextra -Wshadow\
       -I $(SRC_DIR) -I $(PLATFORM_SRC_DIR) -I $(GAME_SRC_DIR)\
	   -I $(OGL_SRC_DIR) \
       $(PLATFORM_CFLAGS) -I$(LIBTMX_DIR)/src -I$(SDLFC_DIR)\
       $(GPROJ_DEFINES)

PERFORMANCE_FLAGS=-O3 -march=native -ffast-math -ffunction-sections -fstrict-aliasing \
				  -fdata-sections -fno-unwind-tables -fno-asynchronous-unwind-tables


CFLAGS_DEBUG=-ggdb -O0 -fsanitize=address -DDEBUG -DGPROJ_DEBUG -DGPROJ_PROFILING

CFLAGS_RELEASE=-Werror $(PERFORMANCE_FLAGS) -DNDEBUG

CFLAGS_PROFILING=$(PERFORMANCE_FLAGS) -DGPROJ_PROFILING -DNDEBUG

EXTERNALS_LIBS=$(SDLFC_DIR)/libSDL_fontcache.a

LD_INCLUDES=-L$(SDLFC_DIR)
LDFLAGS=$(LD_INCLUDES) $(PLATFORM_LDFLAGS) -lSDL_fontcache
ifeq ($(OS),Linux)
	LDFLAGS+=-lGL
else
	LDFLAGS+=-framework OpenGL 
endif


DFLAGS_DEBUG=-g
LDFLAGS_RELEASE=
LDFLAGS_PROFILING=



ifeq ($(CC),)
	CC:=gcc
endif

ifeq ($(BUILD_TYPE),)
	BUILD_TYPE=Debug
endif

ifeq ($(BUILD_TYPE),Release)
	CFLAGS += $(CFLAGS_RELEASE)
	LDFLAGS += $(LDFLAGS_RELEASE)
else ifeq ($(BUILD_TYPE),Profiling)
	CFLAGS += $(CFLAGS_PROFILING)
	LDFLAGS += $(LDFLAGS_PROFILING)
else ifeq ($(BUILD_TYPE),Debug)
	CFLAGS += $(CFLAGS_DEBUG)
	LDFLAGS += $(LDFLAGS_DEBUG)
else
$(error "Unknown BUILD_TYPE")
endif

ifeq ($(ENABLE_LTO),ON)
	CFLAGS += -flto
endif

.PHONY: all clean asm


all: $(EXTERNALS_LIBS) $(BUILD_DIR)/gproj
asm: $(ASM) $(PLATFORM_ASM) $(GAME_ASM)


$(BUILD_DIR)/gproj: $(PLATFORM_OBJS) $(OGL_OBJS) $(OBJS) $(GAME_OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

$(OBJS_DIR)/%.o: $(SRC)
	@mkdir -p $(PLATFORM_OBJS_DIR)
	@mkdir -p $(GAME_OBJS_DIR)
	@mkdir -p $(OGL_OBJS_DIR)
	$(CC) $(CFLAGS) -MP -MD -c $< -o $@

$(ASM_DIR)/%.asm: $(SRC)
	@mkdir -p $(PLATFORM_ASM_DIR)
	@mkdir -p $(GAME_ASM_DIR)
	@mkdir -p $(OGL_ASM_DIR)
	$(CC) $(CFLAGS) -S $< -o $@


#SDL_FontCache lib
$(SDLFC_DIR)/libSDL_fontcache.a:
	@pushd $(SDLFC_DIR) && make -f unix.mak && popd

-include $(shell ls $(OBJS_DIR)/*.d 2>/dev/null)


clean:
	rm -rf $(ASM_DIR)/
	rm -rf $(OBJS_DIR)/

