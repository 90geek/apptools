#include <stdio.h>
#include "hardinfo.h"

int main (int argc,char *argv[])
{
	cpu_info_t *cpu=NULL;

	cpu = get_cpu_info();
	printf("cpu_name %s\n",cpu->cpu_name);
	printf("cpu_idh 0x%llx\n",cpu->id.h);
	printf("cpu_idl 0x%llx\n",cpu->id.l);
	printf("cpu_ver %s\n",cpu->cpu_ver);
	printf("technics %s\n",cpu->technics);
	printf("cpu_tdp %d\n",cpu->cpu_tdp);
	printf("cpu_jt_l %d\n",cpu->cpu_jt_l);
	printf("cpu_jt_h %d\n",cpu->cpu_jt_h);
	printf("cpu_pkg; %s\n",cpu->cpu_pkg);
	printf("cpu_l %d\n",cpu->cpu_l);
	printf("cpu_w %d\n",cpu->cpu_w);
	printf("cpu_h %d\n",cpu->cpu_h);
	printf("cacheL1d %dKB\n",cpu->cacheL1d);
	printf("cacheL1i %dKB\n",cpu->cacheL1i);
	printf("cacheL2 %dKB\n",cpu->cacheL2);
	printf("cacheL3 %dKB\n",cpu->cacheL3);
	printf("cpufreq %dMH\n",cpu->cpufreq);
	return 0;
}
