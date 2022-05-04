#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include "app_types.h"

typedef struct app_platform_s{
  int cpu_type;
  int platform_version;
  int device_type;   /*app_device_e*/
  int harware_version;
}app_platform_t;


void app_platform_init(void) ;
void app_platform_deinit(void) ;
app_platform_t* get_app_platform_info(void);



/****************************************/
void memory_mapping(void);
void memory_release(void);

void * get_gpio_mem_base(void) ;
void * get_pwm_mem_base(void); 

#endif
