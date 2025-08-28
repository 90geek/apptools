
/*
## @file
#
#  Copyright (c) 2018 Loongson Technology Corporation Limited (www.loongson.cn).
#  All intellectual property rights(Copyright, Patent and Trademark) reserved.
#
#  Any violations of copyright or other intellectual property rights of the Loongson Technology
#  Corporation Limited will be held accountable in accordance with the law,
#  if you (or any of your subsidiaries, corporate affiliates or agents) initiate
#  directly or indirectly any Intellectual Property Assertion or Intellectual Property Litigation:
#  (i) against Loongson Technology Corporation Limited or any of its subsidiaries or corporate affiliates,
#  (ii) against any party if such Intellectual Property Assertion or Intellectual Property Litigation arises
#  in whole or in part from any software, technology, product or service of Loongson Technology Corporation
#  Limited or any of its subsidiaries or corporate affiliates, or (iii) against any party relating to the Software.
#
#  THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
#  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR
#  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION).
#
#
##
*/

#ifndef __LS3A_DEF_H__
#define __LS3A_DEF_H__

#include "edk_api.h"

#define LS3A4000_VERSION                        0x0000303030344133 /* 3A4000 */
#define LS2K2000_VERSION                        0x303030324b32 /* 2K2000 */
/* 3A6000 */
#define LS3A6000_VERSION_LO                        0x0000303030364133
#define LS3A6000_VERSION_HI                        0x0000000000000000
/* 3A6000-HV */
#define LS3A6000_HV_VERSION_LO                     0x482D303030364133
#define LS3A6000_HV_VERSION_HI                     0x0000000000000056
/* 3A6000-LL */
#define LS3A6000_LL_VERSION_LO                     0x4C2D303030364133
#define LS3A6000_LL_VERSION_HI                     0x000000000000004C
/* 3A6000M */
#define LS3A6000M_VERSION_LO                       0x004D303030364133
#define LS3A6000M_VERSION_HI                       0x0000000000000000
/* 3A6000-I */
#define LS3A6000_I_VERSION_LO                      0x492D303030364133
#define LS3A6000_I_VERSION_HI                      0x0000000000000000
/* 3A6000-I-HV */
#define LS3A6000_I_HV_VERSION_LO                   0x492D303030364133
#define LS3A6000_I_HV_VERSION_HI                   0x000000000056482D
/* 3A6000-i */
#define LS3A6000_i_VERSION_LO                      0x692D303030364133
#define LS3A6000_i_VERSION_HI                      0x0000000000000000
/* 3A6000-i-HV */
#define LS3A6000_i_HV_VERSION_LO                   0x692D303030364133
#define LS3A6000_i_HV_VERSION_HI                   0x000000000056482D
/* 3A5000 */
#define LS3A5000_VERSION_LO                        0x0000303030354133
#define LS3A5000_VERSION_HI                        0x0000000000000000
/* 3A5000-LL */
#define LS3A5000_LL_VERSION_LO                     0x4C2D303030354133
#define LS3A5000_LL_VERSION_HI                     0x000000000000004C
/* 3A5000M */
#define LS3A5000M_VERSION_LO                       0x004D303030354133
#define LS3A5000M_VERSION_HI                       0x0000000000000000
/* 3B5000 */
#define LS3B5000_VERSION_LO                        0x0000303030354233
#define LS3B5000_VERSION_HI                        0x0000000000000000
/* 3C5000L */
#define LS3C5000L_VERSION_LO                       0x004C303030354333
#define LS3C5000L_VERSION_HI                       0x0000000000000000
/* 3C5000L-LL */
#define LS3C5000L_LL_VERSION_LO                    0x2D4C303030354333
#define LS3C5000L_LL_VERSION_HI                    0x0000000000004C4C
/* 3A5000-K */
#define LS3A5000_K_VERSION_LO                      0x4B2D303030354133
#define LS3A5000_K_VERSION_HI                      0x0000000000000000
/* 3A5000-K-HV */
#define LS3A5000_K_HV_VERSION_LO                   0x4B2D303030354133
#define LS3A5000_K_HV_VERSION_HI                   0x000000000056482D
/* 3A5000-i */
#define LS3A5000_i_VERSION_LO                      0x692D303030354133
#define LS3A5000_i_VERSION_HI                      0x0000000000000000
/* 3A5000-i-HV */
#define LS3A5000_i_HV_VERSION_LO                   0x692D303030354133
#define LS3A5000_i_HV_VERSION_HI                   0x000000000056482D
/* 3A5000-BM */
#define LS3A5000_BM_VERSION_LO                     0x422D303030354133
#define LS3A5000_BM_VERSION_HI                     0x000000000000004D
/* 3A5000-HV */
#define LS3A5000_HV_VERSION_LO                     0x482D303030354133
#define LS3A5000_HV_VERSION_HI                     0x0000000000000056
/* 3C5000 */
#define LS3C5000_VERSION_LO                        0x0000303030354333
#define LS3C5000_VERSION_HI                        0x0000000000000000
/* 3C5000-LL */
#define LS3C5000_LL_VERSION_LO                     0x4C2D303030354333
#define LS3C5000_LL_VERSION_HI                     0x000000000000004C
/* 3D5000 */
#define LS3D5000_VERSION_LO                        0x0000303030354433
#define LS3D5000_VERSION_HI                        0x0000000000000000

#define LSCPU_ID	0x1fe00020
#define LS7A_VER	LS7A_CONFBUS_BASE_ADDR|0x3ff8
#define LS7A_VER180_REG	HT_CONF_TYPE0_ADDR|0x108

#define CPU_I2C0 0x1fe00120
#define NODE0_CPU_I2C1 0x1fe00130
#define NODE4_CPU_I2C1 0x40001fe00130ULL
#define NODE8_CPU_I2C1 0x80001fe00130ULL
#define NODE12_CPU_I2C1 0xC0001fe00130ULL

typedef struct {
		UINT64 l;
		UINT64 h;
}CpuId;

#define GS3_UART0_BASE (UNCACHE_DWM_BASE | 0x1fe001e0)
#define GS3_UART_BASE  GS3_UART0_BASE

#ifdef LOONGSON_3D5000
#define VOL_mV(x) (x)
#else
#define VOL_mV(x) (x / MPS_STEP - 49)
#endif

#ifdef DEVBD2F_SM502
#define GPIOLED_DIR  0xe
#else
#define GPIOLED_DIR  0xf
#endif

#undef USE_GPIO_SERIAL
#ifndef USE_GPIO_SERIAL
#else
#define GPIOLED_SET(x)
#endif

#endif

