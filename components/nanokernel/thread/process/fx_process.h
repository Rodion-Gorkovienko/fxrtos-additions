#ifndef _FX_PROCESS_EDITED_HEADER_
#define _FX_PROCESS_EDITED_HEADER_

/** 
  ******************************************************************************
  *  @file   fx_process.h
  *  @brief  Kernel process subsystem. This module is tightly 
  *  coupled with threads impl.
  ******************************************************************************
  *  Copyright (C) JSC EREMEX, 2008-2020.
  *  Redistribution and use in source and binary forms, with or without 
  *  modification, are permitted provided that the following conditions are met:
  *  1. Redistributions of source code must retain the above copyright notice,
  *     this list of conditions and the following disclaimer.
  *  2. Redistributions in binary form must reproduce the above copyright 
  *     notice, this list of conditions and the following disclaimer in the 
  *     documentation and/or other materials provided with the distribution.
  *  3. Neither the name of the copyright holder nor the names of its 
  *     contributors may be used to endorse or promote products derived from 
  *     this software without specific prior written permission.
  *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
  *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
  *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
  *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
  *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
  *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
  *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  *  POSSIBILITY OF SUCH DAMAGE.
  *****************************************************************************/

#include FX_INTERFACE(LANG_TYPES)
#include FX_INTERFACE(RTL_LIST)
//#include FX_INTERFACE(EX_PROCESS)
#include FX_INTERFACE(FX_MEM_POOL)

#define fx_process_set_exception(id, h, old)
#define fx_process_self() (fx_thread_self()->parent)

typedef enum
{
    FX_PROCESS_PRIO_IDLE = FX_SCHED_ALG_PRIO_NUM / 2,
    FX_PROCESS_PRIO_STANDART = FX_SCHED_ALG_PRIO_NUM / 4,
    FX_PROCESS_PRIO_RT = 0,
}
fx_process_prio_t;

typedef struct {
    fx_process_prio_t prio_type;
    fx_mem_pool_t pool;           // Dynamic memory
    void* mpu_ptr;                  // A pointer to the mpu region corresponding 
                                    // to the process dynamic memory (usually equal to mem_pool)
    rtl_list_linkage_t link;
    bool mem_init;
} fx_process_t;


//
// When process support is disabled the kernel does not handle hardware 
// exceptions, only TERM exception is supported for internal kernel use.
//
#define FX_EXCEPTION_TERM 0

//
// If only one function may be set as exception handler for single exception id 
// then "set" functions may be safely disabled and "get" function should always 
// return default handler.
//
void fx_process_ctor(void);
void fx_process_term(fx_process_t* process);
int fx_process_init(fx_process_t* process, fx_process_prio_t prio_type);
void fx_process_switch(fx_process_t* newp, fx_process_t* oldp);
typedef void (*fx_process_exception_handler_t)(void* t, unsigned id, void* arg);
extern void fx_thread_term_handler(void* target, unsigned id, void* arg);
#define fx_process_get_exception(exc_id) (&fx_thread_term_handler)

FX_METADATA(({ interface: [FX_PROCESS, EDITED] }))

#endif
