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

#include "i2c.h"
#include "edk_api.h"
#include "LsRegDef.h"
#include "platform/app_os.h"
#include "platform/app_types.h"
#include "mem.h"

#define CPU_I2C0_REG_BASE  PHYS_TO_UNCACHED(0x1fe00120) 
#define CPU_I2C1_REG_BASE  PHYS_TO_UNCACHED(0x1fe00130)
#define ALGORITHM_3A 0x3a
#define ALGORITHM_7A 0x7a
#define I2C_CTL_CLK        100000         //BASE_CLK (Unit Khz)  
#define I2C_BUS_RATE       42
/**
  Initialize and set the I2c Control Information.

  @param  CtlAddr               The I2c Controller Address.
  @param  CtlClock              The I2c Controller Clock Frequency.
  @param  Frequency             The Clk Signal's Clock Frequency.

  @retval RETURN_SUCCESS        The I2c controller was setted.
  @retval RETURN_DEVICE_ERROR   The I2c controller could not be setted.

**/
int
I2cInitSetFreq (
   UINTN                                CtlAddr,
   UINTN                                CtlClock,
   UINTN                                Frequency, //Bus clk frequency
   UINT8                                Algorithm
  )
{
  UINT8  PrerLo = 0, PrerHi = 0;
  UINTN  CpuI2cBase = CtlAddr;

  /*Calculate FREQ_LO and FREQ_HI*/
  if (Algorithm == ALGORITHM_3A){
    // Prcescale = clock_a /(4*clock_s) -1
    PrerLo = (CtlClock / (4*Frequency) - 1) & 0xff; //CLK_BASE is Controller clock
    PrerHi = ((CtlClock / (4*Frequency) - 1) & 0xff00) >> 8;
  } else if (Algorithm == ALGORITHM_7A) {
    // Prcescale = clock_b /(5*clock_s) -1
    PrerLo = (CtlClock / (5*Frequency) - 1) & 0xff; //CLK_BASE is Controller clock
    PrerHi = ((CtlClock / (5*Frequency) - 1) & 0xff00) >> 8;
  }

  /*Config I2c's Freq*/
  Readb(CpuI2cBase + CTR_REG) &= ~(1<<7);
  Writeb(CpuI2cBase + PRER_LO_REG,PrerLo);
  Writeb(CpuI2cBase + PRER_HI_REG,PrerHi);
  Readb(CpuI2cBase + CTR_REG) |= (1<<7);

  return 0;
}

/**
  Set Slave Mode to I2c Controller.

  Set a specific controller to Slave Mode.

  @param  CtlAddr               The I2c Controller Address.
  @param  SlaveAddr             Set slave device address.

  @retval RETURN_SUCCESS        The I2c controller was setted.
  @retval RETURN_DEVICE_ERROR   The I2c controller could not be setted.

**/

VOID
I2cSetSlave (
   UINT8                          CtlAddr,
   UINT8                          SlaveAddr //0-6Bit
  )
{
  UINTN   CpuI2cBase = CtlAddr;

  /*Slave Mode and Set Slave addr*/
  Readb(CpuI2cBase + CTR_REG)  &= ~MST_EN;
  Readb(CpuI2cBase + SLV_CTRL) = SlaveAddr | SLV_EN;

  return ;
}

/**
  Reads data from a I2c device into a buffer.

  @param  CtlAddr          The I2c Controller Address.
  @param  DeviceAddr       The Read device address.
  @param  DataAddr         The Read Data offset.
  @param  DataByte         Number of bytes to read from the I2c device.
  @param  Buffer           Pointer to the data buffer to store the data read from the serial device.

  @retval 0                NumberOfBytes is 0.
  @retval >0               The number of bytes read from the I2c device.
                           If this value is less than NumberOfBytes, then the read operation failed.

**/
int
I2cCtlRead (
   UINTN                                CtlAddr,
   UINTN                                DeviceAddr,
   UINTN                                DataAddr,
   UINTN                                DataByte,
    VOID                             *Buffer
  )
{
  UINT8 i;
  UINTN CpuI2cBase = CtlAddr;

  /*Send Write Ops to Dev Addr*/
  Writeb(CpuI2cBase + TXR_REG,DeviceAddr << 1);
  Writeb(CpuI2cBase + CR_REG,CR_START | CR_WRITE);
  while (Readb(CpuI2cBase + SR_REG) & SR_TIP);
  if(Readb(CpuI2cBase + SR_REG) & SR_NOACK){
    printf("Error:No Ack Receive!\n");
    goto again;
  }
  /*Send Write Ops to Data Addr*/
  Writeb(CpuI2cBase + TXR_REG,DataAddr);
  Writeb(CpuI2cBase + CR_REG,CR_WRITE);
  while (Readb(CpuI2cBase + SR_REG) & SR_TIP);
  /*Check ACK*/
  if(Readb(CpuI2cBase + SR_REG) & SR_NOACK){
    printf("Error:No Ack Receive!\n");
    goto again;
  }

  /*Read Data*/
  //Start Read Signal 0Bit=1 is Read, 0 is write.
  Writeb(CpuI2cBase + TXR_REG,(DeviceAddr << 1) | 0x01);
  Writeb(CpuI2cBase + CR_REG,CR_START | CR_WRITE);
  while (Readb(CpuI2cBase + SR_REG) & SR_TIP);
  if(Readb(CpuI2cBase + SR_REG) & SR_NOACK){
    printf("Error:No Ack Receive!\n");
    goto again;
  }

  //Really start to poll to grab Buffer data
  if (Buffer == 0) {
    printf("Error:Input parameter buffer error!!!\n");
    return 2;
  }
  for(i = 0; i < DataByte; i++) {
    Writeb(CpuI2cBase + CR_REG,((i == DataByte - 1) ? (CR_READ | CR_ACK) : CR_READ));
    while (Readb(CpuI2cBase + SR_REG) & SR_TIP);
    ((UINT8 *)Buffer)[i] = Readb(CpuI2cBase + RXR_REG);
  }

again:
  /*Stop*/
  Writeb(CpuI2cBase + CR_REG,CR_STOP);
  Readb(CpuI2cBase + SR_REG);
  while (Readb(CpuI2cBase + SR_REG) & SR_BUSY);

  return 0;
}

/**

  Write data to a I2c device with a buffer.

  @param  CtlAddr          The I2c Controller Address.
  @param  DeviceAddr       The Read device address.
  @param  DataAddr         The Read Data offset.
  @param  DataByte         Number of bytes from write to the I2c device.
  @param  Buffer           Pointer to the data buffer to transfer the data to the I2c device.

  @retval 0                NumberOfBytes is 0.
  @retval >0               The number of bytes read from the I2c device.
                           If this value is less than NumberOfBytes, then the read operation failed.

**/

int
I2cCtlWrite (
   UINTN                                CtlAddr,
   UINTN                                DeviceAddr,
   UINTN                                DataAddr,
   UINTN                                DataByte,
   VOID                             *Buffer
  )
{
  UINT8   i;
  UINTN   CpuI2cBase = CtlAddr;

  /*Store Buffer Point*/
  //BufferTmp = Buffer;

  /*Send Write Ops to Dev Addr*/
  Writeb(CpuI2cBase + TXR_REG,DeviceAddr << 1);
  Writeb(CpuI2cBase + CR_REG,CR_START | CR_WRITE);
  while (Readb(CpuI2cBase + SR_REG) & SR_TIP);
  if(Readb(CpuI2cBase + SR_REG) & SR_NOACK){
    printf("Error:No Ack Receive!\n");
    goto again;
  }

  /*Send Write Ops to Data Addr*/
  Writeb(CpuI2cBase + TXR_REG,DataAddr);
  Writeb(CpuI2cBase + CR_REG,CR_WRITE);
  while (Readb(CpuI2cBase + SR_REG) & SR_TIP);
  /*Check ACK*/
  if(Readb(CpuI2cBase + SR_REG) & SR_NOACK){
    printf("Error:No Ack Receive!\n");
    goto again;
  }

  //Really start to poll to grab Buffer data
  if (Buffer == 0) {
    printf("Error:Input parameter buffer error!!!\n");
    return 2;
  }
  for(i = 0; i < DataByte; i++) {
    Writeb(CpuI2cBase + TXR_REG,*(UINT8*)Buffer);
    Writeb(CpuI2cBase + CR_REG,CR_WRITE);
    while (Readb(CpuI2cBase + SR_REG) & SR_TIP);
    if(Readb(CpuI2cBase + SR_REG) & SR_NOACK){
      printf("Error:No Ack Receive!\n");
      goto again;
    }
    Buffer++;
  }

  /*Stop*/
again:
  Writeb(CpuI2cBase + CR_REG,CR_STOP);
  while (Readb(CpuI2cBase + SR_REG) & SR_BUSY);

  return 0;
}

void LsCpuI2cRead(
   UINT8                                NodeId,
   UINTN                                BaseAddr,
   UINTN                                DevAddr,
   UINTN                                Reg,
   UINT8                                Size,
   VOID                                 *Buffer
    )
{
  void * vaddr = NULL;
  int memoffset=0;
  UINT64 Addr;
  Addr = BaseAddr | ((UINT64)NodeId << NODE_OFFSET);
  printf("Nodeid %d i2cbase 0x%llx,devaddr 0x%x, reg 0x%x, size %d\n",NodeId,Addr,DevAddr,Reg,Size);
  vaddr=p2v_mem_mapping(Addr,0x100, &memoffset);
  I2cInitSetFreq ((UINTN)vaddr, I2C_CTL_CLK, I2C_BUS_RATE, ALGORITHM_3A);
  I2cCtlRead ((UINTN)vaddr, DevAddr, Reg, Size, Buffer);
  p2v_mem_clean(vaddr, memoffset);
}

