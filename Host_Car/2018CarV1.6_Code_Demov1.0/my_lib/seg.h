#ifndef __SEG_H_
#define __SEG_H_

#include "sys.h"
#include "stm32f4xx.h"

#define SEG PCout(13)  
#define SER PBout(15)
#define RCK PHout(11)
#define SCK PHout(10)

void SEG_Show(uint8_t SegSet);
void HC595_Config(void);
void Seg_Config(void);

#endif
