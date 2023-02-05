#ifndef _ETC_H_
#define _ETC_H_
#include "main.h"

typedef enum
{
    Up = 0x01,
    Down = 0x02
} _ETC_Typedef;


void ETC_Start_Angle(_ETC_Typedef Left, _ETC_Typedef Right);
void ETC_CallBackFun(uint8_t *data);
void ETC_Pass(void);
static volatile uint8_t ETC_Flag = 0;


#endif


