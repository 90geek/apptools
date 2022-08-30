
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

#include "loongson/LsRegDef.h"
#include "edk_api.h"
#define NODE_OFFSET 44

// #define PHYS_TO_CACHED(x)       (CACHED_MEMORY_ADDR | (x))
// #define PHYS_TO_UNCACHED(x)     (UNCACHED_MEMORY_ADDR | (x))
#define LS3A4000_VERSION                        0x0000303030344133 /* 3A4000 */

#define LS3A5000_VERSION                        0x0000303030354133 /* 3A5000 */
#define LS3A5000LL_VERSION                      0x4C4C303030354133 /* 3A5000LL */
#define LS3A5000M_VERSION                       0x004D303030354133 /* 3A5000M */
#define LS3B5000_VERSION                        0x0000303030354233 /* 3B5000 */
#define LS3C5000L_VERSION                       0x004C303030354333 /* 3C5000L */
#define LS3C5000LL_VERSION                      0x4C4C303030354333 /* 3C5000LL */
#define LS3A5000I_VERSION                       0x0049303030354133 /* 3A5000I */
#define LS3A5000i_VERSION                       0x0069303030354133 /* 3A5000i */
#define LS3A5000BM_VERSION                      0x4D42303030354133 /* 3A5000BM */
#define LS3A5000HV_VERSION                      0x5648303030354133 /* 3A5000HV */
#define LS3C5000_VERSION                        0x0000303030354333 /* 3C5000 */

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


#endif

