#include <ctype.h>
#include "testtool/testtool.h"
#include "platform/platform_debug.h"
#include "apptools.h"
#include "platform/app_platform.h"
#include "platform/app_spi.h"
#include "vers.h"

#ifdef CAPSULE_SUPPORT
#include "capsule.h"
#endif

#ifdef RUNTIME_SUPPORT
#include "runtime.h"
#endif

#ifdef DMIDECODE_SUPPORT
#include "dmidecode_api.h"
#endif

#ifdef DMIDECODE_SUPPORT
#include "hardinfo.h"
#endif

#ifdef INFOUI_SUPPORT
#include "infoui.h"
#endif

#ifdef DEVMEM_SUPPORT
#include "devmem.h"
#endif

#ifdef UPFLASH_SUPPORT
#include "upflash.h"
#endif

static int check_sudo(void)
{
	if (access("/dev/mem",W_OK) != 0)
	{
		// printf("/dev/mem can be write\n");	
		return 1;
	}
	return 0;
}
static void ShowUsage(void)
{
	printf("Application Tools Programmer\nBy 90geek <bo90geek@gmail.com>\n");
	printf("Version %s\n",VERS);
	printf("Build Time %s\n",BLD);
	printf("%s\n\n",CMMT);
	puts(
		"Usage:\n"
		"apptool [parameter] ... :explain \n"
		"  help :display help infomation\n"
		"  rebios <flashsize> <imgfile>:read uefi bios img from flash,flash size is 4M or 8M\n"
		"  upbiosall <imgfile>:updte uefi bios img to flash\n"
		"  upbios <imgfile>:updte uefi bios img to flash but reserve configuration parameters\n"
		"  fwupdate <imgfile> <inifile>:Parses the ./SystemFirmwareUpdateConfig.ini to update the firmware \n"
		"  rmac <7aspi_paddr> <eth> :read mac addr eg rmac 0x452a0000 0\n"
		"  rmac <probe> <eth> :read mac addr eg rmac 0x452a0000 0\n"
		"  wmac <7aspi_paddr> <eth> <macaddr> :write 7a flash mac addr,eg wmac 0x452a0000 0 11:22:33:44:55:66\n"
		"  wmac <probe> <eth> <macaddr> :write 7a flash mac addr,eg wmac probe 0 11:22:33:44:55:66\n"
		"  dmi :dmidecode cmd,eg dmi -t 0\n"
		"  rcpuid :read cpu old id\n"
		"  devmem :devmem cmd,eg devmem addr\n"
		"  beepon :beep on\n"
		"  beepoff :beep off\n"
		"  tpmid :read 7a spi tpm id \n"
		"  clear7acfg :clear 7a flash advanced config\n"
		"  cmd :into apptool cmdline \n");
}
static void read_mac_addr(int argc, char *argv[])
{
	unsigned char *pbuf=NULL;
	unsigned int eth=0;
	U64 paddr=0;

	if (argc)
	{
		if (!isxdigit(argv[0][0]))
		{
			printf("Input not is 7a spi physical addressi!\n");
			if (!strcmp(argv[0], "probe"))
			{
				printf("The 00:16.0 device address will be used!\n");
			}
		}
		else
		{
			paddr = strtoul(argv[0], NULL, 0);
			set_7a_spi_base_addr(paddr);
		}
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
	read_7a_flash_mac(eth);
}

static void write_mac_addr(int argc, char *argv[])
{
	unsigned char *pbuf=NULL;
	unsigned int eth=0;
	U64 paddr=0;

	if (argc)
	{
		if (!isxdigit(argv[0][0]))
		{
			printf("Input not is 7a spi physical addressi!\n");
			if (!strcmp(argv[0], "probe"))
			{
				printf("The 00:16.0 device address will be used!\n");
			}
		}
		else
		{
			paddr = strtoul(argv[0], NULL, 0);
			set_7a_spi_base_addr(paddr);
		}
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

static void apptools_release(void)
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
	if(check_sudo())
	{
		printf("you need to be root to perform this command!!!\n");
		return Ret;
	}

	if (!strcmp(argv[argv_p], "dmi"))
	{
#ifdef DMIDECODE_SUPPORT
		dmidecode(argv_c, argv + argv_p);
#endif
		return Ret;
	}

	if (!strcmp(argv[argv_p], "devmem"))
	{
#ifdef DEVMEM_SUPPORT
		devmem_init (argv_c, argv + argv_p);
#endif
		return Ret;
	}

	if (!strcmp(argv[argv_p], "rt"))
	{
#ifdef RUNTIME_SUPPORT
		runtime_start (argv_c, argv + argv_p);
#endif
		return Ret;
	}

	TesttoolInit(0);
	app_platform_init();
#ifdef CAPSULE_SUPPORT
	// capsule_init();
#endif
#ifdef HARDINFO_SUPPORT
	hardinfo_init();
#endif
#ifdef UPFLASH_SUPPORT
  update_flash_init() ;
#endif
	if (!strcmp(argv[argv_p], "help"))
	{
		goto _show_usage;
	}

#ifdef UPFLASH_SUPPORT
	if (!strcmp(argv[argv_p], "rebios"))
	{
		printf("strsize is no support %d\n",argv_c);
		argv_c--;
		argv_p++;
		if (argv_c > 2)
			goto _show_usage;
    if(argv_c==0)
		  read_bios_img(NULL, NULL);
    else if(argv_c==1)
		  read_bios_img(NULL, argv[argv_p]);
    else if(argv_c==2)
		  read_bios_img(argv[argv_p+1], argv[argv_p]);
		goto cleanup;
	}
	if (!strcmp(argv[argv_p], "upbiosall"))
	{
		argv_c--;
		argv_p++;
		if (argv_c < 1)
			goto _show_usage;
		update_bios_img(argv[argv_p]);
		goto cleanup;
	}
	if (!strcmp(argv[argv_p], "upbios"))
	{
		argv_c--;
		argv_p++;
		if (argv_c < 1)
			goto _show_usage;
		update_bios_novar(argv[argv_p]);
		goto cleanup;
	}
	if (!strcmp(argv[argv_p], "fwupdate"))
	{
		argv_c--;
		argv_p++;
		if (argv_c < 1)
			goto _show_usage;
    printf("%d--%s--%s\n",argv_c,argv[argv_p],argv[argv_p+1]);
		if (argv_c == 2)
		  system_fw_update(argv[argv_p],argv[argv_p+1]);
    else
		  system_fw_update(argv[argv_p],NULL);
		goto cleanup;
	}
#endif

	if (!strcmp(argv[argv_p], "rmac"))
	{
		argv_c--;
		argv_p++;
		if (argv_c < 1)
			goto _show_usage;
		read_mac_addr(argv_c, argv + argv_p);
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

  if (!strcmp(argv[argv_p], "tpmid"))
	{
    read_7a_tcm_id();
		goto cleanup;
	}
  if (!strcmp(argv[argv_p], "clear7acfg"))
	{
    clear_7a_advanced_config();
		goto cleanup;
	}

	if (!strcmp(argv[argv_p], "infoui"))
	{
		argv_c--;
		argv_p++;
#ifdef INFOUI_SUPPORT
		ls_info_ui (argv_c, argv + argv_p);
#endif
		goto cleanup;
	}

	if (!strcmp(argv[argv_p], "rcpuid"))
	{
#ifdef HARDINFO_SUPPORT
	read_cpu_old_id();
#endif
		goto cleanup;
	}

	if (!strcmp(argv[argv_p], "beepon"))
	{
		BeepOn ();
		goto cleanup;
	}

	if (!strcmp(argv[argv_p], "beepoff"))
	{
		BeepOff ();
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

