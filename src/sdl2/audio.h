#ifndef GPROJ_AUDIO_H_
#define GPROJ_AUDIO_H_
#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>

#define MAX_BGMS ((int)16)
#define MAX_SFXS ((int)16)


extern int audio_load_bgm(const char* path);
extern int audio_load_sfx(const char* path);

extern void audio_play_bgm(int id);
extern void audio_play_sfx(int id);


#endif
