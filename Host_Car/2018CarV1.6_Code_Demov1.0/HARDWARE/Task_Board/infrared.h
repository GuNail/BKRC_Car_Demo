#include "stm32f4xx.h"
#include "sys.h"

#ifndef __INFRARED_H
#define __INFRARED_H

#define RI_TXD PGout(8)  

void Infrared_Init(void);
void Infrared_Send(u8 *s,int n);

#endif




