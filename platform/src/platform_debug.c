#include "testtool/testtool.h"
#include "platform/app_spi.h"
#include "platform/app_mm.h"
#include "platform/platform_debug.h"
#include "platform/app_platform.h"

int read_cpu_i2c_debug(parse_t * pars_p,char *result_p)
{
	int error;
	int nodeid=0,base,dev,size,bus_rate;
	int reg;
	unsigned char buf[100]={0};

	error=cget_integer(pars_p,0,&nodeid);
	if (error)
	{
		tag_current_line(pars_p,"-->nodeid!");
		return 1;
	}
	error=cget_integer(pars_p,0,&base);
	if (error)
	{
		tag_current_line(pars_p,"-->base!");
		return 1;
	}
	error=cget_integer(pars_p,0,&dev);
	if (error)
	{
		tag_current_line(pars_p,"-->dev!");
		return 1;
	}
	error=cget_integer(pars_p,0,&reg);
	if (error)
	{
		tag_current_line(pars_p,"-->reg!");
		return 1;
	}
	error=cget_integer(pars_p,0,&size);
	if (error)
	{
		tag_current_line(pars_p,"-->size!");
		return 1;
	}
	error=cget_integer(pars_p,0,&bus_rate);
	if (error)
	{
		tag_current_line(pars_p,"-->bus_rate!");
		return 1;
	}
	if(size>100)
		size=99;
	printf("usage:\napptool>READ_CPU_I2C 0 1fe00130 4b 8b 2 42\n");
	LsCpuI2cRead(nodeid,base,dev,reg,size,(void *)buf, bus_rate);
	printf("read cpu %d i2cbase 0x%x,devaddr 0x%x, reg 0x%x, size %d, Bus_rate\n",nodeid,base,dev,reg,size, bus_rate);
	app_print_data(buf,size);
	return 0;
}
int read_cpucfg_debug(parse_t * pars_p,char *result_p)
{
	int error;
	int mode;
	int reg;
	unsigned int val;
	error=cget_integer(pars_p,0,&reg);
	if (error)
	{
		tag_current_line(pars_p,"-->reg!");
		return 1;
	}

	ReadCpucfg(val, reg);
	printf("cpucfg 0x%x=0x%x\n",reg, val );
	return 0;
}

int cpu_gpio_set_debug(parse_t * pars_p,char *result_p)
{
	int error;
	int mode;
	int num;
	int reg;

	error=cget_integer(pars_p,0,&mode);
	if (error)
	{
		tag_current_line(pars_p,"-->mode!");
		return 1;
	}
	error=cget_integer(pars_p,0,&num);
	if (error)
	{
		tag_current_line(pars_p,"-->num!");
		return 1;
	}

	if(num>32)
		printf("input num is error :%d \n",mode);

	printf("mode %d num %d \n",mode,num);
	if(mode==0)
	{
		GpioSetOutLow(CPU_GPIO_BASE, num);
	}
	else if(mode==1)
	{
		GpioSetOutHigh(CPU_GPIO_BASE, num);
	}
	else if(mode==2)
	{
		reg=GpioSetInMode(CPU_GPIO_BASE, num);
		printf("Gpio%d=%d \n",num,reg);
	}
	else
		printf("input mode is error :%d \n",mode);
	app_get_time_us();
	return 0;
}
int lscpu_gpio_pwmbeep_on_debug(parse_t * pars_p,char *result_p)
{
	int error;
	int keepms;
	int pwmfreq;

	error=cget_integer(pars_p,0,&keepms);
	if (error)
	{
		tag_current_line(pars_p,"-->keepms!");
		return 1;
	}
	error=cget_integer(pars_p,0,&pwmfreq);
	if (error)
	{
		tag_current_line(pars_p,"-->pwmfreq!");
		return 1;
	}
	printf("keep time %dms,pwm freq %d\n",keepms,pwmfreq);
	LsCpuBeepPwmOn (keepms,pwmfreq);
	printf("lscpu beep pwm success :\n");
	return 0;
}

int ls7a_gpio_pwmbeep_on_debug(parse_t * pars_p,char *result_p)
{
	int error;
	int keepms;
	int pwmfreq;

	error=cget_integer(pars_p,0,&keepms);
	if (error)
	{
		tag_current_line(pars_p,"-->keepms!");
		return 1;
	}
	error=cget_integer(pars_p,0,&pwmfreq);
	if (error)
	{
		tag_current_line(pars_p,"-->pwmfreq!");
		return 1;
	}
	printf("keep time %dms,pwm freq %d\n",keepms,pwmfreq);
	BeepPwmOn (keepms,pwmfreq);
	printf("ls7a beep pwm success :\n");
	return 0;
}
int ls7agpio_beep_on_debug(parse_t * pars_p,char *result_p)
{
	BeepOn ();
	printf("beep on :\n");
	return 0;
}

int ls7agpio_beep_off_debug(parse_t * pars_p,char *result_p)
{
	BeepOff ();
	printf("beep off :\n");
	return 0;
}
int ls7a_gpio_debug(parse_t * pars_p,char *result_p)
{
	int error;
	int mode;
	int num;
	int reg;

	error=cget_integer(pars_p,0,&mode);
	if (error)
	{
		tag_current_line(pars_p,"-->mode!");
		return 1;
	}
	error=cget_integer(pars_p,0,&num);
	if (error)
	{
		tag_current_line(pars_p,"-->num!");
		return 1;
	}

	if(num>64)
		printf("input num is error :%d \n",mode);

	printf("mode %d num %d \n",mode,num);
	if(mode==0)
	{
		Ls7aGpioSetVal(num, 0);
	}
	else if(mode==1)
	{
		Ls7aGpioSetVal(num, 1);
	}
	else if(mode==2)
	{
		reg = Ls7aGpioGetVal(num);
		printf("Gpio%d=%d \n",num,reg);
	}
	else
		printf("input mode is error :%d \n",mode);
	return 0;
}
int lpc_io_read_debug(parse_t * pars_p,char *result_p)
{
	int count;
	unsigned int port;
	unsigned char buf[100]={0};
	int error;

	error=cget_integer(pars_p,0,&port);
	if (error)
	{
		tag_current_line(pars_p,"-->port!");
		return 1;
	}
	error=cget_integer(pars_p,0,&count);
	if (error)
	{
		tag_current_line(pars_p,"-->count!");
		return 1;
	}
	if(count>100)
	{
		printf("Percent count is error %d\n",count);
		return 1;
	}
	ChipsetLpcIoRead (NULL,1,port,count,buf);
	printf("lpc io read read value :\n");
	app_print_data(buf,count);
	return 0;
}
int sio_dev_reg_debug(parse_t * pars_p,char *result_p)
{
	unsigned int dev,reg,data,flag;
	unsigned char buf;
	int error;

	error=cget_integer(pars_p,0,&dev);
	if (error)
	{
		tag_current_line(pars_p,"-->dev!");
		return 1;
	}
	error=cget_integer(pars_p,0,&reg);
	if (error)
	{
		tag_current_line(pars_p,"-->reg!");
		return 1;
	}
	error=cget_integer(pars_p,0,&flag);
	if (error)
	{
		tag_current_line(pars_p,"-->flag!");
		return 1;
	}
	error=cget_integer(pars_p,0,&data);
	if (error)
	{
		tag_current_line(pars_p,"-->data!");
		return 1;
	}

	printf("dev 0x%x, reg 0x%x flag %d data 0x%x\n",dev,reg,flag, data);
	if(data>0xff)
	{
		printf("Percent data is error 0x%x\n",data);
	}
	buf=0xa5;
	ChipsetLpcIoWrite (NULL,1,0x2e,1,&buf);
	ChipsetLpcIoWrite (NULL,1,0x2e,1,&buf);
	buf=0x07;
	ChipsetLpcIoWrite (NULL,1,0x2e,1,&buf);
	ChipsetLpcIoWrite (NULL,1,0x2f,1,&dev);
	buf=0x30;
	ChipsetLpcIoWrite (NULL,1,0x2e,1,&buf);
	buf=0x01;
	ChipsetLpcIoWrite (NULL,1,0x2f,1,&buf);
	buf=0x60;
	ChipsetLpcIoWrite (NULL,1,0x2e,1,&buf);
	buf=0x8;
	ChipsetLpcIoWrite (NULL,1,0x2f,1,&buf);
	buf=0x61;
	ChipsetLpcIoWrite (NULL,1,0x2e,1,&buf);
	buf=0xc0;
	ChipsetLpcIoWrite (NULL,1,0x2f,1,&buf);
	buf=0xaa;
	ChipsetLpcIoWrite (NULL,1,0x2e,1,&buf);
	if(flag==0)
		ChipsetLpcIoWrite (NULL,1,0x8c0+reg,1,&data);
	else
	{
		buf=0x0;
		ChipsetLpcIoRead (NULL,1,0x8c0+reg,1,&buf);
		printf(":0x%x\n",buf);
	}
	return 0;
}
int fan_set_debug(parse_t * pars_p,char *result_p)
{
	int num,pwm,max=0;
	unsigned char buf[20]={0};
	int error;
	LS7A_SMARTFAN_CFG_TABLE Parameter;

	error=cget_integer(pars_p,0,&pwm);
	if (error)
	{
		tag_current_line(pars_p,"-->pwm!");
		return 1;
	}
	error=cget_integer(pars_p,0,&num);
	if (error)
	{
		tag_current_line(pars_p,"-->num!");
		return 1;
	}
	error=cget_integer(pars_p,0,&max);
	if (error || max<100)
	{
		printf("MaxRpm 10000000 is 5Hz\n");
		printf("MaxRpm 5000000 is 10Hz\n");
		printf("MaxRpm 10000 is 5kHz\n");
		printf("MaxRpm 5000 is 10kHz\n");
		printf("MaxRpm 2500 is 20kHz\n");
		printf("MaxRpm 2000 is 25kHz\n");
		printf("MaxRpm is error %d,will used default MaxRpm 2000(25kHz)\n",max);
		max=2000;
	}
	if(num>100)
	{
		printf("Percent num is error %d\n",num);
		return 1;
	}
	Parameter.MinRpm = (max/100) * (100-num);
	Parameter.MaxRpm = max;
	SmartFanSet (pwm, Parameter);
	printf("set pwm %d fan %d % MaxRpm %d PwmFreq %dHz\n", pwm, num,max, GET_TO_PWM_FREQ(max));
	return 0;
}
int fan_set_freq_debug(parse_t * pars_p,char *result_p)
{
	int max,pwm;
	int error;
	LS7A_SMARTFAN_CFG_TABLE Parameter;

	error=cget_integer(pars_p,0,&pwm);
	if (error)
	{
		tag_current_line(pars_p,"-->pwm!");
		return 1;
	}
	error=cget_integer(pars_p,0,&max);
	if (error)
	{
		tag_current_line(pars_p,"-->max!");
		return 1;
	}
	if(max<100)
	{
		printf("max is error %d\n",max);
		return 1;
	}
	printf("set pwm %d fan 50 %\n", pwm);
	Parameter.MinRpm = max/2;// use 50% pwd test
	Parameter.MaxRpm = max;
	SmartFanSet (2, Parameter);
	// max 10000000	is	5Hz
	// max 5000000	is	10Hz
	// max 10000	is	5kHz
	// max 5000  is		10kHz
	// max 2500  is		20kHz
	// max 2000  is		25kHz
	// max*freq=50000000
	printf("set min %d max %d PwmFreq %d\n", Parameter.MinRpm, Parameter.MaxRpm, GET_TO_PWM_FREQ(max));

	return 0;
}
int fan_speed_get_debug(parse_t * pars_p,char *result_p)
{
	int pwm;
	int error;
	unsigned int speed;

	error=cget_integer(pars_p,0,&pwm);
	if (error)
	{
		tag_current_line(pars_p,"-->pwm!");
		return 1;
	}
	speed = SmartFanSpeedGet (pwm);
	printf("get pwm %d fan speed %d RPM\n", pwm, speed);

	return 0;
}
int fan_ctrl_debug(parse_t * pars_p,char *result_p)
{
	FanCtrl();
	return 0;
}
int read_cpu_temp_debug(parse_t * pars_p,char *result_p)
{
	U32 temp0,temp1,temp428;
	int node;
	int error;

	error=cget_integer(pars_p,0,&node);
	if (error)
	{
		tag_current_line(pars_p,"-->node!");
		return 1;
	}

	printf("node %d\n", node);
	lscpu_tempdetect(node,&temp0,&temp1,&temp428);
	printf("cpu temp0 %d ,temp1 %d, temp428 %d\n", temp0,temp1,temp428);
	return 0;
}
int read_7a_temp_debug(parse_t * pars_p,char *result_p)
{
	U32 temp0;

	ls7a_tempdetect(&temp0);
	printf("7a temp0 %d \n", temp0);
	return 0;
}

int read_cpu_freq_debug(parse_t * pars_p,char *result_p)
{
	U32 freq;

	CpuGetFrequency (100000,&freq);
	printf("cpu freq %d\n", freq);
	return 0;
}
int dump_acpi_reg_debug(parse_t * pars_p,char *result_p)
{
	int num;
	int error;

	error=cget_integer(pars_p,0,&num);
	if (error)
	{
		tag_current_line(pars_p,"-->num!");
		return 1;
	}
	if(num==0)
	{
		printf(" start read apci reg:\n");
		AcpiDump();
	}
	else if(num==1)
	{
		printf(" start acpi reboot:\n");
		AcpiReboot();	
	}
	else
		printf("Num is invalid");
	return 0;
}
int write_acpi_reg_debug(parse_t * pars_p,char *result_p)
{
	int reg;
	unsigned int data;
	int error;

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
	printf(" start write apci reg: 0x%x data 0x%x\n",reg,data);
	AcpiRegWrite(reg,data);
	return 0;
}
int dump_gpio_reg_debug(parse_t * pars_p,char *result_p)
{
	printf("start gpio read\n");
	GpioReadOps();
	return 0;
}
int dump_rtc_reg_debug(parse_t * pars_p,char *result_p)
{
	printf("start rtc read\n");
	RtcReadOps();
	return 0;
}
int dump_pci_debug(parse_t * pars_p,char *result_p)
{
	int bdf;
	int error;

	error=cget_integer(pars_p,0,&bdf);
	if (error)
	{
		tag_current_line(pars_p,"-->bdf!");
		return 1;
	}
	printf("bdf 0x%x\n",bdf);
	PciReadOps(bdf);
}

int mps_read_debug(parse_t * pars_p,char *result_p)
{
	int node, mps_addr;
	unsigned int i2cbus;
	int error;

	error=cget_integer(pars_p,0,&node);
	if (error)
	{
		tag_current_line(pars_p,"--> node!");
		return 1;
	}
	error=cget_integer(pars_p,0,&i2cbus);
	if (error)
	{
		tag_current_line(pars_p,"--> i2cbus!");
		return 1;
	}

	error=cget_integer(pars_p,0,&mps_addr);
	if (error)
	{
		tag_current_line(pars_p,"--> mps_addr!");
		return 1;
	}

	printf("usage:\napptool>MPS_READ 0 1fe00130 4b\n");
	printf("node 0x%x,i2cbus 0x%x, mps_addr 0x%x\n",node,i2cbus, mps_addr);
	I2cReadOps(node, i2cbus, mps_addr);
}

int mps_write_vddn_debug(parse_t * pars_p,char *result_p)
{
	int node, mps_addr,step,volmV;
	unsigned int i2cbus;
	int error;

	error=cget_integer(pars_p,0,&node);
	if (error)
	{
		tag_current_line(pars_p,"--> node!");
		return 1;
	}
	error=cget_integer(pars_p,0,&i2cbus);
	if (error)
	{
		tag_current_line(pars_p,"--> i2cbus!");
		return 1;
	}

	error=cget_integer(pars_p,0,&mps_addr);
	if (error)
	{
		tag_current_line(pars_p,"--> mps_addr!");
		return 1;
	}
	error=cget_integer(pars_p,0,&step);
	if (error)
	{
		tag_current_line(pars_p,"--> step!");
		return 1;
	}
	error=cget_integer(pars_p,0,&volmV);
	if (error)
	{
		tag_current_line(pars_p,"--> volmV!");
		return 1;
	}

	printf("node 0x%x,i2cbus 0x%x, mps_addr 0x%x, step %d, volmV %d\n",node,i2cbus, mps_addr, step, volmV);
	I2cWriteOps(node,i2cbus, mps_addr, step, volmV);
}
int avs_read_debug(parse_t * pars_p,char *result_p)
{
	int node, rail_sel, cmd_type;
	unsigned int Val;
	int error;

	error=cget_integer(pars_p,0,&node);
	if (error)
	{
		tag_current_line(pars_p,"--> node!");
		return 1;
	}

	/*RailSel p: 1		n: 0*/
	error=cget_integer(pars_p,0,&rail_sel);
	if (error)
	{
		tag_current_line(pars_p,"--> rail_sel!");
		return 1;
	}
	/*CmdType 
	 * vol:0 Equation: TARGET RAIL VOLTAGE = (Direct value)
	 * cur: 2  Equation: RAIL CURRENT= (Direct value) / 100
	 * temp 3 Equation: TEMPERATURE = (Direct value) / 10
	 * power 5
	*/
	error=cget_integer(pars_p,0,&cmd_type);
	if (error)
	{
		tag_current_line(pars_p,"--> cmd_type!");
		return 1;
	}

	printf("usage:\napptool>AVS_READ 0\n");
	printf("node %d rail_sel 0x%x cmd_type 0x%x\n",node, rail_sel, cmd_type);
	Val = AvsGet(node, rail_sel, cmd_type);
	if (!Val) {
		printf("AVS: Get Vddn error!\r\n");
	} else {
		printf("AVS: Get value is: 0x%x(%d)\n",Val, Val);
		if(cmd_type == 2)// cur
		{
			printf("AVS: Get value is: 0x%x(%d)10mA\n",Val, Val);// Equation: RAIL CURRENT= (Direct value) / 100
			printf("AVS: Get value is: 0x%x(%d)A\n",Val/100, Val/100);// Equation: RAIL CURRENT= (Direct value) / 100
		}else if(cmd_type == 0 && rail_sel == 0) {// vddn
			printf("AVS: Get vddn value is: 0x%x(%d)mV\n",Val, Val);// Equation: RAIL CURRENT= (Direct value) / 100
		}else if(cmd_type == 0 && rail_sel == 1) {// vddp
			printf("AVS: Get vddp value is: 0x%x(%d)mV\n",Val, Val);// Equation: RAIL CURRENT= (Direct value) / 100
		}
	}
}
int avs_read_vol_debug(parse_t * pars_p,char *result_p)
{
	int node;
	unsigned int Val;
	int error;

	error=cget_integer(pars_p,0,&node);
	if (error)
	{
		tag_current_line(pars_p,"--> node!");
		return 1;
	}

	printf("usage:\napptool>AVS_READ 0\n");
	printf("node %d\n",node);
	Val = AvsGetVol(node, 0, 0, 4);
	if (!Val) {
		printf("AVS: Get Vddn error!\r\n");
	} else {
		printf("AVS: Get Vddn value is: 0x%x(%d)\n",Val, Val);
	}
	Val = AvsGetVol(node, 1, 0, 4);
	if (!Val) {
		printf("AVS: Get Vddp error!\r\n");
	} else {
		printf("AVS: Get Vddp value is: 0x%x(%d)\n",Val, Val);
	}
}

int avs_write_vol_debug(parse_t * pars_p,char *result_p)
{
	int node, vddn,vddp;
	int error;

	error=cget_integer(pars_p,0,&node);
	if (error)
	{
		tag_current_line(pars_p,"--> node!");
		return 1;
	}
	error=cget_integer(pars_p,0,&vddn);
	if (error)
	{
		tag_current_line(pars_p,"--> vddn!");
		return 1;
	}

	error=cget_integer(pars_p,0,&vddp);
	if (error)
	{
		tag_current_line(pars_p,"--> vddp!");
		return 1;
	}
	printf("usage:\napptool>AVS_WTIRE 0 +1050 +1200\n");
	printf("node %d,vddn 0x%x(%d), vddp 0x%x(%d)\n",node, vddn, vddn, vddp, vddp);
	AvsAdjustVol (node, vddn, vddp);
}
int set_7a_spi_base_addr_debug(parse_t * pars_p,char *result_p)
{
	unsigned int addr_h,addr_l;
	unsigned char buf[20]={0};
	int error;
	U64 base_addr;

	error=cget_integer(pars_p,0,&addr_h);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_h");
		return 1;
	}

	error=cget_integer(pars_p,0,&addr_l);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_l!");
		return 1;
	}

	printf("Input addr hight 0x%x addr low 0x%x \n", base_addr);
	base_addr=addr_h;
	base_addr=((base_addr&0xffffffff)<<32);
	base_addr|=addr_l;
	set_7a_spi_base_addr(base_addr);
	printf("Base addr  0x%llx \n", base_addr);
	return 0;
}
int read_tistpm_debug(parse_t * pars_p,char *result_p)
{
	int len,offset,cs;
	unsigned char buf[100]={0};
	int i=0, error;

	error=cget_integer(pars_p,0,&offset);
	if (error)
	{
		tag_current_line(pars_p,"-->offset!");
		return 1;
	}
	buf[0]=TisRegRead8(offset);


	printf("value :\n");
	app_print_data(buf,1);
	return 0;
}
int write_tistpm_debug(parse_t * pars_p,char *result_p)
{
	int offset,cs,data;
	unsigned char buf[100]={0};
	int i=0, error;

	error=cget_integer(pars_p,0,&offset);
	if (error)
	{
		tag_current_line(pars_p,"-->offset!");
		return 1;
	}
	error=cget_integer(pars_p,0,&data);
	if (error)
	{
		tag_current_line(pars_p,"-->data!");
		return 1;
	}
	printf("offset 0x%x,data 0x%x :\n",offset,data);
	TisRegWrite8(offset,data);
	return 0;
}
int spi_read_tcm_debug(parse_t * pars_p,char *result_p)
{
	int len,offset,cs;
	unsigned char buf[100]={0};
	int i=0, error;

	error=cget_integer(pars_p,0,&offset);
	if (error)
	{
		tag_current_line(pars_p,"-->offset!");
		return 1;
	}

	error=cget_integer(pars_p,0,&cs);
	if (error)
	{
		tag_current_line(pars_p,"-->cs!");
		return 1;
	}

	error=cget_integer(pars_p,0,&len);
	if (error)
	{
		tag_current_line(pars_p,"-->len!");
		return 1;
	}

	if(len>100)
		len = 100;

	read_7a_tcm(offset, buf, len, cs);

	printf("spi_read read value :\n");
	app_print_data(buf,len);
	return 0;
}
int spi_read_debug(parse_t * pars_p,char *result_p)
{
	int dev,len,offset;
	unsigned char buf[100]={0};
	int i=0, error;

	error=cget_integer(pars_p,0,&dev);
	if (error)
	{
		tag_current_line(pars_p,"-->dev!");
		return 1;
	}
	error=cget_integer(pars_p,0,&offset);
	if (error)
	{
		tag_current_line(pars_p,"-->offset!");
		return 1;
	}

	error=cget_integer(pars_p,0,&len);
	if (error)
	{
		tag_current_line(pars_p,"-->len!");
		return 1;
	}

	if(len>100)
		len = 100;

	app_spi_read(dev, offset, buf, len);

	printf("spi_read read value :\n");
	app_print_data(buf,len);
	return 0;
}

int spi_write_debug(parse_t * pars_p,char *result_p)
{
	int dev,len,offset;
	unsigned int data;
	unsigned char buf[20]={0};
	int i=0, error;

	error=cget_integer(pars_p,0,&dev);
	if (error)
	{
		tag_current_line(pars_p,"-->dev!");
		return 1;
	}
	error=cget_integer(pars_p,0,&offset);
	if (error)
	{
		tag_current_line(pars_p,"-->offset!");
		return 1;
	}

	error=cget_integer(pars_p,0,&data);
	if (error)
	{
		tag_current_line(pars_p,"-->data!");
		return 1;
	}

	buf[0] = (data&0xff);
	buf[1] = (data>>8)&0xff;
	buf[2] = (data>>16)&0xff;
	buf[3] = (data>>24)&0xff;

	app_spi_write(dev,offset, buf, 4);

	return 0;
}

int spi_write_7a_flash_mac_debug(parse_t * pars_p,char *result_p)
{
	int eth;
	int i=0, error;
	char str_mac[200];
	char *pbuf;

	error=cget_integer(pars_p,0,&eth);
	if (error)
	{
		tag_current_line(pars_p,"-->eth!");
		return 1;
	}

	if (cget_string(pars_p,"", str_mac, sizeof(str_mac))==1)
	{
		tag_current_line(pars_p,"Invalid str_mac\n");
		return(1);
	}

	pbuf=str2data_mac(str_mac);

	write_7a_flash_mac(eth, pbuf);

	return 0;
}
int spi_unlock_bios_wp_debug(parse_t * pars_p,char *result_p)
{
	printf("unlock bios write protected !!! \n");
	spi_unlock_bios_wp();
	return 0;
}
int mm_read_byte_debug(parse_t * pars_p,char *result_p)
{
	unsigned int addr_h,addr_l;
	int error;
	U64 base_addr;
	U8 data;

	error=cget_integer(pars_p,0,&addr_h);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_h!");
		return 1;
	}
	error=cget_integer(pars_p,0,&addr_l);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_l!");
		return 1;
	}
	
	base_addr=addr_h;
	base_addr=((base_addr&0xffffffff)<<32);
	base_addr|=addr_l;
	printf("Base addr  0x%llx \n", base_addr);
	app_mm_read(base_addr, (U64 *)&data, 1);
	printf("mm read value :0x%x\n",data);
	return 0;
}
int mm_read_hword_debug(parse_t * pars_p,char *result_p)
{
	unsigned int addr_h,addr_l;
	int error;
	U64 base_addr;
	U16 data;

	error=cget_integer(pars_p,0,&addr_h);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_h!");
		return 1;
	}
	error=cget_integer(pars_p,0,&addr_l);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_l!");
		return 1;
	}
	
	base_addr=addr_h;
	base_addr=((base_addr&0xffffffff)<<32);
	base_addr|=addr_l;
	printf("Base addr  0x%llx \n", base_addr);
	app_mm_read(base_addr, (U64 *)&data, 2);
	printf("mm read value :0x%x\n",data);
	return 0;
}
int mm_read_word_debug(parse_t * pars_p,char *result_p)
{
	unsigned int addr_h,addr_l;
	int error;
	U64 base_addr;
	U32 data;

	error=cget_integer(pars_p,0,&addr_h);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_h!");
		return 1;
	}
	error=cget_integer(pars_p,0,&addr_l);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_l!");
		return 1;
	}
	
	base_addr=addr_h;
	base_addr=((base_addr&0xffffffff)<<32);
	base_addr|=addr_l;
	printf("Base addr  0x%llx \n", base_addr);
	app_mm_read(base_addr,(U64 *)&data, 4);
	printf("mm read value :0x%x\n",data);
	return 0;
}
int mm_read_dword_debug(parse_t * pars_p,char *result_p)
{
	unsigned int addr_h,addr_l;
	int error;
	U64 base_addr;
	U64 data;

	error=cget_integer(pars_p,0,&addr_h);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_h!");
		return 1;
	}
	error=cget_integer(pars_p,0,&addr_l);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_l!");
		return 1;
	}
	
	base_addr=addr_h;
	base_addr=((base_addr&0xffffffff)<<32);
	base_addr|=addr_l;
	printf("Base addr  0x%llx \n", base_addr);
	app_mm_read(base_addr, &data, 8);
	printf("mm read value :0x%llx\n",data);
	return 0;
}
int mm_write_byte_debug(parse_t * pars_p,char *result_p)
{
	unsigned int addr_h,addr_l;
	U32 data;
	int error;
	U64 base_addr;

	error=cget_integer(pars_p,0,&addr_h);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_h!");
		return 1;
	}
	error=cget_integer(pars_p,0,&addr_l);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_l!");
		return 1;
	}
	error=cget_integer(pars_p,0,&data);
	if (error)
	{
		tag_current_line(pars_p,"-->data!");
		return 1;
	}

	base_addr=addr_h;
	base_addr=((base_addr&0xffffffff)<<32);
	base_addr|=addr_l;
	printf("Base addr  0x%llx data=0x%x\n", base_addr,data);
	app_mm_write(base_addr, (U64)data, 1);
	return 0;
}

int mm_write_hword_debug(parse_t * pars_p,char *result_p)
{
	unsigned int addr_h,addr_l;
	U32 data;
	int error;
	U64 base_addr;

	error=cget_integer(pars_p,0,&addr_h);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_h!");
		return 1;
	}
	error=cget_integer(pars_p,0,&addr_l);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_l!");
		return 1;
	}
	error=cget_integer(pars_p,0,&data);
	if (error)
	{
		tag_current_line(pars_p,"-->data!");
		return 1;
	}

	base_addr=addr_h;
	base_addr=((base_addr&0xffffffff)<<32);
	base_addr|=addr_l;
	printf("Base addr  0x%llx data=0x%x\n", base_addr,data);
	app_mm_write(base_addr, (U64)data, 2);
	return 0;
}
int mm_write_word_debug(parse_t * pars_p,char *result_p)
{
	unsigned int addr_h,addr_l;
	U32 data;
	int error;
	U64 base_addr;

	error=cget_integer(pars_p,0,&addr_h);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_h!");
		return 1;
	}
	error=cget_integer(pars_p,0,&addr_l);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_l!");
		return 1;
	}
	error=cget_integer(pars_p,0,&data);
	if (error)
	{
		tag_current_line(pars_p,"-->data!");
		return 1;
	}

	base_addr=addr_h;
	base_addr=((base_addr&0xffffffff)<<32);
	base_addr|=addr_l;
	printf("Base addr  0x%llx data=0x%x\n", base_addr,data);
	app_mm_write(base_addr, (U64)data, 4);
	return 0;
}
int mm_write_dword_debug(parse_t * pars_p,char *result_p)
{
	unsigned int addr_h,addr_l;
	U32 data_h,data_l;
	int error;
	U64 base_addr;
	U64 data;

	error=cget_integer(pars_p,0,&addr_h);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_h!");
		return 1;
	}
	error=cget_integer(pars_p,0,&addr_l);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_l!");
		return 1;
	}
	error=cget_integer(pars_p,0,&data_h);
	if (error)
	{
		tag_current_line(pars_p,"-->data_h!");
		return 1;
	}
	error=cget_integer(pars_p,0,&data_l);
	if (error)
	{
		tag_current_line(pars_p,"-->data_h!");
		return 1;
	}

	base_addr=addr_h;
	base_addr=((base_addr&0xffffffff)<<32);
	base_addr|=addr_l;
	
	data=data_h;
	data=((data&0xffffffff)<<32);
	data|=data_l;

	printf("Base addr  0x%llx data=0x%x\n", base_addr,data);
	app_mm_write(base_addr, data, 8);
	return 0;
}
int mm_read_burst_debug(parse_t * pars_p,char *result_p)
{
	unsigned int addr_h,addr_l;
	int len;
	unsigned char buf[100]={0};
	int error;
	U64 base_addr;

	error=cget_integer(pars_p,0,&addr_h);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_h!");
		return 1;
	}
	error=cget_integer(pars_p,0,&addr_l);
	if (error)
	{
		tag_current_line(pars_p,"-->addr_l!");
		return 1;
	}
	error=cget_integer(pars_p,0,&len);
	if (error)
	{
		tag_current_line(pars_p,"-->len!");
		return 1;
	}

	if(len>100)
		len = 100;
	
	base_addr=addr_h;
	base_addr=((base_addr&0xffffffff)<<32);
	base_addr|=addr_l;
	printf("Base addr  0x%llx \n", base_addr);
	app_mm_read_burst(base_addr, buf, len);
	printf("mm read value :\n");
	app_print_data(buf,len);
	return 0;
}

int cmd_test_debug(parse_t * pars_p,char *result_p)
{
	char cmd[200];
	char *data=NULL;

	/* ------------- */
	if (cget_string(pars_p,"", cmd, sizeof(cmd))==1)
	{
		tag_current_line(pars_p,"Invalid cmd\n");
		return(1);
	}

	printf("cmd  %s \n",cmd);
	data=app_system(cmd);
	printf("%s",data);

	if(data!=NULL)
		app_free(data);
	return 0;
}

void platform_debug_register(void)
{
	register_command ("LS_FAN_SET"			, fan_set_debug , "<Pwm>:0-3, <Percent number>:0-100, <MaxRpm>:NONE/[100:50000000](pwmfreq=50000000/MaxRpm),NONE is default:2000");
	register_command ("LS_FAN_TEST_FREQ"			, fan_set_freq_debug , "<Pwm>:0-3, <MaxRpm> 100~50000000(pwmfreq=50000000/MaxRpm)");
	register_command ("LS_FAN_SPEED"		, fan_speed_get_debug , "<Pwm>:0-3");
	register_command ("LS_FAN_CTRL"		, fan_ctrl_debug , "<NONE>");
	register_command ("LS_CPU_TEMP"			, read_cpu_temp_debug , "<NONE>");
	register_command ("LS_7A_TEMP"			, read_7a_temp_debug , "<NONE>");
	register_command ("LS_CPU_FREQ"			, read_cpu_freq_debug , "<NONE>");
	register_command ("LS_DUMP_ACPI"			, dump_acpi_reg_debug , "<num>:0 dump acpi reg,1 apci reboot");
	register_command ("LS_WRITE_ACPI"			, write_acpi_reg_debug , "<reg>:acpi reg, <data>write velue");
	register_command ("LS_DUMP_GPIO"			, dump_gpio_reg_debug , "<NONE>");
	register_command ("LS_DUMP_RTC"			, dump_rtc_reg_debug , "<NONE>");
	register_command ("LS_DUMP_PCI"			, dump_pci_debug , "<bdf>:eg.spi 00:16.0 bdf for b000");
	register_command ("SET_7A_SPI_BASE"			, set_7a_spi_base_addr_debug , "<base_addr_hi>,<base_addr_lo>");
	register_command ("SPI_READ"			, spi_read_debug , "<spi_dev>,<offset>,<len>:spi_dev 0cpuflash/17a");
	register_command ("SPI_WRITE"			, spi_write_debug , "<spi_dev>,<offset>,<data>:spi_dev 0cpuflash/17a,data 32bit");
	register_command ("SPI_WRITE_MAC"			, spi_write_7a_flash_mac_debug , "<eth> <macaddr>, eg 0 11:22:33:44:55:66");
	register_command ("SPI_UNLOCK_BIOS_WP"			, spi_unlock_bios_wp_debug , "none");
	register_command ("MM_R_B"			, mm_read_byte_debug , "<base_addr_hi>,<base_addr_lo>");
	register_command ("MM_R_HW"			, mm_read_hword_debug , "<base_addr_hi>,<base_addr_lo>");
	register_command ("MM_R_W"			, mm_read_word_debug , "<base_addr_hi>,<base_addr_lo>");
	register_command ("MM_R_DW"			, mm_read_dword_debug , "<base_addr_hi>,<base_addr_lo>");
	register_command ("MM_W_B"			, mm_write_byte_debug , "<base_addr_hi>,<base_addr_lo>,<data>");
	register_command ("MM_W_HW"			, mm_write_hword_debug , "<base_addr_hi>,<base_addr_lo>,<data>");
	register_command ("MM_W_W"			, mm_write_word_debug , "<base_addr_hi>,<base_addr_lo>,<data>");
	register_command ("MM_W_DW"			, mm_write_dword_debug , "<base_addr_hi>,<base_addr_lo>,<data_hi>,<data_lo>");
	register_command ("MM_R_BURST"			, mm_read_burst_debug , "<base_addr_hi>,<base_addr_lo>,<len>");
	register_command ("READ_TCM"			, spi_read_tcm_debug , "<offset>,<cs>,<len>");
	register_command ("READ8_TIS"			, read_tistpm_debug , "<offset>");
	register_command ("WRITE8_TIS"			, write_tistpm_debug , "<offset> <data>");
	register_command ("TEST_CMD"			, cmd_test_debug , "<cmd>:eg cmd lscpu");
	register_command ("LS_LPC_IO_READ"			, lpc_io_read_debug , "<prot> <count>");
	register_command ("LS_SIO_DEV_REG"			, sio_dev_reg_debug , "<dev> <reg> <0(write)/1(read)> <data>");
	register_command ("LS7A_GPIO_BEEP_ON"			, ls7agpio_beep_on_debug , "<none> ");
	register_command ("LS7A_GPIO_BEEP_OFF"			, ls7agpio_beep_off_debug , "<none> ");
	register_command ("LS7A_GPIO_PWMBEEP"			, ls7a_gpio_pwmbeep_on_debug , "<keepms>1~,<pwmfreq>1~ ");
	register_command ("LS7A_GPIO_SET"			, ls7a_gpio_debug , "<mode>0:low,1:hi,2:get,<gpionum>0~63 ");
	register_command ("LSCPU_GPIO_SET"			, cpu_gpio_set_debug , "<mode>0:low,1:hi,2:get,<gpionum>0~31 ");
	register_command ("LSCPU_GPIO_PWMBEEP"			, lscpu_gpio_pwmbeep_on_debug , "<keepms>1~,<pwmfreq>1~ ");
	register_command ("READ_CPUCFG"			, read_cpucfg_debug , "<reg>");
	register_command ("READ_CPU_I2C"			, read_cpu_i2c_debug , "<nodeid>,<i2cbase>,<i2cdev>,<reg>,<size> <bus_rate>");
	register_command ("MPS_READ"			, mps_read_debug , "<node> <i2cbus> <mps_addr>,node 0/4/8/16,i2cbus 1fe00120/1fe00130, mps_addr 3b/4b");
	register_command ("MPS_WRITE_VDDN"			, mps_write_vddn_debug , "<node> <i2cbus> <mps_addr> <step> <volmV>,node 0/4/8/16,i2cbus 1fe00120/1fe00130 mps_addr 3b/4b step +5/+10 volmV +950~+1250");
	register_command ("AVS_READ"			, avs_read_debug , "<node> <rail_sel> <cmd_type>,node 0/1/2/3, rail_sel 0/1 cmd_type 0/2/3/5");
	register_command ("AVS_READ_VOL"			, avs_read_vol_debug , "<node> ,node 0/1/2/3");
	register_command ("AVS_WRITE_VOL"			, avs_write_vol_debug , "<node> <vddn> <vddp> ,node 0/1/2/3");
}
