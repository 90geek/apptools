#include "testtool/testtool.h"
#include "capsule.h"

void lstools_init(void) 
{
  TesttoolInit(0);
  capsule_init();

  TesttoolRun(1);
}
