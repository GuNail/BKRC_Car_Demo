#ifndef _DAO_ZHA_H_
#define _DAO_ZHA_H_

#include "canp_hostcom.h"
#include "main.h"

void Dao_zha_Send(u8 *Data);
void Dao_Zha_Call(void);
void Dao_Zha_CarID(u8 *CarID);
void Dao_Zha_close(void);
void Dao_Zha_Angle_Up(void);
void Dao_Zha_Angle_Down(void);
void Dao_Zha_Open(void);
void Dao_Zha_CallBackFun(uint8_t *data);
void Dao_Zha_pass(u8 *CarID);

static uint8_t Dao_Zha_Flag = 0;
#endif



