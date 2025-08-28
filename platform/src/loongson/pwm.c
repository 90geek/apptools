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
#include "edk_api.h"
#include "platform/app_platform.h"

#define LS_TEMP0 58
#define LS_TEMP1 60
#define LS_TEMP2 62
#define LS_TEMP3 64
#define LS_TEMP4 66
#define LS_TEMP5 68
#define LS_TEMP6 70
#define LS_TEMP7 72
#define LS_TEMP8 75
#define LS_TEMP9 76
#define LS_TEMP10 78
#define LS_TEMP11 79
#define LS_TEMP12 80

UINT64 PwmRegBaseAddr;
UINT64 TempRegBaseAddr;

EFI_STATUS
Ls7aPwm (
	LS7A_SMARTFAN_CFG_TABLE *Parameter
	)
{
	UINT32 X, Y;

	X = Parameter->MinRpm;
	Y = Parameter->MaxRpm;

	Readl (get_pwm_mem_base() + LS7A_PWM0_CTRL) &= ~1;
	Writel (get_pwm_mem_base() + LS7A_PWM0_LOW,X);
	Writel (get_pwm_mem_base() + LS7A_PWM0_FULL,Y);
	Readl (get_pwm_mem_base() + LS7A_PWM0_CTRL) |= 1;

	Readl (get_pwm_mem_base() + LS7A_PWM1_CTRL) &= ~1;
	Writel (get_pwm_mem_base() + LS7A_PWM1_LOW,X);
	Writel (get_pwm_mem_base() + LS7A_PWM1_FULL,Y);
	Readl (get_pwm_mem_base() + LS7A_PWM1_CTRL) |= 1;

	//
	// as now, the 7A Fan control circuit(PWM2) has problem, keep it constant to avoid wearing fan.
	//
	/*
		readl (get_pwm_mem_base + LS7A_PWM2_CTRL) &= ~1;
		outl (get_pwm_mem_base + LS7A_PWM2_LOW,x);
		outl (get_pwm_mem_base + LS7A_PWM2_FULL,y);
		readl (get_pwm_mem_base + LS7A_PWM2_CTRL) |= 1;
	*/
	Readl (get_pwm_mem_base() + LS7A_PWM2_LOW) = 0;
	Readl (get_pwm_mem_base() + LS7A_PWM2_FULL) = 100;
	Readl (get_pwm_mem_base() + LS7A_PWM2_CTRL) = 1;

	Readl (get_pwm_mem_base() + LS7A_PWM3_CTRL) &= ~1;
	Writel (get_pwm_mem_base() + LS7A_PWM3_LOW,X);
	Writel (get_pwm_mem_base() + LS7A_PWM3_FULL,Y);
	Readl (get_pwm_mem_base() + LS7A_PWM3_CTRL) |= 1;
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

	Readl (get_pwm_mem_base() + LS7A_PWM0_CTRL) &= ~1;
	Writel (get_pwm_mem_base() + LS7A_PWM0_LOW,X);
	Writel (get_pwm_mem_base() + LS7A_PWM0_FULL,Y);
	Readl (get_pwm_mem_base() + LS7A_PWM0_CTRL) |= 1;

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

	Readl (get_pwm_mem_base() + LS7A_PWM1_CTRL) &= ~1;
	Writel (get_pwm_mem_base() + LS7A_PWM1_LOW,X);
	Writel (get_pwm_mem_base() + LS7A_PWM1_FULL,Y);
	Readl (get_pwm_mem_base() + LS7A_PWM1_CTRL) |= 1;

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

	Readl (get_pwm_mem_base() + LS7A_PWM2_CTRL) &= ~1;
	Writel (get_pwm_mem_base() + LS7A_PWM2_LOW,X);
	Writel (get_pwm_mem_base() + LS7A_PWM2_FULL,Y);
	Readl (get_pwm_mem_base() + LS7A_PWM2_CTRL) |= 1;

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

	Readl (get_pwm_mem_base() + LS7A_PWM3_CTRL) &= ~1;
	Writel (get_pwm_mem_base() + LS7A_PWM3_LOW,X);
	Writel (get_pwm_mem_base() + LS7A_PWM3_FULL,Y);
	Readl (get_pwm_mem_base() + LS7A_PWM3_CTRL) |= 1;

	return 0;
}

UINT32 Ls7aPwm0Get()
{
	UINT32 cpufanspd;
	UINT32 val32;
	UINT32 P1full,P1low;

	//设置pwm0为测量模式（输入）
	val32 = Readl(get_pwm_mem_base() + LS7A_PWM0_CTRL);
	val32 |= ((0x1<<8) | 0x1);
	Writel(get_pwm_mem_base() + LS7A_PWM0_CTRL, val32);

	// 停止计算
	val32 = Readl(get_pwm_mem_base() + LS7A_PWM0_CTRL);
	val32 &= ~(0x1);
	Writel(get_pwm_mem_base() + LS7A_PWM0_CTRL,val32);

	P1low  = Readl(get_pwm_mem_base() + LS7A_PWM0_LOW);
	P1full = Readl(get_pwm_mem_base() + LS7A_PWM0_FULL);

	val32 = Readl(get_pwm_mem_base() + LS7A_PWM0_CTRL);
	val32 |= 0x1;
	Writel(get_pwm_mem_base() + LS7A_PWM0_CTRL, val32);

	//spd = 50 * 1000000 / P1full * 30;
	cpufanspd = 50 * 1000000 / P1full * 30;
	printf("P1low %d P1full %d cpu_fan speed=%d\r\n",P1low, P1full, cpufanspd);
	return cpufanspd;
}

UINT32 Ls7aPwm1Get()
{
	UINT32 cpufanspd;
	UINT32 val32;
	UINT32 P1full,P1low;

	//设置pwm1为测量模式（输入）
	val32 = Readl(get_pwm_mem_base() + LS7A_PWM1_CTRL);
	val32 |= ((0x1<<8) | 0x1);
	Writel(get_pwm_mem_base() + LS7A_PWM1_CTRL, val32);

	// 停止计算
	val32 = Readl(get_pwm_mem_base() + LS7A_PWM1_CTRL);
	val32 &= ~(0x1);
	Writel(get_pwm_mem_base() + LS7A_PWM1_CTRL,val32);

	P1low  = Readl(get_pwm_mem_base() + LS7A_PWM1_LOW);
	P1full = Readl(get_pwm_mem_base() + LS7A_PWM1_FULL);

	val32 = Readl(get_pwm_mem_base() + LS7A_PWM1_CTRL);
	val32 |= 0x1;
	Writel(get_pwm_mem_base() + LS7A_PWM1_CTRL, val32);

	//spd = 50 * 1000000 / P1full * 30;
	cpufanspd = 50 * 1000000 / P1full * 30;
	printf("P1low %d P1full %d cpu_fan speed=%d\r\n",P1low, P1full, cpufanspd);
	return cpufanspd;
}

UINT32 Ls7aPwm2Get()
{
	UINT32 cpufanspd;
	UINT32 val32;
	UINT32 P1full,P1low;

	//设置pwm2为测量模式（输入）
	val32 = Readl(get_pwm_mem_base() + LS7A_PWM2_CTRL);
	val32 |= ((0x1<<8) | 0x1);
	Writel(get_pwm_mem_base() + LS7A_PWM2_CTRL, val32);

	// 停止计算
	val32 = Readl(get_pwm_mem_base() + LS7A_PWM2_CTRL);
	val32 &= ~(0x1);
	Writel(get_pwm_mem_base() + LS7A_PWM2_CTRL,val32);

	P1low  = Readl(get_pwm_mem_base() + LS7A_PWM2_LOW);
	P1full = Readl(get_pwm_mem_base() + LS7A_PWM2_FULL);

	val32 = Readl(get_pwm_mem_base() + LS7A_PWM2_CTRL);
	val32 |= 0x1;
	Writel(get_pwm_mem_base() + LS7A_PWM2_CTRL, val32);

	//spd = 50 * 1000000 / P1full * 30;
	cpufanspd = 50 * 1000000 / P1full * 30;
	printf("P1low %d P1full %d cpu_fan speed=%d\r\n",P1low, P1full, cpufanspd);
	return cpufanspd;
}
UINT32 Ls7aPwm3Get()
{
	UINT32 cpufanspd;
	UINT32 val32;
	UINT32 P1full,P1low;

	//设置pwm3为测量模式（输入）
	val32 = Readl(get_pwm_mem_base() + LS7A_PWM3_CTRL);
	val32 |= ((0x1<<8) | 0x1);
	Writel(get_pwm_mem_base() + LS7A_PWM3_CTRL, val32);

	// 停止计算
	val32 = Readl(get_pwm_mem_base() + LS7A_PWM3_CTRL);
	val32 &= ~(0x1);
	Writel(get_pwm_mem_base() + LS7A_PWM3_CTRL,val32);

	P1low  = Readl(get_pwm_mem_base() + LS7A_PWM3_LOW);
	P1full = Readl(get_pwm_mem_base() + LS7A_PWM3_FULL);

	val32 = Readl(get_pwm_mem_base() + LS7A_PWM3_CTRL);
	val32 |= 0x1;
	Writel(get_pwm_mem_base() + LS7A_PWM3_CTRL, val32);

	//spd = 50 * 1000000 / P1full * 30;
	cpufanspd = 50 * 1000000 / P1full * 30;
	printf("P1low %d P1full %d cpu_fan speed=%d\r\n",P1low, P1full, cpufanspd);
	return cpufanspd;
}
void ls132_speed()
{
	UINT32 cpufanspd;
	UINT32 val32;
	UINT32 P1full,P1low;
	//设置pwm1为测量模式（输入）
	val32 = Readl(get_pwm_mem_base() + LS7A_PWM1_CTRL);
	val32 &= ~(0x1);
	Writel(get_pwm_mem_base() + LS7A_PWM1_CTRL,val32);

	P1low  = Readl(get_pwm_mem_base() + LS7A_PWM1_LOW);
	P1full = Readl(get_pwm_mem_base() + LS7A_PWM1_FULL);

	val32 = Readl(get_pwm_mem_base() + LS7A_PWM1_CTRL);
	val32 |= 0x1;
	Writel(get_pwm_mem_base() + LS7A_PWM1_CTRL, val32);

	//spd = 50 * 1000000 / P1full * 30;
	cpufanspd = 50 * 1000000 / P1full * 30;
	printf("cpu_fan speed=%d\r\n",cpufanspd);
}

UINT8 cpucount[7];
UINT8 lastFanFlag = 10;
UINT8 fanflag = 0;	//默认情况风扇最低转速
char	cputemp0,cputemp1;
char	cputemp;

UINT8 ls132_tempdetect_to_pwm()
{
	UINT32 val32;
	UINT16 tmp0,tmp1;

#if 1
		val32 = Readl(TempRegBaseAddr);
		tmp0 = val32 & 0xffff;											//取低16位数据
		cputemp0 = tmp0 * 731 / 0x4000 - 273;
		tmp1 = (val32 & (0xffff << 16)) >> 16;			//取高16位数据
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

int SmartFanSet (UINT8 Pwm, LS7A_SMARTFAN_CFG_TABLE SmartFanParameter)
{
	int Status=0;
	
	printf ("SmartFan Init, MinRpm:%d ,MaxRpm;%d \n",\
					SmartFanParameter.MinRpm,SmartFanParameter.MaxRpm);
	switch(Pwm)
	{
		case 0:
			Status = Ls7aPwm0Set(&SmartFanParameter);
			break;
		case 1:
			Status = Ls7aPwm1Set(&SmartFanParameter);
			break;
		case 2:
			Status = Ls7aPwm2Set(&SmartFanParameter);
			break;
		case 3:
			Status = Ls7aPwm3Set(&SmartFanParameter);
			break;
		default:
			printf ("ERROR No Support Pwm %d \n",Pwm);
			break;
	}
	return Status;
}

UINT32 SmartFanSpeedGet (UINT8 Pwm)
{
	switch(Pwm)
	{
		case 0:
			return Ls7aPwm0Get();
			break;
		case 1:
			return Ls7aPwm1Get();
			break;
		case 2:
			return Ls7aPwm2Get();
			break;
		case 3:
			return Ls7aPwm3Get();
			break;
		default:
			printf ("ERROR No Support Pwm %d \n",Pwm);
			break;
	}
	return 0;
}

UINT8 TempToPercent(
	LS7A_SMARTFAN_CFG_TABLE *Parameter
	)
{
	int Status=0;
	UINT32 temp0,temp1, temp;
	UINT8 Percent;

	Status = lscpu_tempdetect(0,&temp0,&temp1,NULL);
	if(Status!=EFI_SUCCESS)
		return 0;

	printf("temp0 %d temp1 %d\n", temp0,temp1);
	temp=(temp0+temp1)/2;
	if(temp<LS_TEMP0){
		Percent =30;
	}else if(temp<LS_TEMP1){
		Percent = 35;
	}else if(temp<LS_TEMP2){
		Percent = 40;
	}else if(temp<LS_TEMP3){
		Percent = 45;
	}else if(temp<LS_TEMP4){
		Percent = 50;
	}else if(temp<LS_TEMP5){
		Percent = 55;
	}else if(temp<LS_TEMP6){
		Percent = 60;
	}else if(temp<LS_TEMP7){
		Percent = 65;
	}else if(temp<LS_TEMP8){
		Percent = 70;
	}else if(temp<LS_TEMP9){
		Percent = 75;
	}else if(temp<LS_TEMP10){
		Percent = 80;
	}else if(temp<LS_TEMP11){
		Percent = 90;
	}else if(temp<LS_TEMP12){
		Percent = 100;
	}else{
		Percent = 100;
	}

	Parameter->MinRpm = 100 * (100-Percent);
	Parameter->MaxRpm = 10000;
	return Percent;
}

UINT8 TempToPercent_LangChao(
	LS7A_SMARTFAN_CFG_TABLE *Parameter
	)
{
	int Status=0;
	UINT32 temp0,temp1, temp;
	UINT8 Percent;

	Status = lscpu_tempdetect(0, &temp0,&temp1, NULL);
	if(Status!=EFI_SUCCESS)
		return 50;

	printf("temp0 %d temp1 %d\n", temp0,temp1);
	temp=(temp0+temp1)/2;
	if(temp<64){
		Percent = 30;
	}else if(temp<66){
		Percent = 30;
	}else if(temp<69){
		Percent = 35;
	}else if(temp<71){
		Percent = 40;
	}else if(temp<72){
		Percent = 45;
	}else if(temp<73){
		Percent = 50;
	}else if(temp<74){
		Percent = 55;
	}else if(temp<75){
		Percent = 60;
	}else if(temp<76){
		Percent = 65;
	}else if(temp<77){
		Percent = 70;
	}else if(temp<78){
		return 75;
	}else if(temp<79){
		Percent = 80;
	}else if(temp<80){
		Percent = 90;
	}else{
		Percent = 100;
	}

	Parameter->MinRpm = 255 * (100-Percent);
	Parameter->MaxRpm = 25500;
	return Percent;
}

void FanCtrl(void)
{
	UINT8 Percent;
	unsigned int Speed;
	LS7A_SMARTFAN_CFG_TABLE Parameter;

	Percent = TempToPercent_LangChao(&Parameter);
	// Percent = TempToPercent(&Parameter);
	if(Percent>100)
	{
		printf("Percent is error %d\n",Percent);
		return ;
	}
	// pwm0 set fan
	SmartFanSet (0, Parameter);
	// pwm1 read speed
	Speed = SmartFanSpeedGet (1);
	printf("fan speed %d RPM\n", Speed);
}
