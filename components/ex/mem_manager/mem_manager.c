#include FX_INTERFACE(EX_MEM_MANAGER)
#include FX_INTERFACE(FX_THREAD)

/*
 Developed by Rodion Gorkovenko
 9306 gr.
*/

fx_mem_pool_t _main_mem_pool;
uint8_t* _main_mem_ptr;

void ex_mem_manager_ctor() {
	hw_mpu_region_set_access(0, (uintptr_t)_shared_code_start, _shared_code_size, HW_MPU_RGN_PERM_PRV_RO_USR_RO, 0);
	hw_mpu_region_set_access(1, (uintptr_t)_all_data_start, _all_data_size, HW_MPU_RGN_PERM_PRV_RW_USR_NO, 0);
	hw_mpu_region_set_access(2, (uintptr_t)_shared_data_start, _shared_data_size, HW_MPU_RGN_PERM_PRV_RW_USR_RW, 0);
	hw_mpu_region_set_access(3, (uintptr_t)_stack_start, _stack_size, HW_MPU_RGN_PERM_PRV_RW_USR_RW, 0);
	_main_mem_ptr = (void*)aligned_alloc(_process_pool_size, _process_pool_size);
	fx_mem_pool_init(&_main_mem_pool);
	fx_mem_pool_add_mem(&_main_mem_pool, (uintptr_t)_main_mem_ptr, _process_pool_size);
	//hw_mpu_set_enable(false, false, false);
	hw_mpu_set_enable(true, true, false);
}

void ex_memory_switch(fx_process_t* newp, fx_process_t* oldp) {
	hw_mpu_region_set_access(4, (uintptr_t)newp->mpu_ptr, _process_pool_size_log, HW_MPU_RGN_PERM_PRV_RW_USR_RW, 0);
}

void* ex_mem_alloc(size_t size) {
	void* ptr;
	fx_mem_pool_alloc(&fx_process_self()->pool, size, &ptr);
	return ptr;
}

void ex_mem_free(void* mem) {
	fx_mem_pool_free(&fx_process_self()->pool, mem);
}

FX_METADATA(({ implementation: [EX_MEM_MANAGER, V1] }))