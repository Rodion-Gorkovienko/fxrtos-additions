#ifndef _EX_PROCESS_V1_HEADER_
#define _EX_PROCESS_V1_HEADER_

/*
 Developed by Rodion Gorkovenko
 9306 gr.
*/

#include FX_INTERFACE(LANG_TYPES)
#include FX_INTERFACE(RTL_LIST)
#include FX_INTERFACE(FX_PROCESS)
#include FX_INTERFACE(EX_THREAD)
#include FX_INTERFACE(HASH_TABLE)
#include FX_INTERFACE(EX_OBJECT_MANAGER)
#include FX_INTERFACE(EX_MEM_MANAGER)

typedef struct _ex_process_t {
	int header; /* Object header */
	fx_process_t* process; /* Address space */
	HT(const char*, int) handle_db; /* Table of heagers of used objects */
	rtl_list_t threads; /* Threads list */
	volatile unsigned int thread_num; /* Number of threads */
	rtl_list_linkage_t link;
} ex_process_t;

void ex_process_ctor(void);
int ex_process_init(ex_process_t* p, fx_process_prio_t proc_prio);
ex_process_t* ex_process_self(void);
int ex_process_start(
	ex_process_t* p,
	void(*f)(void*),
	void* arg,
	unsigned int thread_prio, 
	void* stk, 
	size_t sz);
#define ex_process_add_thread(f, arg, thread_prio, stk, sz) \
	ex_process_start(ex_process_self(), f, arg, thread_prio, stk, sz)
void ex_process_kill(ex_process_t* p);

FX_METADATA(({ interface: [EX_PROCESS, V1] }))

#endif

