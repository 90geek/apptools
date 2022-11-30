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

#include "ChipsetLpc.h"
#include "debug.h"

/**
  Reads data from a Chipset's Lpc Space into a buffer.

  @param  This              Point to the processor interface.
  @param  Width             Number of bytes read each time.
  @param  Offset         MemSpace Offset.
  @param  Count             Number of data read.
  @param  Buffer            Pointer to the data buffer to store the data read from the serial device.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
ChipsetLpcRead (
  IN EFI_LS_LPC_PROTOCOL               *This,
  IN EFI_LS_SERVICE_PROTOCOL_WIDTH     Width,
  IN UINTN                             Offset,
  IN UINTN                             Count,
  IN OUT VOID                          *Buffer,
  IN UINT8                             SpaceSelect
  )
{
  UINT64 ChipsetLpcBase;
  UINT8  DataByte = 0;

  /*Count the number of times to read data*/
  DataByte = Width * Count;

  /*Read Data*/
  if (Buffer == NULL) {
    DbgPrint(DEBUG_INFO,"Error:Input parameter buffer error!!!\n");
    return EFI_BUFFER_TOO_SMALL;
  }

  //
  // Config Lpc Access Mem Space
  //
  switch (SpaceSelect)
  {
    case MEM:
      ChipsetLpcBase = CHIPSET_LPC_MEM_BASE;
      break;

    case IO:
      ChipsetLpcBase = CHIPSET_LPC_IO_BASE;
      break;

    case IOTPM:
      ChipsetLpcBase = CHIPSET_LPC_IOTPM_BASE;
      break;

    default:
      DbgPrint(DEBUG_INFO,"Error:Input parameter SpaceSelect error!!!\n");
      return EFI_BUFFER_TOO_SMALL;
  }

  //
  //Get Data
  //
  void * vaddr = NULL;
  int memoffset=0;
  vaddr=p2v_mem_mapping(ChipsetLpcBase + Offset,(DataByte<4?4:DataByte), &memoffset);
  while (DataByte--) { //1Byte
     *(UINT8*)Buffer = Readb(vaddr); //BufferLen = DataByte
     Buffer++;
  }
  p2v_mem_clean(vaddr, memoffset);

  return EFI_SUCCESS;
}

/**
  Write data to a Chipset's Lpc Space with a buffer.

  @param  This              Point to the processor interface.
  @param  Width             Number of bytes write each time.
  @param  Offset         MemSpace Offset.
  @param  Count             Number of data write.
  @param  Buffer            Pointer to the data buffer to transfer the data to the I2c device.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
ChipsetLpcWrite (
  IN EFI_LS_LPC_PROTOCOL               *This,
  IN EFI_LS_SERVICE_PROTOCOL_WIDTH     Width,
  IN UINTN                             Offset,
  IN UINTN                             Count,
  IN OUT VOID                          *Buffer,
  IN UINT8                             SpaceSelect
  )
{
  UINT64 ChipsetLpcBase;
  UINT8  DataByte = 0;

  /*Count the number of times to read data*/
  DataByte = Width * Count;
  /*Read Data*/
  if (Buffer == NULL) {
    DbgPrint(DEBUG_INFO,"Error:Input parameter buffer error!!!\n");
    return EFI_BUFFER_TOO_SMALL;
  }

  //
  // Config Lpc Access Mem Space
  //
  switch (SpaceSelect)
  {
    case MEM:
      ChipsetLpcBase = CHIPSET_LPC_MEM_BASE;
      break;

    case IO:
      ChipsetLpcBase = CHIPSET_LPC_IO_BASE;
      break;

    case IOTPM:
      ChipsetLpcBase = CHIPSET_LPC_IOTPM_BASE;
      break;

    default:
      DbgPrint(DEBUG_INFO,"Error:Input parameter SpaceSelect error!!!\n");
      return EFI_BUFFER_TOO_SMALL;
  }

  //
  //Get Data
  //
	void * vaddr = NULL;
	int memoffset=0;
  vaddr=p2v_mem_mapping(ChipsetLpcBase + Offset,(DataByte<4?4:DataByte), &memoffset);
  while(DataByte--){ //1Byte
    Writeb(vaddr ,*(UINT8*)Buffer); //BufferLen = DataByte
    Buffer++;
  }
  p2v_mem_clean(vaddr, memoffset);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ChipsetLpcMemRead (
  IN EFI_LS_LPC_PROTOCOL                *This,
  IN EFI_LS_SERVICE_PROTOCOL_WIDTH      Width,
  IN UINTN                              Offset,
  IN UINTN                              Count,
  IN OUT VOID                           *Buffer
  )
{
  return ChipsetLpcRead (This,Width,Offset,Count,Buffer,MEM);
}

EFI_STATUS
EFIAPI
ChipsetLpcMemWrite (
  IN EFI_LS_LPC_PROTOCOL                *This,
  IN EFI_LS_SERVICE_PROTOCOL_WIDTH      Width,
  IN UINTN                              Offset,
  IN UINTN                              Count,
  IN OUT VOID                           *Buffer
  )
{
  return ChipsetLpcWrite (This,Width,Offset,Count,Buffer,MEM);
}

EFI_STATUS
EFIAPI
ChipsetLpcIoRead (
  IN EFI_LS_LPC_PROTOCOL                *This,
  IN EFI_LS_SERVICE_PROTOCOL_WIDTH      Width,
  IN UINTN                              Port,
  IN UINTN                              Count,
  IN OUT VOID                           *Buffer
  )
{
  return ChipsetLpcRead (This,Width,Port,Count,Buffer,IO);
}

EFI_STATUS
EFIAPI
ChipsetLpcIoWrite (
  IN EFI_LS_LPC_PROTOCOL                *This,
  IN EFI_LS_SERVICE_PROTOCOL_WIDTH      Width,
  IN UINTN                              Port,
  IN UINTN                              Count,
  IN OUT VOID                           *Buffer
  )
{
  return ChipsetLpcWrite (This,Width,Port,Count,Buffer,IO);
}

EFI_STATUS
EFIAPI
ChipsetLpcIoTpmRead (
  IN EFI_LS_LPC_PROTOCOL                *This,
  IN EFI_LS_SERVICE_PROTOCOL_WIDTH      Width,
  IN UINTN                              Port,
  IN UINTN                              Count,
  IN OUT VOID                           *Buffer
  )
{
  return ChipsetLpcRead (This,Width,Port,Count,Buffer,IOTPM);
}

EFI_STATUS
EFIAPI
ChipsetLpcIoTpmWrite (
  IN EFI_LS_LPC_PROTOCOL                *This,
  IN EFI_LS_SERVICE_PROTOCOL_WIDTH      Width,
  IN UINTN                              Port,
  IN UINTN                              Count,
  IN OUT VOID                           *Buffer
  )
{
  return ChipsetLpcWrite (This,Width,Port,Count,Buffer,IOTPM);
}
