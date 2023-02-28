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
#include "debug.h"
#include "LsRegDef.h"

#define WritelBit(addr, BitValue, Area, StartBit) \
  (*(volatile UINT32*)(addr)) &= (~(Area<<StartBit)); \
(*(volatile UINT32*)(addr)) |= ((BitValue)<<StartBit)

#define WritelMatchBit(addr, Value, Area, StartBit) \
  (*(volatile UINT32*)(addr)) &= (~(Area<<StartBit)); \
(*(volatile UINT32*)(addr)) |= (Value & (Area<<StartBit))

#define ReadlBit(addr,Area, StartBit) \
  (((*(volatile UINT32*)(addr)) & (Area<<StartBit))>>StartBit)

#define ReadlMatchBit(addr,Area, StartBit) \
  ((*(volatile UINT32*)(addr)) & (Area<<StartBit))


//
// reg about gpio
//
// Port multiplexing register base
#define LS7A_CONF_REG_ADDR_REUSE         (LS7A_CONFBUS_BASE_ADDR + CONF_REUSE_OFFSET)

// The address of pin ctrl register
#define LS7A_GPIOn_REG_ADDR_DIR(GPIO)    (LS7A_GPIO_REG_DIR_BASE + GPIO)
#define LS7A_GPIOn_REG_ADDR_OUT(GPIO)    (LS7A_GPIO_REG_OUT_BASE + GPIO)
#define LS7A_GPIOn_REG_ADDR_IN(GPIO)     (LS7A_GPIO_REG_IN_BASE + GPIO)
#define LS7A_GPIOn_REG_ADDR_INT(GPIO)    (LS7A_GPIO_REG_INT_BASE + GPIO)

// The Val define of pin ctrl register
#define LS7A_GPIO_REG_VAL_DIR_IN         (1)
#define LS7A_GPIO_REG_VAL_DIR_OUT        (0)
#define LS7A_GPIO_REG_VAL_INT_ENABLE     (1)
#define LS7A_GPIO_REG_VAL_INT_DISABLE    (0)


//
// reg about pwm
//
#define LS7A_PWM_REG_OFFSET_LOW          (0x04)
#define LS7A_PWM_REG_OFFSET_FULL         (0x08)
#define LS7A_PWM_REG_OFFSET_CTRL         (0x0C)
#define LS7A_PWMn_REG_ADDR_CTRL(PWM)     (LS7A_PWM0_REG_BASE + (PWM<<0x08) + LS7A_PWM_REG_OFFSET_CTRL)
#define LS7A_PWMn_REG_ADDR_FULL(PWM)     (LS7A_PWM0_REG_BASE + (PWM<<0x08) + LS7A_PWM_REG_OFFSET_FULL)
#define LS7A_PWMn_REG_ADDR_LOW(PWM)      (LS7A_PWM0_REG_BASE + (PWM<<0x08) + LS7A_PWM_REG_OFFSET_LOW)

/*************************************
 * Function: Enable PwmCh device.
 * PwmCh:    Which channel.
 *************************************/
VOID Ls7aPwmFuncEnable(UINT8 PwmCh)
{
  WritelBit(LS7A_CONF_REG_ADDR_REUSE, 0x01, 0x01, PwmCh);
}

/*************************************
 * Function: Open pwm device.
 * PwmCh:    Which channel.
 * Hz:       Working frequency.
 * Duty:     Duty cycle.[100:0]
 * Invert:   0:The cycle starts at a low level.
 *           1:The cycle starts at a high level.
 *************************************/
VOID Ls7aPwmOpen(UINT8 PwmCh, UINT32 Hz, UINT8 Duty, UINT8 Invert)
{
  UINT32 FullVal = (1*1000*1000*1000)/Hz/20;
  UINT32 LowVal  = FullVal-(FullVal*Duty)/100;

  if(PwmCh < LS7A_PWM_MAX){
    WritelBit(LS7A_CONF_REG_ADDR_REUSE, 0x01, 0x01, PwmCh);
    WritelBit(LS7A_PWMn_REG_ADDR_CTRL(PwmCh), 0x00, 0x01, 0x00);
    if(Invert){
      WritelBit(LS7A_PWMn_REG_ADDR_CTRL(PwmCh), 0x01, 0x01, 0x09); // Reverse PWM polarity
    }else{
      WritelBit(LS7A_PWMn_REG_ADDR_CTRL(PwmCh), 0x00, 0x01, 0x09);
    }
    WritelBit(LS7A_PWMn_REG_ADDR_FULL(PwmCh), FullVal, 0xFFFFFFFF, 0x00);
    WritelBit(LS7A_PWMn_REG_ADDR_LOW(PwmCh),  LowVal, 0xFFFFFFFF, 0x00);
    WritelBit(LS7A_PWMn_REG_ADDR_CTRL(PwmCh), 0x01, 0x01, 0x00);
    WritelBit(LS7A_PWMn_REG_ADDR_CTRL(PwmCh), 0x00, 0x01, 0x03);
    WritelBit(LS7A_PWMn_REG_ADDR_CTRL(PwmCh), 0x00, 0x01, 0x04);
  }
}

/*************************************
 * Function: Close pwm device.
 * PwmCh:    Which channel.
 *************************************/
VOID Ls7aPwmClose(UINT8 PwmCh)
{
  if(PwmCh < LS7A_PWM_MAX){
    WritelBit(LS7A_PWMn_REG_ADDR_CTRL(PwmCh), 0x00, 0x01, 0x00);
    WritelBit(LS7A_CONF_REG_ADDR_REUSE, 0x00, 0x01, PwmCh);
  }
}

/*************************************
 * Function: Enable I2cCh device.
 * I2cCh:    Which channel.
 *************************************/
VOID Ls7aI2cFuncEnable(UINT8 I2cCh)
{
  if(I2cCh < LS7A_I2C_MAX){
    if(I2cCh < 2){
      I2cCh+=4;
      WritelBit(LS7A_CONF_REG_ADDR_REUSE, 0x01, 0x01, I2cCh);
    }else if(I2cCh < 4){
      WritelBit(LS7A_CONF_REG_ADDR_REUSE, 0x00, 0x01, 20);
      WritelBit(LS7A_CONF_REG_ADDR_REUSE, 0x01, 0x01, 24);
    }else if(I2cCh == 4 ){
      WritelBit(LS7A_CONF_REG_ADDR_REUSE, 0x02, 0x03, 6);
    }else if(I2cCh == 5 ){
      WritelBit(LS7A_CONF_REG_ADDR_REUSE, 0x02, 0x03, 8);
    }
  }
}

/*************************************
 * Function: Set ls7a gpio Value
 * Gpio:     Which one GPIO. eg: LS7A_GPIO_XX
 * Value:    Set it to high or low. eg:LS7A_GPIO_REG_VAL_OUT_XX
 *************************************/
VOID Ls7aGpioSetVal(UINT8 Gpio, UINT8 Value)
{
  void * vaddr = NULL;
  int memoffset=0;
  vaddr=p2v_mem_mapping(LS7A_GPIOn_REG_ADDR_DIR(Gpio),4, &memoffset);
  Writeb(vaddr, LS7A_GPIO_REG_VAL_DIR_OUT);
  p2v_mem_clean(vaddr, memoffset);
  vaddr=p2v_mem_mapping(LS7A_GPIOn_REG_ADDR_OUT(Gpio),4, &memoffset);
  Writeb(vaddr, !!Value);
  p2v_mem_clean(vaddr, memoffset);
}

/*************************************
 * Function: Get ls7a gpio Value
 * Gpio:     Which one GPIO. eg: LS7A_GPIO_XX
 * Return:   The Level of Pin.
 *************************************/
UINT8 Ls7aGpioGetVal(UINT8 Gpio)
{
  return Readb(LS7A_GPIOn_REG_ADDR_IN(Gpio));
}

/*************************************
* Function: ls7a gpio interrupt enable
* Gpio:     Which one GPIO. eg: LS7A_GPIO_XX
 *************************************/
VOID Ls7aGpioEnableInt(UINT8 Gpio)
{
  switch(Gpio){
    case LS7A_GPIO_50:
      Writeb(LS7A_GPIOn_REG_ADDR_DIR(Gpio), LS7A_GPIO_REG_VAL_DIR_IN);
      Writeb(LS7A_GPIOn_REG_ADDR_INT(Gpio), LS7A_GPIO_REG_VAL_INT_ENABLE);
    default:
      break;
  }
}
