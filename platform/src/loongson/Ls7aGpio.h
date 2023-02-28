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

#ifndef _LS7A_GPIO_H_
#define _LS7A_GPIO_H_

//
//the multiplexing function of gpio. 7a1000
//
#define LS7A_GPIO_HIGH    (1)
#define LS7A_GPIO_LOW     (0)

#define LS7A_GPIO_01  (1)                  // VSB_GATEn

#define LS7A_GPIO_02  (2)
#define LS7A_GPIO_GPIO2                  0 // REUSE[BIT26]
#define LS7A_GPIO_02_CLKOUT25M           1 // REUSE[BIT26]

#define LS7A_GPIO_03  (3)
#define LS7A_GPIO_FUN_GPIO3              0 // REUSE[BIT27]
#define LS7A_GPIO_FUN_CLKOUTFLEX         1 // REUSE[BIT27]

#define LS7A_GPIO_04  (4)
#define LS7A_GPIO_REUSE_PWM0             LS7A_GPIO_04
#define LS7A_GPIO_FUN_GPIO4              0 // REUSE[BIT00]
#define LS7A_GPIO_FUN_PWM0               1 // REUSE[BIT00]
#define LS7A_GPIO_05  (5)
#define LS7A_GPIO_REUSE_PWM1             LS7A_GPIO_05
#define LS7A_GPIO_FUN_GPIO5              0 // REUSE[BIT01]
#define LS7A_GPIO_FUN_PWM1               1 // REUSE[BIT01]
#define LS7A_GPIO_06  (6)
#define LS7A_GPIO_REUSE_PWM2             LS7A_GPIO_06
#define LS7A_GPIO_FUN_GPIO6              0 // REUSE[BIT02]
#define LS7A_GPIO_FUN_PWM2               1 // REUSE[BIT02]
#define LS7A_GPIO_07  (7)
#define LS7A_GPIO_REUSE_PWM3             LS7A_GPIO_07
#define LS7A_GPIO_FUN_GPIO7              0 // REUSE[BIT03]
#define LS7A_GPIO_FUN_PWM3               1 // REUSE[BIT03]

#define LS7A_GPIO_08  (8)// I2C0 SCL
#define LS7A_GPIO_09  (9)// I2C0 SDA
#define LS7A_GPIO_FUN_GPIO8_9             0 // REUSE[BIT04]
#define LS7A_GPIO_FUN_I2C0                1 // REUSE[BIT04]

#define LS7A_GPIO_10  (10)// I2C1 SCL
#define LS7A_GPIO_11  (11)// I2C1 SDA
#define LS7A_GPIO_FUN_GPIO10_11           1 // REUSE[BIT05]
#define LS7A_GPIO_FUN_I2C1                1 // REUSE[BIT05]

#define LS7A_GPIO_12  (12)// SPI_CS0/I2C4_SCL
#define LS7A_GPIO_13  (13)// SPI_CS1/I2C4_SDA
#define LS7A_GPIO_FUN_GPIO12_13           0 // REUSE[BIT07:BIT06]
#define LS7A_GPIO_FUN_SPI_CSN0_1          1 // REUSE[BIT07:BIT06]
#define LS7A_GPIO_FUN_I2C4                2 // REUSE[BIT07:BIT06]

#define LS7A_GPIO_14  (14)// SPI_CS2/I2C5_SCL
#define LS7A_GPIO_15  (15)// SPI_CS3/I2C5_SDA
#define LS7A_GPIO_FUN_GPIO14_15           0 // REUSE[BIT09:BIT08]
#define LS7A_GPIO_FUN_SPI_CSN2_3          1 // REUSE[BIT09:BIT08]
#define LS7A_GPIO_FUN_I2C5                2 // REUSE[BIT09:BIT08]


#define LS7A_GPIO_16  (16)// SPI_SDI
#define LS7A_GPIO_17  (17)// SPI_SD0
#define LS7A_GPIO_18  (18)// SPI_SCK
#define LS7A_GPIO_FUN_GPIO16_17_18        0 // REUSE[BIT10]
#define LS7A_GPIO_FUN_SPI                 1 // REUSE[BIT10]

#define LS7A_GPIO_19  (19)// AC97_BITCLK/HDA_BITCLK
#define LS7A_GPIO_20  (20)// AC97_SYNC/HDA_SYNC
#define LS7A_GPIO_21  (21)// AC97_RESET/HDA_RESET
#define LS7A_GPIO_22  (22)// AC97_SDO/HDA_SDO
#define LS7A_GPIO_23  (23)// AC97_SDI/HDA_SDI0
#define LS7A_GPIO_24  (24)// xxx/HDA_SDI1
#define LS7A_GPIO_25  (25)// xxx/HDA_SDI2
#define LS7A_GPIO_FUN_GPIO19__25          0 // REUSE[BIT12:BIT11]
#define LS7A_GPIO_FUN_AC97                1 // REUSE[BIT12:BIT11]
#define LS7A_GPIO_FUN_HDA                 2 // REUSE[BIT12:BIT11]

#define LS7A_GPIO_26  (26)
#define LS7A_GPIO_FUN_GPIO26               0 // REUSE[BIT13]
#define LS7A_GPIO_FUN_SATA0_LED            1 // REUSE[BIT13]
#define LS7A_GPIO_27  (27)
#define LS7A_GPIO_FUN_GPIO27               0 // REUSE[BIT14]
#define LS7A_GPIO_FUN_SATA1_LED            1 // REUSE[BIT14]
#define LS7A_GPIO_28  (28)
#define LS7A_GPIO_FUN_GPIO28               0 // REUSE[BIT15]
#define LS7A_GPIO_FUN_SATA2_LED            1 // REUSE[BIT15]

#define LS7A_GPIO_29  (29)
#define LS7A_GPIO_FUN_GPIO29               0 // REUSE[BIT16]
#define LS7A_GPIO_FUN_USB_OC0              1 // REUSE[BIT16]
#define LS7A_GPIO_30  (30)
#define LS7A_GPIO_FUN_GPIO30               0 // REUSE[BIT17]
#define LS7A_GPIO_FUN_USB_OC1              1 // REUSE[BIT17]
#define LS7A_GPIO_31  (31)
#define LS7A_GPIO_FUN_GPIO31               0 // REUSE[BIT18]
#define LS7A_GPIO_FUN_USB_OC2              1 // REUSE[BIT18]

#define LS7A_GPIO_32  (32)// UARTFULL_DCD/UART3_RXD/I2C2_SCL
#define LS7A_GPIO_33  (33)// UARTFULL_RI /UART3_TXD/I2C2_SDA
#define LS7A_GPIO_FUN_GPIO32_33_SEL         0 // REUSE[BIT23,BIT19]
#define LS7A_GPIO_FUN_UARTFULL_RI_DCD_SEL   1 // REUSE[BIT23,BIT19]
#define LS7A_GPIO_FUN_I2C2_SEL              2 // REUSE[BIT23,BIT19]
#define LS7A_GPIO_FUN_UARTFULL_RI_DCD       0 // REUSE[BIT31]
#define LS7A_GPIO_FUN_UART3                 1 // REUSE[BIT31]
#define LS7A_GPIO_34  (34)// UARTFULL_DSR/UART2_RXD/I2C3_SCL
#define LS7A_GPIO_35  (35)// UARTFULL_DTR/UART2_TXD/I2C3_SDA
#define LS7A_GPIO_FUN_GPIO34_35_SEL         0 // REUSE[BIT24,BIT20]
#define LS7A_GPIO_FUN_UARTFULL_DSR_DTR_SEL  1 // REUSE[BIT24,BIT20]
#define LS7A_GPIO_FUN_I2C3_SEL              2 // REUSE[BIT24,BIT20]
#define LS7A_GPIO_FUN_UARTFULL_DSR_DTR      0 // REUSE[BIT30]
#define LS7A_GPIO_FUN_UART2                 1 // REUSE[BIT30]
#define LS7A_GPIO_36  (36)// UARTFULL_CTS/UART1_RXD
#define LS7A_GPIO_37  (37)// UARTFULL_RTS/UART1_TXD
#define LS7A_GPIO_FUN_GPIO36_37_SEL         0 // REUSE[BIT21]
#define LS7A_GPIO_FUN_UARTFULL_CTS_RTS_SEL  1 // REUSE[BIT21]
#define LS7A_GPIO_FUN_UARTFULL_CTS_RTS      0 // REUSE[BIT29]
#define LS7A_GPIO_FUN_UART1                 1 // REUSE[BIT29]
#define LS7A_GPIO_38  (38)// UART0_RXD/UARTFULL_RXD
#define LS7A_GPIO_39  (39)// UART0_TXD/UARTFULL_TXD
#define LS7A_GPIO_FUN_GPIO38_39_SEL         0 // REUSE[BIT22]
#define LS7A_GPIO_FUN_UART_RXD_TXD_SEL      1 // REUSE[BIT22]
#define LS7A_GPIO_FUN_UART0                 0 // REUSE[BIT28]
#define LS7A_GPIO_FUN_UARTFULL_RXD_TXD      1 // REUSE[BIT28]

#define LS7A_GPIO_40  (40)// LPC_AD0
#define LS7A_GPIO_41  (41)// LPC_AD1
#define LS7A_GPIO_42  (42)// LPC_AD2
#define LS7A_GPIO_43  (43)// LPC_AD3
#define LS7A_GPIO_44  (44)// LPC_SERIRQ
#define LS7A_GPIO_45  (45)// LPC_FRAMEn
#define LS7A_GPIO_FUN_GPIO40__45            0 // REUSE[BIT25]
#define LS7A_GPIO_FUN_LPC                   1 // REUSE[BIT25]

// The pin only use input mode at uboot
#define LS7A_GPIO_46  (46)// SYS_CLKSEL0
#define LS7A_GPIO_47  (47)// SYS_CLKSEL1
#define LS7A_GPIO_48  (48)// SYS_CLKSEL2
#define LS7A_GPIO_49  (49)// SYS_CLKSEL3
#define LS7A_GPIO_50  (50)// SYS_CLKSEL4
#define LS7A_GPIO_51  (51)// SYS_CLKSEL5
#define LS7A_GPIO_52  (52)// SYS_CLKSEL6
#define LS7A_GPIO_53  (53)// SYS_CLKSEL7
#define LS7A_GPIO_56  (56)// SYS_CLKSEL8

#define LS7A_GPIO_54  (54) // SYS_PCIEBRGMODE

#define LS7A_GPIO_55  (55) //HT8_2

#define LS7A_GPIO_MAX (LS7A_GPIO_56)

//
//Ls7a pwm  api
//
#define  LS7A_PWM_CH0     (0)
#define  LS7A_PWM_CH1     (1)
#define  LS7A_PWM_CH2     (2)
#define  LS7A_PWM_CH3     (3)
#define  LS7A_PWM_MAX     (4)
VOID Ls7aPwmFuncEnable(UINT8 PwmCh);
VOID Ls7aPwmOpen(UINT8 PwmCh, UINT32 Hz, UINT8 Duty, UINT8 Invert);
VOID Ls7aPwmClose(UINT8 PwmCh);

//
//Ls7a Gpio api
//
VOID  Ls7aGpioSetVal(UINT8 Gpio, UINT8 Value);
UINT8 Ls7aGpioGetVal(UINT8 Gpio);
VOID  Ls7aGpioEnableInt(UINT8 Gpio);


//
//Ls7a I2c api
//
#define  LS7A_I2C_CH0     (0)
#define  LS7A_I2C_CH1     (1)
#define  LS7A_I2C_CH2     (2)
#define  LS7A_I2C_CH3     (3)
#define  LS7A_I2C_CH4     (4)
#define  LS7A_I2C_CH5     (5)
#define  LS7A_I2C_MAX     (6)
VOID  Ls7aI2cFuncEnable(UINT8 I2cCh);

//
//DVO channel select
//
#define DVO_CH0                  (0)
#define DVO_CH1                  (1)
#define DC_INIT                  (2)
#define DVO_CH_END               (3)

//
//Ls7a1000  gpio i2c rum Mode
//
#define GPIO_I2C_MODE_PEI        (0)
#define GPIO_I2C_MODE_DXE        (1)

//
//gpio i2c  Api
//
VOID    Ls7aI2cGpioInit(UINT8 Mode, UINT32 Freq);
VOID    Ls7aI2cGpioExit(VOID);
BOOLEAN Ls7aI2cSendData(UINT8 DvoCh, UINT8 DevAddr,UINT8 RegAddr,UINT8 Data);
BOOLEAN Ls7aI2cSendDataBlock(UINT8 DvoCh, UINT8 DevAddr,UINT8 *Data,UINT8 Size);
BOOLEAN Ls7aI2cReceiveData(UINT8 DvoCh, UINT8 DevAddr,UINT8 RegAddr, UINT8 *RecvDate);
BOOLEAN Ls7aI2cReceiveDataBlock(UINT8 DvoCh, UINT8 DevAddr, UINT8 RegAddr, UINT8 *RecvDate, UINT8 Size);

#endif
