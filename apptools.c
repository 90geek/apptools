
// #include "platform/pbi_os.h"
// #include "platform/pbi_platform.h"
// #include "platform/pbi_watchdog.h"
// #include "platform/fpga.h"
// #include "platform/pbi_types.h"
// #include "platform/platform_device.h"
// #include "pbi_event/pbi_event.h"
// #include "system/system.h"

#include "testtool/testtool.h"

void lstools_init(void) 
{
  TesttoolInit(0);


  TesttoolRun(1);
}
