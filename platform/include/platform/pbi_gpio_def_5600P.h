#ifndef _PBI_GPIO_DEF_5600P_H_
#define _PBI_GPIO_DEF_5600P_H_

#define 	PIO_LCD_SER			24	/*data*/
#define 	PIO_LCD_RCLK		22  /*strobe*/
#define 	PIO_LCD_SRCLK		16	 /*clock*/

#define 	PIO_LED_LOCK		23
#define 	PIO_LED_ALARM		25

#define 	PIO_KEY1			19
#define 	PIO_KEY2			17
#define 	PIO_KEY3			18
#define 	PIO_KEY4			20
#define 	PIO_KEY5			21
#define 	PIO_KEY6			15

#define 	PIO_M_F_RST			28
#define 	PIO_M_F_STATUS		42
#define 	PIO_M_F_DCLK		43
#define 	PIO_M_F_DATA0		99
#define 	PIO_M_F_CFG			41
#define 	PIO_M_F_DONE		45
#define 	PIO_M_F_INT			29

#define 	PIO_IP_F_RST		38	//4*8+6
#define 	PIO_IP_F_STATUS		50	//(6*8+2)
#define 	PIO_IP_F_DCLK		36	//4*8+4
#define 	PIO_IP_F_DATA0		39	//4*8+7
#define 	PIO_IP_F_CFG		48	//6*8+0
#define 	PIO_IP_F_DONE		49	//(6*8+1)

#define 	PIO_CI_RST			52
#define 	PIO_CI_WAKE_UP		51
#define 	PIO_CI_IRQ			53

#define 	PIO_NOTRSE_TUNER 	93

#define 	PIO_CS_RST 			79 //(9*8+7)

#define 	PIO_HD_RESET 		102 //(12*8+6)	 9293 Reset
#define 	PIO_HD_INT 			96	//(12*8)	 9293 INT
#define 	PIO_9024_RESET 		97	//(12*8+1)	 9024 Reset
#define 	PIO_9024_INT 		98	//(12*8+2)   9024 INT

#define 	PIO_I2C1_SCL 		46
#define 	PIO_I2C1_SDA 		40

#define 	PIO_I2C2_SCL 		101
#define 	PIO_I2C2_SDA 		100

//#define 	PIO_I2C3_SCL 		71
//#define 	PIO_I2C3_SDA 		78

#define 	PIO_I2C5_SCL 		91
#define 	PIO_I2C5_SDA 		90

//#define 	PIO_I2C6_SCL 		54
//#define 	PIO_I2C6_SDA 		55

#define 	PIO_DETECT1			8 //(1*8+0)
#define 	PIO_DETECT2			26	//(3*8+2)
#define 	PIO_DETECT3			14	//(1*8+6)

#endif

