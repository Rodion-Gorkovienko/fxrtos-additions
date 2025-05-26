#ifndef _EX_THREAD_COMMON_HEADER_
#define _EX_THREAD_COMMON_HEADER_

/*
 Developed by Rodion Gorkovenko
 9306 gr.
*/

#include FX_INTERFACE(FX_THREAD)
#include FX_INTERFACE(RTL_LIST)
#include FX_INTERFACE(EX_OBJECT_MANAGER)
#include FX_INTERFACE(EX_MEM_MANAGER)


#define ex_thread_init(a, b, c, d, e, f, g) \
    ex_thread_init_ex(fx_process_self(), a, b, c, d, e, f, g)

typedef struct _ex_thread_t {
	ex_object_header_t header; /* Object header */
	fx_thread_t* kthread; /* Kernel thread */
	int* kstack; /* Stack of the thread */
    size_t stack_size;
    rtl_list_linkage_t link;
    rtl_list_linkage_t global_link;
}
ex_thread_t;

void ex_thread_ctor();

int ex_thread_init_ex(
    fx_process_t* parent, ex_thread_t* thread,
    void (*func)(void*), void* arg,
    unsigned int priority,
    void* stack, size_t stack_sz,
    bool create_suspended);

int ex_thread_deinit(ex_thread_t* thread);

#define ex_thread_terminate(thread) fx_thread_terminate(thread->kthread)
#define ex_thread_exit() fx_thread_exit()
#define ex_thread_join(thread) fx_thread_join(thread->kthread)
#define ex_thread_suspend() fx_thread_suspend()
#define ex_thread_resume(thread) fx_thread_resume(thread->kthread)
#define ex_thread_sleep(ticks) fx_thread_sleep(ticks)
#define ex_thread_delay_until(prev_wake, increment) fx_thread_delay_until(prev_wake, increment)
ex_thread_t* ex_thread_self(void); 
#define ex_thread_yield() fx_thread_yield()
#define ex_thread_get_params(thread, t, v) fx_thread_get_params(thread->kthread, t, v)
#define ex_thread_set_params(thread, t, v) fx_thread_set_params(thread->kthread, t, v)
#define ex_thread_wait_event(evt, cancel_event) fx_thread_wait_event(evt, cancel_event)
#define ex_thread_timedwait_event(evt, timeout) fx_thread_timedwait_event(evt, timeout)


FX_METADATA(({ interface: [EX_THREAD, COMMON] }))

#endif

