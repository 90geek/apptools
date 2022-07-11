#ifndef __HARD_INFO__
#define __HARD_INFO__

#include "platform/app_types.h"

typedef struct _cpu_info_t_{
	const char *cpu_name;
	U64 cpu_id;
	const char *cpu_ver;
	const char *technics;
	U32 cpu_tdp;
	U8 cpu_jt; //Junction Temperature
	const char *cpu_pkg;
	U8 cpu_l;
	U8 cpu_w;
	U8 cpu_h;
  U32 cacheL1;
  U32 cacheL2;
  U32 cacheL3;
  U32 cpufreq;
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

cpu_info_t *get_cpu_info(void);
int get_sensors(ls_sensors_t *sen);
int hardinfo_init(void);

#endif
