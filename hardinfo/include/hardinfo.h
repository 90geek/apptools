#ifndef __HARD_INFO__
#define __HARD_INFO__

#include "platform/app_types.h"
typedef struct {
		U64 l;
		U64 h;
} cpuid;
typedef struct _cpu_info_t_{
	char cpu_name[100];
	cpuid id;
	char cpu_ver[100];
	char technics[100];
	char cpu_tdp[8];
	char cpu_jt_l[8]; //Junction Temperature
	char cpu_jt_h[8]; //Junction Temperature
	char cpu_pkg[8];
	char cpu_l[8];
	char cpu_w[8];
	char cpu_h[8];
	char cacheL1d[100];
	char cacheL1i[100];
	char cacheL2[100];
	char cacheL3[100];
	char microarch[100];
}cpu_info_t;


typedef struct _ls7a_ver_t_{
	char ls7a_ver[20];
	U16 ls7a_id;
	U16 ls7a_108reg;
}ls7a_ver_t;

typedef struct _ls_sensors_t_{
	U32 cputemp0; 
	U32 cputemp1;
	U32 ls7atemp0; 
	U32 ls7atemp1;
	U32 fan0;
	U32 fan1;
}ls_sensors_t;

char *get_cpu_name(void);
char *get_cpu_version(void);
char *get_cpu_current_speed(void);
char *get_cpu_max_speed(void);
char *get_cpu_min_speed(void);
char *get_cpu_arch(void);
char *get_cpu_core_num(void);
char *get_cpu_thread_num(void);
char *get_cpu_cacheL1d(void);
char *get_cpu_cacheL1i(void);
char *get_cpu_cacheL2(void);
char *get_cpu_cacheL3(void);

cpu_info_t *get_cpu_info(void);
int get_sensors(ls_sensors_t *sen);
char *get_bios_version(void);
char *get_product_name(void);
int hardinfo_init(void);

#endif
