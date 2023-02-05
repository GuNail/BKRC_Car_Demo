#ifndef _JIAO_TONG_H_
#define _JIAO_TONG_H_
#include "main.h"

typedef enum
{
    Red = 0x01,
    Green = 0x02,
    Yellow = 0x03
} _Jiao_Tong_Color_;

void Jian_Tong_Send_A(u8 *data);
void Jiao_Tong_Reco_A(void);
void Jiao_Tong_Color_A(_Jiao_Tong_Color_ Color);
void Jiao_Tong_Red_A(void);
void Jiao_Tong_Green_A(void);
void Jiao_Tong_Yellow_A(void);

void Jian_Tong_Send_B(u8 *data);
void Jiao_Tong_Reco_B(void);
void Jiao_Tong_Color_B(_Jiao_Tong_Color_ Color);
void Jiao_Tong_Red_B(void);
void Jiao_Tong_Green_B(void);
void Jiao_Tong_Yellow_B(void);

void Jiao_Tong_CallBackFun(uint8_t *data);

#endif

