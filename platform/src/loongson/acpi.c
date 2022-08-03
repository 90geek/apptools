#include "acpi.h"
#include "edk_api.h"
#include "LsRegDef.h"
#include "mem.h"
#include <stdio.h>

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

