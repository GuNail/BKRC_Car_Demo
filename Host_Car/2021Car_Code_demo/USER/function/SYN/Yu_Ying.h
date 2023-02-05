#ifndef _SYN_H_
#define _SYN_H_
#include "syn7318.h"
#include "main.h"

typedef enum
{
    Specify_1 = 0x01,
    Specify_2 = 0x02,
    Specify_3 = 0x03,
    Specify_4 = 0x04,
    Specify_5 = 0x05,
    Specify_6 = 0x06,
    Specify_7 = 0x07,
} _Yu_Ying_Specify;

void Yu_Ying_Specify(_Yu_Ying_Specify Specify);
void Yu_Ying_Random(void);
void Yu_Ying_Set_RTC_Date(uint8_t hex_year, uint8_t hex_month, uint8_t hex_day);
void Yu_Ying_Get_RTC_Date(void);
void Yu_Ying_Set_RTC_Time(uint8_t hex_hour, uint8_t hex_min, uint8_t hex_sec);
void Yu_Ying_Get_RTC_Time(void);
void Yu_Ying_Play(uint8_t* strPut);


#endif

