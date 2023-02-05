#ifndef _LED_DIS_H
#define _LED_DIS_H_
#include "main.h"
#include "canp_hostcom.h"

void LED_DIS_Send(u8 *Data);
void LED_DIS_1seg(u8 *data);
void LED_DIS_2seg(u8 *data);

void LED_DIS_TimeStop(void);
void LED_DIS_TimeStart(void);
void LED_DIS_TimeClear(void);
void LED_DIS_Distan_char(u8 *data);
void LED_DIS_Distan_num(uint16_t data);

void LED_DIS_ultrasonic(void);

#endif


