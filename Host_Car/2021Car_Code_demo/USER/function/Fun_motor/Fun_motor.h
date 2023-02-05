#ifndef _FUN_MOTOR_H_
#define _FUN_MOTOR_H_
#include "main.h"

#define Get_sp		60
#define Get_mp		410

#define Track_MaxError	0// Ѱ����Ѱ��0xFF�����ʱ��ֵ 1Ϊ0.002s��1000Ϊ 2s

#define official	0			//�Ƿ�ʹ�ùٷ�Ѱ�� 0 �� 1��

void Stop(void);
void Track(int sp);
void Go_Back(int sp, uint16_t mp);
void LeftX(uint8_t sp, uint16_t angle);
void RightX(uint8_t sp, uint16_t angle);
void Track_wheel_L(uint8_t sp);
void Track_wheel_R(uint8_t sp);
void Track_Mp(int sp, uint16_t mp);

void Fun_Track_Go(void);
void Fun_Track(int sp);
void Fun_Go_Back(int sp, uint16_t mp);
void Fun_Track_Mp(int sp, uint16_t mp);

void Fun_Track_wheel_L(uint8_t sp);
void Fun_Track_wheel_R(uint8_t sp);

void Fun_RightX(uint8_t sp, uint16_t angle);
void Fun_LeftX(uint8_t sp, uint16_t angle);

void Fun_Track_Amend(int i, uint8_t time, uint8_t Go, uint8_t back, uint8_t cont);

void Fun_FindCard_Amend(int i, uint8_t time, uint8_t Go, uint8_t back, uint8_t cont);

uint8_t Track_Card(uint8_t sp, uint16_t mp);

void Find_Line(void);

#endif
