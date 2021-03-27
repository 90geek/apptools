/*
 * pbi_modules.h
 *
 *  Created on: 2017Äê11ÔÂ17ÈÕ
 *      Author: qqtian
 */

#ifndef __PBI_MODULE_H__
#define __PBI_MODULE_H__



typedef struct pbi_module_s
{
	char *module_name;
	int module_version;
	void * module_support;

} pbi_module_t;


#endif /*  */
