#ifndef __TBA_H
#define __TBA_H
#include "stm32f4xx.h"
#include "main.h"
#include "sys.h"
#define L_LED  0x01
#define R_LED  0x02

#define SEG PCout(13)  
#define SER PBout(15)
#define RCK PHout(11)
#define SCK PHout(10)


void Tba_Init(void);
void Set_tba_Beep(uint8_t swch);
void Set_tba_WheelLED(uint8_t LorR,uint8_t swch);

uint8_t Get_tba_phsis_value(void);

void A72_Control_LED(uint8_t LorR);

void open_tba_leftLED(void);
void open_tba_rightLED(void);
void open_tba_allLED(void);
void close_tba_allLED(void);

void SEG_Show(uint8_t SegSet);
void HC595_Config(void);

#endif

