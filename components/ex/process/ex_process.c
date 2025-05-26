#include FX_INTERFACE(EX_PROCESS)

/*
 Developed by Rodion Gorkovenko
 9306 gr.
*/

static rtl_list_t ex_processes;

void ex_process_ctor() {
	rtl_list_init(&(ex_processes));
}

int ex_process_init(ex_process_t* process, fx_process_prio_t proc_prio)
{
	fx_process_t* ker_proc;
	fx_mem_pool_alloc(&_main_mem_pool, sizeof(fx_process_t), (void**)&ker_proc);
	fx_process_init(ker_proc, proc_prio);
	process->process = ker_proc;
	rtl_list_insert(rtl_list_last(&(ex_processes)), &process->link);
	rtl_list_init(&(process->threads));
	//OBJECT MANAGMENT
	ex_process_object_table_init(process);
	return FX_STATUS_OK;
}

ex_process_t* ex_process_self(void) {
	fx_process_t* ker_proc = fx_process_self();
    rtl_list_t* head = &(ex_processes);
    rtl_list_t* n = NULL;
	ex_process_t* t = NULL;

    for (n = rtl_list_first(head); n != head; n = rtl_list_next(n))
    {
        t = rtl_list_entry(n, ex_process_t, link);
        if (t->process == ker_proc)
        {
            break;
        }
    }
	return t;
}

int ex_process_start(
	ex_process_t* p,
	void(*f)(void*),
	void* arg,
	unsigned int thread_prio,
	void* stk,
	size_t sz) {
	ex_thread_t* t0;
	fx_mem_pool_alloc(&_main_mem_pool, sizeof(ex_thread_t), (void**)&t0);
	int res = ex_thread_init_ex(p->process, t0, f, arg, thread_prio, stk, sz, false);
	//OBJECT MANAGMENT
	ex_object_add_thread(p, t0);
	rtl_list_insert(rtl_list_last(&(p->threads)), &t0->link);
	return res;
}

void ex_process_kill(ex_process_t* p) {
	//OBJECT MANAGMENT
	ex_process_object_table_deinit(p);
	rtl_list_t* head = &(p->threads);
	rtl_list_t* n = NULL;
	rtl_list_t* prev = NULL;

	for (n = rtl_list_first(head); n != head; n = rtl_list_next(n))
	{
		if (prev != NULL) {
			rtl_list_remove(prev);
		}
		ex_thread_t* thr = rtl_list_entry(n, ex_thread_t, link);
		ex_thread_terminate(thr);
		ex_thread_deinit(thr);
		prev = n;
	}
	fx_process_term(p->process);
	fx_mem_pool_free(&_main_mem_pool, p->process);
	rtl_list_remove(&p->link);
}

FX_METADATA(({ implementation: [EX_PROCESS, V1] }))