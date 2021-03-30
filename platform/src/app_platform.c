// #include "platform/pbi_platform.h"
// #include "platform/pbi_i2c.h"
// #include "platform/pbi_spi.h"
// #include "platform/pbi_uart.h"
// #include "platform/pbi_gpio.h"
// #include "platform/pbi_key.h"
// #include "platform/pbi_led.h"
// #include "platform/fpga.h"
#include "platform_debug.h"

#ifdef PLATFORM_MSTAR
#include "mstar/pbi_platform_mstar.h"
#endif

#ifdef PLATFORM_HISI
#include "hisi/pbi_platform_hisi.h"
#endif

static pbi_platform_t platform;

int get_platform_mod_ext_ver(void)
{
	return module_ext_version;
}

void pbi_platform_init(void) 
{

	platform.cpu_type = 0;
	platform.platform_version = 1;

#ifdef DEVICE_5600P
	platform.device_type = DCH_5600P;
#else
#pragma message("device type not defined")
#endif

#ifdef PLATFORM_LA
	mstar_init();
#endif

	app_spi_init();

	platform_debug_register();

}

void pbi_platform_deinit(void) 
{


}

pbi_platform_t* get_pbi_platform_info(void)
{
	return &platform;
}

