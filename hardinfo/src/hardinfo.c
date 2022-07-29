#include "platform/app_os.h"
#include "platform/app_types.h"
#include "platform/app_mm.h"
#include "testtool/testtool.h"
#include "hardinfo.h"
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include "loongson/loongson3_def.h"

cpu_info_t cpu_info[] ={
	{"3A5000",		{LS3A5000_VERSION,		0}, "3A5000",	"14nm",		 "35",  "0", "70", "LGA","37", "37","3", "64K", "64K", "256K", "16384K", "LA464"},
	{"3A5000LL",	{LS3A5000LL_VERSION,	0}, "3A5000LL",	"14nm",  "30",  "0", "70", "LGA","37", "37","3", "64K", "64K", "256K", "16384K", "LA464"},
	{"3A5000M",		{LS3A5000M_VERSION,		0}, "3A5000M",	"14nm",  "18",  "0", "70", "LGA","37", "37","3", "64K", "64K", "256K", "16384K", "LA464"},
	{"3B5000",		{LS3B5000_VERSION,		0}, "3B5000",	"14nm",		 "25",  "0", "70", "LGA","37", "37","3", "64K", "64K", "256K", "16384K", "LA464"},
	{"3C5000L",		{LS3C5000L_VERSION,		0}, "3C5000L",	"14nm",  "200", "0", "70", "LGA","70", "44","4", "64K", "64K", "256K", "16384K", "LA464"},
	{"3C5000L",		{LS3C5000LL_VERSION,	0}, "3C5000LL",	"14nm",  "160", "0", "85", "LGA","70", "44","4", "64K", "64K", "256K", "16384K", "LA464"},
	{"3C5000I",		{LS3A5000I_VERSION,		0}, "3A5000I",	"14nm",  "18",  "0", "85", "LGA","37", "37","3", "64K", "64K", "256K", "16384K", "LA464"},
	{"3C5000i",		{LS3A5000i_VERSION,		0}, "3A5000i",	"14nm",  "18" , "0", "85", "LGA","37", "37","3", "64K", "64K", "256K", "16384K", "LA464"},
	{"3A5000BM",	{LS3A5000BM_VERSION,	0}, "3A5000BM",	"14nm",  "18",  "0", "70", "LGA","37", "37","3", "64K", "64K", "256K", "16384K", "LA464"},
	{"3A5000HV",	{LS3A5000HV_VERSION,	0}, "3A5000HV",	"14nm",  "35",  "0", "70", "LGA","37", "37","3", "64K", "64K", "256K", "16384K", "LA464"},
	{"3A4000",		{LS3A4000_VERSION,		0}, "3A4000",	"28nm",		 "35",  "0", "70", "LGA","37", "37","3", "64K", "64K", "256K", "16384K", "GS464V"},
	{"UNKNOW",		{0,										0},	"UNKNOW",		"0nm",		"0",	"0", "0",		"LGA","0", "0","0", "0K", "0K", "0K", "0K", "UNKNOW"}
};
static ls7a_ver_t ls7a;
static char buffer[100];

char *get_cpu_name(void)
{
	char *data=NULL;
  char *tmp=NULL;
	
	data=app_system("cat /proc/cpuinfo | grep 'model name'");
	// printf("***%s***\n",data);
	if(data==NULL)
	{
		sprintf(buffer,"%s","unknow");
		goto done;
	}
	sprintf(buffer,"%s",strstr(data,":")+1+strspn(strstr(data,":")+1," "));
	// printf("***%s***\n",buffer);
	if((tmp = strstr(buffer, "\n")))
		*tmp = '\0';
done:
	if(data!=NULL)
		app_free(data);
	return buffer;
}
char *get_cpu_version(void)
{
	char *data=NULL;
  char *tmp=NULL;
	
	data=app_system("dmidecode -t processor | grep 'Version:'");
	if(data==NULL)
	{
		sprintf(buffer,"%s","unknow");
		goto done;
	}
	sprintf(buffer,"%s",strstr(data,":")+1+strspn(strstr(data,":")+1," "));
	if((tmp = strstr(buffer, "\n")))
		*tmp = '\0';
done:
	if(data!=NULL)
		app_free(data);
	return buffer;
}
char *get_cpu_current_speed(void)
{
	char *data=NULL;
  char *tmp=NULL;
	
	data=app_system("cat /proc/cpuinfo | grep 'cpu MHz'");
	if(data==NULL)
	{
		if(data==NULL)
		data=app_system("cat /proc/cpuinfo | grep 'CPU MHz'");
		if(data==NULL)
		{
			sprintf(buffer,"%s","unknow");
			goto done;
		}
	}
	sprintf(buffer,"%s",strstr(data,":")+1+strspn(strstr(data,":")+1," "));
	if((tmp = strstr(buffer, "\n")))
		*tmp = '\0';
done:
	if(data!=NULL)
		app_free(data);
	return buffer;
}
char *get_cpu_max_speed(void)
{
	char *data=NULL;
  char *tmp=NULL;
	
	data=app_system("dmidecode -t 4 | grep 'Max Speed:'");
	if(data==NULL)
	{
		sprintf(buffer,"%s","unknow");
		goto done;
	}
	sprintf(buffer,"%s",strstr(data,":")+1+strspn(strstr(data,":")+1," "));
	if((tmp = strstr(buffer, "\n")))
		*tmp = '\0';
done:
	if(data!=NULL)
		app_free(data);
	return buffer;
}
char *get_cpu_min_speed(void)
{
	char *data=NULL;
  char *tmp=NULL;
	
	data=app_system("lscpu | grep 'CPU min MHz:'");
	if(data==NULL)
	{
		sprintf(buffer,"%s","unknow");
		goto done;
	}
	sprintf(buffer,"%s",strstr(data,":")+1+strspn(strstr(data,":")+1," "));
	if((tmp = strstr(buffer, "\n")))
		*tmp = '\0';
done:
	if(data!=NULL)
		app_free(data);
	return buffer;
}
char *get_cpu_arch(void)
{
	char *data=NULL;
  char *tmp=NULL;
	
	data=app_system("lscpu | grep 'Architecture:'");
	if(data==NULL)
  {
		data=app_system("lscpu | grep '架构：'");
		if(data==NULL)
		{
			sprintf(buffer,"%s","unknow");
			goto done;
		}
		else
			sprintf(buffer,"%s",strstr(data,"：")+3+strspn(strstr(data,"：")+3," "));
	}
	else
		sprintf(buffer,"%s",strstr(data,":")+1+strspn(strstr(data,":")+1," "));
	if((tmp = strstr(buffer, "\n")))
		*tmp = '\0';
done:
	if(data!=NULL)
		app_free(data);
	return buffer;
}

char *get_cpu_core_num(void)
{
	char *data=NULL;
  char *tmp=NULL;
	
	data=app_system("lscpu | grep 'Core(s) per socket:'");
	if(data==NULL)
	{
		data=app_system("lscpu | grep '每个座的核数：'");
		if(data==NULL)
		{
			sprintf(buffer,"%s","unknow");
			goto done;
		}
		else
			sprintf(buffer,"%s",strstr(data,"：")+3+strspn(strstr(data,"：")+3," "));
	}
	else
		sprintf(buffer,"%s",strstr(data,":")+1+strspn(strstr(data,":")+1," "));
	if((tmp = strstr(buffer, "\n")))
		*tmp = '\0';
done:
	if(data!=NULL)
		app_free(data);
	return buffer;
}
char *get_cpu_thread_num(void)
{
	char *data=NULL;
  char *tmp=NULL;
	
	data=app_system("lscpu | grep 'Thread(s) per core:'");
	if(data==NULL)
	{
		data=app_system("lscpu | grep '每个核的线程数：'");
		if(data==NULL)
		{
			sprintf(buffer,"%s","unknow");
			goto done;
		}
		else
			sprintf(buffer,"%s",strstr(data,"：")+3+strspn(strstr(data,"：")+3," "));
	}
	else
		sprintf(buffer,"%s",strstr(data,":")+1+strspn(strstr(data,":")+1," "));
	if((tmp = strstr(buffer, "\n")))
		*tmp = '\0';
done:
	if(data!=NULL)
		app_free(data);
	return buffer;
}
char *get_cpu_cacheL1d(void)
{
	char *data=NULL;
  char *tmp=NULL;
	data = app_system("lscpu | grep 'L1d cache:'");
	if(data==NULL)
	{
		data = app_system("lscpu | grep 'L1d 缓存：'");
		if(data==NULL)
		{
			sprintf(buffer,"%s","unknow");
			goto done;
		}
		else
			sprintf(buffer,"%s",strstr(data,"：")+3+strspn(strstr(data,"：")+3," "));
	}
	else
		sprintf(buffer,"%s",strstr(data,":")+1+strspn(strstr(data,":")+1," "));
	if((tmp = strstr(buffer, "\n")))
		*tmp = '\0';
done:
	if(data!=NULL)
		app_free(data);
	return buffer;
}
char *get_cpu_cacheL1i(void)
{
	char *data=NULL;
  char *tmp=NULL;
	
	data=app_system("lscpu | grep 'L1i cache:'");
	if(data==NULL)
	{
		data = app_system("lscpu | grep 'L1i 缓存：'");
		if(data==NULL)
		{
			sprintf(buffer,"%s","unknow");
			goto done;
		}
		else
			sprintf(buffer,"%s",strstr(data,"：")+3+strspn(strstr(data,"：")+3," "));
	}
	else
		sprintf(buffer,"%s",strstr(data,":")+1+strspn(strstr(data,":")+1," "));
	if((tmp = strstr(buffer, "\n")))
		*tmp = '\0';
done:
	if(data!=NULL)
		app_free(data);
	return buffer;
}
char *get_cpu_cacheL2(void)
{
	char *data=NULL;
  char *tmp=NULL;
	
	data=app_system("lscpu | grep 'L2 cache:'");
	if(data==NULL)
	{
		data = app_system("lscpu | grep 'L2 缓存：'");
		if(data==NULL)
		{
			sprintf(buffer,"%s","unknow");
			goto done;
		}
		else
			sprintf(buffer,"%s",strstr(data,"：")+3+strspn(strstr(data,"：")+3," "));
	}
	else
		sprintf(buffer,"%s",strstr(data,":")+1+strspn(strstr(data,":")+1," "));
	if((tmp = strstr(buffer, "\n")))
		*tmp = '\0';
done:
	if(data!=NULL)
		app_free(data);
	return buffer;
}
char *get_cpu_cacheL3(void)
{
	char *data=NULL;
  char *tmp=NULL;
	
	data=app_system("lscpu | grep 'L3 cache:'");
	if(data==NULL)
	{
		data = app_system("lscpu | grep 'L3 缓存：'");
		if(data==NULL)
		{
			sprintf(buffer,"%s","unknow");
			goto done;
		}
		else
			sprintf(buffer,"%s",strstr(data,"：")+3+strspn(strstr(data,"：")+3," "));
	}
	else
		sprintf(buffer,"%s",strstr(data,":")+1+strspn(strstr(data,":")+1," "));
	if((tmp = strstr(buffer, "\n")))
		*tmp = '\0';
done:
	if(data!=NULL)
		app_free(data);
	return buffer;
}
cpu_info_t *get_cpu_info(void)
{
	S32 i;
	U64 cpuid_l=0;
	U64 cpuid_h=0;
	
	app_mm_read(LSCPU_ID, (U64 *)&cpuid_l, 8);
	app_mm_read(LSCPU_ID+8, (U64 *)&cpuid_h, 8);

	printf("cpu_id_l 0x%llx,cpu_id_h 0x%llx\n",cpuid_l,cpuid_h);
	for(i=0;;i++)
	{
		// printf("cpu_name %s\n",cpu_info[i].cpu_name);
		if(cpu_info[i].id.l==cpuid_l&&	\
			cpu_info[i].id.h==cpuid_h&&  \
			strcmp(cpu_info[i].cpu_name,"UNKNOW")!=0)
		{
			return &cpu_info[i];
		}
		if(strcmp(cpu_info[i].cpu_name,"UNKNOW")==0 || cpu_info[i].id.l==0)
			break;
	}

	sprintf(cpu_info[i].cpu_name,"%s",get_cpu_name());
	sprintf(cpu_info[i].cpu_ver,"%s",get_cpu_version());
	sprintf(cpu_info[i].cacheL1d,"%s",get_cpu_cacheL1d());
	sprintf(cpu_info[i].cacheL1i,"%s",get_cpu_cacheL1i());
	sprintf(cpu_info[i].cacheL2,"%s",get_cpu_cacheL2());
	sprintf(cpu_info[i].cacheL3,"%s",get_cpu_cacheL3());
	return &cpu_info[i];
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
char *get_bios_version(void)
{
	char *data=NULL;
	
	data=app_system("dmidecode -t bios | grep Version:");
	sscanf(data, "%*[^ ] %s", buffer);
	
	if(data!=NULL)
		app_free(data);
	return buffer;
}

char *get_product_name(void)
{
	char *data=NULL;
	
	data=app_system("dmidecode -t 1 | grep 'Product Name:'");
	sscanf(data, "%*[^ ] %*[^ ] %s", buffer);
	
	if(data!=NULL)
		app_free(data);
	return buffer;
}
int cpu_info_debug(parse_t * pars_p,char *result_p)
{
	cpu_info_t *cpu=NULL;

	cpu = get_cpu_info();
	if(cpu==NULL)
	{
		printf("cpu info is no support!\n");
		return 1;
	}
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

int ls_sensors_debug(parse_t * pars_p,char *result_p)
{
	ls_sensors_t sen;
	int err;

	err=get_sensors(&sen);
	if(err!=0)
	{
		printf("ls7a is no support!\n");
	}

	printf("cputemp0 %d\n",sen.cputemp0);
	printf("cputemp1 %d\n",sen.cputemp1);

	return 0;
}
int get_biosi_ver_debug(parse_t * pars_p,char *result_p)
{

	char *p;
	p = get_bios_version();
	printf("bios version : %s\n",p);
	return 0;
}
void hardinfo_debug_register(void)
{
	register_command ("CPU_INFO" , cpu_info_debug , "<NONE>");
	register_command ("LS7A_VER" , ls7a_ver_debug , "<NONE>");
	register_command ("LS_SENSORS" , ls_sensors_debug , "<NONE>");
	register_command ("LS_BIOS_VER" , get_biosi_ver_debug , "<NONE>");
}

int hardinfo_init(void)
{
	hardinfo_debug_register();
}
