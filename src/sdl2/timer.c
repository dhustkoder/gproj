#include <assert.h>
#include "render.h"
#include "timer.h"

#ifdef GPROJ_PROFILING

typedef double timer_hp_clk_t;
timer_hp_clk_t timer_hp_frequency;

#define GPROJ_MAX_PROFILING_IDS 512

#define TIMER_HP_CLK_FMT "%.12lf"

#define timer_high_precision_counter()   ((timer_hp_clk_t)SDL_GetPerformanceCounter())
#define timer_high_precision_frequency() (timer_hp_frequency)

#define timer_profiler_start() timer_high_precision_counter()

#define timer_profiler_end(start) \
	((timer_high_precision_counter() - start) / timer_high_precision_frequency())


struct block {
	int max_hits;
	int hits;
	timer_hp_clk_t counter;
	timer_hp_clk_t adder;
	timer_hp_clk_t result;
};

static const char* ids[GPROJ_MAX_PROFILING_IDS];
static int ids_cnt;
static int stack[GPROJ_MAX_PROFILING_IDS];
static int stack_cnt;
static struct block blocks[GPROJ_MAX_PROFILING_IDS];


void timer_profiler_init()
{
	const int err = SDL_InitSubSystem(SDL_INIT_TIMER);
	((void)err);
	assert(err == 0);
	timer_hp_frequency = SDL_GetPerformanceFrequency();
}

void timer_profiler_block_start(const char* const id,
                                const int max_hits)
{
	struct block* blk = NULL;
	int idx = -1;
	
	for (int i = 0; i < ids_cnt; ++i) {
		if (ids[i] == id) {
			idx = i;
			blk = &blocks[idx];
			break;
		}
	}

	if (idx == -1) {
		idx = ids_cnt++;
		ids[idx] = id;
		blk = &blocks[idx];
		blk->max_hits = max_hits;
		blk->hits = 0;
		blk->result = 0;
		blk->adder = 0;
		assert(ids_cnt <= GPROJ_MAX_PROFILING_IDS);
	}

	stack[stack_cnt++] = idx;
	blk->counter = timer_profiler_start();
}

void timer_profiler_block_end()
{
	const int idx = stack[--stack_cnt];
	struct block* blk = &blocks[idx];
	
	blk->adder += timer_profiler_end(blk->counter);
	blk->hits++;

	if (blk->hits == blk->max_hits) {
		blk->result = blk->adder / blk->hits;
		blk->adder = 0;
		blk->hits = 0;
	}

	const char* const id = ids[idx];
	render_text("[PROFILER] %s: " TIMER_HP_CLK_FMT, id, blk->result);
}




#endif