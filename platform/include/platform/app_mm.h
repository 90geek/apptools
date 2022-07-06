#ifndef _APP_MM_H_
#define _APP_MM_H_
#include "platform/app_platform.h"

void app_mm_read(U64 pbase, U64 *datas, int read_cnt);
void app_mm_write(U64 pbase, U64 datas, int write_cnt);
void app_mm_read_burst(U64 pbase, unsigned char * datas, int read_cnt);
#endif
