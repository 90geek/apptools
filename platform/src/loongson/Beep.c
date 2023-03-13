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

#include "edk_api.h"
#include "Ls7aGpio.h"
#include "Beep.h"
#include "CpuGpioLib.h"
#include "platform/app_os.h"
#include "LsRegDef.h"

#define GPIO0                          0
//#define PWM_FREQ                       2000 //2Khz
VOID
GpioSimulatePwm (
    UINT16  GpioNum,
    UINT32  PwmFreq,  //unit hz
    UINT32  KeepMs    //unit ms
    )
{
    UINTN  Count, Ticks, Start, End;
    UINT64 FreqCoverTime = 1000*1000 / PwmFreq / 2; // 1/pwmFreq=cycle,cycle*1000*1000=us

    Count = KeepMs * 1000;
    Start = app_get_time_us();
    End = Start + Count;

    do {
        Ls7aGpioSetVal (GpioNum,0);
        app_sleep_us(FreqCoverTime);
        Ls7aGpioSetVal (GpioNum,1);
        app_sleep_us(FreqCoverTime);
        Ticks = app_get_time_us();
    } while (Ticks < End);
}

VOID
LsCpuGpioSimulatePwm (
    UINT16  GpioNum,
    UINT32  PwmFreq,  //unit hz
    UINT32  KeepMs    //unit ms
    )
{
    UINTN  Count, Ticks, Start, End;
    UINT64 FreqCoverTime = 1000*1000 / PwmFreq / 2; // 1/pwmFreq=cycle,cycle*1000*1000=us

    Count = KeepMs * 1000;
    Start = app_get_time_us();
    End = Start + Count;

    do {
        GpioSetOutLow(CPU_GPIO_BASE,0);
        app_sleep_us(FreqCoverTime);
        GpioSetOutHigh(CPU_GPIO_BASE,0);
        app_sleep_us(FreqCoverTime);
        Ticks = app_get_time_us();
    } while (Ticks < End);
}

EFI_STATUS
BeepPwmOn (
  UINT32   KeepMs,
  UINT32   PwmFreq
  )
{
  GpioSimulatePwm (GPIO0,PwmFreq,KeepMs);
  return EFI_SUCCESS;
}

EFI_STATUS
LsCpuBeepPwmOn (
  UINT32   KeepMs,
  UINT32   PwmFreq
  )
{
  LsCpuGpioSimulatePwm (GPIO0,PwmFreq,KeepMs);
  return EFI_SUCCESS;
}
EFI_STATUS
BeepOn (
  VOID
  )
{
  Ls7aGpioSetVal (GPIO0,1); // High is open
  return EFI_SUCCESS;
}

EFI_STATUS
BeepOff (
  VOID
  )
{
  Ls7aGpioSetVal (GPIO0,0); // Low is close
  return EFI_SUCCESS;
}

LS_BEEP_OPS  LsBeepOps;
EFI_STATUS
RegBeepFunc (
    )
{
  LsBeepOps.BeepOn        =  BeepOn;
  LsBeepOps.BeepOff       =  BeepOff;
  LsBeepOps.BeepPwmOn     =  BeepPwmOn;

  return EFI_SUCCESS;
}
