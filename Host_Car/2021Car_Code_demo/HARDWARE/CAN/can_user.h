#include "stm32f4xx.h"
#ifndef __CAN_USER_H__
#define __CAN_USER_H__

#ifdef __CAN_USER_C__
#define GLOBAL
#else
#define GLOBAL extern
#endif

#define  ZIGB_RX_MAX    200

extern uint8_t Zigb_Rx_Buf[ZIGB_RX_MAX];
extern uint8_t Zigbee_Rx_num;
extern uint8_t Zigbee_Rx_flag;  //接收完成标志位
extern uint32_t canu_zibe_rxtime;
extern uint8_t ETC_open_Flag;
extern uint8_t AGV_data_Falg;

GLOBAL void Canuser_Init(void);
GLOBAL void Canuser_main(void);

GLOBAL void Can_ZigBeeRx_Save(uint8_t res);

void Can_ZigBeeRx_Check(void);

extern void ETC_move(uint8_t sp,uint16_t mp);
void Full_End_Car(void);

#undef GLOBAL


#endif //__CAN_USER_H__

