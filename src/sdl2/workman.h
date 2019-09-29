#ifndef GPROJ_WORKMAN_H_
#define GPROJ_WORKMAN_H_


typedef void(*work_fn_ptr)(void*);


extern void workman_init(void);
extern void workman_term(void);
extern void workman_push_work(work_fn_ptr fn, void* arg);
extern void workman_work_until_empty(void);
extern void workman_work_until_term(void);






#endif