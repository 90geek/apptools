#include "platform/app_os.h"
#include "platform/app_mm.h"
#include "platform/app_platform.h"

void read_mm(U64 pbase, unsigned char * datas, int read_cnt) 
{
	void * vaddr = NULL;

	vaddr=p2v_mem_mapping(pbase,read_cnt);
	memcpy(datas,vaddr,read_cnt);
	p2v_mem_clean(vaddr);
}

void write_mm(U64 pbase, unsigned char * datas, int write_cnt)
{
	void * vaddr = NULL;
	vaddr=p2v_mem_mapping(pbase,write_cnt);
	memcpy(vaddr,datas,write_cnt);
	p2v_mem_clean(vaddr);
}

void read_mm_byte(U64 pbase, U8 *datas) 
{
	unsigned char buf[8] ={0};
	read_mm(pbase, buf, 1);
	*datas = (buf[0]<<0);
}
void write_mm_byte(U64 pbase, U8 datas)
{
	unsigned char buf[8] ={0};
	buf[0]=(datas&0xff);
	write_mm(pbase, buf, 1);
}

void read_mm_hword(U64 pbase, U16 *datas) 
{
	unsigned char buf[8] ={0};
	read_mm(pbase, buf, 2);
	*datas = (buf[1]<<8)+(buf[0]<<0);
}
void write_mm_hword(U64 pbase, U16 datas)
{
	unsigned char buf[8] ={0};
	buf[0]=(datas&0xff);
	buf[1]=((datas>>8)&0xff);
	write_mm(pbase, buf, 2);
}

void read_mm_word(U64 pbase, U32 *datas) 
{
	unsigned char buf[8] ={0};
	read_mm(pbase, buf, 4);
	*datas = (buf[3]<<24)+(buf[2]<<16)+(buf[1]<<8)+(buf[0]<<0);
}
void write_mm_word(U64 pbase, U32 datas)
{
	unsigned char buf[8] ={0};
	buf[0]=(datas&0xff);
	buf[1]=((datas>>8)&0xff);
	buf[2]=((datas>>16)&0xff);
	buf[3]=((datas>>24)&0xff);
	write_mm(pbase, buf, 4);
}

void read_mm_dword(U64 pbase, U64 *datas) 
{
	unsigned char buf[8] ={0};
	U32 data_h,data_l;
	read_mm(pbase, buf, 8);
	// app_print_data(buf,8);
	data_l = ((buf[0]&0xff)<<0);
	data_l |= ((buf[1]&0xff)<<8);
	data_l |= ((buf[2]&0xff)<<16);
	data_l |= ((buf[3]&0xff)<<24);
	data_h = ((buf[4]&0xff)<<0);
	data_h |= ((buf[5]&0xff)<<8);
	data_h |= ((buf[6]&0xff)<<16);
	data_h |= ((buf[7]&0xff)<<24);
	*datas=data_h;
	*datas=((*datas&0xffffffff)<<32);
	*datas|=data_l;
}
void write_mm_dword(U64 pbase, U64 datas)
{
	unsigned char buf[8] ={0};
	buf[0]=(datas&0xff);
	buf[1]=((datas>>8)&0xff);
	buf[2]=((datas>>16)&0xff);
	buf[3]=((datas>>24)&0xff);
	buf[4]=((datas>>32)&0xff);
	buf[5]=((datas>>40)&0xff);
	buf[6]=((datas>>48)&0xff);
	buf[7]=((datas>>56)&0xff);
	write_mm(pbase, buf, 8);
}

#define NOT_USED printf("%s not used\n", __FUNCTION__)
void app_mm_init(void)
{
	NOT_USED;
}

void app_mm_read(U64 pbase, U64 *datas, int read_cnt) 
{
	switch(read_cnt)
	{
		case 1:
			read_mm_byte(pbase,(U8 *)datas);
			break;
		case 2:
			read_mm_hword(pbase,(U16 *)datas);
			break;
		case 4:
			read_mm_word(pbase,(U32 *)datas);
			break;
		case 8:
			read_mm_dword(pbase,(U64 *)datas);
			break;
		default:
		printf("mm %d is no suport!!!\n",read_cnt);
	}
}

void app_mm_write(U64 pbase, U64 datas, int write_cnt)
{
	switch(write_cnt)
	{
		case 1:
			write_mm_byte(pbase, (U8)datas);
			break;
		case 2:
			write_mm_hword(pbase, (U16)datas);
			break;
		case 4:
			write_mm_word(pbase, (U32)datas);
			break;
		case 8:
			write_mm_dword(pbase, (U64)datas);
			break;
		default:
			printf("mm is no suport!!!\n",write_cnt);
	}
}

void app_mm_read_burst(U64 pbase, unsigned char * datas, int read_cnt)
{
	read_mm(pbase, datas, read_cnt);
}
