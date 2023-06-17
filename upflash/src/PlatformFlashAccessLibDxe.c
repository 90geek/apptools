/** @file
  Platform Flash Access library.

  Copyright (c) 2016, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "platform/app_platform.h"
#include "PlatformFlashAccessLib.h"

//
// SPI default opcode slots
//
#define SPI_OPCODE_JEDEC_ID_INDEX        0
#define SPI_OPCODE_READ_ID_INDEX         1
#define SPI_OPCODE_WRITE_S_INDEX         2
#define SPI_OPCODE_WRITE_INDEX           3
#define SPI_OPCODE_READ_INDEX            4
#define SPI_OPCODE_ERASE_INDEX           5
#define SPI_OPCODE_READ_S_INDEX          6
#define SPI_OPCODE_CHIP_ERASE_INDEX      7

#define SPI_ERASE_SECTOR_SIZE            BLKSIZE  //This is the chipset requirement

STATIC EFI_PHYSICAL_ADDRESS     mInternalFdAddress;
// EFI_SPI_PROTOCOL                *mSpiProtocol;
// FLASH_DEVICE_OPERATION_PROTOCOL  *mSpiProtocol;

/**
  Writes specified number of bytes from the input buffer to the address

  @param[in]      WriteAddress  The flash address to be written.
  @param[in, out] NumBytes      The number of bytes.
  @param[in]      Buffer        The data buffer to be written.

  @return The status of flash write.
**/
EFI_STATUS
FlashFdWrite (
  IN  UINTN                               WriteAddress,
  IN OUT UINTN                            *NumBytes,
  IN  UINT8                               *Buffer
  )
{
	write_cpu_spi_flash(WriteAddress, Buffer, *NumBytes);
	return 0;
}

/**
  Erase a certain block from address LbaWriteAddress

  @param[in] WriteAddress  The flash address to be erased.

e @return The status of flash erase.
**/
EFI_STATUS
FlashFdErase (
  IN UINTN                                WriteAddress
  )
{
  // EFI_STATUS  Status=0;
  UINTN       Count;

	Count = SPI_ERASE_SECTOR_SIZE;
	erase_block_cpu_spi_flash(WriteAddress, Count);
	return 0;
}

EFI_STATUS
FlashFdRead (
  IN  UINTN                               ReadAddress,
  IN OUT UINTN                            *NumBytes,
  IN  UINT8                               *Buffer
  )
{
	read_cpu_spi_flash(ReadAddress, Buffer, *NumBytes);
  return 0;
}
/**
  Perform flash write opreation.

  @param[in] FirmwareType      The type of firmware.
  @param[in] FlashAddress      The address of flash device to be accessed.
  @param[in] FlashAddressType  The type of flash device address.
  @param[in] Buffer            The pointer to the data buffer.
  @param[in] Length            The length of data buffer in bytes.

  @retval EFI_SUCCESS           The operation returns successfully.
  @retval EFI_WRITE_PROTECTED   The flash device is read only.
  @retval EFI_UNSUPPORTED       The flash device access is unsupported.
  @retval EFI_INVALID_PARAMETER The input parameter is not valid.
**/
EFI_STATUS
EFIAPI
PerformFlashWrite (
  IN PLATFORM_FIRMWARE_TYPE       FirmwareType,
  IN EFI_PHYSICAL_ADDRESS         FlashAddress,
  IN FLASH_ADDRESS_TYPE           FlashAddressType,
  IN VOID                         *Buffer,
  IN UINTN                        Length
  )
{
  EFI_STATUS          Status;
  UINTN               SectorNum;
  UINTN               UpdateCount=-1;
  UINTN               Index;
  UINTN               NumBytes;
  VOID                *TempBuffer;

  DEBUG((DEBUG_INFO, "Flash start address: 0x%x(%a),Length 0x%x\n", (UINTN)FlashAddress, (((UINTN)FlashAddressType==FlashAddressTypeAbsoluteAddress)?"AbsoluteAddress":"RelativeAddress"), Length));
  if (FlashAddressType == FlashAddressTypeAbsoluteAddress) {
   FlashAddress = FlashAddress - mInternalFdAddress;
  }

  // Status = gBS->AllocatePool(EfiRuntimeServicesCode,Length,(VOID **)&TempBuffer);
  // if (EFI_ERROR (Status)){
  //   return Status;
  // }
	TempBuffer = app_malloc(Length);
  NumBytes=Length;
  FlashFdRead ((UINTN)FlashAddress,&NumBytes,TempBuffer);
  printf("Start Update : \n");
  ////
  //// Erase & Write
  ////
  SectorNum = Length / SPI_ERASE_SECTOR_SIZE;
  for (Index = 0; Index < SectorNum; Index++){
    // if (CompareMem(
    //   (UINT8 *)(UINTN)(TempBuffer) + Index * SPI_ERASE_SECTOR_SIZE,
    //   (UINT8 *)Buffer + Index * SPI_ERASE_SECTOR_SIZE,
    //   SPI_ERASE_SECTOR_SIZE) == 0) {
		if(memcmp(
      (UINT8 *)(UINTN)(TempBuffer) + Index * SPI_ERASE_SECTOR_SIZE,
      (UINT8 *)Buffer + Index * SPI_ERASE_SECTOR_SIZE,
      SPI_ERASE_SECTOR_SIZE) == 0) {
      // DEBUG((DEBUG_INFO, "Sector - 0x%x - skip\n", Index));
      continue;
    }
   // DEBUG((DEBUG_INFO, "Sector - 0x%x - update...\n", Index));
   printf(".");
	 fflush(stdout);
   if(((UpdateCount+1)%16) == 0)
      printf("\n");
   UpdateCount++;

   Status = FlashFdErase (
              (UINTN)FlashAddress + Index * SPI_ERASE_SECTOR_SIZE
              );
    if (Status != EFI_SUCCESS){
      break;
    }
    NumBytes = SPI_ERASE_SECTOR_SIZE;
    Status = FlashFdWrite (
               (UINTN)FlashAddress + Index * SPI_ERASE_SECTOR_SIZE,
               &NumBytes,
               (UINT8 *)Buffer + Index * SPI_ERASE_SECTOR_SIZE
               );
    if (Status != EFI_SUCCESS){
      break;
    }
  }
  printf("\n");
  printf("Data Length=0x%x(%d),SectorNum=0x%x(%d),Sector UpdateCount=0x%x(%d),Update Success!!!\n",  \
      Length,Length,  \
      SectorNum,SectorNum, \
      UpdateCount,UpdateCount);

  app_free (TempBuffer);
  return EFI_SUCCESS;
}

/**
  Platform Flash Access Lib Constructor.

  @param[in]  ImageHandle       The firmware allocated handle for the EFI image.
  @param[in]  SystemTable       A pointer to the EFI System Table.

  @retval EFI_SUCCESS  Constructor returns successfully.
**/
// EFI_STATUS
// EFIAPI
// PerformFlashAccessLibConstructor (
//   IN EFI_HANDLE                         ImageHandle,
//   IN EFI_SYSTEM_TABLE                   *SystemTable
//   )
// {
//   EFI_STATUS  Status;

//   // mInternalFdAddress = (EFI_PHYSICAL_ADDRESS)(UINTN)PcdGet64(PcdFlashFdBase);
//   DEBUG((DEBUG_INFO, "PcdFlashBaseAddress - 0x%lx\n", mInternalFdAddress));

//   Status = gBS->LocateProtocol (
//                   &gFlashDeviceOperationProtocolGuid,
//                   NULL,
//                   (VOID **) &mSpiProtocol
//            );
//   if(EFI_ERROR(Status)){
//     DEBUG((EFI_D_ERROR, "Flash Operation Protocol Not Installed.\n"));
//     ASSERT_EFI_ERROR(Status);
//   }

//   return EFI_SUCCESS;
// }
