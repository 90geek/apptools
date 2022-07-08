#include "testtool/testtool.h"
#include "platform/app_spi.h"
#include "platform/app_mm.h"
#include "platform/platform_debug.h"
#include "platform/app_platform.h"

int fan_set_debug(parse_t * pars_p,char *result_p)
{
	int num,level;
	unsigned char buf[20]={0};
	int error;
	error=cget_integer(pars_p,0,&num);
	if (error)
	{
		tag_current_line(pars_p,"-->num!");
		return 1;
	}

  SmartFanSet (num);

	printf("set fan %d %\n", num);

	return 0;
}

int read_cpu_temp_debug(parse_t * pars_p,char *result_p)
{
	U32 temp0,temp1;

	lscpu_tempdetect(&temp0,&temp1);
	printf("cpu temp0 %d ,temp1 %d\n", temp0,temp1);
	return 0;
}

int read_cpu_freq_debug(parse_t * pars_p,char *result_p)
{
	U32 freq;

	CpuGetFrequency (100000,&freq);
	printf("cpu freq %d\n", freq);
	return 0;
}
int set_7a_spi_base_addr_debug(parse_t * pars_p,char *result_p)
{
	unsigned int addr_h,addr_l;
	unsigned char buf[20]={0};
	int error;
	U64 base_addr;

	error=cget_integer(pars_p,0,&addr_h);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_h");
		return 1;
	}

	error=cget_integer(pars_p,0,&addr_l);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_l!");
		return 1;
	}

	printf("Input addr hight 0x%x addr low 0x%x \n", base_addr);
	base_addr=addr_h;
	base_addr=((base_addr&0xffffffff)<<32);
	base_addr|=addr_l;
	set_7a_spi_base_addr(base_addr);
	printf("Base addr  0x%llx \n", base_addr);
	return 0;
}

int spi_read_debug(parse_t * pars_p,char *result_p)
{
	int dev,len,offset;
	unsigned char buf[100]={0};
	int i=0, error;

	error=cget_integer(pars_p,0,&dev);
	if (error)
	{
		tag_current_line(pars_p,"-->dev!");
		return 1;
	}
	error=cget_integer(pars_p,0,&offset);
	if (error)
	{
		tag_current_line(pars_p,"-->offset!");
		return 1;
	}

	error=cget_integer(pars_p,0,&len);
	if (error)
	{
		tag_current_line(pars_p,"-->len!");
		return 1;
	}

	if(len>100)
		len = 100;

	app_spi_read(dev, offset, buf, len);

	printf("spi_read read value :\n");
	app_print_data(buf,len);
	return 0;
}

int spi_write_debug(parse_t * pars_p,char *result_p)
{
	int dev,len,offset;
	unsigned int data;
	unsigned char buf[20]={0};
	int i=0, error;

	error=cget_integer(pars_p,0,&dev);
	if (error)
	{
		tag_current_line(pars_p,"-->dev!");
		return 1;
	}
	error=cget_integer(pars_p,0,&offset);
	if (error)
	{
		tag_current_line(pars_p,"-->offset!");
		return 1;
	}

	error=cget_integer(pars_p,0,&data);
	if (error)
	{
		tag_current_line(pars_p,"-->data!");
		return 1;
	}

	buf[0] = (data&0xff);
	buf[1] = (data>>8)&0xff;
	buf[2] = (data>>16)&0xff;
	buf[3] = (data>>24)&0xff;

	app_spi_write(dev,offset, buf, 4);

	return 0;
}

int update_bios_debug(parse_t * pars_p,char *result_p)
{
	char path[200];

	/* ------------- */
	if (cget_string(pars_p,"", path, sizeof(path))==1)
	{
		tag_current_line(pars_p,"Invalid path\n");
		return(1);
	}

	printf("bios path  %s \n",path);
	update_bios(path);
	return 0;
}
int spi_write_7a_flash_mac_debug(parse_t * pars_p,char *result_p)
{
	int eth;
	int i=0, error;
	char str_mac[200];
	char *pbuf;

	error=cget_integer(pars_p,0,&eth);
	if (error)
	{
		tag_current_line(pars_p,"-->eth!");
		return 1;
	}

	if (cget_string(pars_p,"", str_mac, sizeof(str_mac))==1)
	{
		tag_current_line(pars_p,"Invalid str_mac\n");
		return(1);
	}

	pbuf=str2data_mac(str_mac);

	write_7a_flash_mac(eth, pbuf);

	return 0;
}

int mm_read_byte_debug(parse_t * pars_p,char *result_p)
{
	unsigned int addr_h,addr_l;
	int error;
	U64 base_addr;
	U8 data;

	error=cget_integer(pars_p,0,&addr_h);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_h!");
		return 1;
	}
	error=cget_integer(pars_p,0,&addr_l);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_l!");
		return 1;
	}
	
	base_addr=addr_h;
	base_addr=((base_addr&0xffffffff)<<32);
	base_addr|=addr_l;
	printf("Base addr  0x%llx \n", base_addr);
	app_mm_read(base_addr, (U64 *)&data, 1);
	printf("mm read value :0x%x\n",data);
	return 0;
}
int mm_read_hword_debug(parse_t * pars_p,char *result_p)
{
	unsigned int addr_h,addr_l;
	int error;
	U64 base_addr;
	U16 data;

	error=cget_integer(pars_p,0,&addr_h);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_h!");
		return 1;
	}
	error=cget_integer(pars_p,0,&addr_l);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_l!");
		return 1;
	}
	
	base_addr=addr_h;
	base_addr=((base_addr&0xffffffff)<<32);
	base_addr|=addr_l;
	printf("Base addr  0x%llx \n", base_addr);
	app_mm_read(base_addr, (U64 *)&data, 2);
	printf("mm read value :0x%x\n",data);
	return 0;
}
int mm_read_word_debug(parse_t * pars_p,char *result_p)
{
	unsigned int addr_h,addr_l;
	int error;
	U64 base_addr;
	U32 data;

	error=cget_integer(pars_p,0,&addr_h);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_h!");
		return 1;
	}
	error=cget_integer(pars_p,0,&addr_l);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_l!");
		return 1;
	}
	
	base_addr=addr_h;
	base_addr=((base_addr&0xffffffff)<<32);
	base_addr|=addr_l;
	printf("Base addr  0x%llx \n", base_addr);
	app_mm_read(base_addr,(U64 *)&data, 4);
	printf("mm read value :0x%x\n",data);
	return 0;
}
int mm_read_dword_debug(parse_t * pars_p,char *result_p)
{
	unsigned int addr_h,addr_l;
	int error;
	U64 base_addr;
	U64 data;

	error=cget_integer(pars_p,0,&addr_h);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_h!");
		return 1;
	}
	error=cget_integer(pars_p,0,&addr_l);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_l!");
		return 1;
	}
	
	base_addr=addr_h;
	base_addr=((base_addr&0xffffffff)<<32);
	base_addr|=addr_l;
	printf("Base addr  0x%llx \n", base_addr);
	app_mm_read(base_addr, &data, 8);
	printf("mm read value :0x%llx\n",data);
	return 0;
}
int mm_write_byte_debug(parse_t * pars_p,char *result_p)
{
	unsigned int addr_h,addr_l;
	U32 data;
	int error;
	U64 base_addr;

	error=cget_integer(pars_p,0,&addr_h);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_h!");
		return 1;
	}
	error=cget_integer(pars_p,0,&addr_l);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_l!");
		return 1;
	}
	error=cget_integer(pars_p,0,&data);
	if (error)
	{
		tag_current_line(pars_p,"-->data!");
		return 1;
	}

	base_addr=addr_h;
	base_addr=((base_addr&0xffffffff)<<32);
	base_addr|=addr_l;
	printf("Base addr  0x%llx data=0x%x\n", base_addr,data);
	app_mm_write(base_addr, (U64)data, 1);
	return 0;
}

int mm_write_hword_debug(parse_t * pars_p,char *result_p)
{
	unsigned int addr_h,addr_l;
	U32 data;
	int error;
	U64 base_addr;

	error=cget_integer(pars_p,0,&addr_h);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_h!");
		return 1;
	}
	error=cget_integer(pars_p,0,&addr_l);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_l!");
		return 1;
	}
	error=cget_integer(pars_p,0,&data);
	if (error)
	{
		tag_current_line(pars_p,"-->data!");
		return 1;
	}

	base_addr=addr_h;
	base_addr=((base_addr&0xffffffff)<<32);
	base_addr|=addr_l;
	printf("Base addr  0x%llx data=0x%x\n", base_addr,data);
	app_mm_write(base_addr, (U64)data, 2);
	return 0;
}
int mm_write_word_debug(parse_t * pars_p,char *result_p)
{
	unsigned int addr_h,addr_l;
	U32 data;
	int error;
	U64 base_addr;

	error=cget_integer(pars_p,0,&addr_h);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_h!");
		return 1;
	}
	error=cget_integer(pars_p,0,&addr_l);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_l!");
		return 1;
	}
	error=cget_integer(pars_p,0,&data);
	if (error)
	{
		tag_current_line(pars_p,"-->data!");
		return 1;
	}

	base_addr=addr_h;
	base_addr=((base_addr&0xffffffff)<<32);
	base_addr|=addr_l;
	printf("Base addr  0x%llx data=0x%x\n", base_addr,data);
	app_mm_write(base_addr, (U64)data, 4);
	return 0;
}
int mm_write_dword_debug(parse_t * pars_p,char *result_p)
{
	unsigned int addr_h,addr_l;
	U32 data_h,data_l;
	int error;
	U64 base_addr;
	U64 data;

	error=cget_integer(pars_p,0,&addr_h);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_h!");
		return 1;
	}
	error=cget_integer(pars_p,0,&addr_l);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_l!");
		return 1;
	}
	error=cget_integer(pars_p,0,&data_h);
	if (error)
	{
		tag_current_line(pars_p,"-->data_h!");
		return 1;
	}
	error=cget_integer(pars_p,0,&data_l);
	if (error)
	{
		tag_current_line(pars_p,"-->data_h!");
		return 1;
	}

	base_addr=addr_h;
	base_addr=((base_addr&0xffffffff)<<32);
	base_addr|=addr_l;
	
	data=data_h;
	data=((data&0xffffffff)<<32);
	data|=data_l;

	printf("Base addr  0x%llx data=0x%x\n", base_addr,data);
	app_mm_write(base_addr, data, 8);
	return 0;
}
int mm_read_burst_debug(parse_t * pars_p,char *result_p)
{
	unsigned int addr_h,addr_l;
	int len;
	unsigned char buf[100]={0};
	int error;
	U64 base_addr;

	error=cget_integer(pars_p,0,&addr_h);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_h!");
		return 1;
	}
	error=cget_integer(pars_p,0,&addr_l);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_l!");
		return 1;
	}
	error=cget_integer(pars_p,0,&len);
	if (error)
	{
		tag_current_line(pars_p,"-->len!");
		return 1;
	}

	if(len>100)
		len = 100;
	
	base_addr=addr_h;
	base_addr=((base_addr&0xffffffff)<<32);
	base_addr|=addr_l;
	printf("Base addr  0x%llx \n", base_addr);
	app_mm_read_burst(base_addr, buf, len);
	printf("mm read value :\n");
	app_print_data(buf,len);
	return 0;
}

void platform_debug_register(void)
{
	register_command ("LS_FAN_WRITE" 			, fan_set_debug , "<Percent number>:0-100");
	register_command ("LS_CPU_TEMP"			, read_cpu_temp_debug , "");
	register_command ("LS_CPU_FREQ"			, read_cpu_freq_debug , "");
	register_command ("SET_7A_SPI_BASE"			, set_7a_spi_base_addr_debug , "<base_addr_hi>,<base_addr_lo>");
	register_command ("SPI_READ"			, spi_read_debug , "<spi_dev>,<offset>,<len>:spi_dev 0cpuflash/17a");
	register_command ("SPI_WRITE"			, spi_write_debug , "<spi_dev>,<offset>,<data>:spi_dev 0cpuflash/17a,data 32bit");
	register_command ("SPI_BIOS"			, update_bios_debug , "<path>:eg ./LS3A50007A.fd");
	register_command ("SPI_WRITE_MAC"			, spi_write_7a_flash_mac_debug , "<eth> <macaddr>, eg 0 11:22:33:44:55:66");
	register_command ("MM_R_B"			, mm_read_byte_debug , "<base_addr_hi>,<base_addr_lo>");
	register_command ("MM_R_HW"			, mm_read_hword_debug , "<base_addr_hi>,<base_addr_lo>");
	register_command ("MM_R_W"			, mm_read_word_debug , "<base_addr_hi>,<base_addr_lo>");
	register_command ("MM_R_DW"			, mm_read_dword_debug , "<base_addr_hi>,<base_addr_lo>");
	register_command ("MM_W_B"			, mm_write_byte_debug , "<base_addr_hi>,<base_addr_lo>,<data>");
	register_command ("MM_W_HW"			, mm_write_hword_debug , "<base_addr_hi>,<base_addr_lo>,<data>");
	register_command ("MM_W_W"			, mm_write_word_debug , "<base_addr_hi>,<base_addr_lo>,<data>");
	register_command ("MM_W_DW"			, mm_write_dword_debug , "<base_addr_hi>,<base_addr_lo>,<data_hi>,<data_lo>");
	register_command ("MM_R_BURST"			, mm_read_burst_debug , "<base_addr_hi>,<base_addr_lo>,<len>");
}
