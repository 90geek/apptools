#ifndef _PBI_GPIO_H_
#define _PBI_GPIO_H_

/**/
#ifdef DEVICE_2210P
#include "pbi_gpio_def_2210P.h"
#pragma message("pbi_gpio_def_2210P")

#elif defined DEVICE_3200P
#include "pbi_gpio_def_3200P.h"
#pragma message("pbi_gpio_def_3200P")

#elif defined DEVICE_5600P
#include "pbi_gpio_def_5600P.h"
#pragma message("pbi_gpio_def_5600P")

#elif defined DEVICE_3800D
#include "pbi_gpio_def_3800D.h"
#pragma message("pbi_gpio_def_3800D")

#else

#endif

typedef enum {
	PBI_GPIO_INPUT,
	PBI_GPIO_OUTPUT
}pbi_gpio_dir_e;


extern  void pbi_gpio_init(void);
extern int pbi_gpio_read(unsigned int gpio_no);
extern void pbi_gpio_write(unsigned int gpio_no, int level);
extern void pbi_gpio_set_dir(unsigned int gpio_no,  pbi_gpio_dir_e direction);
extern int pbi_gpio_get_dir(unsigned int gpio_no);

	
#endif
