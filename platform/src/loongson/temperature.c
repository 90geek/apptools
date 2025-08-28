/*
## @file
#
#  Copyright (c) 2018 Loongson Technology Corporation Limited (www.loongson.cn).
#  All intellectual property rights(Copyright, Patent and Trademark) reserved.
#
#  Any violations of copyright or other intellectual property rights of the Loongson Technology
#  Corporation Limited will be held accountable in accordance with the law,
#  if you (or any of your subsidiaries, corporate affiliates or agents) initiate
#  directly or indirectly any Intellectual Property Assertion or Intellectual Property Litigation:
#  (i) against Loongson Technology Corporation Limited or any of its subsidiaries or corporate affiliates,
#  (ii) against any party if such Intellectual Property Assertion or Intellectual Property Litigation arises
#  in whole or in part from any software, technology, product or service of Loongson Technology Corporation
#  Limited or any of its subsidiaries or corporate affiliates, or (iii) against any party relating to the Software.
#
#  THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
#  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR
#  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION).
#
#
##
*/
#include "LsRegDef.h"
#include "edk_api.h"
#include "mem.h"
#include "platform/app_platform.h"

UINT8 lscpu_tempdetect(UINT8 node, UINT32 *temp0,UINT32 *temp1, UINT32 *temp428)
{
	UINT32 Data = 0;
	UINT16 TempSensorDigital0,TempSensorDigital1; //UINT16
	INT8	 TempSensorAnalog0,TempSensorAnalog1;		//INT8 (-40 - 125)
	void * vaddr = NULL;
	int memoffset;
	UINT64 Base;
	Base= CPU_TEMP_428_SAMPLE_BASE | ((UINT64)node<<NODE_OFFSET);

  vaddr=p2v_mem_mapping(Base,4,&memoffset);
	if(vaddr==NULL)
		return EFI_LOAD_ERROR;
	Data = Read32((U64)vaddr);
	p2v_mem_clean(vaddr,memoffset);
	// printf("Cpu Temp reg428 %d\n",Data);
	if(temp428 != NULL)
		*temp428=Data;

	Base= CPU_TEMP_SAMPLE_BASE | ((UINT64)node<<NODE_OFFSET);
  vaddr=p2v_mem_mapping(Base +TEMP_SENSOR_VALUE_OFFSET,4,&memoffset);
	if(vaddr==NULL)
		return EFI_LOAD_ERROR;
	Data = Read32((U64)vaddr);
	p2v_mem_clean(vaddr,memoffset);

	TempSensorDigital0 = Data & 0xffff;
	TempSensorDigital1 = (Data & (0xffff << 16)) >> 16; //48-32 16
	if(CheckCpuName("3C6000/S") ||
		CheckCpuName("3C6000/D") ||
		CheckCpuName("3C6000/Q") ||
		CheckCpuName("3A6000") ||
		CheckCpuName("3B6000")
			)
	{
		TempSensorAnalog0 = TempSensorDigital0 * 820 / 0x4000 - 311 ;
		TempSensorAnalog1 = TempSensorDigital1 * 820 / 0x4000 - 311 ;
	}
	else
	{
		//Calculate Temp
		TempSensorAnalog0 = TempSensorDigital0 * 731 / 0x4000 - 273 ;
		TempSensorAnalog1 = TempSensorDigital1 * 731 / 0x4000 - 273 ;
	}
	// *(INT8 *)Temperature = (TempSensorAnalog0 + TempSensorAnalog1) / 2;
	if(temp0 != NULL)
		*temp0=TempSensorAnalog0;
	if(temp1 != NULL)
	{
		if(CheckCpu(LS2K2000_VERSION,0))
			*temp1=TempSensorAnalog0;
		else
			*temp1=TempSensorAnalog1;
	}
	return EFI_SUCCESS;
}

UINT8 ls7a_tempdetect(UINT32 *temp0)
{
	UINT32 Data = 0;
	void * vaddr = NULL;
	int memoffset,i;
	UINT32 ConfReg,ConfRegSet;
	INT8	TempSensorAnalog1[5];		//INT8 (-40 - 125)
	INT32	TempAll=0;

  vaddr=p2v_mem_mapping(LS7A_TEMP_SAMPLE_BASE,0x20,&memoffset);
	if(vaddr==NULL)
		return EFI_LOAD_ERROR;
	ConfReg = Read32((U64)vaddr);
	for(i=0x0;i<=4;i++)
	{
		ConfRegSet = ConfReg & ~(0x3 << 2) | (i << 4) | 0x3;
		Write32((U64)vaddr, ConfReg);
		sleep(1);
		Data = Read32((U64)vaddr+0x14);
		if((Data>>14)&1)
			printf("overfolw\n");
		else
			Data=Data & 0x3fff;
		TempSensorAnalog1[i] = Data * 731 / 0x4000 - 273 ;
		printf("TempSensorAnalog1[%d]=%d\n",i,TempSensorAnalog1[i]);
		TempAll+=TempSensorAnalog1[i];
	}
	p2v_mem_clean(vaddr,memoffset);

	*temp0 = TempAll/5;
	return EFI_SUCCESS;
}
