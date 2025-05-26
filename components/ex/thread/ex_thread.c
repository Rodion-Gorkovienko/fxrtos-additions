#include FX_INTERFACE(EX_THREAD)

/*
 Developed by Rodion Gorkovenko
 9306 gr.
*/

static rtl_list_t ex_threads;

void ex_thread_ctor() {
    rtl_list_init(&(ex_threads));
}

int ex_thread_init_ex(
    fx_process_t* parent, 
    ex_thread_t* thread, void (*func)(void*), void* arg,
    unsigned int priority,
    void* stack, size_t stack_sz,
    bool create_suspended){
    fx_thread_t* kthread;
    fx_mem_pool_alloc(&_main_mem_pool, sizeof(fx_thread_t), (void**)&kthread);
    fx_thread_init_ex(parent,
        kthread, func, arg,
        priority,
        stack, stack_sz,
        create_suspended);
    thread->kthread = kthread;
    thread->kstack = stack;
    thread->stack_size = stack_sz;
    rtl_list_insert(rtl_list_last(&(ex_threads)), &thread->global_link);
    return FX_STATUS_OK;
}

int ex_thread_deinit(ex_thread_t* thread) {
    fx_thread_deinit(thread->kthread);
    rtl_list_remove(&thread->global_link);
    return FX_STATUS_OK;
}

ex_thread_t* ex_thread_self(void) {
    fx_thread_t* ker_thread = fx_thread_self();
    rtl_list_t* head = &(ex_threads);
    rtl_list_t* n = NULL;
    ex_thread_t* t = NULL;

    for (n = rtl_list_first(head); n != head; n = rtl_list_next(n))
    {
        t = rtl_list_entry(n, ex_thread_t, global_link);
        if (t->kthread == ker_thread)
        {
            break;
        }
    }
    return t;
}

FX_METADATA(({ implementation: [EX_THREAD, COMMON] }))