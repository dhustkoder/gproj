#include "logger.h"
#include "render.h"
#include "timer.h"

#ifdef GPROJ_PROFILING

typedef double timer_hp_clk_t;
timer_hp_clk_t timer_hp_frequency;

#define GPROJ_MAX_PROFILING_IDS 512

#define TIMER_HP_CLK_FMT ".12lf"

#define timer_high_precision_counter()   ((timer_hp_clk_t)0)

#define timer_profiler_start() timer_high_precision_counter()
#define timer_profiler_end(start) \
	((timer_high_precision_counter() - start) / timer_hp_frequency)


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


void timer_profiler_init(void)
{
	LOG_DEBUG("HP FREQUENCY: %lf", timer_hp_frequency);
}

void timer_profiler_term(void)
{

}

void timer_profiler_block_start(const char* const id,
                                const int max_hits)
{
	struct block* blk = NULL;
	struct block* prevblk = NULL;
	timer_hp_clk_t waste_start;

	if (stack_cnt > 0) {
		waste_start = timer_profiler_start();
		prevblk = &blocks[stack[stack_cnt - 1]];
	}

	int idx;
	for (idx = 0; idx < ids_cnt; ++idx) {
		if (ids[idx] == id) {
			blk = &blocks[idx];
			break;
		}
	}

	if (blk == NULL) {
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

	if (prevblk != NULL) {
		const timer_hp_clk_t wasted = timer_profiler_end(waste_start);
		prevblk->adder -= wasted;
	}

	blk->counter = timer_profiler_start();
}

void timer_profiler_block_end(void)
{
	assert(stack_cnt > 0);
	timer_hp_clk_t waste_start;
	struct block* prevblk = NULL;


	const int idx = stack[--stack_cnt];
	struct block* const blk = &blocks[idx];
	blk->adder += timer_profiler_end(blk->counter);

	if (stack_cnt > 0) {
		waste_start = timer_profiler_start();
		prevblk = &blocks[stack[stack_cnt - 1]];
	}

	blk->hits++;

	if (blk->hits == blk->max_hits) {
		blk->result = (blk->adder / blk->hits);
		blk->adder = 0;
		blk->hits = 0;
	}

	const char* const id = ids[idx];
	render_text("[PROFILER] %s: %" TIMER_HP_CLK_FMT " SECONDS", id, blk->result);

	if (prevblk != NULL) {
		const timer_hp_clk_t wasted = timer_profiler_end(waste_start);
		prevblk->adder -= wasted;
	}
}




#endif

