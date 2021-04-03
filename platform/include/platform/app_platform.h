#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include "app_types.h"

// typedef enum {
// 	DCH_3400PA,
// }app_device_e;

typedef struct app_platform_s{
  int cpu_type;
  int platform_version;
  int device_type;   /*app_device_e*/
  int harware_version;
}app_platform_t;


void app_platform_init(void) ;
void app_platform_deinit(void) ;
app_platform_t* get_app_platform_info(void);

int get_platform_mod_ver(void);
int get_platform_mod_ext_ver(void);


#endif
