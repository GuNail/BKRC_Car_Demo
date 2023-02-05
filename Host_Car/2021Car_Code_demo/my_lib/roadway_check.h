#ifndef __ROADWAY_H
#define __ROADWAY_H
//#include "stm32f4xx.h"

extern volatile uint8_t Find_Line_Track_Flag;
extern volatile int Car_Spend;
extern volatile uint8_t Track_Card_Flag;
extern volatile uint8_t wheel_L_Flag;
extern volatile uint8_t wheel_R_Flag;
extern volatile uint8_t wheel_Nav_Flag;
extern volatile uint8_t Go_Flag;
extern volatile uint8_t Back_Flag;
extern volatile uint8_t Track_Flag;
extern volatile uint8_t Stop_Flag;
extern volatile uint8_t MP_Track_flag;
extern volatile uint8_t Track_Go_Flag;
extern volatile uint8_t Track_Back_Flag;
extern volatile uint8_t Track_Back_Mp_Flag;
extern volatile uint8_t Track_Go_Mp_Flag;
extern volatile uint16_t temp_MP;
extern volatile uint16_t temp_Nav;

void Stop_Flag_Cheack(void);
void RFID_Cheack(void);

void Roadway_Check(void);
void Roadway_Flag_clean(void);
void Roadway_mp_syn(void);
void Roadway_nav_syn(void);
uint16_t Roadway_mp_Get(void);
uint16_t Roadway_nav_Get(void);



void Control(int L_Spend,int R_Spend);
void Track_Correct_MP(uint8_t gd);
extern uint8_t Roadway_GoBack_Check(void);
void roadway_check_TimInit(uint16_t arr,uint16_t psc);

#endif



