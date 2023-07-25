#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include "app_types.h"
#include "loongson/edk_api.h"
#include "loongson/debug.h"
#include "loongson/spi.h"
#include "loongson/mem.h"
#include "loongson/temperature.h"
#include "loongson/cpu.h"
#include "loongson/pwm.h"
#include "loongson/ls7a.h"
#include "loongson/mps.h"
#include "loongson/ChipsetLpc.h"
#include "loongson/Beep.h"
#include "loongson/CpuGpioLib.h"
#include "loongson/Ls7aGpio.h"
#include "loongson/LsAsm.h"

typedef struct app_platform_s{
  int cpu_type;
  int platform_version;
  int device_type;   /*app_device_e*/
  int harware_version;
}app_platform_t;

void* str2data_mac(char *szMacStr);
void app_platform_init(void) ;
void app_platform_deinit(void) ;
app_platform_t* get_app_platform_info(void);

/****************************************/
#endif
