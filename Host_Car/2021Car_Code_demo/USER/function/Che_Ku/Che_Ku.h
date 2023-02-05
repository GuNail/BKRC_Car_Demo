#ifndef _CHE_KU_H_
#define _CHE_KU_H_
#include "main.h"

typedef enum
{
    Floor_1 = 0x01,
    Floor_2 = 0x02,
    Floor_3 = 0x03,
    Floor_4 = 0x04
} _Floor_Typedef;

void Che_Ku_Send_A(u8 *data);
void Che_Ku_Floor_Choose_A(uint8_t Floor);
void Che_Ku_Floor_1_A(void);
void Che_Ku_Floor_2_A(void);
void Che_Ku_Floor_3_A(void);
void Che_Ku_Floor_4_A(void);
void Che_Ku_CallBack_Floor_A(void);
void Che_Ku_CallBack_IrF_A(void);

void Che_Ku_Send_B(u8 *data);
void Che_Ku_Floor_Choose_B(_Floor_Typedef Floor);
void Che_Ku_Floor_1_B(void);
void Che_Ku_Floor_2_B(void);
void Che_Ku_Floor_3_B(void);
void Che_Ku_Floor_4_B(void);
void Che_Ku_CallBack_Floor_B(void);
void Che_Ku_CallBack_IrF_B(void);

void Che_ku_CallBackFun(u8 *data);

void Che_ku_wait_Floor_A(_Floor_Typedef Floor);
void Che_ku_wait_Floor_B(_Floor_Typedef Floor);
void Che_Ku_wait_Ird_A(void);
void Che_Ku_wait_Ird_B(void);

static uint8_t Che_Ku_Floor_Flag = 0;
static uint8_t Che_Ku_IRD_Flag = 0;

#endif

