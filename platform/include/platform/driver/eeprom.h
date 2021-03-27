#ifndef _EEPROM_
#define _EEPROM_


typedef enum ee_prom_e{
	EE_24C02,
	EE_24C04, 
	EE_24C08, 
	EE_24C16, 
	//EE_24C32,
	//EE_24C64,
	EE_MAX
}ee_prom_t;



#ifdef DEVICE_2210P
#define EEPROM_I2C_BUS 1
#define EEPROM_SLAVE_ADDR 0xA4
#elif defined DEVICE_5600P
#define EEPROM_I2C_BUS 0
#define EEPROM_SLAVE_ADDR 0xA0
#else
#define EEPROM_I2C_BUS 0
#define EEPROM_SLAVE_ADDR 0xA0
#endif

#define WRITE_FLAG 0x12

int eeprom_read_page(ee_prom_t model, int page_offset, unsigned char *data, int length);
int eeprom_write_page(ee_prom_t model, int page_offset, unsigned char *data , int length);
int eeprom_read_byte(ee_prom_t model, int byte_offset, unsigned char *data);
int eeprom_write_byte(ee_prom_t model, int byte_offset, unsigned char data);
int eeprom_read_bytes(ee_prom_t model, int byte_offset, unsigned char *data, int length);
int eeprom_write_bytes(ee_prom_t model, int byte_offset, unsigned char *data, int length);


#endif
