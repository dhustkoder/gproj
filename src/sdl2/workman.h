#ifndef GPROJ_WORKMAN_H_
#define GPROJ_WORKMAN_H_
#include <limits.h>
#include "timer.h"

#ifndef GPROJ_MAX_WORKMAN_QUEUE_SIZE
#error "NEED GPROJ_MAX_WORKMAN_QUEUE_SIZE DEFINITION"
#endif

#if GPROJ_MAX_WORKMAN_QUEUE_SIZE > INT_MAX || GPROJ_MAX_WORKMAN_QUEUE_SIZE < 16 || ((GPROJ_MAX_WORKMAN_QUEUE_SIZE % 2) != 0)
#error "GPROJ_MAX_WORKMAN_QUEUE_SIZE out of range; Must fit into an int. Must be multiple of 2"
#endif



union work_arg {
	int8_t i8;
	uint8_t u8;
	int16_t i16;
	uint16_t u16;
	int32_t i32;
	uint32_t u32;
	int64_t i64;
	uint64_t u64;
	void* ptr;
	timer_clk_t clk;
};

typedef void(*work_ptr_t)(union work_arg arg);


extern void workman_init(void);
extern void workman_term(void);
extern void workman_push_work(work_ptr_t fn, union work_arg arg);
extern void workman_work_until_all_finished(void);
extern void workman_work_until_term(void);
extern void workman_push_sleep(timer_clk_t ms);




#endif