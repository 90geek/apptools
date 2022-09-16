#include "ls7a.h"
#include "edk_api.h"
#include "LsRegDef.h"
#include "mem.h"
#include "platform/app_os.h"

void delay (unsigned long a)
{
		while(a--);
}

void GpioReadOps(void)
{
	void * p = NULL;
	int status ;
	int memoffset;

	p = p2v_mem_mapping(LS7A_GPIO_REG_OUT_BASE,4096, &memoffset);
	printf("mmap addr start : %p \n",p);
	/*Debug Gpio*/
	printf("Gpio Reg Read Start ...\n");
	int i = 0;
	unsigned char j = 0;
	unsigned int tmp_tmp = 0;
	printf(" %s %d	\n",__func__,__LINE__);

	/*Set Gpio*/
	*(volatile unsigned int *)(p + 4) = 0x0;
	int a = 0x1000;
// while(1){
	//start
	a = 0x1000;
	while(a--){
	*(volatile unsigned int *)(p ) = 0xffffff;
	delay(0x20000);
	*(volatile unsigned int *)(p ) = 0x0;
	delay(0x20000);
	}
	sleep(1);
// }
	printf("---------------GPio Set %s %d  \n",__func__,__LINE__);
	p2v_mem_clean(p, memoffset);
}

void RtcReadOps(void)
{
	void * p = NULL;
	int status ;
	int memoffset;

	/*Transfer Virtul to Phy Addr*/
	p = p2v_mem_mapping(LS7A_RTC_REG_BASE,15*4, &memoffset);
	/*Debug Rtc*/
	printf("Rtc Reg Read Start ...\n");
	int i = 0;
	unsigned char j = 0;
	unsigned int tmp_tmp = 0;
#if 1
	unsigned char regbuf[] = {0x20,0x24,0x28,0x2c,0x30,0x34,0x38,0x3c,0x40,0x60,0x64,0x68,0x6c,0x70,0x74};
	unsigned char buflen = sizeof(regbuf);
	for(i = 0,j = 0; i < buflen; i++){
		j=regbuf[i];
		tmp_tmp = (*(volatile unsigned int *)(p + j));
		printf("RegNum:%x		 RegVal:%x \n",j,tmp_tmp);
	}
#endif
		p2v_mem_clean(p, memoffset);
}
void AcpiReadOps(void)
{
	void * p = NULL;
	int status ;
	int memoffset;

	p = p2v_mem_mapping(LS7A_ACPI_BASE_ADDR,4096, &memoffset);
	printf("mmap addr start : %p \n",p);
	/*Debug ACPI*/
	int i = 0;
	unsigned int tmp_tmp = 0;
	for(i = 0; i< 0x50 ; i=i+4){
		tmp_tmp = (*(volatile unsigned int *)(p + i));
		printf("RegNum:%x		 RegVal:%x \n",i,tmp_tmp);
	}

	/*Debug ACPI*/
	for(i=0;i<5;i++){
		tmp_tmp = (*(volatile unsigned int *)(p + 0xc));
		printf("AcpiStatus..	RegNum:0xc		RegVal:%x \n",tmp_tmp);
		(*(volatile unsigned int *)(p + 0xc)) |= tmp_tmp;
		tmp_tmp = (*(volatile unsigned int *)(p + 0x28));
		printf("AcpiStatus..	RegNum:0x28		 RegVal:%x \n",tmp_tmp);
		(*(volatile unsigned int *)(p + 0x28)) |= tmp_tmp;
		printf("AcpiStatus.. Clear Event Status \n");
	}
		p2v_mem_clean(p, memoffset);
		printf("--------------Release mem Map----------------\n");
}
void AcpiReboot(void)
{
	char * p = NULL;
	int status ;
	int memoffset;

	p = p2v_mem_mapping(LS7A_RTC_REG_BASE,8, &memoffset);
	Writeb (p + 0x30, 0x1);
	p2v_mem_clean(p, memoffset);
}

void PciReadOps(UINT32 a)
{
  void * p = NULL;
  int status ;
	int memoffset;

	p = p2v_mem_mapping(LS7A_RTC_REG_BASE,4096, &memoffset);
  // p = vtpa(this->devaddr,fd);
  printf("Read Pci Bar Mem Start, Please Input Address offset composed of Bus,dev,func: " );
  printf(" Auto Read Pci(Bus:%d Device:%d Function:%d) 0x10 Bar Mem Start, 0x%lx \n",(a>>16),((a>>11)&0x1f),((a>>8)&7), *(volatile unsigned long *)(p + a + 0x10));
  printf(" Auto Read Pci(Bus:%d Device:%d Function:%d) 0x10 Bar Mem Start, 0x%lx \n",(a>>16),((a>>11)&0x1f),((a>>8)&7), *(volatile unsigned long *)(p + a + 0x4));
	p2v_mem_clean(p, memoffset);
}
