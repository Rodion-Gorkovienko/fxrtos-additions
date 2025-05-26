#include FX_INTERFACE(EX_CTOR)

void ex_ctor() 
{
	ex_mem_manager_ctor();
	ex_thread_ctor();
	ex_process_ctor();
	ex_object_manager_ctor();
}

FX_METADATA(({ implementation: [EX_CTOR, V1] }))