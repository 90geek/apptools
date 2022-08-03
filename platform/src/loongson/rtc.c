#include "LsRegDef.h"
#include "edk_api.h"
#include "mem.h"
#include <stdio.h>

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

