#include "testtool/testtool.h"
#include "capsule.h"
#include "platform/platform_debug.h"
#include "apptools.h"
#include "platform/app_platform.h"
#include "platform/app_spi.h"

static void ShowUsage(void)
{
	puts(
		"Usage:\n"
		"apptool [parameter] ... :explain \n"
		"  upbios :updte uefi bios\n"
		"  cmd into apptool cmdline \n");
}

void apptools_release(void)
{
    memory_release();
		return;

}

int main (int argc,char *argv[])
{
  int argv_c = argc - 1, argv_p = 1;
  int Ret = 0;

	if (argc == 1)
	{
	_show_usage:
		ShowUsage();
		goto cleanup;
	}

  TesttoolInit(0);
  app_platform_init();
  // capsule_init();
	if (!strcmp(argv[argv_p], "upbios"))
	{
		argv_c--;
		argv_p++;
		if (argv_c < 1)
			goto _show_usage;
    update_bios(argv[argv_p]);
		goto cleanup;
	}

	if (!strcmp(argv[argv_p], "cmd"))
  {
    TesttoolRun(1);
  }
cleanup:
  apptools_release();

  return Ret;
}

