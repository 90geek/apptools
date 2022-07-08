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
static ls7a_ver_t ls7a;

cpu_info_t *get_cpu_info(void)
{
	S32 i;
	U64 cpuid=0;

	app_mm_read(LSCPU_ID, (U64 *)&cpuid, 8);

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

ls7a_ver_t *get_7a_version(void)
{
	U16 ls7a_id;
	U16 ls7a_108reg;

	app_mm_read(LS7A_VER+2, (U64 *)&ls7a_id, 2);
	app_mm_read(LS7A_VER180_REG, (U64 *)&ls7a_108reg, 1);
	
	// printf("0x%x 0x%x\n",ls7a_id,ls7a_108reg);
	switch(ls7a_id)
	{
		case 0x0:
			strcpy(ls7a.ls7a_ver,"ACTT");
			break;
		case 0x1:
			strcpy(ls7a.ls7a_ver,"BCTT");
			break;
		case 0x3:
			strcpy(ls7a.ls7a_ver,"CATT");
			break;
		default:
			strcpy(ls7a.ls7a_ver,"UNKNOW");
	}
	ls7a.ls7a_id = ls7a_id;
	ls7a.ls7a_108reg = ls7a_108reg;

	return &ls7a;
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

int ls7a_ver_debug(parse_t * pars_p,char *result_p)
{
	ls7a_ver_t *ls7a=NULL;

	ls7a=get_7a_version();
	if(ls7a==NULL)
	{
		printf("ls7a is no support!\n");
	}

	printf("ls7a_ver %s\n",ls7a->ls7a_ver);
	printf("ls7a_id 0x%x\n",ls7a->ls7a_id);
	printf("ls7a_108reg 0x%x\n",ls7a->ls7a_108reg);

	return 0;
}

void hardinfo_debug_register(void)
{
	register_command ("CPU_INFO" , cpu_info_debug , "<NONE>");
	register_command ("LS7A_VER" , ls7a_ver_debug , "<NONE>");
}

int hardinfo_init(void)
{
	hardinfo_debug_register();
}
