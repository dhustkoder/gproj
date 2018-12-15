#include "timer.h"
#include "render.h"
#include "actors.h"


static const struct actor* actors[64];
static int actors_cnt = 0;


int actors_add(const struct actor* const actor)
{
	actors[actors_cnt] = actor;
	return actors_cnt++;
}

void actors_update(void)
{
	render_actors(actors, actors_cnt);
}

