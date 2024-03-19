#ifndef __LS_7A_H__
#define __LS_7A_H__

#include "edk_api.h"
void RtcReadOps(void);
void GpioReadOps(void);
void AcpiRegWrite(unsigned char reg, unsigned int data);
void AcpiDump(void);
void AcpiReboot(void);
void PciReadOps(UINT32 a);

#endif
