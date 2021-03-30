#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include "pbi_types.h"

typedef enum {
	DCH_5600P,
	DCH_2210P,
	DCH_3200P,
	DCH_6XXXP,
	DCH_3800D,
	DCH_3400PA,
}pbi_device_e;

typedef struct pbi_platform_s{
	int cpu_type;
	int platform_version;
	int device_type;   /*pbi_device_e*/
	int harware_version;
}pbi_platform_t;


void pbi_platform_init(void) ;
void pbi_platform_deinit(void) ;
pbi_platform_t* get_pbi_platform_info(void);

int get_platform_mod_ver(void);
int get_platform_mod_ext_ver(void);


#endif
