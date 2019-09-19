#include <assert.h>
#include <SDL2/SDL_mixer.h>
#include "log.h"
#include "audio.h"

extern int sfxs_cnt;
extern int bgms_cnt;
extern Mix_Chunk* sfxs[MAX_SFXS];
extern Mix_Music* bgms[MAX_BGMS];


int audio_load_bgm(const char* const path)
{
	bgms[bgms_cnt] = Mix_LoadMUS(path);
	return bgms_cnt++;
}

int audio_load_sfx(const char* const path)
{
	((void)path);
	assert(path && false && "NEED IMPLEMENTATION");
	return 0;
}

void audio_play_bgm(const int id)
{
	assert(id < bgms_cnt);
	if (Mix_PlayMusic(bgms[id], -1) != 0) {
		LOG_ERR("Couldn't play music \'%d\': %s\n",
		        id, SDL_GetError());
	}
}

void audio_play_sfx(const int id)
{
	((void)id);
	assert(id && false && "NEED IMPLEMENTATION");
}




