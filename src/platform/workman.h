#ifndef GPROJ_WORKMAN_H_
#define GPROJ_WORKMAN_H_
#include "timer.h"


#define GPROJ_MAX_WORKMAN_QUEUE_SIZE (16)

#ifndef GPROJ_MAX_WORKMAN_QUEUE_SIZE
#error "NEED GPROJ_MAX_WORKMAN_QUEUE_SIZE DEFINITION"
#endif

#if GPROJ_MAX_WORKMAN_QUEUE_SIZE > INT_MAX || GPROJ_MAX_WORKMAN_QUEUE_SIZE < 16 || ((GPROJ_MAX_WORKMAN_QUEUE_SIZE % 2) != 0)
#error "GPROJ_MAX_WORKMAN_QUEUE_SIZE out of range; Must fit into an int. Must be multiple of 2"
#endif



union work_arg {
	s8 s8;
	u8 u8;
	s16 s16;
	u16 u16;
	s32 s32;
	u32 u32;
	uptr uptr;
	sptr sptr;
	timer_clk_t clk;
	void* ptr;
};

typedef void(*work_ptr_t)(union work_arg arg);


extern void workman_init(void);
extern void workman_term(void);
extern void workman_push_work(work_ptr_t fn, union work_arg arg);
extern void workman_work_until_all_finished(void);
extern void workman_work_until_term(void);
extern void workman_push_sleep(timer_clk_t ms);




#endif
