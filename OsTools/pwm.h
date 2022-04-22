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
#ifndef  _PWM_H_
#define  _PWM_H_

#define LS7A_MISC_BASE			(0x0)
#define LS7A_PWM_BASE		(LS7A_MISC_BASE + 0x20000)

/*PWM*/
#define LS7A_PWM0_REG_BASE		(LS7A_MISC_BASE + 0x0)
#define LS7A_PWM0_LOW			  (LS7A_PWM0_REG_BASE + 0x4)
#define LS7A_PWM0_FULL			(LS7A_PWM0_REG_BASE + 0x8)
#define LS7A_PWM0_CTRL			(LS7A_PWM0_REG_BASE + 0xc)

#define LS7A_PWM1_REG_BASE		(LS7A_MISC_BASE + 0x100)
#define LS7A_PWM1_LOW			(LS7A_PWM1_REG_BASE + 0x4)
#define LS7A_PWM1_FULL			(LS7A_PWM1_REG_BASE + 0x8)
#define LS7A_PWM1_CTRL			(LS7A_PWM1_REG_BASE + 0xc)

#define LS7A_PWM2_REG_BASE		(LS7A_MISC_BASE + 0x200)
#define LS7A_PWM2_LOW			(LS7A_PWM2_REG_BASE + 0x4)
#define LS7A_PWM2_FULL			(LS7A_PWM2_REG_BASE + 0x8)
#define LS7A_PWM2_CTRL			(LS7A_PWM2_REG_BASE + 0xc)

#define LS7A_PWM3_REG_BASE		(LS7A_MISC_BASE + 0x300)
#define LS7A_PWM3_LOW			(LS7A_PWM3_REG_BASE + 0x4)
#define LS7A_PWM3_FULL			(LS7A_PWM3_REG_BASE + 0x8)
#define LS7A_PWM3_CTRL			(LS7A_PWM3_REG_BASE + 0xc)

#define CPU_TEMP_REG       (0x1fe00198 + 0x4)

void PwmInitInstance(void);

#endif
