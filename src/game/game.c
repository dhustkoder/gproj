#include "audio.h"
#include "map.h"
#include "actors.h"
#include "characters.h"
#include "game.h"


//static int bgm_id;


void game_init()
{
	characters_init();
	map_load("map01.tmx");
	//bgm_id = audio_load_bgm("bloodlines.ogg");
	//audio_play_bgm(bgm_id);
}

void game_step(uint32_t now, float dt)
{
	map_update(now, dt);
	characters_update(now, dt);
}

void game_term()
{
	map_free();
}
