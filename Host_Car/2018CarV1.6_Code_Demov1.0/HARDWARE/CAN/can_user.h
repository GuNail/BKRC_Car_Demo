#include "stm32f4xx.h"
#ifndef __CAN_USER_H__
#define __CAN_USER_H__

#ifdef __CAN_USER_C__
#define GLOBAL
#else
#define GLOBAL extern
#endif

#define  ZIGB_RX_MAX    200
#define  WIFI_MAX_NUM   200

extern uint8_t Wifi_Rx_Buf[ WIFI_MAX_NUM ];
extern uint8_t Zigb_Rx_Buf[ ZIGB_RX_MAX ];
extern uint8_t Wifi_Rx_num;
extern uint8_t Wifi_Rx_flag;  //接收完成标志位
extern uint8_t Zigbee_Rx_num;
extern uint8_t Zigbee_Rx_flag;  //接收完成标志位
extern uint8_t DX_Bzw100 ;  //特殊地形判断标志位
extern uint8_t DX_Bzw101 ;  //特殊地形判断标志位
extern uint8_t DX_Bzw102 ;  //特殊地形判断标志位
extern uint8_t DX_Bzw103 ;  //特殊地形判断标志位
extern uint8_t DX_Bzw200 ;  //特殊地形判断标志位
extern uint8_t DX_Bzw201 ;  //特殊地形判断标志位
extern uint8_t DX_Bzw202 ;  //特殊地形判断标志位
extern uint8_t DX_Bzw203 ;  //特殊地形判断标志位
extern uint8_t DX_Bzw204 ;  //特殊地形判断标志位
extern uint8_t DX_Bzw;
extern uint8_t TXDX_Bzw  ;  //特殊地形完成标志位
extern uint8_t CK_Bzw    ; 
extern uint8_t Host_AGV_Return_Flag;
extern uint8_t AGV_data_Falg;
extern uint32_t canu_wifi_rxtime;
extern uint32_t canu_zibe_rxtime;
extern uint8_t CP_data[6];           //车牌信息存放数组
extern uint8_t DZCP_data[6];
GLOBAL void Canuser_Init(void);
GLOBAL void Canuser_main(void);

GLOBAL void Can_WifiRx_Save(uint8_t res);
GLOBAL void Can_ZigBeeRx_Save(uint8_t res);

void Can_WifiRx_Check(void);
void Can_ZigBeeRx_Check(void);
void DX_Panduan(void);  //特殊地形自动判断函数（可无视特殊地形与RFID卡）
void TXDX_Panduan(void);  
void TXDX_RFID(void);   //  判断特殊地形和RFID完成函数
void Red_Card_Track(uint8_t sp);
extern uint8_t ETC_open_Flag;
extern void ETC_move(uint8_t sp,uint16_t mp);
void Full_End_Car(void);

#undef GLOBAL


#endif //__CAN_USER_H__

