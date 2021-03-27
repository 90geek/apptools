/*
 * fpga_reg.h
 *
 *  Created on: Mar 17, 2016
 *      Author: qqtian
 */

#ifndef FPGA_REG_H_
#define FPGA_REG_H_

#ifdef DEVICE_2210P
/******************相关寄存器地址*******************************/
#define FPGA_SDRAM_BASE			0x0		
#define FPGA_REG_BASE			0x09800000  //to 0x98001FF
#define FPGA_TSCTL_REG			0x0800800
#define FPGA_TSCTL_REG_0		0x09801000

#define FPGA_VER_REG 			(FPGA_REG_BASE + (0xF0<<2))
#define FPGA_DOC_VER_REG 		(FPGA_REG_BASE + (0xF1<<2))

#elif defined DEVICE_5600P
/******************相关寄存器地址*******************************/
#define FPGA_SDRAM_BASE			0x0
#define FPGA_REG_BASE    		0x1001000  //to 0x98001FF
#define FPGA_TSCTL_REG			0x1000800
#define FPGA_TSCTL_REG_0		0x1004000

#define FPGA_VER_REG 			(FPGA_REG_BASE + (0xF0<<2))
#define FPGA_DOC_VER_REG 		(FPGA_REG_BASE + (0xF1<<2))


#elif defined DEVICE_3800D
/******************相关寄存器地址*******************************/
#define FPGA_SDRAM_BASE			0x8000000
#define FPGA_REG_BASE    		0x0800  
#define FPGA_TSCTL_REG			0x1000
#define FPGA_TSCTL_REG_0		0x1400

#define FPGA_VER_REG 			(FPGA_REG_BASE + (0xF0<<2))
#define FPGA_DOC_VER_REG 		(FPGA_REG_BASE + (0xF1<<2))

#else

/******************相关寄存器地址*******************************/
#define FPGA_SDRAM_BASE			0x0
#define FPGA_REG_BASE			0x09800000  //to 0x98001FF
#define FPGA_TSCTL_REG			0x0800800
#define FPGA_CTL_REG			0x09800800	//to 0x98009FF
#define FPGA_TSCTL_REG_0		0x0804000

#define FPGA_VER_REG 			(FPGA_REG_BASE + (0xF0<<2))
#define FPGA_DOC_VER_REG 		(FPGA_REG_BASE + (0xF1<<2))
#endif

#endif /* FPGA_REG_H_ */
