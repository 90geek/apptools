#ifndef _PBI_SPI_H_
#define _PBI_SPI_H_

#if (defined DEVICE_2210P) || (defined DEVICE_3200P)

#ifdef DEVICE_2210P
#define MAX_SPI_DEV_NUM  1
#endif
#ifdef DEVICE_3200P
#define MAX_SPI_DEV_NUM  4
#endif

void pbi_spi_init(void);

void pbi_spi_read_reg32(int spi_dev,unsigned int reg, unsigned int *data);
void pbi_spi_write_reg32(int spi_dev,unsigned int reg, unsigned int data);

void pbi_spi_read_reg16(int spi_dev,unsigned int reg, unsigned short *data);
void pbi_spi_write_reg16(int spi_dev,unsigned int reg, unsigned short data);

void pbi_spi_read_reg8(int spi_dev,unsigned int reg, unsigned char *data) ;
void pbi_spi_write_reg8(int spi_dev,unsigned int reg, unsigned char data);

void pbi_spi_read_reg_burst(int spi_dev, unsigned int reg,unsigned short* datas, int read_cnt);
void pbi_spi_write_reg_burst(int spi_dev, unsigned int reg, unsigned short * datas, int write_cnt);

void pbi_spi_read(int spi_dev, unsigned char * datas, int read_cnt);
void pbi_spi_write(int spi_dev, unsigned char * datas, int write_cnt);

#else

//#ifdef DEVICE_5600P
#define MAX_SPI_DEV_NUM  1

void pbi_spi_init(void);
void pbi_spi_read(int spi_dev, unsigned char * datas, int read_cnt);
void pbi_spi_write(int spi_dev, unsigned char * datas, int write_cnt);

#endif
#endif
