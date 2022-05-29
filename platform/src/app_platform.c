#include "platform/app_platform.h"
#include "platform/app_spi.h"
#include "platform/platform_debug.h"
#include "platform/app_os.h"

static app_platform_t platform;
void app_platform_init(void) 
{
	memory_mapping();
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

