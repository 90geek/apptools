/*
 * app_platorm_debug.c
 *
 *  Created on: 2017Äê11ÔÂ17ÈÕ
 *      Author: qqtian
 */


// #include "platform/app_gpio.h"
// #include "platform/app_key.h"
// #include "platform/app_lcd.h"
// #include "platform/app_i2c.h"
// #include "platform/fpga_reg.h"
// #include "platform/fpga.h"
// #include "platform/app_led.h"
// #include "platform/app_reset.h"

// #include "platform/driver/eeprom.h"
#include "testtool/testtool.h"
#include "platform/app_spi.h"
#include "platform_debug.h"


//int gpio_write_debug(parse_t * pars_p,char *result_p)
//{
//	int num,level;
//	unsigned char buf[20]={0};
//	int error;
//	error=cget_integer(pars_p,0,&num);
//	if (error)
//	{
//		tag_current_line(pars_p,"-->num!");
//		return 1;
//	}

//	error=cget_integer(pars_p,0,&level);
//	if (error)
//	{
//		tag_current_line(pars_p,"-->level!");
//		return 1;
//	}

//	app_gpio_set_dir(num, app_GPIO_OUTPUT);

//	app_gpio_write(num, level);

//	printf("GPIO NUM %d output set to %d\n", num,level);

//	return 0;
//}

//int gpio_get_direction_debug(parse_t * pars_p,char *result_p)
//{
//	int num,level;
//	unsigned char buf[20]={0};
//	int error;
//	error=cget_integer(pars_p,0,&num);
//	if (error)
//	{
//		tag_current_line(pars_p,"-->num!");
//		return 1;
//	}

//	printf("GPIO NUM Direction is %d\n", app_gpio_get_dir(num));

//	return 0;
//}
//int gpio_set_direction_debug(parse_t * pars_p,char *result_p)
//{
//	int num,level;
//	unsigned char buf[20]={0};
//	int error;
//	error=cget_integer(pars_p,0,&num);
//	if (error)
//	{
//		tag_current_line(pars_p,"-->num!");
//		return 1;
//	}

//	error=cget_integer(pars_p,0,&level);
//	if (error)
//	{
//		tag_current_line(pars_p,"-->level!");
//		return 1;
//	}
//	printf("Set GPIO NUM  %d Direction is %d\n", num, level);

//	app_gpio_set_dir(num, level);

//	return 0;
//}

//int gpio_read_debug(parse_t * pars_p,char *result_p)
//{
//	int num;
//	unsigned char buf[20]={0};
//	int i=0, error;

//	error=cget_integer(pars_p,0,&num);
//	if (error)
//	{
//		tag_current_line(pars_p,"-->num!");
//		return 1;
//	}
//	app_gpio_set_dir(num, app_GPIO_INPUT);

//	i = app_gpio_read(num);

//	printf("GPIO NUM %d input is %d\n", num, i);

//	return 0;
//}


//int gpio_iotest_debug(parse_t * pars_p,char *result_p)
//{

//	led_test();
//	return 0;
//}

//int gpio_keytest_debug(parse_t * pars_p,char *result_p)
//{
//	int i = 0;
//	/*LED test*/
//	printf("**********KEY Test**********\n");

//	for(i = 0;i< 20;i++)
//		Get_Key_Value();
//	printf("**********KEY Test END**********\n");
//	return 0;
//}

//int gpio_lcdtest_debug(parse_t * pars_p,char *result_p)
//{

//	/*LCD test*/
//	printf("**********LCD Test**********\n");
//	Lcd_Init();
//	printf("**********LCD Init END**********\n");
//	Lcd_Clear();
//	printf("**********LCD Clear END**********\n");
//	Lcd_Display_All("Welcome To", "app World!");
//	//printf("**********LCD Display END**********\n");
//	//Lcd_Flash_Cursor(SECOND_LINE, 20);
//	printf("**********LCD Test ENDs**********\n");
//	return 0;
//}


//int gpio_alarm_led_debug(parse_t * pars_p,char *result_p)
//{
//	int index, status;
//	int error;

//	error=cget_integer(pars_p,0,&index);
//	if (error)
//	{
//		tag_current_line(pars_p,"-->index!");
//		return 1;
//	}

//	error=cget_integer(pars_p,0,&status);
//	if (error)
//	{
//		tag_current_line(pars_p,"-->status!");
//		return 1;
//	}

//	alarm_led(index, status);

//	return 0;
//}

//int gpio_tuner_led_debug(parse_t * pars_p,char *result_p)
//{
//	int index, status;
//	int error;

//	error=cget_integer(pars_p,0,&index);
//	if (error)
//	{
//		tag_current_line(pars_p,"-->index!");
//		return 1;
//	}

//	error=cget_integer(pars_p,0,&status);
//	if (error)
//	{
//		tag_current_line(pars_p,"-->status!");
//		return 1;
//	}

//	tuner_led(index, status);

//	return 0;
//}


//int hardware_detect_debug(parse_t * pars_p,char *result_p)
//{
//	char detect = 0;
//	detect = app_gpio_read(PIO_DETECT1);
//	detect |= (app_gpio_read(PIO_DETECT2)<<1);
//	detect  |= (app_gpio_read(PIO_DETECT3)<<2);
//#ifdef PIO_DETECT4
//	detect  |= (app_gpio_read(PIO_DETECT4)<<3);
//#endif
//#ifdef PIO_DETECT5
//	detect  |= (app_gpio_read(PIO_DETECT5)<<4);
//#endif
//	printf("\nHardware Version is 0x%x\n\n", detect);
//	return 0;
//}

//#ifdef DEVICE_2210P
//int gpio_ip_reset_debug(parse_t * pars_p,char *result_p)
//{
//	char value = 0;

//	value = app_gpio_read(PIO_RST_IP_ADDR);
//	printf("IP address Pin is %d\n\n", value);
//	return 0;
//}
//#endif

//int fpga_init_debug(parse_t * pars_p,char *result_p)
//{
//	fpga_init();
//	return 0;
//}

//int fpga_config_debug(parse_t * pars_p,char *result_p)
//{
//	int fpga;
//	int error;

//	error=cget_integer(pars_p,0,&fpga);

//	if(MAIN_FPGA == fpga)
//		setup_fpga(MAIN_FPGA, "/application/fpga/fpga.rbf");
//	else
//		setup_fpga(IP_FPGA, "/application/fpga/tsip_fpga.rbf");
//	return 0;
//}

//int fpga_read_debug(parse_t * pars_p,char *result_p)
//{
//	int address, data_width;
//	int error;

//	error=cget_integer(pars_p,0,&address);
//	if (error)
//	{
//		tag_current_line(pars_p,"-->address!");
//		tag_current_line(pars_p,"FPGA_READ <address><data width, 8/16/32>");
//		return 1;
//	}

//	error=cget_integer(pars_p,0,&data_width);
//	if (error)
//	{
//		tag_current_line(pars_p,"-->data_width!");
//		tag_current_line(pars_p,"FPGA_READ <address><data width, 8/16/32>");
//		return 1;
//	}

//	if(data_width!= 8 && data_width!= 16 && data_width!= 32)
//	{
//		tag_current_line(pars_p,"FPGA_READ <address><data width, 8/16/32>");
//		data_width = 16;
//	}

//	if(data_width == 8)
//	{
//		printf("fpga read register(8bits) 0x%x = 0x%02x\n", address, fpga_read8(0,address));
//	}
//	else if(data_width ==16)
//	{
//		printf("fpga read register(16bits) 0x%x = 0x%04x\n", address, fpga_read16(0,address));
//	}
//	else if(data_width ==32)
//	{
//		printf("fpga read register(32bits) 0x%x = 0x%08x\n", address, fpga_read32(0,address));
//	}

//	return 0;
//}

//int fpga_burst_read_debug(parse_t * pars_p,char *result_p)
//{
//	int address, len;
//	int error;

//	error=cget_integer(pars_p,0,&address);
//	if (error)
//	{
//		tag_current_line(pars_p,"-->address!");
//		return 1;
//	}

//	error=cget_integer(pars_p,0,&len);
//	if (error)
//	{
//		tag_current_line(pars_p,"-->len!");
//		return 1;
//	}

//#if (defined DEVICE_2210P)||(defined DEVICE_5600P) || (defined DEVICE_3800D)
//	unsigned short* datas = (unsigned short*)app_malloc(len*2);

//	fpga_read_burst(0,address, datas, len);
//	printf("fpga read burst register 0x%x = \n", address);
//	app_print_data((U8*)datas,len*2);

//	app_free(datas);
//#else
//	printf("not support!");
//#endif

//	return 0;
//}




//int fpga_write_debug(parse_t * pars_p,char *result_p)
//{
//	int address, data_width, data;
//	int error;

//	error=cget_integer(pars_p,0,&address);
//	if (error)
//	{
//		tag_current_line(pars_p,"-->address!");
//		tag_current_line(pars_p,"FPGA_WRITE <address><data width, 8/16/32><data>");
//		return 1;
//	}

//	error=cget_integer(pars_p,0,&data_width);
//	if (error)
//	{
//		tag_current_line(pars_p,"-->data_width");
//		tag_current_line(pars_p,"FPGA_WRITE <address><data width, 8/16/32><data>");
//		return 1;
//	}

//	if(data_width != 8 && data_width!= 16 && data_width!=32)
//	{
//		tag_current_line(pars_p,"FPGA_WRITE <address><data width, 8/16/32><data>");
//		data_width = 16;
//	}
//	error=cget_integer(pars_p,0,&data);
//	if (error)
//	{
//		tag_current_line(pars_p,"-->data!");
//		tag_current_line(pars_p,"FPGA_WRITE <address><data width, 8/16/32><data>");
//		return 1;
//	}

//	printf("fpga write register(%dbits) 0x%x = 0x%x\n", data_width, address,data);

//	if(data_width == 8)
//	{
//		fpga_write8(0,address,data&0xff);
//	}
//	else if(data_width == 16)
//	{
//		fpga_write16(0,address,data&0xffff);
//	}
//	else if(data_width == 32)
//	{
//		fpga_write32(0,address,data&0xffffffff);
//	}

//	return 0;
//}

//int fpga_burst_write_debug(parse_t * pars_p,char *result_p)
//{
//	int address, data_width, data;
//	int error;

//	error=cget_integer(pars_p,0,&address);
//	if (error)
//	{
//		tag_current_line(pars_p,"-->address!");
//		return 1;
//	}

//#if (defined DEVICE_2210P)||(defined DEVICE_5600P)||(defined DEVICE_3800D)
//		U16 burst_data[10];
//		int i = 0;

//		burst_data[0] = 0x2211;
//		burst_data[1] = 0x4433;
//		burst_data[2] = 0x6655;
//		burst_data[3] = 0x8877;
//		burst_data[4] = 0xaa99;
//		burst_data[5] = 0xccbb;
//		burst_data[6] = 0xeedd;
//		burst_data[7] = 0x00ff;
//		burst_data[8] = 0x2211;
//		burst_data[9] = 0x4433;
//		printf("burst write, address = 0x%x:", address);
//		app_print_data((U8 *)burst_data,10*2);

//		fpga_write_burst(0, address, burst_data, 10);
//#else
//		printf("not support!");
//#endif

//	return 0;
//}


#if 0
int spi_read_debug(parse_t * pars_p,char *result_p)
{
	int reg, len;
	unsigned char buf[20]={0};
	int i=0, error;

	error=cget_integer(pars_p,0,&len);
	if (error)
	{
		tag_current_line(pars_p,"-->len!");
		return 1;
	}

	if(len>20)
		len = 20;

	app_spi_read(0, buf, len);

	printf("spi_read read value :\n");
	for(i=0;i<len;i++)
		printf("%02x ", buf[i]);
	printf("\n");

	return 0;
}


int spi_read_reg_debug(parse_t * pars_p,char *result_p)
{
	int reg;
	unsigned char buf[20]={0};
	int i=0, error;

	error=cget_integer(pars_p,0,&reg);
	if (error)
	{
		tag_current_line(pars_p,"-->len!");
		return 1;
	}


	app_spi_read_reg(0, reg, buf, 2);

	printf("spi_read reg = 0x%x,  value : %02x %02x\n", reg,  buf[0], buf[1]);

	return 0;
}



int spi_write_debug(parse_t * pars_p,char *result_p)
{
	int reg, data;
	unsigned char buf[20]={0};
	int i=0, error;

	error=cget_integer(pars_p,0,&reg);
	if (error)
	{
		tag_current_line(pars_p,"-->reg!");
		return 1;
	}

	error=cget_integer(pars_p,0,&data);
	if (error)
	{
		tag_current_line(pars_p,"-->data!");
		return 1;
	}

	buf[0] = (reg>>8)&0xff;
	buf[1] = (reg&0xff);
	buf[2] = (data>>8)&0xff;
	buf[3] = (data&0xff);

	app_spi_write(0, buf, 4);

	return 0;
}
#endif

// int i2c_write_debug(parse_t * pars_p,char *result_p)
// {
// 	int bus, slave_addr, reg, data;
// 	int error;
// 	U8 datas[4], regs[4];


// 	error=cget_integer(pars_p,0,&bus);
// 	if (error)
// 	{
// 		tag_current_line(pars_p,"-->bus!");
// 		return 1;
// 	}

// 	error=cget_integer(pars_p,0,&slave_addr);
// 	if (error)
// 	{
// 		tag_current_line(pars_p,"-->slave_addr!");
// 		return 1;
// 	}

// 	error=cget_integer(pars_p,0,&reg);
// 	if (error)
// 	{
// 		tag_current_line(pars_p,"-->reg!");
// 		return 1;
// 	}

// 	error=cget_integer(pars_p,0,&data);
// 	if (error)
// 	{
// 		tag_current_line(pars_p,"-->data!");
// 		return 1;
// 	}

// #if 0
// 	regs[0] = (reg>>24)&0xff;
// 	regs[1] = (reg>>16)&0xff;
// 	regs[2] = (reg>>8)&0xff;
// 	regs[3] = (reg)&0xff;


// 	datas[0] = (data>>24)&0xff;
// 	datas[1] = (data>>16)&0xff;
// 	datas[2] = (data>>8)&0xff;
// 	datas[3] = (data)&0xff;
// #else
// 	regs[0] = (reg)&0xff;


// 	datas[0] = (data)&0xff;
// #endif

// 	error = app_i2c_write(bus,slave_addr,regs,1,datas,1);

// 	printf("app_i2c_write bus=%d, slave addr =0x%x, reg=0x%x, data = 0x%x, ret = %d\n"
// 			, bus, slave_addr, reg, data, error);


// 	return 0;
// }


// int i2c_read_debug(parse_t * pars_p,char *result_p)
// {
// 	int bus, slave_addr, reg, data;
// 	int error;
// 	U8 datas[4], regs[4];


// 	error=cget_integer(pars_p,0,&bus);
// 	if (error)
// 	{
// 		tag_current_line(pars_p,"-->bus!");
// 		return 1;
// 	}

// 	error=cget_integer(pars_p,0,&slave_addr);
// 	if (error)
// 	{
// 		tag_current_line(pars_p,"-->slave_addr!");
// 		return 1;
// 	}

// 	error=cget_integer(pars_p,0,&reg);
// 	if (error)
// 	{
// 		tag_current_line(pars_p,"-->reg!");
// 		return 1;
// 	}


// #if 0
// 	regs[0] = (reg>>24)&0xff;
// 	regs[1] = (reg>>16)&0xff;
// 	regs[2] = (reg>>8)&0xff;
// 	regs[3] = (reg)&0xff;

// 	error = app_i2c_read(bus,slave_addr,regs,4,datas,4);

// 	data = (datas[0]<<24)+(datas[1]<<16)+(datas[2]<<8)+datas[3];
// #else
// 	regs[0] = (reg)&0xff;

// 	error = app_i2c_read(bus,slave_addr,regs,1,datas,1);

// 	data = datas[0];
// #endif

// 	printf("app_i2c_read bus=%d, slave addr =0x%x, reg=0x%x, data = 0x%x, ret = %d\n"
// 			, bus, slave_addr, reg, data, error);

// 	return 0;
// }

// static int eeprom_bytes_read_debug(parse_t * pars_p,char *result_p)
// {
// 	int eeprom_mode = 0;
// 	int offset, length=0;
// 	int error = 0, i=0;
// 	U8 datas[100]= {0};

// 	error=cget_integer(pars_p,0,&eeprom_mode);
// 	if (error)
// 	{
// 		tag_current_line(pars_p,"-->eeprom_mode!");
// 		return 1;
// 	}

// 	error=cget_integer(pars_p,0,&offset);
// 	if (error)
// 	{
// 		tag_current_line(pars_p,"-->offset!");
// 		return 1;
// 	}

// 	error=cget_integer(pars_p,0,&length);
// 	if (error)
// 	{
// 		tag_current_line(pars_p,"-->length!");
// 		return 1;
// 	}

// 	if(length > 100)
// 		length = 100;

// 	eeprom_read_bytes(eeprom_mode, offset, datas, length);

// 	printf("offset 0x%x, length=%d:\n", offset, length);
// 	for(i=0;i<length;i++)
// 	{
// 		printf("%02x \n", datas[i]);
// 	}

// 	return 1;
// }


// static int eeprom_bytes_write_debug(parse_t * pars_p,char *result_p)
// {
// 	U8 data[32] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16, 17,18,19,20,21,22,23,24,25,26};
// 	int eeprom_mode = 0;
// 	int offset;
// 	int error = 0, i=0;

// 	error=cget_integer(pars_p,0,&eeprom_mode);
// 	if (error)
// 	{
// 		tag_current_line(pars_p,"-->eeprom_mode!");
// 		return 1;
// 	}

// 	error=cget_integer(pars_p,0,&offset);
// 	if (error)
// 	{
// 		tag_current_line(pars_p,"-->offset!");
// 		return 1;
// 	}

// 	for(i=0;i<16;i++)
// 	{
// 		data[i] = rand()&0xff;
// 	}

// 	printf("\n offset %x:\n", offset);

// 	for(i=0;i<16;i++)
// 	{
// 		printf("%02x\n", data[i]);
// 	}

// 	eeprom_write_bytes(eeprom_mode, offset, data, 16);
// 	return 1;
// }

// static int eeprom_read_byte_debug(parse_t * pars_p,char *result_p)
// {
// 	int eeprom_mode = 0;
// 	int offset;
// 	int error = 0, i=0;
// 	U8 datas[32]= {0};

// 	error=cget_integer(pars_p,0,&eeprom_mode);
// 	if (error)
// 	{
// 		tag_current_line(pars_p,"-->eeprom_mode!");
// 		return 1;
// 	}

// 	error=cget_integer(pars_p,0,&offset);
// 	if (error)
// 	{
// 		tag_current_line(pars_p,"-->offset!");
// 		return 1;
// 	}
// 	eeprom_read_byte(eeprom_mode, offset, datas);

// 	printf("offset %d=0x%2x\n", offset, datas[0]);

// 	return 1;
// }

// static int eeprom_write_byte_debug(parse_t * pars_p,char *result_p)
// {
// 	int eeprom_mode = 0;
// 	int byte = 0xa;
// 	int offset;
// 	int error = 0;

// 	error=cget_integer(pars_p,0,&eeprom_mode);
// 	if (error)
// 	{
// 		tag_current_line(pars_p,"-->eeprom_mode!");
// 		return 1;
// 	}

// 	error=cget_integer(pars_p,0,&offset);
// 	if (error)
// 	{
// 		tag_current_line(pars_p,"-->offset!");
// 		return 1;
// 	}

// 	error=cget_integer(pars_p,0,&byte);
// 	if (error)
// 	{
// 		tag_current_line(pars_p,"-->byte!");
// 		return 1;
// 	}
// 	eeprom_write_byte(eeprom_mode, offset, byte&0xff);

// 	return 1;
// }

// static int ci_reset_debug(parse_t * pars_p,char *result_p)
// {
// 	int id;
// 	int error = 0;

// 	error=cget_integer(pars_p,0,&id);
// 	if (error)
// 	{
// 		tag_current_line(pars_p,"-->id!");
// 		return 1;
// 	}

// 	reset_ci(id, 1000);

// 	return 1;
// }

// static int tuner_reset_debug(parse_t * pars_p,char *result_p)
// {
// 	int id;
// 	int error = 0;

// 	error=cget_integer(pars_p,0,&id);
// 	if (error)
// 	{
// 		tag_current_line(pars_p,"-->id!");
// 		return 1;
// 	}

// 	reset_tuner(id);

// 	return 1;
// }

// static int eeprom_auto_test_debug(parse_t * pars_p,char *result_p)
// {
// 	U8 data[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
// 	int eeprom_mode = 0;
// 	int offset;
// 	int error = 0, i=0;
// 	U8 datas[32]={0};

// 	error=cget_integer(pars_p,0,&eeprom_mode);
// 	if (error)
// 	{
// 		tag_current_line(pars_p,"-->eeprom_mode!");
// 		return 1;
// 	}

// 	error=cget_integer(pars_p,0,&offset);
// 	if (error)
// 	{
// 		tag_current_line(pars_p,"-->offset!");
// 		return 1;
// 	}

// 	eeprom_write_bytes(eeprom_mode, offset, data, 16);

// 	eeprom_read_bytes(eeprom_mode, offset, datas, 16);

// 	printf("\n offset= %d:\n", offset);
// 	for(i=0;i<16;i++)
// 	{
// 		printf("%02x\n", datas[i]);
// 	}
// 	return 1;
// }

void platform_debug_register(void)
{
	// register_command ("DRV_GPIO_WRITE" 			, gpio_write_debug , "<IO number>,< Level>");
	// register_command ("DRV_GPIO_READ"			, gpio_read_debug , "<IO number>");
	// register_command ("DRV_GPIO_GET_DIRECTION"	, gpio_get_direction_debug , "<IO number>");
	// register_command ("DRV_GPIO_SET_DIRECTION"	, gpio_set_direction_debug , "<IO number>,< in/out>");
	
	// register_command ("DRV_GPIO_TEST"			, gpio_iotest_debug , "none");
// #ifdef DEVICE_2210P
	// register_command ("DRV_GPIO_IP_RST"			, gpio_ip_reset_debug , "none");
// #endif
	// register_command ("DRV_KEY_TEST"			, gpio_keytest_debug , "none");
	// register_command ("DRV_LCD_TEST"			, gpio_lcdtest_debug , "none");
	// register_command ("DRV_ALARM_LED"			, gpio_alarm_led_debug , "<index><status, 0/green, 1/red, 2/off>");
	// register_command ("DRV_TUNER_LED"			, gpio_tuner_led_debug , "<index><status, 0/green, 1/red, 2/off>");

	// register_command ("DRV_EEPROM_READ_BYTE"	, eeprom_read_byte_debug , "<eeprom type><offset>");
	// register_command ("DRV_EEPROM_READ_BYTES"	, eeprom_bytes_read_debug , "<eeprom type><offset><length>");
	// register_command ("DRV_EEPROM_WRITE_BYTE"	, eeprom_write_byte_debug, "<eeprom type><offset><data>");
	// register_command ("DRV_EEPROM_WRITE_BYTES"	, eeprom_bytes_write_debug, "<eeprom type><offset>");
	// register_command ("DRV_EEPROM_AUTO_TEST"	, eeprom_auto_test_debug, "<eeprom type><offset>");

	// register_command ("DRV_DETECT_IO"			, hardware_detect_debug , "none");

	// register_command ("FPGA_INIT"				, fpga_init_debug , "none");
	// register_command ("FPGA_CONFIG"				, fpga_config_debug , "<fpga, 0/1>");
	// register_command ("FPGA_READ"				, fpga_read_debug , "<address><data width, 8/16/32>");
	// register_command ("FPGA_WRITE"				, fpga_write_debug , "<address><data width, 8/16/32><data>");
	// register_command ("FPGA_BURST_READ"			, fpga_burst_read_debug , "<address><length>");
	// register_command ("FPGA_BURST_WRITE"		, fpga_burst_write_debug , "<address>");

	// register_command ("DRV_I2C_READ"			, i2c_read_debug , "<bus>,<slave addr>,<reg>");
	// register_command ("DRV_I2C_WRITE"			, i2c_write_debug , "<bus>,<slave addr>,<reg>,<data>");

	// register_command ("DRV_CI_RESET"			, ci_reset_debug , "<id>");

	// register_command ("DRV_TUNER_RESET"		, tuner_reset_debug , "<id>");

// #ifdef PLATFORM_MSTAR
	// mi_debug_register();
// #endif
}
