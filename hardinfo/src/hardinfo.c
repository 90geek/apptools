#include "platform/app_os.h"
#include "platform/app_types.h"
#include "platform/app_mm.h"
#include "testtool/testtool.h"
#include "hardinfo.h"
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include "loongson3_def.h"

cpu_info_t cpu_info[] ={
	{"3A5000", LS3A5000_VERSION, "3A5000", "14nm",0,100,"LGA",37, 37,3},
	{"3A5000LL", LS3A5000LL_VERSION, "3A5000LL", "14nm",0,100,"LGA",37, 37,3},
	{"3A5000M", LS3A5000M_VERSION, "3A5000M", "14nm",0,100,"LGA",37, 37,3},
	{"3B5000", LS3B5000_VERSION, "3B5000", "14nm",0,100,"LGA",37, 37,3},
	{"3C5000L", LS3C5000L_VERSION, "3C5000L", "14nm",0,100,"LGA",37, 37,3},
	{"3C5000L", LS3C5000LL_VERSION, "3C5000LL", "14nm",0,100,"LGA",37, 37,3},
	{"3C5000I", LS3A5000I_VERSION, "3A5000I", "14nm",0,100,"LGA",37, 37,3},
	{"3C5000i", LS3A5000i_VERSION, "3A5000i", "14nm",0,100,"LGA",37, 37,3},
	{"3A5000BM", LS3A5000BM_VERSION, "3A5000BM", "14nm",0,100,"LGA",37, 37,3},
	{"3A5000HV", LS3A5000HV_VERSION, "3A5000HV", "14nm",0,100,"LGA",37, 37,3},
	{NULL, 0, NULL, "14nm",0,100,"LGA",37, 37,3}
};

cpu_info_t *get_cpu_info(void)
{
	S32 i;
	U64 cpuid=0;

	app_mm_read(0x1fe00020, &cpuid, 8);

	printf("cpu_id 0x%llx\n",cpuid);
	for(i=0;;i++)
	{
		// printf("cpu_name %s\n",cpu_info[i].cpu_name);
		if(cpu_info[i].cpu_id==cpuid&&cpu_info[i].cpu_name!=NULL)
		{
			return &cpu_info[i];
		}
	
		if(cpu_info[i].cpu_name==NULL)
			break;
	}
	return NULL;
}

int cpu_info_debug(parse_t * pars_p,char *result_p)
{
	cpu_info_t *cpu=NULL;

	cpu = get_cpu_info();
	if(cpu==NULL)
	{
		printf("cpu info is no support!\n");
	}

	printf("cpu_name %s\n",cpu->cpu_name);
	printf("cpu_id 0x%llx\n",cpu->cpu_id);
	printf("cpu_ver %s\n",cpu->cpu_ver);
	printf("technics %s\n",cpu->technics);
	printf("cpu_tdp %d\n",cpu->cpu_tdp);
	printf("cpu_jt %d\n",cpu->cpu_jt);
	printf("cpu_pkg; %s\n",cpu->cpu_pkg);
	printf("cpu_l %d\n",cpu->cpu_l);
	printf("cpu_w %d\n",cpu->cpu_w);
	printf("cpu_h %d\n",cpu->cpu_h);

	return 0;
}

void hardinfo_debug_register(void)
{
	register_command ("CPU_INFO" , cpu_info_debug , "<>");
}

int hardinfo_init(void)
{
	hardinfo_debug_register();
}
