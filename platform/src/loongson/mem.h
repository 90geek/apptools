#ifndef __MEM_H__
#define __MEM_H__
#include "platform/app_types.h"
int get_memdev_fd(void);
void memory_mapping(void);
void memory_release(void);

void * get_gpio_mem_base(void) ;
void * get_pwm_mem_base(void); 

void *p2v_mem_mapping(unsigned long long paddr,int size);
int p2v_mem_clean(void *vaddr);
void *ls_mem_chunk(U64 base, U32 len, const char *devmem);

#endif
