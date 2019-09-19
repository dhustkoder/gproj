#include "audio.h"
#include "map.h"
#include "actors.h"
#include "characters.h"
#include "game.h"



void game_init()
{
	characters_init();
	map_load("../assets/map01.tmx");
	audio_play_bgm(audio_load_bgm("../assets/bloodlines.ogg"));
}

void game_step(uint32_t now, float dt)
{
	map_update(now);
	characters_update(now, dt);
	actors_update(now, dt);
}

void game_term()
{
	map_free();
}
