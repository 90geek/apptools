#ifndef _APP_SPI_H_
#define _APP_SPI_H_
#include "platform/app_platform.h"
void update_bios(char *file_path);
U64 get_7a_spi_base_addr(void);
void set_7a_spi_base_addr(U64 base_addr);
void read_7a_spi(int offset, unsigned char * datas, int read_cnt);
void write_7a_spi(int offset, unsigned char * datas, int write_cnt);

void app_spi_init(void);
void app_spi_read(int spi_dev, unsigned char * datas, int read_cnt);
void app_spi_write(int spi_dev, unsigned char * datas, int write_cnt);

#endif
