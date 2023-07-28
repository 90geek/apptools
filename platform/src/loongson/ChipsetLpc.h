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
#ifndef  __CHIPSET_LPC_H__
#define  __CHIPSET_LPC_H__

#include "LsRegDef.h"
#include "edk_api.h"
#include "mem.h"

#define   HT1_MEM_BASE              PHYS_TO_UNCACHED(0xe0000000000)
#define   CHIPSET_LPC_IO_BASE       PHYS_TO_UNCACHED(0xefdfc000000)
#define   CHIPSET_LPC_IOTPM_BASE    (CHIPSET_LPC_IO_BASE + 0x10000)
#define   CHIPSET_LPC_MEM_BASE      (HT1_MEM_BASE + 0x12000000)
#define   CHIPSET_LPC_CTL_BASE      (HT1_MEM_BASE + 0x10002000)

#define   MEM      0x1
#define   IO       0x2
#define   IOTPM    0x3

#define   LPC_SIO_UART_A            (CHIPSET_LPC_IO_BASE + 0x3f8)
#define   LPC_SIO_UART_B            (CHIPSET_LPC_IO_BASE + 0x2f8)
#define   LPC_SIO_UART_C            (CHIPSET_LPC_IO_BASE + 0x3e0)
#define   LPC_SIO_UART_D            (CHIPSET_LPC_IO_BASE + 0x2e0)
#define   LPC_SIO_UART_E            (CHIPSET_LPC_IO_BASE + 0x3e8)
#define   LPC_SIO_UART_F            (CHIPSET_LPC_IO_BASE + 0x2e8)

#define EFI_LS_LPC_PROTOCOL VOID
typedef enum {
  EfiDataWidthUint8  = 0x1,
  EfiDataWidthUint16 = 0x2,
  EfiDataWidthUint32 = 0x4,
  EfiDataWidthUint64 = 0x8
} EFI_LS_SERVICE_PROTOCOL_WIDTH;

EFI_STATUS
EFIAPI
ChipsetLpcMemRead (
  IN EFI_LS_LPC_PROTOCOL                *This,
  IN EFI_LS_SERVICE_PROTOCOL_WIDTH      Width,
  IN UINTN                              Offset,
  IN UINTN                              Count,
  IN OUT VOID                           *Buffer
  );

EFI_STATUS
EFIAPI
ChipsetLpcMemWrite (
  IN EFI_LS_LPC_PROTOCOL                *This,
  IN EFI_LS_SERVICE_PROTOCOL_WIDTH      Width,
  IN UINTN                              Offset,
  IN UINTN                              Count,
  IN OUT VOID                           *Buffer
  );

EFI_STATUS
EFIAPI
ChipsetLpcIoRead (
  IN EFI_LS_LPC_PROTOCOL                *This,
  IN EFI_LS_SERVICE_PROTOCOL_WIDTH      Width,
  IN UINTN                              Port,
  IN UINTN                              Count,
  IN OUT VOID                           *Buffer
  );

EFI_STATUS
EFIAPI
ChipsetLpcIoWrite (
  IN EFI_LS_LPC_PROTOCOL                *This,
  IN EFI_LS_SERVICE_PROTOCOL_WIDTH      Width,
  IN UINTN                              Port,
  IN UINTN                              Count,
  IN OUT VOID                           *Buffer
  );

EFI_STATUS
EFIAPI
ChipsetLpcIoTpmRead (
  IN EFI_LS_LPC_PROTOCOL                *This,
  IN EFI_LS_SERVICE_PROTOCOL_WIDTH      Width,
  IN UINTN                              Port,
  IN UINTN                              Count,
  IN OUT VOID                           *Buffer
  );

EFI_STATUS
EFIAPI
ChipsetLpcIoTpmWrite (
  IN EFI_LS_LPC_PROTOCOL                *This,
  IN EFI_LS_SERVICE_PROTOCOL_WIDTH      Width,
  IN UINTN                              Port,
  IN UINTN                              Count,
  IN OUT VOID                           *Buffer
  );

#endif
