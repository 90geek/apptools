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
#include "spi.h"
#include "loongson3_def.h"
#ifdef SHOW_PROGRESS
#include "platform/app_os.h"
#endif

#define INVALID_OFFSET(x)     ((x > 0x800000)||(x < 0x0)) ? TRUE:FALSE
#define TCM_INVALID_OFFSET(x)     ((x > 0xffffffff)||(x < 0x0)) ? TRUE:FALSE
#define INVALID_NUM(x)        ((x > 0x800000)||( x <= 0x0)) ? TRUE:FALSE
#define IS_SST25VF032B(M,D,C) ((M == 0xBF)&&(D == 0x25)&&(C == 0x4A)) ? TRUE:FALSE

STATIC UINT8 ValueRegSpcr  = 0xFF;
STATIC UINT8 ValueRegSpsr  = 0xFF;
STATIC UINT8 ValueRegSper  = 0xFF;
STATIC UINT8 ValueRegParam = 0xFF;
STATIC UINT64 SPI_REG_BASE = 0x0;

VOID
SpiFlashSetCs (
  INTN Bit
  )
{
  UINT8          Val;

  if (Bit)
    Val = 0x11;
  else
    Val = 0x01;
  REGSET(CSCCHIPERASET,Val);

  SpiFlashDelayUs (3);
}

VOID
SpiSetCs1 (
  INTN Bit
  )
{
  UINT8          Val;

  if (Bit)
    Val = 0x22;
  else
    Val = 0x02;
  REGSET(CSCCHIPERASET,Val);

  SpiFlashDelayUs (3);
}

VOID
SpiCs (
  UINT8 Cs,
  INTN Bit
  )
{

  if (Cs == 0)
    SpiFlashSetCs (Bit);
  else
    SpiSetCs1 (Bit);
}

VOID ResetSfcParamReg()
{
  if(CheckCpu(LS3C5000_VERSION_LO,LS3C5000_VERSION_HI) ||
     CheckCpu(LS3C5000_LL_VERSION_LO,LS3C5000_LL_VERSION_HI) ||
     CheckCpu(LS3D5000_VERSION_LO,LS3D5000_VERSION_HI)
      )
    REGSET(REG_PARAM,0x27);
  else
    REGSET(REG_PARAM,0x47);
}

UINT8
SpiFlashWriteByteCmd (
  UINT8 Value
  )
{
  UINT8 Ret;
  INT32 TimeOut = 100000;

  REGSET (REG_SPDR, Value);
  while ((REGGET(REG_SPSR) & 0x01) && TimeOut--);
  Ret = REGGET(REG_SPDR);
  if (TimeOut < 0)
    ASSERT (0);

  return Ret;
}

UINT8
SpiFlashReadByteCmd (
  VOID
  )
{
  return SpiFlashWriteByteCmd(0x00);
}

VOID
SpiFlashInit (
  VOID
  )
{
  if (ValueRegSpcr == 0xFF) {
    ValueRegSpcr = REGGET(REG_SPCR);
  }
  if (ValueRegSpsr == 0xFF) {
    ValueRegSpsr = REGGET(REG_SPSR);
  }
  if (ValueRegSper == 0xFF) {
    ValueRegSper = REGGET(REG_SPER);
  }
  if (ValueRegParam == 0xFF) {
    ValueRegParam = REGGET(REG_PARAM);
  }

  if(CheckCpu(LS3C5000_VERSION_LO,LS3C5000_VERSION_HI) ||
     CheckCpu(LS3C5000_LL_VERSION_LO,LS3C5000_LL_VERSION_HI) ||
     CheckCpu(LS3D5000_VERSION_LO,LS3D5000_VERSION_HI)
      )
  {
    //[spre:spr] [01:00] means clk_div=8
    REGSET(REG_SPCR, 0x52);//[1:0] [0:0]spr
    REGSET(REG_SPSR, 0xc0);
    REGSET(REG_SPER, 0x04);//[1:0] [0:1]spre
    REGSET(REG_PARAM, 0x20);
    REGSET(REG_TIME, 0x01);
  }
  else
  {
    //[spre:spr] [01:00] means clk_div=8
    REGSET(REG_SPCR, 0x50);//[1:0] [0:0]spr
    REGSET(REG_SPSR, 0xc0);
    REGSET(REG_SPER, 0x05);//[1:0] [0:1]spre
    REGSET(REG_PARAM, 0x40);
    REGSET(REG_TIME, 0x01);
  }
}

VOID
SpiFlashReset (
  VOID
  )
{

  REGSET(REG_SPCR, ValueRegSpcr);
  REGSET(REG_SPSR, ValueRegSpsr);
  REGSET(REG_SPER, ValueRegSper);
  REGSET(REG_PARAM, ValueRegParam);

  ValueRegSpcr  = 0xFF;
  ValueRegSpsr  = 0xFF;
  ValueRegSper  = 0xFF;
  ValueRegParam = 0xFF;
}

UINT8 SpiFlashReadStatus(VOID)
{
  UINT8 Val;

  REGSET(REG_SOFTCS,0x01);
  SpiFlashWriteByteCmd(RDSR);
  Val = SpiFlashReadByteCmd();
  REGSET(REG_SOFTCS,0x11);

  return Val;
}

UINTN
SpiFlashWait (
  VOID
  )
{
  UINTN Ret;
  INT32 TimeOut = 100000;
  INT32 Count = 5;
  do {
    Ret = SpiFlashReadStatus();
    if (TimeOut == 0)
    {
      Count--;
      if (Count < 0 )
        ASSERT (0);
      TimeOut = 100000;
      SpiFlashDelayUs(2);
      DEBUG((DEBUG_INFO, "TimeOut Count is %d, Status Reg=0x%x\n",Count,Ret));
    }
  } while ((Ret & 1) && TimeOut--);

  return Ret;
}

VOID SpiFlashRDID(UINT8 *Manu,UINT8 *Device,UINT8 *Capa)
{
  SpiFlashWait();

  REGSET(REG_SOFTCS,0x01);
  SpiFlashWriteByteCmd(0x9f);

  *Manu = SpiFlashReadByteCmd();
  //DbgPrint(DEBUG_INFO,"Manufacturer's ID:0x%x\n",*Manu);

  *Device = SpiFlashReadByteCmd();
  //DbgPrint(DEBUG_INFO,"Device ID-memory_type:0x%x\n",*Device);

  *Capa = SpiFlashReadByteCmd();
  //DbgPrint(DEBUG_INFO,"Device ID-memory_capacity:0x%x\n",*Capa);

  REGSET(REG_SOFTCS,0x11);
}

VOID SpiFlashWriteEnable(VOID)
{
  SpiFlashWait();
  REGSET(REG_SOFTCS,0x01);
  SpiFlashWriteByteCmd(WREN);
  REGSET(REG_SOFTCS,0x11);
}

VOID SpiFlashEWRS(VOID)
{
  SpiFlashWait();
  REGSET(REG_SOFTCS,0x01);
  SpiFlashWriteByteCmd(EWSR);
  REGSET(REG_SOFTCS,0x11);
}

VOID SpiFlashWriteDisable(VOID)
{
  SpiFlashWait();
  REGSET(REG_SOFTCS,0x01);
  SpiFlashWriteByteCmd(WRDI);
  REGSET(REG_SOFTCS,0x11);
}

VOID SpiFlashWriteStatus(UINT8 Val)
{
  SpiFlashEWRS();
  REGSET(REG_SOFTCS,0x01);
  SpiFlashWriteByteCmd(WRSR);
  SpiFlashWriteByteCmd(Val);
  REGSET(REG_SOFTCS,0x11);

  SpiFlashWriteDisable();
}

VOID
SpiFlashDisableWriteProtection (
  VOID
  )
{

  UINT8 Val;

  Val = SpiFlashWait ();
  Val &= ~(BP0 | BP1 | BP2);
  SpiFlashWriteStatus(Val);
  SpiFlashWait ();

#if ENABLE_WRITE_PROTECTION
  /*some flash not support 0x50 need the follow code*/
  SpiFlashSetCs (0);
  SpiFlashWriteByteCmd (WREN);
  SpiFlashSetCs (1);
  SpiFlashSetCs (0);
  SpiFlashWriteByteCmd (WRSR);
  SpiFlashWriteByteCmd (Val);
  SpiFlashSetCs (1);
  SpiFlashWait ();
  SpiFlashSetCs (0);
  SpiFlashWriteByteCmd (WREN);
  SpiFlashSetCs (1);
#endif
}

VOID
SpiFlashEnableWriteProtection (
  VOID
  )
{
  UINT8 Val;

  Val = (BP0 | BP1 | BP2);
  SpiFlashWriteStatus(Val);
  SpiFlashWait ();
#if ENABLE_WRITE_PROTECTION
  /*some flash not support 0x50 need the follow code*/
  SpiFlashSetCs (0);
  SpiFlashWriteByteCmd (WREN);
  SpiFlashSetCs (1);
  SpiFlashSetCs (0);
  SpiFlashWriteByteCmd (WRSR);
  SpiFlashWriteByteCmd (Val);
  SpiFlashSetCs (1);
  SpiFlashWait ();

  SpiFlashSetCs (0);
  SpiFlashWriteByteCmd (WRDI);
  SpiFlashSetCs (1);
  /*wait command executed done*/
  SpiFlashWait();
#endif
}

VOID
SpiFlashEraseBlock (
  UINTN  Addr0,
  UINTN  Addr1,
  UINTN  Addr2
  )
{
  SpiFlashSetCs (0);
  SpiFlashWriteByteCmd (WREN);
  SpiFlashSetCs (1);
  SpiFlashWait();

  SpiFlashSetCs (0);
  SpiFlashWriteByteCmd (BLKERASE);
  SpiFlashWriteByteCmd (Addr2);
  SpiFlashWriteByteCmd (Addr1);
  SpiFlashWriteByteCmd (Addr0);
  SpiFlashSetCs (1);
}

VOID
SpiFlashWriteByte (
  UINTN  Addr0,
  UINTN  Addr1,
  UINTN  Addr2,
  UINT8  Buf
  )
{
  SpiFlashSetCs (0);
  SpiFlashWriteByteCmd (WREN);
  SpiFlashSetCs (1);

  SpiFlashSetCs (0);
  SpiFlashWriteByteCmd (BYTE_WRITE);
  SpiFlashWriteByteCmd (Addr2);
  SpiFlashWriteByteCmd (Addr1);
  SpiFlashWriteByteCmd (Addr0);
  SpiFlashWriteByteCmd (Buf);
  SpiFlashSetCs (1);
  SpiFlashWait ();
}

VOID
SpiFlashSpiEraseBlocks (
  UINTN  Off,
  UINTN  Num
  )
{
  UINTN  Offset = Off;
  UINTN  Addr0;
  UINTN  Addr1;
  UINTN  Addr2;

  do {
    Addr0 =  Offset & 0xff;
    Addr1 =  (Offset >> 8) & 0xff;
    Addr2 =  (Offset >> 16) & 0xff;

    SpiFlashEraseBlock (Addr0, Addr1, Addr2);

    Offset += BLKSIZE;
    SpiFlashWait();
  } while (Offset < Num + Off);
}

VOID
SpiFlashSpiWriteBuffer (
  UINTN    Offset,
  VOID     *Buffer,
  UINTN    Num
  )
{
  UINTN  Pos = Offset;
  UINT8  *Buf = (UINT8 *) Buffer;
  UINTN  Index = 0;
  UINTN  Addr0;
  UINTN  Addr1;
  UINTN  Addr2;

  while (Index < Num) {
    Addr0 = (Pos + Index) & 0xff;
    Addr1 = ((Pos + Index) >> 8) & 0xff;
    Addr2 = ((Pos + Index) >> 16) & 0xff;

    SpiFlashWriteByte (Addr0, Addr1, Addr2, Buf[Index]);
    Index++;
  }
}

#ifdef SHOW_PROGRESS
VOID
SpiFlashEraseAndWriteBlocksShow (
  UINTN      Offset,
  VOID       *Buffer,
  UINTN      Num
  )
{
  UINTN  Pos = Offset;
  UINT8  *Buf = (UINT8 *) Buffer;
  UINTN  Index = 0;
  UINTN  Addr0;
  UINTN  Addr1;
  UINTN  Addr2;

  /* Erase blocks step */
  Print("Erase   : \n");
  app_ProgressInit();
  do {
    if ((Pos % (4 * BLKSIZE)) == 0) {
      app_ProgressShow(Pos * 100 / Num);
      fflush(stdout);
    }
    Addr0 =  Pos & 0xff;
    Addr1 =  (Pos >> 8) & 0xff;
    Addr2 =  (Pos >> 16) & 0xff;

    SpiFlashEraseBlock (Addr0, Addr1, Addr2);
    SpiFlashDelayUs(1);
    Pos += BLKSIZE;
    SpiFlashWait();
  } while (Pos < Num + Offset);
  app_ProgressDone();

  /* Write blocks step */
  Pos = Offset;
  Print("Program : \n");
  app_ProgressInit();
  while (Index < Num) {
    if ((Index % 0x4000) == 0) {
      app_ProgressShow(Index * 100 / Num);
      fflush(stdout);
    }
    Addr0 = (Pos + Index) & 0xff;
    Addr1 = ((Pos + Index) >> 8) & 0xff;
    Addr2 = ((Pos + Index) >> 16) & 0xff;

    SpiFlashWriteByte (Addr0, Addr1, Addr2, Buf[Index]);
    Index++;
  }
  app_ProgressDone();
}
#endif

VOID
SpiFlashEraseAndWriteBlocks (
  UINTN      Offset,
  VOID       *Buffer,
  UINTN      Num
  )
{
  UINTN  Pos = Offset;
  UINT8  *Buf = (UINT8 *) Buffer;
  UINTN  Index = 0;
  UINTN  Addr0;
  UINTN  Addr1;
  UINTN  Addr2;

  /* Erase blocks step */
  Print("Erase   : ");
  do {
    if ((Pos % (4 * BLKSIZE)) == 0) {
      Print("*");
      fflush(stdout);
    }
    Addr0 =  Pos & 0xff;
    Addr1 =  (Pos >> 8) & 0xff;
    Addr2 =  (Pos >> 16) & 0xff;

    SpiFlashEraseBlock (Addr0, Addr1, Addr2);
    SpiFlashDelayUs(1);
    Pos += BLKSIZE;
    SpiFlashWait();
  } while (Pos < Num + Offset);
  Print("   Erase OK.\n");

  /* Write blocks step */
  Pos = Offset;
  Print("Program : ");
  while (Index < Num) {
    if ((Index % 0x4000) == 0) {
      Print("*");
      fflush(stdout);
    }
    Addr0 = (Pos + Index) & 0xff;
    Addr1 = ((Pos + Index) >> 8) & 0xff;
    Addr2 = ((Pos + Index) >> 16) & 0xff;

    SpiFlashWriteByte (Addr0, Addr1, Addr2, Buf[Index]);
    Index++;
  }
  Print("   Program OK.\n");
}

UINTN
SpiFlashSpiReadBuffer (
  UINTN  Offset,
  VOID  *Buffer,
  UINTN  Num
  )
{
  UINTN   Pos = Offset;
  UINT8  *Buf = (UINT8 *) Buffer;
  UINTN   Index;

  SpiFlashSetCs (0);
  SpiFlashWriteByteCmd (FAST_READ);
  SpiFlashWriteByteCmd ((Pos >> 16) & 0xff);
  SpiFlashWriteByteCmd ((Pos >> 8) & 0xff);
  SpiFlashWriteByteCmd (Pos & 0xff);
  SpiFlashWriteByteCmd (0);
  for (Index = 0; Index < Num; Index++)
    Buf[Index] = SpiFlashReadByteCmd ();

  SpiFlashSetCs (1);

  return Index;
}

UINTN
GetLs3ASpiRegBaseAddr(
  VOID
  )
{
  return LS3ASPIFLASHADDR;
}

UINTN
GetLs7ASpiRegBaseAddr(
  VOID
  )
{
  UINTN BaseAddr;

  // BaseAddr = *(volatile UINT32 *)(PHYS_TO_UNCACHED(0xefdfe000010) | (22 << 11));
  // BaseAddr &= 0xfffffff0;
  // BaseAddr |=UNCACHED_MEMORY_ADDR;
  BaseAddr=get_7a_spi_base_addr();
  return BaseAddr;
}

VOID
SpiFlashSetRegBase (
  UINTN BaseRegAddr
  )
{
  SPI_REG_BASE = BaseRegAddr;
}

VOID
SpiFlashErase (
  UINTN      Offset,
  UINTN      Num,
  UINTN      BaseRegAddr
  )
{
  if((INVALID_OFFSET(Offset)) || INVALID_NUM(Num)){
    ASSERT(0);
  }

  SpiFlashSetRegBase(BaseRegAddr);
  SpiFlashInit ();
  SpiFlashDisableWriteProtection ();
  SpiFlashSpiEraseBlocks (Offset, Num);
  SpiFlashEnableWriteProtection ();
  SpiFlashReset ();
  ResetSfcParamReg();
}

UINTN
SpiFlashRead (
  UINTN      Offset,
  VOID       *Buffer,
  UINTN      Num,
  UINTN      BaseRegAddr
  )
{
  UINTN Ret;

  if(!Buffer ||(INVALID_OFFSET(Offset)) || INVALID_NUM(Num)){
    ASSERT(0);
  }

  SpiFlashSetRegBase(BaseRegAddr);
  SpiFlashInit ();
  Ret = SpiFlashSpiReadBuffer (Offset, Buffer, Num);
  SpiFlashReset ();
  ResetSfcParamReg();

  return Ret;
}

VOID
SpiFlashWrite (
  UINTN        Offset,
  VOID         *Buffer,
  UINTN        Num,
  UINTN        BaseRegAddr
  )
{
  if(!Buffer ||(INVALID_OFFSET(Offset)) || INVALID_NUM(Num)){
    ASSERT(0);
  }

  SpiFlashSetRegBase(BaseRegAddr);
  SpiFlashInit ();
  SpiFlashDisableWriteProtection ();
  SpiFlashSpiWriteBuffer (Offset, Buffer, Num);
  SpiFlashEnableWriteProtection ();
  SpiFlashReset ();
  ResetSfcParamReg();
}

VOID
UnlockFlashWriteProtected (
  UINTN        BaseRegAddr
  )
{

  SpiFlashSetRegBase(BaseRegAddr);
  SpiFlashInit ();
  SpiFlashDisableWriteProtection ();
  SpiFlashReset ();
  ResetSfcParamReg();
}

VOID
UpdateBiosInSpiFlash (
  UINTN      Offset,
  VOID       *Buffer,
  UINTN      Num,
  UINTN      BaseRegAddr
  )
{
  if(!Buffer ||(INVALID_OFFSET(Offset)) || INVALID_NUM(Num)){
    ASSERT(0);
  }

  // UINT16 PllRegVal = Read16(LS3APROC_PLL_REG);
  // Write16(LS3APROC_PLL_REG, (0x7777 ^ (1 << ((BOOTCORE_ID << 2) + 3))) & PllRegVal);//shutdown slave core
  // UINTN MiscRegVal = Read64(LS3APROC_MISC_REG);
  // if (MiscRegVal & (1 << 8))
  //   Write64(LS3APROC_MISC_REG, (~(1 << 8)) & MiscRegVal);//shutdown 132

  SpiFlashSetRegBase(BaseRegAddr);
  SpiFlashInit ();
  SpiFlashDisableWriteProtection ();
#ifdef SHOW_PROGRESS
  SpiFlashEraseAndWriteBlocksShow (Offset, Buffer, Num);
#else
  SpiFlashEraseAndWriteBlocks (Offset, Buffer, Num);
#endif
  SpiFlashEnableWriteProtection ();
  SpiFlashReset ();
  ResetSfcParamReg();

  // Write16(LS3APROC_PLL_REG, PllRegVal);
  // if (MiscRegVal & (1 << 8))
  //   Write64(LS3APROC_MISC_REG, MiscRegVal);
}

VOID
SpiFlashSafeWrite (
  UINTN        Offset,
  VOID         *Buffer,
  UINTN        Num,
  UINTN        BaseRegAddr
  )
{
  UINT64 SectorStart;
  UINT64 SectorNum;
  UINT8  *Buff;

  if(!Buffer ||(INVALID_OFFSET(Offset)) || INVALID_NUM(Num)){
    ASSERT(0);
  }

  SectorStart = Offset / BLKSIZE;
  SectorNum   = ((Offset + Num - 1) / BLKSIZE) - (Offset / BLKSIZE) + 1;

  Buff = AllocateZeroPool(SectorNum*BLKSIZE);

  if(!Buff){
    ASSERT(0);
  }

  SpiFlashRead(SectorStart*BLKSIZE,Buff,SectorNum*BLKSIZE,BaseRegAddr);
  CopyMem(&Buff[Offset%BLKSIZE],Buffer,Num);
  SpiFlashErase(Offset,Num,BaseRegAddr);
  SpiFlashWrite(SectorStart*BLKSIZE,Buff,SectorNum*BLKSIZE,BaseRegAddr);
  FreePool(Buff);
}

UINTN
SpiTcmRead (
  UINTN      Offset,
  VOID       *Buffer,
  UINTN      Num,
  UINTN      BaseRegAddr,
  UINT8      Cs
  )
{
  // UINTN Ret;
  UINTN      addr=Offset;
  UINTN      count=0;
  UINT8      data[4]={0};

  if(!Buffer ||(TCM_INVALID_OFFSET(Offset)) || INVALID_NUM(Num)){
    // ASSERT(0);
    return 0;
  }
  // DEBUG((DEBUG_INFO,"func %a,BaseRegAddr=0x%lx, offset=0x%x,num=%d\n",__FUNCTION__,BaseRegAddr,Offset,Num));
  SpiFlashSetRegBase(BaseRegAddr);
  SpiFlashInit ();
  REGSET(REG_SPER, 0x01);//spre:01  mode 0
  if(Cs==0)
    REGSET(REG_SOFTCS ,0x01);  //enable cs0
  else if(Cs==1)
    REGSET(REG_SOFTCS ,0x02);  //enable cs1
  else
    DEBUG((DEBUG_INFO,"No support cs %d\n",Cs));

  // addr
  REGSET(REG_SPDR,((addr >> 24)&0xff));
  while(((REGGET(REG_SPSR))&SPI_BUSY) == SPI_BUSY ){
  }
  REGGET(REG_SPDR);
  REGSET(REG_SPDR,((addr >> 16)&0xff));
  while(((REGGET(REG_SPSR))&SPI_BUSY)  == SPI_BUSY ){
  }
  REGGET(REG_SPDR);
  REGSET(REG_SPDR,((addr >> 8)&0xff));
  while(((REGGET(REG_SPSR))&SPI_BUSY) == SPI_BUSY ){
  }
  REGGET(REG_SPDR);
  REGSET(REG_SPDR,(addr & 0xff));
  while(((REGGET(REG_SPSR))&SPI_BUSY) == SPI_BUSY ){
  }
  REGGET(REG_SPDR);// addr end

  // REGSET(REG_SPDR,0x00);
  for(count=0;count<Num;count++)
  {
    REGSET(REG_SPDR,0x00);
    while(((REGGET(REG_SPSR))&SPI_BUSY)  == SPI_BUSY ){
    }
    data[count] = REGGET(REG_SPDR);
    printf("0x%x\n",data[count]);
  }
  if(Cs==0)
    REGSET(REG_SOFTCS ,0x11); //disable cs0
  else if(Cs==1)
    REGSET(REG_SOFTCS ,0x22); //disable cs1
  else
    DEBUG((DEBUG_INFO,"No support cs %d\n",Cs));

  memcpy(Buffer,data,4);
  SpiFlashReset ();
  ResetSfcParamReg();

  return 0;
}
VOID
SpiTpmSpiWriteBuffer (
  UINT8    Cs,
  VOID     *Buffer,
  UINTN    Num
  )
{

  UINT8  *Buf = (UINT8 *) Buffer;
  UINTN  Index = 0;

  SpiCs (Cs, 0);
  while (Index < Num) {
    DEBUG((EFI_D_ERROR, "[TCM]: Wx Buf[%x] = 0x%x\n", Index,Buf[Index]));
    Buf[Index] = SpiFlashWriteByteCmd (Buf[Index]);
    DEBUG((EFI_D_ERROR, "[TCM]: Rx Buf[%x] = 0x%x\n", Index,Buf[Index]));
    Index++;
  }
  SpiCs (Cs, 1);
  SpiFlashWait ();
}

UINTN
SpiTpmSpiReadBuffer (
  IN  UINT8 Cs,
  VOID  *Buffer,
  UINTN  Num
  )
{
  UINT8  *Buf = (UINT8 *) Buffer;
  UINTN   Index;

  SpiCs (Cs, 0);
  // write reg addr
  while (Index < 4) {
    // DEBUG((EFI_D_ERROR, "[TCM]: Wx Buf[%x] = 0x%x\n", Index,Buf[Index]));
    Buf[Index] = SpiFlashWriteByteCmd (Buf[Index]);
    // DEBUG((EFI_D_ERROR, "[TCM]: Rx Buf[%x] = 0x%x\n", Index,Buf[Index]));
    Index++;
  }
  // start read
  for (Index = 0; Index < Num; Index++)
  {
    Buf[Index] = SpiFlashReadByteCmd ();
    // DEBUG((EFI_D_ERROR, "[TCM]: In SpiTpm.c, line %d.Read Buf[%x] = 0x%x\n", __LINE__,Index,Buf[Index]));
  }
  SpiCs (Cs, 1);

  return Index;
}

VOID
SpiTpmSetRegBase (
  UINTN BaseRegAddr
  )
{
  SPI_REG_BASE = BaseRegAddr;
}

UINTN
SpiRead (
  IN  UINT8  Dev,
  IN  UINT8  Cs,
  VOID       *Buffer,
  UINTN      Num
  )
{
  UINTN Ret;

  if(!Buffer || INVALID_NUM(Num)){
    ASSERT(0);
  }

  // p to v
  void * vaddr = NULL;
  int memoffset=0;
  UINTN DevBase = 0;
  if (Dev == 0) {
    DevBase = GetLs3ASpiRegBaseAddr();
  }else if (Dev == 1) {
    if(GetLs7ASpiRegBaseAddr()==0)
    {
      printf("7a spi base addr is 0\n");
      return 0;
    }
    DevBase = GetLs7ASpiRegBaseAddr();
  } else {
    DEBUG((EFI_D_ERROR, "No Support Device %d, defualt 3a!", Dev));
    DevBase = GetLs3ASpiRegBaseAddr();
  }
  vaddr=p2v_mem_mapping(DevBase, 8, &memoffset);
  if(vaddr==NULL)
    return 0;
 
  SpiTpmSetRegBase(vaddr);
  SpiFlashInit ();
  Ret = SpiTpmSpiReadBuffer (Cs, Buffer, Num);
  SpiFlashReset ();
  ResetSfcParamReg();
  // p to v
  p2v_mem_clean(vaddr, memoffset);

  return Ret;
}

VOID
SpiWrite (
  UINT8        Dev,
  UINT8        Cs,
  VOID         *Buffer,
  UINTN        Num
  )
{
  if(!Buffer || INVALID_NUM(Num)){
    ASSERT(0);
  }

  // p to v
  void * vaddr = NULL;
  int memoffset=0;
  UINTN DevBase = 0;
  if (Dev == 0) {
    DevBase = GetLs3ASpiRegBaseAddr();
  }else if (Dev == 1) {
    if(GetLs7ASpiRegBaseAddr()==0)
    {
      printf("7a spi base addr is 0\n");
      return ;
    }
    DevBase = GetLs7ASpiRegBaseAddr();
  } else {
    DEBUG((EFI_D_ERROR, "No Support Device %d, defualt 3a!", Dev));
    DevBase = GetLs3ASpiRegBaseAddr();
  }
  vaddr=p2v_mem_mapping(DevBase,8, &memoffset);
  if(vaddr==NULL)
    return ;

  SpiTpmSetRegBase(vaddr);
  SpiFlashInit ();
  SpiTpmSpiWriteBuffer (Cs, Buffer, Num);
  SpiFlashReset ();
  ResetSfcParamReg();

  // p to v
  p2v_mem_clean(vaddr, memoffset);
}


UINT16
EFIAPI
ReadSpiTpmVid(
  UINT8 Cs
  )
{
  UINT16               VID = 0;
  UINT8                Buffer[8] = {0x83,0xd4,0x0f,0x0};

  //For Loongson 3A5000 Platform SPI TCM
  // *(volatile UINT16 *)(0x80000efdfe000004 | (22 << 11)) = 0x47;

  SpiWrite (1, 1, Buffer, 8);
  DEBUG((EFI_D_ERROR, ": In SpiTpmLib.c, line %d.Buffer1=%x, Buffer2=%x, Buffer3=%x,Buffer4=%x\n", __LINE__,Buffer[4],Buffer[5],Buffer[6],Buffer[7]));

  //read ID from DR
  VID = Buffer[4] << 8 | Buffer[5];
  //DEBUG((EFI_D_ERROR, ": In SpiTpmLib.c, line %d. VID = 0x%x\n", __LINE__,VID));

  // sw16 (VID);
  DEBUG((EFI_D_ERROR, ": In SpiTpmLib.c, line %d. VID = 0x%x\n", __LINE__,VID));

  return VID;
}


UINT8
EFIAPI
TisRegRead8(
  IN  UINTN Addr
  )
{
  UINT8                Data;
  UINT8                Buffer[8] = {0x80, 0x0}; //command 0x80 is read 1byte

  DEBUG((EFI_D_ERROR, "0x%x:",Addr));
  Buffer[1] = (Addr & 0x00ff0000) >> 16;
  Buffer[2] = (Addr & 0x0000ff00) >> 8;
  Buffer[3] = Addr & 0x000000ff;

  //send command
  SpiRead (1, 1, Buffer, 1);

  Data = Buffer[0];
  DEBUG((EFI_D_ERROR, ": In SpiTpmLib.c, line %d.Data=%x \n", __LINE__,Data));
  return Data;
}

UINT8
EFIAPI
TisRegWrite8 (
  IN UINTN Addr,
  IN UINT8  Data
  )
{
  UINT8                Buffer[8] = {0x03, 0x0}; //command 0x03

  DEBUG((EFI_D_ERROR, "%x:",Addr));
  Buffer[1] = (Addr & 0x00ff0000) >> 16;
  Buffer[2] = (Addr & 0x0000ff00) >> 8;
  Buffer[3] = Addr & 0x000000ff;
  DEBUG((EFI_D_ERROR, ": In SpiTpmLib.c, line %d.Buffer1=%x,Buffer2=%x, Buffer3=%x,Buffer4=%x\n", __LINE__,Buffer[4],Buffer[5],Buffer[6],Buffer[7]));
  Buffer[4] = Data;

  //send command
  SpiWrite (1, 1, Buffer, 5);

  return Data;
}

