#ifndef __MPS_H__
#define __MPS_H__

void I2cReadOps(int node,unsigned int i2cbus, int mps_addr);
void I2cWriteOps(int node,unsigned int i2cbus, int mps_addr, int step, int volmV);

#endif
