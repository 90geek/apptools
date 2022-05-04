#include "testtool/testtool.h"
#include "capsule.h"
#include "platform/platform_debug.h"
#include "apptools.h"

int main (int argc,char *argv[])
{
  int Ret = 0;

  TesttoolInit(0);
  app_platform_init();
  // capsule_init();

  TesttoolRun(1);

  return Ret;
}

