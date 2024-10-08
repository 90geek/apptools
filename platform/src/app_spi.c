#include "platform/app_os.h"
#include "platform/app_spi.h"
#include "platform/app_platform.h"
#include "loongson/debug.h"

U64 spi_base_addr;

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
void read_7a_tcm_id(void)
{
	void * vaddr = NULL;
	int memoffset=0;
	// unsigned char datas[100]={0};
	unsigned int datas=0;


	if(GetLs7ASpiRegBaseAddr()==0)
	{
		printf("7a spi base addr is 0\n");
		return ;
	}
	vaddr=p2v_mem_mapping(GetLs7ASpiRegBaseAddr(),0xff0, &memoffset);
	if(vaddr==NULL)
		return ;
	printf("\n7A spi Cs0:\n");
	SpiTcmRead ((U64)0x80d40000,(void *)&datas,(U64)1,(U64)vaddr,0);
	if(datas==0xff)
	{
		printf("CS0 No Link!!!\n");
		goto CS1;
	}
	printf("tpm Base Valeu: 0x%x\n",datas);
	SpiTcmRead ((U64)0x83d40f00,(void *)&datas,(U64)4,(U64)vaddr,0);
	printf("tpm Vendor Id: 0x%x\n",(datas&0xffff0000)>>16);
	SpiTcmRead ((U64)0x83d40f04,(void *)&datas,(U64)1,(U64)vaddr,0);
	printf("tpm rev Id: 0x%x\n\n",(datas&0xff));
CS1:
	printf("\n7A spi Cs1:\n");
	SpiTcmRead ((U64)0x80d40000,(void *)&datas,(U64)1,(U64)vaddr,1);
	if(datas==0xff)
	{
		printf("CS1 No Link!!!\n");
		goto END;
	}
	printf("tpm Base Valeu: 0x%x\n",datas);
	SpiTcmRead ((U64)0x83d40f00,(void *)&datas,(U64)4,(U64)vaddr,1);
	printf("tpm VendorId: 0x%x\n",(datas&0xffff));
	printf("tpm DeviceId: 0x%x\n",(datas&0xffff0000)>>16);
	SpiTcmRead ((U64)0x83d40f04,(void *)&datas,(U64)1,(U64)vaddr,1);
	printf("tpm rev Id: 0x%x\n\n",(datas&0xff));
END:
	p2v_mem_clean(vaddr, memoffset);
}
void read_7a_tcm(unsigned int offset, unsigned char * datas, int read_cnt, int cs) 
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
	SpiTcmRead ((U64)offset,(void *)datas,(U64)read_cnt,(U64)vaddr, cs);
	p2v_mem_clean(vaddr, memoffset);
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

void clear_7a_advanced_config(void) 
{
	unsigned char datas[0x7f]={0xff};
	printf("7A Flash Advanced Cfg Data:");
	read_7a_spi(0x80, datas, 0x7f);
	app_print_data(datas,0x7f);
	printf("7A Flash Advanced Cfg Start Clear:");
	memset(datas,0xff,0x7f);
	write_7a_spi(0x80, datas, 0x7f);
	printf("7A Flash Advanced Cfg Clear end Data:");
	read_7a_spi(0x80, datas, 0x7f);
	app_print_data(datas,0x7f);
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
void write_block_cpu_spi_flash(unsigned int offset, unsigned char * datas, int write_cnt)
{
	void * vaddr = NULL;
	int memoffset=0;
	if((offset&0xfff!=0) || (write_cnt&0xfff!=0))
	{
		printf("Not is Block size\n");
		return;
	}
	vaddr=p2v_mem_mapping(GetLs3ASpiRegBaseAddr(),write_cnt, &memoffset);
	if(vaddr==NULL)
		return;
	SpiFlashWrite ((U64)offset, (void *)datas, (U64)write_cnt,(U64)vaddr);
	p2v_mem_clean(vaddr, memoffset);
}
void erase_cpu_spi_flash(unsigned int offset, int byte_cnt)
{
	void * vaddr = NULL;
	int memoffset=0;
	UINT64	SectorStart;
	UINT64	SectorNum;
	UINT8		*Buff;
	UINT64	Num = byte_cnt;

	if (byte_cnt == 0) {
		return ;
	}
	vaddr=p2v_mem_mapping(GetLs3ASpiRegBaseAddr(),byte_cnt, &memoffset);
	if(vaddr==NULL)
		return;
	SectorStart = offset / BLKSIZE;
	SectorNum		= ((offset + Num - 1) / BLKSIZE) - (offset / BLKSIZE) + 1;
	Buff = malloc (SectorNum * BLKSIZE);
	if(!Buff){
		ASSERT(0);
	}

	SpiFlashRead (SectorStart * BLKSIZE, Buff, SectorNum * BLKSIZE, (U64)vaddr);
	memset (Buff + offset % BLKSIZE, Num, 0xFF);
	SpiFlashErase ((U64)offset, Num,(U64)vaddr);
	SpiFlashWrite (SectorStart * BLKSIZE, Buff, SectorNum * BLKSIZE, (U64)vaddr);
	free(Buff);

	p2v_mem_clean(vaddr, memoffset);
}
void erase_block_cpu_spi_flash(unsigned int offset, int byte_cnt)
{
	void * vaddr = NULL;
	int memoffset=0;
	if((offset&0xfff!=0) || (byte_cnt&0xfff!=0))
	{
		printf("Not is Block size\n");
		return;
	}
	vaddr=p2v_mem_mapping(GetLs3ASpiRegBaseAddr(),byte_cnt, &memoffset);
	if(vaddr==NULL)
		return;
	SpiFlashErase ((U64)offset, byte_cnt,(U64)vaddr);
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

