#ifndef _EX_MEM_MANAGER_V1_HEADER_
#define _EX_MEM_MANAGER_V1_HEADER_

/*
 Developed by Rodion Gorkovenko
 9306 gr.
*/

#include FX_INTERFACE(HW_MPU)
#include FX_INTERFACE(FX_PROCESS)
#include FX_INTERFACE(FX_MEM_POOL)

__attribute__((weak)) uint8_t* _shared_code_start = (uint8_t*)0x8000000;
__attribute__((weak)) size_t _shared_code_size = 14;

__attribute__((weak)) uint8_t* _all_data_start = (uint8_t*)0x20000000;
__attribute__((weak)) size_t _all_data_size = 16;

__attribute__((weak)) uint8_t* _shared_data_start = (uint8_t*)0x20000000;
__attribute__((weak)) size_t _shared_data_size = 12;

__attribute__((weak)) uint8_t* _stack_start = (uint8_t*)0x20006000;
__attribute__((weak)) size_t _stack_size = 9;

__attribute__((weak)) size_t _process_pool_size_log = 10;
__attribute__((weak)) size_t _process_pool_size = 2048;

extern fx_mem_pool_t _main_mem_pool;
extern uint8_t* _main_mem_ptr;

void* aligned_alloc(size_t alignment, size_t size);
void free(void*);

void ex_mem_manager_ctor();
void ex_memory_switch(fx_process_t* newp, fx_process_t* oldp);
void* ex_mem_alloc(size_t size);
void ex_mem_free(void* mem);

FX_METADATA(({ interface: [EX_MEM_MANAGER, V1] }))

#endif

