#include FX_INTERFACE(FX_PROCESS)
#include FX_INTERFACE(EX_MEM_MANAGER)

/*
 Developed by Rodion Gorkovenko
 9306 gr.
*/


FX_METADATA(({ implementation: [FX_PROCESS, EDITED] }))

static rtl_list_t fx_processes;


void fx_process_ctor() {
	rtl_list_init(&(fx_processes));
}

int fx_process_init(
	fx_process_t* process,
	fx_process_prio_t prio_type)
{
	rtl_list_insert(rtl_list_last(&(fx_processes)), &process->link);
	process->prio_type = prio_type;
	fx_mem_pool_init(&process->pool);
	process->mem_init = false;

	return FX_STATUS_OK;
}

void fx_process_switch(fx_process_t* newp, fx_process_t* oldp) {
	if (!(newp->mem_init)) {
		void* mem = (void*)aligned_alloc(_process_pool_size, _process_pool_size);
		fx_mem_pool_add_mem(&newp->pool, (uintptr_t)mem, _process_pool_size);
		newp->mpu_ptr = mem;
		ex_memory_switch(newp, oldp);
		newp->mem_init = true;
	}
	else {
		if (newp != oldp) {
			ex_memory_switch(newp, oldp);
		}
	}
}

void fx_process_term(fx_process_t* process) {
	fx_mem_pool_deinit(&process->pool);
	free(process->mpu_ptr);
	rtl_list_remove(&process->link);
}
