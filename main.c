#include <ctype.h>
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
		"  wmac 7aspi_paddr eth macaddr :write 7a flash mac addr,eg wmac 0x452a0000 0 11:22:33:44:55:66\n"
		"  cmd into apptool cmdline \n");
}
void write_mac_addr(int argc, char *argv[])
{
	unsigned char *pbuf=NULL;
	unsigned int eth=0;
	U64 paddr=0;

	if (argc)
	{
		if (!isxdigit(argv[0][0]))
		{
			printf("Please input a numeric 7a spi physical address!\n");
			return;
		}
		paddr = strtoul(argv[0], NULL, 0);
		set_7a_spi_base_addr(paddr);
		argc--;
		argv++;
	}

	if (argc)
	{
		if (!isdigit(argv[0][0]))
		{
			printf("Please input a numeric eth %c\n",argv[0][0]);
			return;
		}
		eth = strtoul(argv[0], NULL, 0);
		argc--;
		argv++;
	}

	if (argc)
	{
		if (!isxdigit(argv[0][0]))
		{
			printf("Please input a numeric mac %c\n",argv[0][0]);
			return;
		}
		pbuf=str2data_mac(argv[0]);
		argc--;
		argv++;
	}

	write_7a_flash_mac(eth,pbuf);
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

	if (!strcmp(argv[argv_p], "wmac"))
	{
		argv_c--;
		argv_p++;
		if (argv_c < 1)
			goto _show_usage;
		write_mac_addr(argv_c, argv + argv_p);

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

