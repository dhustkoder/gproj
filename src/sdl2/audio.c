#include <assert.h>
#include <SDL2/SDL_mixer.h>
#include "logger.h"
#include "audio.h"

static int sfxs_cnt = 0;
static int bgms_cnt = 0;
static Mix_Chunk* sfxs[MAX_SFXS];
static Mix_Music* bgms[MAX_BGMS];


void audio_init()
{

}

void audio_term()
{
	for (int i = 0; i < bgms_cnt; ++i)
		Mix_FreeMusic(bgms[i]);
	for (int i = 0; i < sfxs_cnt; ++i)
		Mix_FreeChunk(sfxs[i]);
}

int audio_load_bgm(const char* const path)
{
	assert(path != NULL);
	bgms[bgms_cnt] = Mix_LoadMUS(path);
	assert(bgms[bgms_cnt] != NULL);
	return bgms_cnt++;
}

int audio_load_sfx(const char* const path)
{	
	assert(path != NULL);
	sfxs[sfxs_cnt] = Mix_LoadWAV(path);
	assert(sfxs[sfxs_cnt] != NULL);
	return sfxs_cnt++;
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
	assert(id < sfxs_cnt);
	if (Mix_PlayChannel(id, sfxs[id], -1) != 0) {
		LOG_ERR("Couldn't play music \'%d\': %s\n",
		        id, SDL_GetError());
	}
}

void audio_stop_bgm(void)
{
	Mix_HaltMusic();
}

void audio_stop_sfx(const int id)
{
	assert(id < sfxs_cnt);
	Mix_HaltChannel(id);

}

