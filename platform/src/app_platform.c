// #include "platform/app_platform.h"
// #include "platform/app_i2c.h"
// #include "platform/app_uart.h"
// #include "platform/app_gpio.h"
// #include "platform/app_key.h"
// #include "platform/app_led.h"
// #include "platform/fpga.h"
#include "platform/app_spi.h"
#include "platform_debug.h"

// #ifdef PLATFORM_MSTAR
// #include "mstar/app_platform_mstar.h"
// #endif

// #ifdef PLATFORM_HISI
// #include "hisi/app_platform_hisi.h"
// #endif

static app_platform_t platform;

int get_platform_mod_ext_ver(void)
{
	return module_ext_version;
}

void app_platform_init(void) 
{

	platform.cpu_type = 0;
	platform.platform_version = 1;

// #ifdef DEVICE_5600P
// 	platform.device_type = DCH_5600P;
// #else
// #pragma message("device type not defined")
// #endif

// #ifdef PLATFORM_LA
	// mstar_init();
// #endif

	app_spi_init();

	platform_debug_register();

}

void app_platform_deinit(void) 
{


}

app_platform_t* get_app_platform_info(void)
{
	return &platform;
}

