#ifndef __HARD_INFO__
#define __HARD_INFO__

#include "platform/app_types.h"

typedef struct _cpu_info_t{
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
}cpu_info_t;

cpu_info_t *get_cpu_info(void);
int hardinfo_init(void);
#endif
