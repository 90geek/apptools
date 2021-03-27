
/*****************************************************************************
* (c) Copyright Pro. Broadband Inc. PBI-DHE
*
* File name : fpga.h
* Description : 
* Created : QQTian
* History : ; Initial Version:
******************************************************************************/
#ifndef __FPGA_H
#define __FPGA_H
/*******************************************************/
/* Includes */
/*******************************************************/
#include "pbi_os.h"
#ifdef __cplusplus
extern "C" {
#endif
/*******************************************************/
/* Exported Defines and Macros */
/*******************************************************/


/**/

/*******************************************************/
/* Exported Types */
/*******************************************************/
/*******************************************************/
/* Exported Variables */
/*******************************************************/

/*******************************************************/
/* External */
/*******************************************************/
/*******************************************************/
/* Exported Functions */
/*******************************************************/

#define MAIN_FPGA 0
#define IP_FPGA 1

void fpga_init(void);
void fpga_config(int fpga_index);

int setup_fpga(int fpga_index, char* bin_file);

U32 get_fpga_version(int fpga_index);

U32 fpga_read8(int fpga_index, U32 address);

void fpga_write8(int fpga_index, U32 address, U32 data) ;

U32 fpga_read16(int fpga_index, U32 address);

void fpga_write16(int fpga_index, U32 address, U32 data) ;

U32  fpga_read32(int fpga_index, U32 address);

void fpga_write32(int fpga_index, U32 address, U32 data);

void fpga_read_burst(int fpga_index, U32 address, U16 *datas, int length);

void fpga_write_burst(int fpga_index, U32 address, U16 *datas, int length) ;


#ifdef __cplusplus
}
#endif
#endif /* #ifndef __FPGA_H */
/* End of fpga.h --------------------------------------------------------- */
