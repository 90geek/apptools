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

UINT64 TempRegBaseAddr;

UINT8 lscpu_tempdetect(UINT32 *temp0,UINT32 *temp1)
{
	UINT32 Data = 0;
	UINT16 TempSensorDigital0,TempSensorDigital1; //UINT16
	INT8	 TempSensorAnalog0,TempSensorAnalog1;		//INT8 (-40 - 125)
	void * vaddr = NULL;
	int memoffset;

  vaddr=p2v_mem_mapping(CPU_TEMP_SAMPLE_BASE +TEMP_SENSOR_VALUE_OFFSET,4,&memoffset);
	if(vaddr==NULL)
		return EFI_LOAD_ERROR;
	Data = Read32((U64)vaddr);
	p2v_mem_clean(vaddr,memoffset);

	TempSensorDigital0 = Data & 0xffff;
	TempSensorDigital1 = (Data & (0xffff << 16)) >> 16; //48-32 16
	//Calculate Temp
	TempSensorAnalog0 = TempSensorDigital0 * 731 / 0x4000 - 273 ;
	TempSensorAnalog1 = TempSensorDigital1 * 731 / 0x4000 - 273 ;
	// *(INT8 *)Temperature = (TempSensorAnalog0 + TempSensorAnalog1) / 2;
	*temp0=TempSensorAnalog0;
	*temp1=TempSensorAnalog1;

	return EFI_SUCCESS;
}

