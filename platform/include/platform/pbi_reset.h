#ifndef _PBI_RESET_H_
#define _PBI_RESET_H_

#include "platform/pbi_types.h"

void reset_ch7038(void) ;
void reset_cs8416(void) ;
void reset_lmh0040(void) ;
void reset_ci(int id, int ms);
void reset_tuner(int id);

void reset_sil9024(void);
void reset_sil9293(void);

void reset_lmh1983(void);

#endif
