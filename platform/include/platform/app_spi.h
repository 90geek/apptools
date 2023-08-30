#ifndef _APP_SPI_H_
#define _APP_SPI_H_
#include "platform/app_platform.h"

U64 get_7a_spi_base_addr(void);
void set_7a_spi_base_addr(U64 base_addr);
void read_7a_tcm(unsigned int offset, unsigned char * datas, int read_cnt, int cs);
void read_7a_spi(unsigned int offset, unsigned char * datas, int read_cnt);
void read_7a_flash_mac(unsigned int eth);
void write_7a_spi(unsigned int offset, unsigned char * datas, int write_cnt);
void write_7a_flash_mac(unsigned int eth, unsigned char * datas);
void read_cpu_spi_flash(unsigned int offset, unsigned char * datas, int read_cnt) ;
void write_cpu_spi_flash(unsigned int offset, unsigned char * datas, int write_cnt);
void erase_cpu_spi_flash(unsigned int offset, int byte_cnt);
void erase_block_cpu_spi_flash(unsigned int offset, int byte_cnt);
void read_7a_tcm_id(void);

void app_spi_init(void);
void app_spi_read(int spi_dev,unsigned int offset, unsigned char * datas, int read_cnt);
void app_spi_write(int spi_dev,unsigned int offset, unsigned char * datas, int write_cnt);

#endif
