#include "platform/app_os.h"
#include "platform/app_spi.h"
#include "platform/app_platform.h"

#define FLASH_SIZE 0x400000
U64 spi_base_addr;

int update_bios(char *file_path)
{
	int from_fd=0,len=0,count=0;
	unsigned char  *ptr1 = NULL,*ptr2 = NULL;
	int Ret = 0;
	void * vaddr = NULL;
	int memoffset=0;

	if((from_fd=open(file_path,O_RDONLY))==-1)
	{
		printf("Open %s Error\n",file_path);
		return 1;
	}

	len=lseek(from_fd,0,SEEK_END);
	lseek(from_fd,0,SEEK_SET);

	count=len;
	ptr1=app_malloc(len*sizeof(char));
	memset(ptr1,0x0,len*sizeof(char));
	ptr2=ptr1;

	Ret=read(from_fd,ptr1,len);
	if(Ret==-1)
	{
		printf("Load FPGA File Error\n");
		close(from_fd);
		return 1;
	}
	printf("Load Bios File, Size is %d\n",Ret);

	vaddr=p2v_mem_mapping(GetLs3ASpiRegBaseAddr(),FLASH_SIZE, &memoffset);
	if(vaddr==NULL)
		return 1;

	if(Ret>FLASH_SIZE)
	{
		printf("file size > flash size\n");
		return 1;
	}
	UpdateBiosInSpiFlash(0,ptr2,Ret,(U64)vaddr);
	p2v_mem_clean(vaddr, memoffset);

	return 0;
}

U64 get_7a_spi_base_addr(void)
{
	if(spi_base_addr!=0)
		return spi_base_addr;
	else
	{
		spi_base_addr=app_get_pcie_region("00:16.0");
		return spi_base_addr;
	}

}
void set_7a_spi_base_addr(U64 base_addr)
{
	spi_base_addr=base_addr;
}

void read_7a_spi(unsigned int offset, unsigned char * datas, int read_cnt) 
{
	void * vaddr = NULL;
	int memoffset=0;

	if(GetLs7ASpiRegBaseAddr()==0)
	{
		printf("7a spi base addr is 0\n");
		return ;
	}
	vaddr=p2v_mem_mapping(GetLs7ASpiRegBaseAddr(),read_cnt, &memoffset);
	if(vaddr==NULL)
		return ;
	SpiFlashRead ((U64)offset,(void *)datas,(U64)read_cnt,(U64)vaddr);
	p2v_mem_clean(vaddr, memoffset);
}

void write_7a_spi(unsigned int offset, unsigned char * datas, int write_cnt)
{
	void * vaddr = NULL;
	int memoffset=0;
	if(GetLs7ASpiRegBaseAddr()==0)
	{
		printf("7a spi base addr is 0\n");
		return ;
	}
	vaddr=p2v_mem_mapping(GetLs7ASpiRegBaseAddr(),write_cnt, &memoffset);
	if(vaddr==NULL)
		return;
	SpiFlashSafeWrite ((U64)offset, (void *)datas, (U64)write_cnt,(U64)vaddr);
	p2v_mem_clean(vaddr, memoffset);
}

void write_7a_flash_mac(unsigned int eth, unsigned char * datas)
{
	void * vaddr = NULL;
	unsigned int offset=0;
	int memoffset=0;

	if(GetLs7ASpiRegBaseAddr()==0)
	{
		printf("7a spi base addr is 0\n");
		return ;
	}

	offset=(eth==0?0x0:0x10);
	vaddr=p2v_mem_mapping(GetLs7ASpiRegBaseAddr(),8, &memoffset);
	if(vaddr==NULL)
		return;
	SpiFlashSafeWrite ((U64)offset, (void *)datas, (U64)6,(U64)vaddr);
	p2v_mem_clean(vaddr, memoffset);
}

void read_7a_flash_mac(unsigned int eth)
{
	void * vaddr = NULL;
	unsigned int offset=0;
	int memoffset=0;
	char datas[8]={0};

	if(GetLs7ASpiRegBaseAddr()==0)
	{
		printf("7a spi base addr is 0\n");
		return ;
	}

	offset=(eth==0?0x0:0x10);
	vaddr=p2v_mem_mapping(GetLs7ASpiRegBaseAddr(),8, &memoffset);
	if(vaddr==NULL)
		return;
	SpiFlashRead ((U64)offset,(void *)datas,(U64)6,(U64)vaddr);
	printf("eth %d mac addr:\n",eth);
	app_print_data(datas,6);
	p2v_mem_clean(vaddr, memoffset);
}


void read_cpu_spi_flash(unsigned int offset, unsigned char * datas, int read_cnt) 
{
	void * vaddr = NULL;
	int memoffset=0;

	vaddr=p2v_mem_mapping(GetLs3ASpiRegBaseAddr(),read_cnt, &memoffset);
	if(vaddr==NULL)
		return;
	SpiFlashRead ((U64)offset,(void *)datas,(U64)read_cnt,(U64)vaddr);
	p2v_mem_clean(vaddr, memoffset);
}

void write_cpu_spi_flash(unsigned int offset, unsigned char * datas, int write_cnt)
{
	void * vaddr = NULL;
	int memoffset=0;
	vaddr=p2v_mem_mapping(GetLs3ASpiRegBaseAddr(),write_cnt, &memoffset);
	if(vaddr==NULL)
		return;
	SpiFlashSafeWrite ((U64)offset, (void *)datas, (U64)write_cnt,(U64)vaddr);
	p2v_mem_clean(vaddr, memoffset);
}
#define NOT_USED printf("%s not used\n", __FUNCTION__)
void app_spi_init(void)
{
	// NOT_USED;
}

void app_spi_read(int spi_dev,unsigned int offset, unsigned char * datas, int read_cnt) 
{
	switch(spi_dev)
	{
		case 0:
			read_cpu_spi_flash(offset,datas,read_cnt);
			break;
		case 1:
			read_7a_spi(offset,datas,read_cnt);
		break;
		default:
		printf("spi dev %d is no suport!!!\n",spi_dev);
	}
}

void app_spi_write(int spi_dev,unsigned int offset, unsigned char * datas, int write_cnt)
{
	switch(spi_dev)
	{
		case 0:
			write_cpu_spi_flash(offset,datas,write_cnt);
			break;
		case 1:
			write_7a_spi(offset,datas,write_cnt);
		break;
		default:
		printf("spi dev %d is no suport!!!\n",spi_dev);
	}
}

