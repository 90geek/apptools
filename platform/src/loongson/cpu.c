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
#include "loongson3_def.h"

#define LS3A_CPU_FREQ_CONFIG_BASE  PHYS_TO_UNCACHED(0x1fe001b0)  //Frequency configuration register
#define LS2K_CPU_FREQ_CONFIG_BASE  PHYS_TO_UNCACHED(0x100104a0)  //Frequency configuration register

void CpuIdToCpuName(CpuId Value, UINT8* Output) {
	UINT8 i;
	UINT8 CpuIdArray[16]={0};
	for (i = 0; i < 8; i++) {
			CpuIdArray[i] = (Value.l >> (8 * i)) & 0xFF;
	}
	for (; i < 16; i++) {
			CpuIdArray[i] = (Value.h >> (8 * i)) & 0xFF;
	}
	app_ascii_to_str((char *)CpuIdArray,sizeof(CpuIdArray) , Output);
}

void CpuNameToCpuId(UINT8* CpuName, CpuId *Output) {
	UINT8 i;
	UINT8 CpuIdArray[16]={0};
	app_str_to_ascii(CpuName, CpuIdArray);
	Output->l=*(UINT64 *)CpuIdArray;
	Output->h=*(UINT64 *)(CpuIdArray + 8);
}

/**
	This module is Get the operating frequency of the processor.

	@param[in] This						Point to the processor interface.
	@param[in] Buffer					Read processor frequency to buffer.

	@retval EFI_SUCCESS				The entry point is executed successfully.
	@retval other							Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
CpuGetFrequency (
	IN UINT32	clk_ref,
	OUT VOID				*Frequency //&UINT32 Mhz
	)
{
	UINT32 Data = 0;
	UINT64 CpuFre = 0;
	UINT64 CoreLoopc = 0;
	UINT64 CoreDiv = 0;
	UINT64 DivRefc = 0;
	void * vaddr = NULL;
	int memoffset=0;

	if(CheckCpu(LS2K2000_VERSION,0))
	{
		 vaddr=p2v_mem_mapping(LS2K_CPU_FREQ_CONFIG_BASE,4, &memoffset);
		 if(vaddr==NULL)
			return EFI_LOAD_ERROR;

		 Data = Read32((U64)vaddr);
		 CoreDiv = (Data & 0x3f80) >> 7;//L1 PLL PARAM: DIV_OUT
		 CoreLoopc = (Data & 0x3fec0000) >> 21;//L1 PLL PARAM: DIV_LOOPC

		 Data = Read32((U64)vaddr + 0x4);
		 DivRefc = (Data & 0x7f); //L1 PLL PARAM: DIV_REFC
		 p2v_mem_clean(vaddr, memoffset);
	}
	else
	{
		 vaddr=p2v_mem_mapping(LS3A_CPU_FREQ_CONFIG_BASE,4, &memoffset);
		 if(vaddr==NULL)
			return EFI_LOAD_ERROR;

		 Data = Read32((U64)vaddr);
		 DivRefc = (Data & 0xfc000000) >> 26; //L1 PLL PARAM: DIV_REFC
		 Data = Read32((U64)vaddr + 0x4);
		 p2v_mem_clean(vaddr, memoffset);
		 CoreLoopc = (Data & 0x1ff);//L1 PLL PARAM: DIV_LOOPC

		 CoreDiv = (Data & 0xfc00) >> 10;//L1 PLL PARAM: DIV_OUT
	}

	CpuFre = (clk_ref / DivRefc * CoreLoopc * 1000)/(CoreDiv);//CLK_REF default 100MHZ

	*(UINT32*)Frequency = CpuFre / 1000000;

	 return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
GetCpuId (
	CpuId *CpuId
)
{
	void * vaddr = NULL;
	int memoffset=0;

	 vaddr=p2v_mem_mapping(LSCPU_ID,16, &memoffset);
	 if(vaddr==NULL)
		return EFI_LOAD_ERROR;

	 CpuId->l = Read64((U64)vaddr);
	 CpuId->h = Read64((U64)vaddr + 0x8);
	 p2v_mem_clean(vaddr, memoffset);
	 return EFI_SUCCESS;
}
UINT8 GetCpuInfo(
	VOID
)
{
	CpuId CpuId,ToCpuId;
	EFI_STATUS Status;
	CHAR8 CpuName[20]={0};
	
	Status = GetCpuId(&CpuId);
	if(Status!=EFI_SUCCESS)
		return 0;
	printf("CpuId.l 0x%llx\n",CpuId.l);
	printf("CpuId.h 0x%llx\n",CpuId.h);
	CpuIdToCpuName(CpuId, CpuName);
	printf("CPU Name:%s\n", CpuName);
	CpuNameToCpuId(CpuName, &ToCpuId);
	printf("CpuName to CpuId.l 0x%llx\n",ToCpuId.l);
	printf("CpuName to CpuId.h 0x%llx\n",ToCpuId.h);
	return 0;
}
//
// check cpu id
//
UINT8 CheckCpuName(
	CHAR8 *CpuName
)
{
	CpuId CpuId,ToCpuId;
	EFI_STATUS Status;
	
	CpuNameToCpuId(CpuName, &ToCpuId);
	Status = GetCpuId(&CpuId);
	if(Status!=EFI_SUCCESS)
		return 0;

	if(CpuId.l==ToCpuId.l&&	\
		CpuId.h==ToCpuId.h)
	{
		return 1;
	}
	return 0;
}
//
// check cpu id
//
UINT8 CheckCpu(
	UINT64 cpuid_l,
	UINT64 cpuid_h
)
{
	CpuId CpuId;
	EFI_STATUS Status;
	
	Status = GetCpuId(&CpuId);
	if(Status!=EFI_SUCCESS)
		return 0;

	if(CpuId.l==cpuid_l&&	\
		CpuId.h==cpuid_h)
	{
		return 1;
	}
	return 0;
}
