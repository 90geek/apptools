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
#include "LsRegDef.h"
#include "platform/app_types.h"
#include "platform/app_platform.h"

UINT64 PwmRegBaseAddr;
UINT64 TempRegBaseAddr;
//
// Ls7a-SmartFan
//
typedef struct {
  UINT32  MinRpm;
  UINT32  MaxRpm;
}LS7A_SMARTFAN_CFG_TABLE;
LS7A_SMARTFAN_CFG_TABLE  SmartFanParameter;

EFI_STATUS
Ls7aPwm (
  LS7A_SMARTFAN_CFG_TABLE *Parameter
  )
{
  UINT32 X, Y;

  X = Parameter->MinRpm;
  Y = Parameter->MaxRpm;

  Readl (get_pwm_mem_base + LS7A_PWM0_CTRL) &= ~1;
  Writel (get_pwm_mem_base + LS7A_PWM0_LOW,X);
  Writel (get_pwm_mem_base + LS7A_PWM0_FULL,Y);
  Readl (get_pwm_mem_base + LS7A_PWM0_CTRL) |= 1;

  Readl (get_pwm_mem_base + LS7A_PWM1_CTRL) &= ~1;
  Writel (get_pwm_mem_base + LS7A_PWM1_LOW,X);
  Writel (get_pwm_mem_base + LS7A_PWM1_FULL,Y);
  Readl (get_pwm_mem_base + LS7A_PWM1_CTRL) |= 1;

  //
  // as now, the 7A Fan control circuit(PWM2) has problem, keep it constant to avoid wearing fan.
  //
  /*
    readl (get_pwm_mem_base + LS7A_PWM2_CTRL) &= ~1;
    outl (get_pwm_mem_base + LS7A_PWM2_LOW,x);
    outl (get_pwm_mem_base + LS7A_PWM2_FULL,y);
    readl (get_pwm_mem_base + LS7A_PWM2_CTRL) |= 1;
  */
  Readl (get_pwm_mem_base + LS7A_PWM2_LOW) = 0;
  Readl (get_pwm_mem_base + LS7A_PWM2_FULL) = 100;
  Readl (get_pwm_mem_base + LS7A_PWM2_CTRL) = 1;

  Readl (get_pwm_mem_base + LS7A_PWM3_CTRL) &= ~1;
  Writel (get_pwm_mem_base + LS7A_PWM3_LOW,X);
  Writel (get_pwm_mem_base + LS7A_PWM3_FULL,Y);
  Readl (get_pwm_mem_base + LS7A_PWM3_CTRL) |= 1;
  return 0;
}

EFI_STATUS
Ls7aPwm0Set (
  LS7A_SMARTFAN_CFG_TABLE *Parameter
  )
{
  UINT32 X, Y;

  X = Parameter->MinRpm;
  Y = Parameter->MaxRpm;

  Readl (get_pwm_mem_base + LS7A_PWM0_CTRL) &= ~1;
  Writel (get_pwm_mem_base + LS7A_PWM0_LOW,X);
  Writel (get_pwm_mem_base + LS7A_PWM0_FULL,Y);
  Readl (get_pwm_mem_base + LS7A_PWM0_CTRL) |= 1;

  return 0;
}
EFI_STATUS
Ls7aPwm1Set (
  LS7A_SMARTFAN_CFG_TABLE *Parameter
  )
{
  UINT32 X, Y;

  X = Parameter->MinRpm;
  Y = Parameter->MaxRpm;

  Readl (get_pwm_mem_base + LS7A_PWM1_CTRL) &= ~1;
  Writel (get_pwm_mem_base + LS7A_PWM1_LOW,X);
  Writel (get_pwm_mem_base + LS7A_PWM1_FULL,Y);
  Readl (get_pwm_mem_base + LS7A_PWM1_CTRL) |= 1;

  return 0;
}
EFI_STATUS
Ls7aPwm2Set (
  LS7A_SMARTFAN_CFG_TABLE *Parameter
  )
{
  UINT32 X, Y;

  X = Parameter->MinRpm;
  Y = Parameter->MaxRpm;

  Readl (get_pwm_mem_base + LS7A_PWM2_CTRL) &= ~1;
  Writel (get_pwm_mem_base + LS7A_PWM2_LOW,X);
  Writel (get_pwm_mem_base + LS7A_PWM2_FULL,Y);
  Readl (get_pwm_mem_base + LS7A_PWM2_CTRL) |= 1;

  return 0;
}
EFI_STATUS
Ls7aPwm3Set (
  LS7A_SMARTFAN_CFG_TABLE *Parameter
  )
{
  UINT32 X, Y;

  X = Parameter->MinRpm;
  Y = Parameter->MaxRpm;

  Readl (get_pwm_mem_base + LS7A_PWM3_CTRL) &= ~1;
  Writel (get_pwm_mem_base + LS7A_PWM3_LOW,X);
  Writel (get_pwm_mem_base + LS7A_PWM3_FULL,Y);
  Readl (get_pwm_mem_base + LS7A_PWM3_CTRL) |= 1;

  return 0;
}
void ls132_speed()
{
  UINT32 cpufanspd;
  UINT32 val32;
  UINT32 P1full,P1low;
  //设置pwm1为测量模式（输入）
  val32 = Readl(get_pwm_mem_base + LS7A_PWM1_CTRL);
  val32 &= ~(0x1);
  Writel(get_pwm_mem_base + LS7A_PWM1_CTRL,val32);

  P1low  = Readl(get_pwm_mem_base + LS7A_PWM1_LOW);
  P1full = Readl(get_pwm_mem_base + LS7A_PWM1_FULL);

  val32 = Readl(get_pwm_mem_base + LS7A_PWM1_CTRL);
  val32 |= 0x1;
  Writel(get_pwm_mem_base + LS7A_PWM1_CTRL, val32);

  //spd = 50 * 1000000 / P1full * 30;
  cpufanspd = 50 * 1000000 / P1full * 30;
  printf("cpu_fan speed=%d\r\n",cpufanspd);
}

UINT8 cpucount[7];
UINT8 lastFanFlag = 10;
UINT8 fanflag = 0;  //默认情况风扇最低转速
char  cputemp0,cputemp1;
char  cputemp;

UINT8 ls132_tempdetect()
{
  UINT32 val32;
  UINT16 tmp0,tmp1;

#if 1
    val32 = Readl(TempRegBaseAddr);
    tmp0 = val32 & 0xffff;                      //取低16位数据
    cputemp0 = tmp0 * 731 / 0x4000 - 273;
    tmp1 = (val32 & (0xffff << 16)) >> 16;      //取高16位数据
    cputemp1 = tmp1 * 731 / 0x4000 - 273;
#endif
    cputemp = (cputemp0 + cputemp1) / 2;

    if(cputemp <= 38) {
      cpucount[0]++;
      if(cpucount[0] == 3) {
        fanflag = 0;
        cpucount[0] = 0;
      }
      cpucount[1]=0;
      cpucount[2]=0;
      cpucount[3]=0;
      cpucount[4]=0;
      cpucount[5]=0;
    } else if(cputemp > 38 && cputemp <=45) {
      cpucount[1]++;
      if(cpucount[1] == 3) {
        fanflag = 1;
        cpucount[1] = 0;
      }
      cpucount[0]=0;
      cpucount[2]=0;
      cpucount[3]=0;
      cpucount[4]=0;
      cpucount[5]=0;
    //} else if(cputemp > 45 && cputemp <=55) {
    } else if(cputemp > 45 && cputemp <=46) {
      cpucount[2]++;
      if(cpucount[2] == 3) {
        fanflag = 2;
        cpucount[2] = 0;
      }
      cpucount[0]=0;
      cpucount[1]=0;
      cpucount[3]=0;
      cpucount[4]=0;
      cpucount[5]=0;
    //} else if(cputemp > 55 && cputemp <=65) {
    } else if(cputemp > 46 && cputemp <=47) {
      cpucount[3]++;
      if(cpucount[3] == 3) {
        fanflag = 3;
        cpucount[3] = 0;
      }
      cpucount[0]=0;
      cpucount[1]=0;
      cpucount[2]=0;
      cpucount[4]=0;
      cpucount[5]=0;
    } else if(cputemp > 47 && cputemp <=48) {
      cpucount[4]++;
      if(cpucount[4] == 3) {
        fanflag = 4;
        cpucount[4] = 0;
      }
      cpucount[0]=0;
      cpucount[1]=0;
      cpucount[2]=0;
      cpucount[3]=0;
      cpucount[5]=0;
    } else if(cputemp > 48 ) {
      cpucount[5]++;
      if(cpucount[5] == 3) {
        fanflag = 5;
        cpucount[5] = 0;
      }
      cpucount[0]=0;
      cpucount[1]=0;
      cpucount[2]=0;
      cpucount[3]=0;
      cpucount[4]=0;
    } else {
      /*do nothing*/
    }
  printf("cpu tempdetect=%d,fanflag=%d,lastFanFlag=%d\n",cputemp,fanflag,lastFanFlag);
  return fanflag;
}

int SmartFanSet (UINT8 Percent)
{
  int Status=0;

  SmartFanParameter.MinRpm = 100 * Percent;
  SmartFanParameter.MaxRpm = 10000;
  printf ("SmartFan Init, MinRpm:%d ,MaxRpm;%d \n",\
          SmartFanParameter.MinRpm,SmartFanParameter.MaxRpm);

  Status = Ls7aPwm0Set(&SmartFanParameter);

  return Status;
}

// void SetFan(DevNode *this,int fd)
// {
//   unsigned long long c = 0;
//   int status ;
//   char RecordName[100];
// 	void * p = NULL;

//   p = vtpa(this->devaddr,fd);
//   get_pwm_mem_base=p;
//   // printf("pwm base addr:0x%llx\n",get_pwm_mem_base);
//   printf("Please Input Fan Percent(0~100):");
//   status = scanf("%s",RecordName);
//   sscanf (RecordName,"%d",&c);
//   // c = atoi(RecordName);
//   if(c>=0&&c<=100)
//   {
//     printf("Set Fan for %d%\n",c);
//     SmartFanSet (100-c);
//   }
//   else
//     printf("Invalid parameter Please Input:0~100\n");
// }

// void ReadFanSpeed(DevNode *this,int fd)
// {
//   unsigned long long c = 0;
//   int status ;
//   char RecordName[100];
// 	void * p = NULL;

//   p = vtpa(this->devaddr,fd);
//   get_pwm_mem_base=p;
//   // printf("pwm base addr:0x%llx\n",get_pwm_mem_base);
//   ls132_speed();
// }

// void cpu_temperature(DevNode *this,int fd)
// {
//   unsigned long long c = 0;
//   int status ;
//   char RecordName[100];
// 	void * p = NULL;

//   p = vtpa(this->devaddr,fd);
//   TempRegBaseAddr=p;
//   // printf("temp base addr:0x%llx\n",TempRegBaseAddr);
//   ls132_tempdetect ();
// }
// DevNode PwmInstance = {
//     "pwm",
//     NULL,
//     LS7A_MISC_BASE_ADDR + LS7A_PWM_BASE,
//     NULL,
//     NULL
// };

// DevNode TempInstance = {
//     "temp",
//     NULL,
//     CPU_TEMP_REG,
//     NULL,
//     NULL
// };

// Cmd PwmCmd[3] = {
//   {"-set fan",SetFan},
//   {"-read fan speed",ReadFanSpeed},
//   {NULL,NULL}
// };
// Cmd TempCmd[2] = {
//   {"-temperature", cpu_temperature},
//   {NULL,NULL}
// };
// void PwmInitInstance(void)
// {
//    PwmInstance.CmdInstance = PwmCmd;
//    DevInstanceInsert(&PwmInstance);

//    TempInstance.CmdInstance = TempCmd;
//    DevInstanceInsert(&TempInstance);
// }
