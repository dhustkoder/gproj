#ifndef GPROJ_WORKMAN_H_
#define GPROJ_WORKMAN_H_
#include "timer.h"



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
extern void workman_work_until_empty(void);
extern void workman_work_until_term(void);
extern void workman_push_sleep(timer_clk_t ms);




#endif