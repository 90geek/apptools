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
#include "edk_api.h"
#include "debug.h"
#include "LsRegDef.h"
#include "CpuGpioLib.h"
#include "Gpio.h"
#include "mem.h"

/**
  This function is to set Cpu's Gpio to high level.

  @param[in] This           Point to the processor interface.
  @param[in] GpioNum        Gpio ID.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
GpioSetOutHigh (
  IN UINTN                                GpioBase,
  IN UINT8                                GpioNum
  )
{
  UINT32 Reg;

  void * vaddr = NULL;
  int memoffset=0;
  vaddr=p2v_mem_mapping(GpioBase,0x20, &memoffset);

  /*Set output High level*/
  Reg = Read32(vaddr + GPIO_IN_DATA_OFFSET);
  Reg |= (1 << GpioNum);
  Write32(vaddr + GPIO_OUTPUT_DATA_OFFSET, Reg);

  /* Enable output*/
  Reg = Read32(vaddr + GPIO_OUTPUT_ENABLE_OFFSET);
  Reg &= ~(1 << GpioNum);
  Write32(vaddr + GPIO_OUTPUT_ENABLE_OFFSET, Reg);

  /* Enable function*/
  Reg = Read32(vaddr + GPIO_FUNC_ENABLE_OFFSET);
  Reg &= ~(1 << GpioNum);
  Write32(vaddr + GPIO_OUTPUT_ENABLE_OFFSET, Reg);
  p2v_mem_clean(vaddr, memoffset);

  return EFI_SUCCESS;
}

/**
  This function is to set Cpu's Gpio to Low level.

  @param[in] This           Point to the processor interface.
  @param[in] GpioNum        Gpio ID.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
GpioSetOutLow (
  IN UINTN                         GpioBase,
  IN UINT8                         GpioNum
  )
{
  UINT32 Reg;
  void * vaddr = NULL;
  int memoffset=0;
  vaddr=p2v_mem_mapping(GpioBase,0x20, &memoffset);

  /*Set output low level*/
  Reg = Read32(vaddr + GPIO_IN_DATA_OFFSET);
  Reg &= ~(1 << GpioNum);
  Write32(vaddr + GPIO_OUTPUT_DATA_OFFSET, Reg);

  /* Enable output*/
  Reg = Read32(vaddr + GPIO_OUTPUT_ENABLE_OFFSET);
  Reg &= ~(1 << GpioNum);
  Write32(vaddr + GPIO_OUTPUT_ENABLE_OFFSET, Reg);

  /* Enable function*/
  Reg = Read32(vaddr + GPIO_FUNC_ENABLE_OFFSET);
  Reg &= ~(1 << GpioNum);
  Write32(vaddr + GPIO_OUTPUT_ENABLE_OFFSET, Reg);
  p2v_mem_clean(vaddr, memoffset);

  return EFI_SUCCESS;
}

/**
  This function is to set Cpu's Gpio to input mode.

  @param[in] This           Point to the processor interface.
  @param[in] GpioNum        Gpio ID.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
GpioSetInMode (
  IN UINTN                         GpioBase,
  IN UINT8                         GpioNum
  )
{
  UINT32 Reg;
  void * vaddr = NULL;
  int memoffset=0;
  vaddr=p2v_mem_mapping(GpioBase,0x20, &memoffset);

  /* Enable Input*/
  Reg = Read32(vaddr + GPIO_OUTPUT_ENABLE_OFFSET);
  Reg |= (1 << GpioNum);
  Write32(vaddr + GPIO_OUTPUT_ENABLE_OFFSET, Reg);

  /* Enable function*/
  Reg = Read32(vaddr + GPIO_FUNC_ENABLE_OFFSET);
  Reg &= ~(1 << GpioNum);
  Write32(vaddr + GPIO_OUTPUT_ENABLE_OFFSET, Reg);

  // Get Input Pin Status  return value is High or Low;
  Reg=(Read32 (vaddr + GPIO_IN_DATA_OFFSET) & (1 << GpioNum));
  p2v_mem_clean(vaddr, memoffset);

  return Reg;

}
