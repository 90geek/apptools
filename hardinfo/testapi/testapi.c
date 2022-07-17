#include <stdio.h>
#include "hardinfo.h"

int main (int argc,char *argv[])
{
	cpu_info_t *cpu=NULL;

	cpu = get_cpu_info();
	printf("cpu_name: %s\n",cpu->cpu_name);
	printf("cpu_idh: 0x%llx\n",cpu->id.h);
	printf("cpu_idl: 0x%llx\n",cpu->id.l);
	printf("cpu_ver: %s\n",cpu->cpu_ver);
	printf("technics: %s\n",cpu->technics);
	printf("cpu_tdp: %s\n",cpu->cpu_tdp);
	printf("cpu_jt_l: %s\n",cpu->cpu_jt_l);
	printf("cpu_jt_h: %s\n",cpu->cpu_jt_h);
	printf("cpu_pkg: %s\n",cpu->cpu_pkg);
	printf("cpu_l: %s\n",cpu->cpu_l);
	printf("cpu_w: %s\n",cpu->cpu_w);
	printf("cpu_h: %s\n",cpu->cpu_h);
	printf("cacheL1d: %s\n",cpu->cacheL1d);
	printf("cacheL1i: %s\n",cpu->cacheL1i);
	printf("cacheL2: %s\n",cpu->cacheL2);
	printf("cacheL3: %s\n",cpu->cacheL3);
	printf("microarch: %s\n",cpu->microarch);
	
	printf("cpu_crrent_speed: %s\n",get_cpu_current_speed());
	printf("cpu_max_speed: %s\n",get_cpu_max_speed());
	printf("cpu_min_speed: %s\n",get_cpu_min_speed());
	printf("cpu_arch: %s\n",get_cpu_arch());
	printf("cpu_core_num: %s\n",get_cpu_core_num());
	printf("cpu_thread_num: %s\n",get_cpu_thread_num());
	printf("product_name: %s\n",get_product_name());

	return 0;
}
