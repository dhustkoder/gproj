LIBS_ROOT=C:\\LIBS
SDL2_ROOT=$(LIBS_ROOT)\\SDL2-2.0.10
SDL2_TTF_ROOT=$(LIBS_ROOT)\\SDL2_ttf-2.0.15
SDL2_IMAGE_ROOT=$(LIBS_ROOT)\\SDL2_image-2.0.5
SDL2_MIXER_ROOT=$(LIBS_ROOT)\\SDL2_mixer-2.0.4

SDL2_INCLUDES= \
	/I$(SDL2_ROOT)\\include \
	/I$(SDL2_TTF_ROOT)\\include \
	/I$(SDL2_IMAGE_ROOT)\\include \
	/I$(SDL2_MIXER_ROOT)\\include
SDL2_LIBS= \
	$(SDL2_ROOT)\\lib\\x64\\SDL2.lib \
	$(SDL2_ROOT)\\lib\\x64\\SDL2main.lib \
	$(SDL2_TTF_ROOT)\\lib\\x64\\SDL2_ttf.lib \
	$(SDL2_IMAGE_ROOT)\\lib\\x64\\SDL2_image.lib \
	$(SDL2_MIXER_ROOT)\\lib\\x64\\SDL2_mixer.lib

EXTERNAL_INCLUDES=/Iexternals\\SDL_FontCache
EXTERNAL_LIBS=externals\\SDL_FontCache\\SDL_FontCache.lib

INCLUDES=/Isrc /Isrc\\sdl2 /Isrc\\game $(SDL2_INCLUDES) $(EXTERNAL_INCLUDES)
LIBS=$(SDL2_LIBS) $(EXTERNAL_LIBS)
SRC=src\\*.c src\\sdl2\\*.c src\\game\\*.c

CC=cl

CFLAGS=\
	/D_CRT_SECURE_NO_WARNINGS \
	/wd4028 \
	/wd4214 \
	/W4 \
	$(INCLUDES) \
	$(LIBS)
LDFLAGS=/SUBSYSTEM:CONSOLE /ENTRY:mainCRTStartup

CFLAGS_RELEASE=/Ox
LDFLAGS_RELEASE=

CFLAGS_DEBUG=/DGPROJ_DEBUG /DDEBUG
LDFLAGS_DEBUG=

CFLAGS+=$(CFLAGS_DEBUG)
LDFLAGS+=$(LDFLAGS_DEBUG)


all: compile clean

compile:
	if not exist "build" mkdir build
	$(CC) $(CFLAGS) $(SRC) $(LIBS) /Febuild\gproj.exe /link $(LDFLAGS)

clean:
	del *.obj
