#include FX_INTERFACE(FX_PROCESS)

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
	return FX_STATUS_OK;
}
