SHELL := /bin/bash

#
#
#

GPROJ_SCR_WIDTH:=360
GPROJ_SCR_HEIGHT:=240

GPROJ_MAX_BGMS=1
GPROJ_MAX_SFXS=16
GPROJ_MAX_WORKMAN_QUEUE_SIZE=16


GPROJ_DEFINES=-DGPROJ_SCR_WIDTH=$(GPROJ_SCR_WIDTH)\
	-DGPROJ_SCR_HEIGHT=$(GPROJ_SCR_HEIGHT)\
	-DGPROJ_MAX_BGMS=$(GPROJ_MAX_BGMS)\
	-DGPROJ_MAX_SFXS=$(GPROJ_MAX_SFXS) \
	-DGPROJ_MAX_WORKMAN_QUEUE_SIZE=$(GPROJ_MAX_WORKMAN_QUEUE_SIZE)


PLATFORM_LSTR=sdl2
PLATFORM_USTR=SDL2

PLATFORM_CFLAGS=$(shell sdl2-config --cflags) -DPLATFORM_SDL2
PLATFORM_LDFLAGS=$(shell sdl2-config --libs) -lSDL2_image -lSDL2_mixer -lSDL2_ttf

EXTERNALS_DIR=./externals
LIBTMX_DIR=$(EXTERNALS_DIR)/tmx
SDLFC_DIR=$(EXTERNALS_DIR)/SDL_FontCache

BUILD_DIR=./build
OBJS_DIR=./objs
ASM_DIR=./asm
SRC_DIR=./src

GAME_SRC_DIR=$(SRC_DIR)/game
GAME_OBJS_DIR=$(OBJS_DIR)/game
GAME_ASM_DIR=$(ASM_DIR)/game

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

PLATFORM_OBJS=$(patsubst $(PLATFORM_SRC_DIR)/%.c, $(PLATFORM_OBJS_DIR)/%.o, $(wildcard $(PLATFORM_SRC_DIR)/*.c))
PLATFORM_ASM=$(patsubst $(PLATFORM_SRC_DIR)/%.c, $(PLATFORM_ASM_DIR)/%.asm, $(wildcard $(PLATFORM_SRC_DIR)/*.c))



CFLAGS=-std=c99 -Wall -Wextra -Wshadow\
       -I $(SRC_DIR) -I $(PLATFORM_SRC_DIR) -I $(GAME_SRC_DIR)\
       $(PLATFORM_CFLAGS) -I$(LIBTMX_DIR)/src -I$(SDLFC_DIR)\
       $(GPROJ_DEFINES)

CFLAGS_DEBUG=-g -O0 -fsanitize=address -DDEBUG -DGPROJ_DEBUG -DGPROJ_PROFILING

CFLAGS_RELEASE=-Werror -O3 -march=native -ffast-math -fstrict-aliasing \
	       -ffunction-sections -fdata-sections -fno-unwind-tables  \
	       -fno-asynchronous-unwind-tables -DNDEBUG

CFLAGS_PROFILING=-O3 -DGPROJ_PROFILING -DNDEBUG

EXTERNALS_LIBS=$(LIBTMX_DIR)/build/libtmx.a $(SDLFC_DIR)/libSDL_fontcache.a

LD_INCLUDES=-L$(LIBTMX_DIR)/build -L$(SDLFC_DIR)
LDFLAGS=$(LD_INCLUDES) $(PLATFORM_LDFLAGS) -lSDL_fontcache -ltmx -lxml2 -lz
LDFLAGS_DEBUG=-g
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


$(BUILD_DIR)/gproj: $(PLATFORM_OBJS) $(OBJS) $(GAME_OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

$(OBJS_DIR)/%.o: $(SRC)
	@mkdir -p $(PLATFORM_OBJS_DIR)
	@mkdir -p $(GAME_OBJS_DIR)
	$(CC) $(CFLAGS) -MP -MD -c $< -o $@

$(ASM_DIR)/%.asm: $(SRC)
	@mkdir -p $(PLATFORM_ASM_DIR)
	@mkdir -p $(GAME_ASM_DIR)
	$(CC) $(CFLAGS) -S $< -o $@


# LIBTMX build commands
$(LIBTMX_DIR)/build/libtmx.a:
	@mkdir -p $(LIBTMX_DIR)/build
	@pushd $(LIBTMX_DIR)/build && cmake .. && make && popd
$(SDLFC_DIR)/libSDL_fontcache.a:
	@pushd $(SDLFC_DIR) && make -f unix.mak && popd

-include $(shell ls $(OBJS_DIR)/*.d 2>/dev/null)


clean:
	rm -rf $(ASM_DIR)/
	rm -rf $(OBJS_DIR)/

