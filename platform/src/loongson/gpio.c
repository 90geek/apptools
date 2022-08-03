#include "LsRegDef.h"
#include "edk_api.h"
#include "mem.h"
#include <stdio.h>

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
while(1){
	//start
	a = 0x1000;
	while(a--){
	*(volatile unsigned int *)(p ) = 0xffffff;
	delay(0x20000);
	*(volatile unsigned int *)(p ) = 0x0;
	delay(0x20000);
	}
	sleep(1);
}
	printf("---------------GPio Set %s %d  \n",__func__,__LINE__);
	p2v_mem_clean(p, memoffset);
}

