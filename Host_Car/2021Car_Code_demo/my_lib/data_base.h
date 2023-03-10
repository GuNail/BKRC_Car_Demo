#ifndef __DATA_BASE_H
#define __DATA_BASE_H

#include "stm32f4xx.h"
//**********************A72数据保存****************************//






//***************测试用数据*******************//

static u8 H_S[4]={0x80,0x7F,0x05,~(0x05)};	 //照片上翻
static u8 H_X[4]={0x80,0x7F,0x1B,~(0x1B)};	 //照片下翻

static u8 H_1[4]={0x00,0xFF,0x0C,~(0x0C)};	 //光源档位加1
static u8 H_2[4]={0x00,0xFF,0x18,~(0x18)};	 //光源档位加2
static u8 H_3[4]={0x00,0xFF,0x5E,~(0x5E)};	 //光源档位加3

static u8 H_SD[4]={0x00,0xFF,0x45,~(0x45)};	  //隧道风扇系统打开

static u8 HW_K[6]={0x03,0x05,0x14,0x45,0xDE,0x92};  //报警器打开
static u8 HW_G[6]={0x67,0x34,0x78,0xA2,0xFD,0x27};	//报警器关闭

static u8 CP_G1[6]={0xFF,0x12,0x01,0x00,0x00,0x00};//立体显示车牌
static u8 CP_G2[6]={0xFF,0x13,0x01,0x00,0x00,0x00};

static u8 DZ_K[8]={0x55,0x03,0x01,0x01,0x00,0x00,0x02,0xBB};  //道闸开启
static u8 DZ_G[8]={0x55,0x03,0x01,0x02,0x00,0x00,0x03,0xBB};  //道闸关闭	

static u8 SMG_SHOW[8]={0x55,0x04,0x02,0xA1,0xB2,0xC3,0x18,0xBB};  //数码管显示
static u8 SMG_JSK[8]={0x55,0x04,0x03,0x01,0x00,0x00,0x04,0xBB};  //数码管开始计时
static u8 SMG_JSG[8]={0x55,0x04,0x03,0x00,0x00,0x00,0x03,0xBB};  //数码管关闭计时
static u8 SMG_JL[8]={0x55,0x04,0x04,0x00,0x02,0x00,0x06,0xBB};  //数码管显示距离

static u8 CP_SHOW1[6]={0xFF,0x20,0x4E,0x34,0x30,0x30};
static u8 CP_SHOW2[6]={0xFF,0x10,0x59,0x36,0x41,0x33};

static uint8_t RFID_Card[16];

#define Principal_Length 50
#define Follower_Length  50

extern uint8_t Principal_Tab[Principal_Length]; 
extern uint8_t Follower_Tab[Follower_Length];

#endif

