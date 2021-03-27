#ifndef _PBI_I2C_H_
#define _PBI_I2C_H_

#define MAX_I2C_NUMBER 5

void pbi_i2c_init(void);

int pbi_i2c_read(unsigned int i2c_bus, unsigned char dev_addr,  unsigned char*   reg_addr, 
					unsigned int addr_width, unsigned char* datas, unsigned int data_length);

int pbi_i2c_write(unsigned int i2c_bus, unsigned char dev_addr,  unsigned char*  reg_addr, 
					unsigned int addr_width, unsigned char* datas, unsigned int data_length);


#endif
