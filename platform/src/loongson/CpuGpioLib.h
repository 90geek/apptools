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
#ifndef  __CPU_GPIO_LIB_H__
#define  __CPU_GPIO_LIB_H__


#include "edk_api.h"

#define GPIO0                   0
#define CPU_GPIO_BASE           PHYS_TO_UNCACHED(0x1fe00500)
/* set GPIO as output
 * x : 0x1 << offset
 */
/******************************************************
 *used: t0~t2
 ******************************************************/
#define GPIO_SET_OUTPUT(x)                          \
    li.d    t0, (UNCACHE_DWM_BASE | 0x1fe00500);    \
    ld.w    t1, t0, 0xc;                            \
    li.w    t2, x;                                  \
    or      t1, t1, t2;                             \
    st.w    t1, t0, 0x8;                            \
    ld.w    t1, t0, 0x0;                            \
    or      t1, t1, t2;                             \
    xor     t1, t1, t2;                             \
    st.w    t1, t0, 0x0;                            \
    ld.w    t1, t0, 0x4;                            \
    or      t1, t1, t2;                             \
    xor     t1, t1, t2;                             \
    st.w    t1, t0, 0x4;

/* clear GPIO as output
 *  * x : 0x1 <<offsest
 *	*/
#define GPIO_CLEAR_OUTPUT(x)                        \
    li.d    a0, (UNCACHE_DWM_BASE | 0x1fe00500);    \
    ld.w    a1, a0, 0xc;                            \
    li.w    a2, x;                                  \
    or      a1, a1, a2;                             \
    xor     a1, a1, a2;                             \
    st.w    a1, a0, 0x8;                            \
    ld.w    a1, a0, 0x0;                            \
    or      a1, a1, a2;                             \
    xor     a1, a1, a2;                             \
    st.w    a1, a0, 0x0;                            \
    ld.w    a1, a0, 0x4;                            \
    or      a1, a1, a2;                             \
    xor     a1, a1, a2;                             \
    st.w    a1, a0, 0x4;

EFI_STATUS
EFIAPI GpioSetOutHigh(
  IN UINTN                         GpioBase,
  IN UINT8                         GpioNum
  );

EFI_STATUS
EFIAPI GpioSetOutLow(
  IN UINTN                         GpioBase,
  IN UINT8                         GpioNum
  );

EFI_STATUS
EFIAPI GpioSetInMode(
  IN UINTN                         GpioBase,
  IN UINT8                         GpioNum
  );

#endif
