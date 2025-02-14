#ifndef __SPI_H_
#define __SPI_H_

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
#include "platform/app_platform.h"
#include "platform/app_os.h"
#include "platform/app_spi.h"

#define SHOW_PROGRESS

#define GPIO_DATA_REG  PHYS_TO_UNCACHED(0x1fe0011c)
#define GPIO_CTRL_REG  PHYS_TO_UNCACHED(0x1fe00120)

#define GPIO_0        (0x1<<0)
#define GPIO_1        (0x1<<1)
#define GPIO_2        (0x1<<2)
#define GPIO_3        (0x1<<3)
#define GPIO_12       (0x1<<12)
#define GPIO_CS_BIT           GPIO_0
#define GPIO_CPU_CS_ENABLE    GPIO_3

#define SPE_OFFSET     6
#define MSTR_OFFSET    4
#define CPOL_OFFSET    3
#define CPHA_OFFSET    2
#define SPR_OFFSET     0
#define SPE            (1<<SPE_OFFSET)
#define MSTR           (1<<MSTR_OFFSET)

#define BP0    (1 << 2)
#define BP1    (1 << 3)
#define BP2    (1 << 4)
#define BP3    0

#define REG_SPCR   0x00
#define REG_SPSR   0x01
#define REG_SPDR   0x02
#define REG_SPER   0x03
#define REG_SOFTCS 0x05
#define REG_TIME   0x06
#define REG_PARAM  0x04
#define WREN       0x06
#define WRDI       0x04
#define RDID       0x90
#define RDSR       0x05
#define WRSR       0x01
#define READ       0x03
#define AAI        0x02
#define EBSY       0x70
#define DBSY       0x80
#define EWSR       0x50
#define FAST_READ  0x0B
#define BYTE_WRITE 0x02     /* Byte Program Enable */
#define AAI_WRITE  0xad     /* Byte Program Enable */
#define BE4K       0x20     /* 4K Byte block Rrase, Sector Erase */
#define BE4KSIZE   0x1000   /* 4K Byte block Rrase, Sector Erase */
#define BE32K      0x52     /* 32K Byte block Rrase, Sector Erase */
#define BE32KSIZE  0x8000   /* 32K Byte block Rrase, Sector Erase */
#define BE64K      0xD8     /* 64K Byte block Rrase, Sector Erase */
#define BE64KSIZE  0x10000  /* 64K Byte block Rrase, Sector Erase */
#define CHIPERASE  0xC7     /* Chip Erase */
#define BLKSIZE    0x1000
#define BLKERASE   BE4K
#define MACID_ADDR 0x00
#define DEVID_ADDR 0x01
#define VOID_ADDR  0x00
#define VOID_CMD   0x00
#define CHIPERASESET               0x4
#define CSCCHIPERASET              0x5
#define PROGRAM_AAI_MODE           0x1
#define SPI_FREE                   0x00
#define SPI_BUSY                   0x01

#define STORE8(Addr, Value)    *(volatile UINT8 *)((Addr)) = ((UINT8) (Value & 0xff))
#define LOAD8(Addr)            *(volatile UINT8 *)((Addr))
#define REGSET(Id,Value)       STORE8(SPI_REG_BASE + Id, Value)
#define REGGET(Id)             LOAD8(SPI_REG_BASE + Id)

#define LS3ASPIFLASHADDR     PHYS_TO_UNCACHED(0x1fe001f0)
#define LS7ASPIFLASHADDR     GetLs7ASpiRegBaseAddr()
#define SpiFlashDelayUs      //app_sleep_us

#define LS3APROC_PLL_REG     PHYS_TO_UNCACHED(0x1fe001d0)
#define LS3APROC_MISC_REG    PHYS_TO_UNCACHED(0x1fe00420)

UINTN
GetLs3ASpiRegBaseAddr(
  VOID
  );

UINTN
GetLs7ASpiRegBaseAddr(
  VOID
  );

VOID
UnlockFlashWriteProtected (
  UINTN      BaseRegAddr
  );

VOID
SpiFlashErase (
  UINTN      Offset,
  UINTN      Num,
  UINTN      BaseRegAddr
  );

UINTN
SpiFlashRead (
  UINTN      Offset,
  VOID       *Buffer,
  UINTN      Num,
  UINTN      BaseRegAddr
  );

VOID
SpiFlashWrite (
  UINTN        Offset,
  VOID         *Buffer,
  UINTN        Num,
  UINTN        BaseRegAddr
  );

VOID
SpiFlashSafeWrite (
  UINTN        Offset,
  VOID         *Buffer,
  UINTN        Num,
  UINTN        BaseRegAddr
  );

VOID
UpdateBiosInSpiFlash (
  UINTN      Offset,
  VOID       *Buffer,
  UINTN      Num,
  UINTN      BaseRegAddr
  );

VOID
SpiFlashSetCs (
  INTN Bit
  );

UINTN
SpiTcmRead (
  UINTN      Offset,
  VOID       *Buffer,
  UINTN      Num,
  UINTN      BaseRegAddr,
  UINT8      Cs
  );

UINT8
EFIAPI
TisRegRead8(
  IN  UINTN Addr
  );

UINT8
EFIAPI
TisRegWrite8 (
  IN UINTN Addr,
  IN UINT8  Data
  );

#endif
