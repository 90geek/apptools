#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "version.h"
#include "config.h"
#include "types.h"
#include "util.h"
#include "dmidecode.h"
#include "dmiopt.h"
#include "dmioem.h"
#include "dmioutput.h"
#include "testtool/testtool.h"

int dmi_debug(parse_t * pars_p,char *result_p)
{

	return 0;
}

void dmi_debug_register(void)
{
	// register_command ("CPU_INFO" , cpu_info_debug , "<NONE>");
}

int dmi_init(void)
{
	dmi_debug_register();
}
