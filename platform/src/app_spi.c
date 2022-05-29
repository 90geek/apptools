#include "platform/app_os.h"
#include "platform/app_spi.h"
#include "platform/app_platform.h"

#define FLASH_SIZE 0x400000
U64 spi_base_addr;

void update_bios(char *file_path)
{
  int from_fd=0,len=0,count=0;
  unsigned char  *ptr1 = NULL,*ptr2 = NULL;
  int Ret = 0;
  void * vaddr = NULL;

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

  vaddr=p2v_mem_mapping(GetLs3ASpiRegBaseAddr(),FLASH_SIZE);

  if(Ret>FLASH_SIZE)
  {
    printf("file size > flash size\n");
    return 1;
  }
  UpdateBiosInSpiFlash(0,ptr2,Ret,(U64)vaddr);
  p2v_mem_clean(vaddr);
}

U64 get_7a_spi_base_addr(void)
{
  return spi_base_addr;
}
void set_7a_spi_base_addr(U64 base_addr)
{
  spi_base_addr=base_addr;
}

void read_7a_spi(int offset, unsigned char * datas, int read_cnt) 
{
  void * vaddr = NULL;

  vaddr=p2v_mem_mapping(GetLs7ASpiRegBaseAddr(),FLASH_SIZE);
  SpiFlashRead ((U64)offset,(void *)datas,(U64)read_cnt,(U64)vaddr);
  p2v_mem_clean(vaddr);
}

void write_7a_spi(int offset, unsigned char * datas, int write_cnt)
{
  void * vaddr = NULL;
  vaddr=p2v_mem_mapping(GetLs7ASpiRegBaseAddr(),FLASH_SIZE);
  SpiFlashSafeWrite ((U64)offset, (void *)datas, (U64)write_cnt,(U64)vaddr);
  p2v_mem_clean(vaddr);
}
#define NOT_USED printf("%s not used\n", __FUNCTION__)
void app_spi_init(void)
{
  NOT_USED;
}

void app_spi_read(int spi_dev, unsigned char * datas, int read_cnt) 
{
}

void app_spi_write(int spi_dev, unsigned char * datas, int write_cnt)
{
}

