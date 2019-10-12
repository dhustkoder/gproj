#ifndef GPROJ_AUDIO_H_
#define GPROJ_AUDIO_H_
#include <SDL.h>


#define GPROJ_MAX_BGMS (1)
#define GPROJ_MAX_SFXS (16)


extern void audio_init();
extern void audio_term();

extern int audio_load_bgm(const char* path);
extern int audio_load_sfx(const char* path);

extern void audio_play_bgm(int id);
extern void audio_play_sfx(int id);

extern void audio_stop_bgm(void);
extern void audio_stop_sfx(int id);

#endif
