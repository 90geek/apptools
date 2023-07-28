#include "platform/app_os.h"
#include "testtool/testtool.h"
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include "upflash.h"
#include "PlatformFlashAccessLib.h"
#include "iniparser.h"

#define MAX_LINE_LENGTH           512
#define FLASH_SIZE 0x800000
BOOLEAN  mNvRamUpdated = FALSE;
#define INI_FILE_PATH "./SystemFirmwareUpdateConfig.ini"


EFI_STATUS
ParseUpdateDataFile (
  IN      UINT8                         *DataBuffer,
  IN      UINTN                         BufferSize,
  IN OUT  CONFIG_HEADER                 *ConfigHeader,
  IN OUT  UPDATE_CONFIG_DATA            **UpdateArray
  )
{
  EFI_STATUS                            Status;
  CHAR8                                 *SectionName;
  CHAR8                                 Entry[MAX_LINE_LENGTH];
  INTN                                 Num;
  UINT64                                Num64;
  UINTN                                 Index;
  EFI_GUID                              FileGuid;
  VOID                                  *Context;
  dictionary  *                         ini;

  //
  // First process the data buffer and get all sections and entries
  //
  ini = iniparser_load(DataBuffer);
  if (ini==NULL) {
      fprintf(stderr, "cannot parse file: %s\n", INI_FILE_PATH);
      return -1 ;
  }
  iniparser_dump(ini, stderr);
  Num = iniparser_getint(ini, "Head:NumOfUpdate", -1);
  printf("NumOfUpdate:      [%d]\n", Num);
  if (Num == -1) {
    DEBUG((DEBUG_ERROR, "NumOfUpdate not found\n"));
    return EFI_NOT_FOUND;
  }

  ConfigHeader->NumOfUpdates = Num;
  *UpdateArray = AllocateZeroPool ((sizeof (UPDATE_CONFIG_DATA) * Num));
  if (*UpdateArray == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  for (Index = 0 ; Index < ConfigHeader->NumOfUpdates ; Index++) {
    //
    // Get the section name of each update
    //
    sprintf(Entry, "%s:%s%d","Head","Update",Index);
    SectionName = (CHAR8 *)iniparser_getstring(ini, Entry, NULL);
    printf("%s\n", SectionName);
    if (SectionName == NULL) {
      DEBUG((DEBUG_ERROR, "[%d] %a not found\n", Index, Entry));
      return EFI_NOT_FOUND;
    }
    //
    // The section name of this update has been found.
    // Now looks for all the config data of this update
    //
    (*UpdateArray)[Index].Index = Index;
    sprintf(Entry, "%s:%s",SectionName,"FirmwareType");
    Num = iniparser_getint(ini, Entry, -1);
    printf("%s:      [%d]\n",Entry, Num);
    if (Num == -1) {
        DEBUG((DEBUG_ERROR, "[%d] FirmwareType not found\n", Index));
      return EFI_NOT_FOUND;
    }

    (*UpdateArray)[Index].FirmwareType = (PLATFORM_FIRMWARE_TYPE) Num;
    sprintf(Entry, "%s:%s",SectionName,"AddressType");
    Num = iniparser_getint(ini, Entry, -1);
    printf("%s:      [%d]\n",Entry, Num);
    if (Num == -1) {
      DEBUG((DEBUG_ERROR, "[%d] AddressType not found\n", Index));
      return EFI_NOT_FOUND;
    }
    (*UpdateArray)[Index].AddressType = (FLASH_ADDRESS_TYPE) Num;

    sprintf(Entry, "%s:%s",SectionName,"BaseAddress");
    Num64 = iniparser_getint(ini, Entry, -1);
    printf("%s:      [%d]\n",Entry, Num);
    if (Num64 == -1) {
      DEBUG((DEBUG_ERROR, "[%d] BaseAddress not found\n", Index));
      return EFI_NOT_FOUND;
    }
    (*UpdateArray)[Index].BaseAddress = (EFI_PHYSICAL_ADDRESS) Num64;

    //
    // FileBuid
    //
    // Status = GetGuidFromDataFile(
    //            Context,
    //            SectionName,
    //            "FileGuid",
    //            &FileGuid
    //            );
    // if (EFI_ERROR(Status)) {
    //   DEBUG((DEBUG_ERROR, "[%d] FileGuid not found\n", Index));
    //   return EFI_NOT_FOUND;
    // }

    // CopyGuid(&((*UpdateArray)[Index].FileGuid), &FileGuid);

    sprintf(Entry, "%s:%s",SectionName,"Length");
    Num = iniparser_getint(ini, Entry, -1);
    printf("%s:      [%d]\n",Entry, Num);
    if (Num == -1) {
      DEBUG((DEBUG_ERROR, "[%d] Length not found\n", Index));
      return EFI_NOT_FOUND;
    }

    (*UpdateArray)[Index].Length = (UINTN) Num;

    //
    // ImageOffset
    //
    sprintf(Entry, "%s:%s",SectionName,"ImageOffset");
    Num = iniparser_getint(ini, Entry, -1);
    printf("%s:      [%d]\n",Entry, Num);
    if (Num == -1) {
      DEBUG((DEBUG_ERROR, "[%d] ImageOffset not found\n", Index));
      return EFI_NOT_FOUND;
    }


    (*UpdateArray)[Index].ImageOffset = (UINTN) Num;
  }

  //
  // Now all configuration data got. Free those temporary buffers
  //
  // CloseIniFile(Context);

  return EFI_SUCCESS;
}

/**
  Update System Firmware image component.

  @param[in]  SystemFirmwareImage     Points to the System Firmware image.
  @param[in]  SystemFirmwareImageSize The length of the System Firmware image in bytes.
  @param[in]  ConfigData              Points to the component configuration structure.
  @param[out] LastAttemptVersion      The last attempt version, which will be recorded in ESRT and FMP EFI_FIRMWARE_IMAGE_DESCRIPTOR.
  @param[out] LastAttemptStatus       The last attempt status, which will be recorded in ESRT and FMP EFI_FIRMWARE_IMAGE_DESCRIPTOR.

  @retval EFI_SUCCESS             The System Firmware image is updated.
  @retval EFI_WRITE_PROTECTED     The flash device is read only.
**/
EFI_STATUS
PerformUpdate (
  IN VOID                         *SystemFirmwareImage,
  IN UINTN                        SystemFirmwareImageSize,
  IN UPDATE_CONFIG_DATA           *ConfigData,
  OUT UINT32                      *LastAttemptVersion,
  OUT UINT32                      *LastAttemptStatus
  )
{
  EFI_STATUS                   Status;

  DEBUG((DEBUG_INFO, "PlatformUpdate:"));
  DEBUG((DEBUG_INFO, "  BaseAddress - 0x%lx,", ConfigData->BaseAddress));
  DEBUG((DEBUG_INFO, "  ImageOffset - 0x%x,", ConfigData->ImageOffset));
  DEBUG((DEBUG_INFO, "  Legnth - 0x%x\n", ConfigData->Length));
  Status = PerformFlashWrite (
             ConfigData->FirmwareType,
             ConfigData->BaseAddress,
             ConfigData->AddressType,
             (VOID *)((UINTN)SystemFirmwareImage + (UINTN)ConfigData->ImageOffset),
             ConfigData->Length
             );
  if (!EFI_ERROR(Status)) {
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_SUCCESS;
    if (ConfigData->FirmwareType == PlatformFirmwareTypeNvRam) {
      mNvRamUpdated = TRUE;
    }
  } else {
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_UNSUCCESSFUL;
  }
  return Status;
}
/**
  Update System Firmware image.

  @param[in]  SystemFirmwareImage     Points to the System Firmware image.
  @param[in]  SystemFirmwareImageSize The length of the System Firmware image in bytes.
  @param[in]  ConfigImage             Points to the config file image.
  @param[in]  ConfigImageSize         The length of the config file image in bytes.
  @param[out] LastAttemptVersion      The last attempt version, which will be recorded in ESRT and FMP EFI_FIRMWARE_IMAGE_DESCRIPTOR.
  @param[out] LastAttemptStatus       The last attempt status, which will be recorded in ESRT and FMP EFI_FIRMWARE_IMAGE_DESCRIPTOR.

  @retval EFI_SUCCESS             The System Firmware image is updated.
  @retval EFI_WRITE_PROTECTED     The flash device is read only.
**/
EFI_STATUS
UpdateImage (
  IN VOID                         *SystemFirmwareImage,
  IN UINTN                        SystemFirmwareImageSize,
  IN VOID                         *ConfigImage,
  IN UINTN                        ConfigImageSize,
  OUT UINT32                      *LastAttemptVersion,
  OUT UINT32                      *LastAttemptStatus
  )
{
  EFI_STATUS                            Status;
  UPDATE_CONFIG_DATA                    *ConfigData;
  UPDATE_CONFIG_DATA                    *UpdateConfigData;
  CONFIG_HEADER                         ConfigHeader;
  UINTN                                 Index;

  if ((ConfigImage == NULL) && ((access(INI_FILE_PATH,F_OK))==-1)) {
    DEBUG((DEBUG_INFO, "PlatformUpdate (NoConfig):"));
    DEBUG((DEBUG_INFO, "  BaseAddress - 0x%x,", 0));
    DEBUG((DEBUG_INFO, "  Length - 0x%x\n", SystemFirmwareImageSize));
    // ASSUME the whole System Firmware include NVRAM region.
    Status = PerformFlashWrite (
               PlatformFirmwareTypeNvRam,
               0,
               FlashAddressTypeRelativeAddress,
               SystemFirmwareImage,
               SystemFirmwareImageSize
               );
    if (!EFI_ERROR(Status)) {
      *LastAttemptStatus = LAST_ATTEMPT_STATUS_SUCCESS;
      mNvRamUpdated = TRUE;
    } else {
      *LastAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_UNSUCCESSFUL;
    }
    return Status;
  }
  else if((ConfigImage == NULL) && ((access(INI_FILE_PATH,F_OK))!=-1)){
    ConfigImage = INI_FILE_PATH,F_OK;
  }

  DEBUG((DEBUG_INFO, "PlatformUpdate (With Config):%s\n",ConfigImage));
  ConfigData        = NULL;
  memset (&ConfigHeader, 0,sizeof(ConfigHeader));
  // ZeroMem (&ConfigHeader, sizeof(ConfigHeader));
  Status            = ParseUpdateDataFile (
                        ConfigImage,
                        ConfigImageSize,
                        &ConfigHeader,
                        &ConfigData
                        );
  DEBUG((DEBUG_INFO, "ParseUpdateDataFile - %r\n", Status));
  if (EFI_ERROR(Status)) {
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_UNSUCCESSFUL;
    return EFI_INVALID_PARAMETER;
  }
  DEBUG((DEBUG_INFO, "ConfigHeader.NumOfUpdates - 0x%x\n", ConfigHeader.NumOfUpdates));
  // DEBUG((DEBUG_INFO, "PcdEdkiiSystemFirmwareFileGuid - %g\n", PcdGetPtr(PcdEdkiiSystemFirmwareFileGuid)));

  Index = 0;
  UpdateConfigData = ConfigData;
  while (Index < ConfigHeader.NumOfUpdates) {
    // if (CompareGuid(&UpdateConfigData->FileGuid, PcdGetPtr(PcdEdkiiSystemFirmwareFileGuid))) {
      DEBUG((DEBUG_INFO, "FileGuid - %g (processing)\n", &UpdateConfigData->FileGuid));
      Status = PerformUpdate (
                 SystemFirmwareImage,
                 SystemFirmwareImageSize,
                 UpdateConfigData,
                 LastAttemptVersion,
                 LastAttemptStatus
                 );
      //
      // Shall updates be serialized so that if an update is not successfully completed,
      // the remaining updates won't be performed.
      //
      if (EFI_ERROR (Status)) {
        break;
      }
    // } else {
    //   DEBUG((DEBUG_INFO, "FileGuid - %g (ignored)\n", &UpdateConfigData->FileGuid));
    // }

    Index++;
    UpdateConfigData++;
  }

  return Status;
}
int system_fw_update(char *file_path,char *ini_path)
{
	int from_fd=0,len=0,count=0;
	unsigned char  *ptr1 = NULL,*ptr2 = NULL;
	int Ret = 0;
	void * vaddr = NULL;
	int memoffset=0;
	UINT32      LastAttemptVersion;
	UINT32      LastAttemptStatus;

	if((from_fd=open(file_path,O_RDONLY))==-1)
	{
		printf("Open %s Error\n",file_path);
		return 1;
	}

	len=lseek(from_fd,0,SEEK_END);
	lseek(from_fd,0,SEEK_SET);

	count=len;
	ptr1=app_malloc(len*sizeof(char));
	memset(ptr1,0x0,len*sizeof(char));
	ptr2=ptr1;

	Ret=read(from_fd,ptr1,len);
	if(Ret==-1)
	{
		printf("Load FPGA File Error\n");
		close(from_fd);
		return 1;
	}
	printf("Load Bios File, Size is %d\n",Ret);
	UpdateImage((void *)ptr2, Ret, ini_path, 0, &LastAttemptVersion, &LastAttemptStatus);
	return 0;
}
int update_bios_novar(char *file_path)
{
	int size=0x10000;
	unsigned int offset=0x20000;
	unsigned char *datas=NULL;

	printf("variable offset 0x%x, size 0x%x\n",offset, size);
	datas=app_malloc(size*sizeof(unsigned char));
	memset(datas,0x0,size*sizeof(unsigned char));
	app_spi_read(0,offset, datas, size);
	// app_print_data(datas,size);
	update_bios_img(file_path);
	app_spi_write(0,offset, datas, size);
	free(datas);
	return 0;
}
int update_bios_img(char *file_path)
{
	int from_fd=0,len=0,count=0;
	unsigned char  *ptr1 = NULL,*ptr2 = NULL;
	int Ret = 0;
	void * vaddr = NULL;
	int memoffset=0;

	if((from_fd=open(file_path,O_RDONLY))==-1)
	{
		printf("Open %s Error\n",file_path);
		return 1;
	}

	len=lseek(from_fd,0,SEEK_END);
	lseek(from_fd,0,SEEK_SET);

	count=len;
	ptr1=app_malloc(len*sizeof(char));
	memset(ptr1,0x0,len*sizeof(char));
	ptr2=ptr1;

	Ret=read(from_fd,ptr1,len);
	if(Ret==-1)
	{
		printf("Load FPGA File Error\n");
		close(from_fd);
		return 1;
	}
	printf("Load Bios File, Size is %d\n",Ret);

	vaddr=p2v_mem_mapping(GetLs3ASpiRegBaseAddr(),FLASH_SIZE, &memoffset);
	if(vaddr==NULL)
		return 1;

	if(Ret>FLASH_SIZE)
	{
		printf("file size > flash size\n");
		return 1;
	}
	UpdateBiosInSpiFlash(0,ptr2,Ret,(U64)vaddr);
	p2v_mem_clean(vaddr, memoffset);
	// close(from_fd);

	return 0;
}
int update_bios_debug(parse_t * pars_p,char *result_p)
{
	char path[200];

	/* ------------- */
	if (cget_string(pars_p,"", path, sizeof(path))==1)
	{
		tag_current_line(pars_p,"Invalid path\n");
		return(1);
	}

	printf("bios path  %s \n",path);
	update_bios_img(path);
	return 0;
}
void upflash_debug_register(void)
{
	register_command ("SPI_BIOS"			, update_bios_debug , "<path>:eg ./LS3A50007A.fd");
}
void update_flash_init(void) 
{
	upflash_debug_register();
	
}
