#include "platform/app_os.h"
#include "platform/app_spi.h"
// #include "platform/pbi_gpio.h"
// #include "platform/platform_device.h"

#ifdef PLATFORM_MSTAR
#include "mstar/pbi_spi_mstar.h"
#elif defined PLATFORM_HISI
#include "hisi/pbi_spi_hisi.h"
#endif
#define NOT_USED printf("%s not used\n", __FUNCTION__)
void pbi_spi_init(void)
{
#ifdef PLATFORM_MSTAR
	pbi_spi_init_mstar();
#elif defined PLATFORM_HISI
	pbi_spi_init_hisi();
#endif

}

void pbi_spi_read_reg32(int spi_dev,unsigned int reg, unsigned int *data)
{	
#ifdef PLATFORM_MSTAR
	pbi_spi_read_reg32_mstar(spi_dev, reg,data);
#elif defined PLATFORM_HISI
	NOT_USED;
#else
	UNUSED(spi_dev);
#endif

}

void pbi_spi_write_reg32(int spi_dev,unsigned int reg, unsigned int data)
{

#ifdef PLATFORM_MSTAR
	pbi_spi_write_reg32_mstar(spi_dev,reg, data);
#elif defined PLATFORM_HISI
	NOT_USED;
#endif


}

void pbi_spi_read_reg16(int spi_dev,unsigned int reg, unsigned short *data) 
{
#ifdef PLATFORM_MSTAR
	pbi_spi_read_reg16_mstar(spi_dev,reg,data) ;
#elif defined PLATFORM_HISI
	NOT_USED;
#endif

}

void pbi_spi_write_reg16(int spi_dev,unsigned int reg, unsigned short data)
{
#ifdef PLATFORM_MSTAR
	pbi_spi_write_reg16_mstar(spi_dev, reg, data);
#elif defined PLATFORM_HISI
	NOT_USED;
#endif
}


void pbi_spi_read_reg8(int spi_dev,unsigned int reg, unsigned char *data) 
{
#ifdef PLATFORM_MSTAR
	pbi_spi_read_reg8_mstar(spi_dev, reg, data);
#elif defined PLATFORM_HISI
	NOT_USED;
#endif

}

void pbi_spi_write_reg8(int spi_dev,unsigned int reg, unsigned char data)
{
#ifdef PLATFORM_MSTAR
	pbi_spi_write_reg8_mstar( spi_dev, reg, data);
#elif defined PLATFORM_HISI
	NOT_USED;
#endif

}

void pbi_spi_read_reg_burst(int spi_dev, unsigned int reg,unsigned short* datas, int read_cnt)
{	
#ifdef PLATFORM_MSTAR
	pbi_spi_read_reg_burst_mstar( spi_dev, reg, datas, read_cnt);
#elif defined PLATFORM_HISI
	NOT_USED;
#endif

}

void pbi_spi_write_reg_burst(int spi_dev, unsigned int reg, unsigned short * datas, int write_cnt)
{
#ifdef PLATFORM_MSTAR
	pbi_spi_write_reg_burst_mstar(spi_dev, reg, datas, write_cnt);
#elif defined PLATFORM_HISI
	NOT_USED;
#endif

}

void pbi_spi_read(int spi_dev, unsigned char * datas, int read_cnt) 
{
#ifdef PLATFORM_MSTAR
	pbi_spi_read_mstar(spi_dev, datas, read_cnt);
#elif defined PLATFORM_HISI
	pbi_spi_read_hisi(spi_dev, datas, read_cnt);
#endif

}

void pbi_spi_write(int spi_dev, unsigned char * datas, int write_cnt)
{
#ifdef PLATFORM_MSTAR
	pbi_spi_write_mstar(spi_dev, datas, write_cnt);
#elif defined PLATFORM_HISI
	 pbi_spi_write_hisi(spi_dev, datas, write_cnt);
#endif

}

