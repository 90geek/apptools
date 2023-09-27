#include "edk_api.h"
#include "loongson3_def.h"
#include "mem.h"
#include "i2c.h"
#include "platform/app_types.h"
#include "platform/app_os.h"

//#define SPI_CONFUSE_SPACE (0x0efdfe000000 + 0x8000/*Need 4K align*/ ) //b000 is Spi,so add 0x3000
//#define MPS_ADDR 0x3b //single
// #define MPS_ADDR 0x4b

#define I2C_CTL_CLK 100000
#define I2C_BUS_RATE 42
#define ALGORITHM_3A 0x3a

void I2cReadOps(int node,unsigned int i2cbus, int mps_addr)
{
	void * p = NULL;
	UINT64 I2cRegBaseAddr;
	UINT64 pi2c;
	UINT16 Val16;
	int memoffset=0;

	pi2c=node;
	pi2c=i2cbus|(pi2c<<44);
	p=p2v_mem_mapping(pi2c ,4096, &memoffset);

	I2cRegBaseAddr = (UINT64)p;
	I2cInitSetFreq (I2cRegBaseAddr, 100000, 42, 0x3a); //Lock Freq

	Val16 = 0x0; //page0
	I2cCtlWrite (I2cRegBaseAddr, mps_addr, 0, 0x1, &Val16);

	//Vddn IOUT
	I2cCtlRead (I2cRegBaseAddr, mps_addr, 0x8c, 0x2, &Val16);
	printf ("\nVddn: \n");
	printf ("Current IOUT:%d(0x%x)A \n",Val16,Val16);

	//Vddn VOUT
	I2cCtlRead (I2cRegBaseAddr, mps_addr, 0x8b, 0x2, &Val16);
	printf ("Voltage VOUT:%d(0x%x)mV \n", Val16,Val16);

	//Vddn POUT
	I2cCtlRead (I2cRegBaseAddr, mps_addr, 0x96, 0x2, &Val16);
	printf ("Power POUT:%d(0x%x)W \n", Val16,Val16);

	I2cCtlRead (I2cRegBaseAddr, mps_addr, 0x97, 0x2, &Val16);
	printf ("Power PIN %d(0x%x)W \n",Val16,Val16);

	/*Vp sensed*/
	Val16 = 0x1;//page 1
	I2cCtlWrite (I2cRegBaseAddr, mps_addr, 0, 0x1, &Val16);

	//Vddp IOUT
	I2cCtlRead (I2cRegBaseAddr, mps_addr, 0x8c, 0x2, &Val16);
	printf ("\nVddp:\n");
	printf ("Current IOUT %d(0x%x)A\n",Val16,Val16);

	I2cCtlRead (I2cRegBaseAddr, mps_addr, 0x8b, 0x2, &Val16);
	printf ("Voltage VOUT %d(0x%x)mV\n",Val16,Val16);

	I2cCtlRead (I2cRegBaseAddr, mps_addr, 0x96, 0x2, &Val16);
	printf ("Power POUT:%d(0x%x)W\n",Val16,Val16);

	Val16 = 0x0;
	I2cCtlWrite (I2cRegBaseAddr, mps_addr, 0x0, 0x1, &Val16);
	p2v_mem_clean(p, memoffset);
}

void I2cWriteOps(int node,unsigned int i2cbus, int mps_addr, int step, int volmV)
{
	void * p = NULL;
	UINT64 I2cRegBaseAddr;
	UINT32 Val16;
	UINT64 pi2c;
	int memoffset=0;

	pi2c=node;
	pi2c=i2cbus|(pi2c<<52);
	printf("i2c physical addr 0x%llx,mps_addr 0x%x,step %d, volmV %d\n",pi2c,mps_addr,step, volmV);
	p=p2v_mem_mapping(pi2c ,4096, &memoffset);


	I2cRegBaseAddr = (UINT64)p;
	I2cInitSetFreq (I2cRegBaseAddr, 100000, 42, 0x3a); //Lock Freq

	Val16 = 0x0; //a2
	I2cCtlWrite (I2cRegBaseAddr, mps_addr, 0, 0x1, &Val16);

	Val16 = 0x210;
	//Vddn IOUT
	I2cCtlWrite (I2cRegBaseAddr, mps_addr, 0x5e, 0x2, &Val16);

	Val16 = volmV / step - 49;
	I2cCtlWrite (I2cRegBaseAddr, mps_addr, 0x21, 0x2, &Val16);

	Val16 = 0x0; //a2
	I2cCtlWrite (I2cRegBaseAddr, mps_addr, 0x0, 0x1, &Val16);
}

