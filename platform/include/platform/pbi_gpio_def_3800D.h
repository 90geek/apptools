#ifndef _PBI_GPIO_DEF_3800D_H_
#define _PBI_GPIO_DEF_3800D_H_

/*LCD and Key*/
#define 	PIO_LCD_SER			24	/*data*/
#define 	PIO_LCD_RCLK		22  /*strobe*/
#define 	PIO_LCD_SRCLK		16	 /*clock*/

#define 	PIO_KEY1			19
#define 	PIO_KEY2			17
#define 	PIO_KEY3			18
#define 	PIO_KEY4			20
#define 	PIO_KEY5			21
#define 	PIO_KEY6			15

/*FPGA Config*/
#define 	PIO_M_F_RST			28	//3*8+4
#define 	PIO_M_F_STATUS		42	//5*8+2
#define 	PIO_M_F_DCLK		43	//5*8+3
#define 	PIO_M_F_DATA0		97	//12*8+1
#define 	PIO_M_F_CFG			41	//5*8+1
#define 	PIO_M_F_DONE		45	//5*8+5
#define 	PIO_M_F_INT			29	//3*8+5

#define 	PIO_IP_F_RST		96	//12*8+0
#define 	PIO_IP_F_STATUS		98	//(12*8+2)
#define 	PIO_IP_F_DCLK		100	//12*8+4
#define 	PIO_IP_F_DATA0		99	//12*8+3
#define 	PIO_IP_F_CFG		101	//12*8+5
#define 	PIO_IP_F_DONE		102	//(12*8+6)

/*I2C*/
#define 	PIO_I2C1_SCL 		23	//2*8+7
#define 	PIO_I2C1_SDA 		25  //3*8+1

#define 	PIO_I2C2_SCL 		91	//11*8+3
#define 	PIO_I2C2_SDA 		92	//11*8+4

#define 	PIO_I2C3_SCL 		54	//6*8+6
#define 	PIO_I2C3_SDA 		55	//6*8+7

#define 	PIO_I2C4_SCL 		67	//8*8+3
#define 	PIO_I2C4_SDA 		66	//8*8+2

#define 	PIO_I2C5_SCL 		71	//8*8+7
#define 	PIO_I2C5_SDA 		78  //8*9+6

/*Hardware Detect*/
#define 	PIO_DETECT1			8 //(1*8+0)
#define 	PIO_DETECT2			26	//(3*8+2)
#define 	PIO_DETECT3			14	//(1*8+6)
#define 	PIO_DETECT4			40 //(5*8+0)
#define 	PIO_DETECT5			46	//(5*8+6)


/*CI*/
#define 	PIO_CI_IRQ1			64	//8*8+0
#define 	PIO_CI_IRQ2			65	//8*8+1
#define 	PIO_CI_IRQ3			68	//8*8+4
#define 	PIO_CI_IRQ4			69	//8*8+5

#define 	PIO_CI_RST1			70	//8*8+6
#define 	PIO_CI_RST2			53	//6*8+5
#define 	PIO_CI_RST3			52	//6*8+4
#define 	PIO_CI_RST4			51	//6*8+3

#define 	PIO_CI_WAKE_UP1		60	//7*8+4
#define 	PIO_CI_WAKE_UP2		61	//7*8+5
#define 	PIO_CI_WAKE_UP3		62	//7*8+6
#define 	PIO_CI_WAKE_UP4		61	//7*8+7

/*Power And Fans*/
#define PIO_PWD_TEST1	31//3*8+7
#define PIO_PWD_TEST2	30//3*8+6
#define PIO_PWD_FAN1	90//11*8+2
#define PIO_PWD_FAN2	89//11*8+1


/*Tuner Reset*/
#define PIO_TUNER_RST1	50//6*8+2
#define PIO_TUNER_RST2	38//4*8+6
#define PIO_TUNER_RST3	49//6*8+1
#define PIO_TUNER_RST4	37//4*8+5
#define PIO_TUNER_RST5	48//6*8+0
#define PIO_TUNER_RST6	36//4*8+4
#define PIO_TUNER_RST7	39//4*8+7
#define PIO_TUNER_RST8	35//4*8+3

/* Tuner Led control with FPGA*/

#endif

