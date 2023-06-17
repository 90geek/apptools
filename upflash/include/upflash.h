#ifndef __UPDATE_FLASH_H__
#define __UPDATE_FLASH_H__
#include "platform/app_platform.h"

int update_bios_novar(char *file_path);
int update_bios_img(char *file_path);
int system_fw_update(char *file_path);
void update_flash_init(void);

#endif
