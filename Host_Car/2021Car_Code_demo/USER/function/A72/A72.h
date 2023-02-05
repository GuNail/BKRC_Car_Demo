#ifndef _A72_H_
#define _A72_H_

#define  WIFI_MAX_NUM   200
#include "stm32f4xx.h"
void A72_QR_Code_Task(void);
void A72_Tx_Task_A(void);
void A72_Tx_Task_B(void);


extern uint8_t Wifi_Rx_Buf[ WIFI_MAX_NUM ];
extern uint8_t Wifi_Rx_num;
extern uint8_t Wifi_Rx_flag;  //接收完成标志位
extern uint32_t Wifi_rxtime;

void A72_Wifi_Send(uint8_t *data);
void A72_Trffic_light_Task_A(void);

void WifiRx_Save(uint8_t res);
void Normal_data(void)	;
void Abnormal_data(void);
void WifiRx_Check(void);
#endif

