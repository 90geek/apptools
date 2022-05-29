#ifndef __MEM_H__
#define __MEM_H__

void memory_mapping(void);
void memory_release(void);

void * get_gpio_mem_base(void) ;
void * get_pwm_mem_base(void); 

void *p2v_mem_mapping(unsigned long long paddr,int size);
int p2v_mem_clean(void *vaddr);


#endif
