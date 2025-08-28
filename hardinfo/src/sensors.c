#include "platform/app_os.h"
#include "platform/app_types.h"
#include "platform/app_mm.h"
#include "testtool/testtool.h"
#include "hardinfo.h"
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include "loongson/loongson3_def.h"

int get_sensors(ls_sensors_t *sen)
{
	lscpu_tempdetect(0, &sen->cputemp0,&sen->cputemp1, NULL);
	// printf("cpu temp0 %d ,temp1 %d\n", sen->cputemp0,sen->cputemp1);
  return 0;
}
