#include <stdint.h>
#include "stm32f4xx.h"
#include "canP_HostCom.h"
#include "delay.h"
#include "data_channel.h"
#include "roadway_check.h"

#define __CAN_USER_C__
#include "can_user.h"
#include "tba.h"
#include "data_base.h"
#include "cba.h"
#include "infrared.h"
#include "syn7318.h"
#include <string.h>
#include "Timer.h"
#include "AGV_Car.h"

#include "Dao_Zha.h"
#include "ETC.h"
#include "Jiao_Tong.h"
#include "Che_Ku.h"

uint8_t Zigb_Rx_Buf[ ZIGB_RX_MAX ];
uint8_t Zigbee_Rx_num ;
uint8_t Zigbee_Rx_flag ;  //接收完成标志位
uint8_t volatile ETC_Open_Flag=0;

uint8_t Host_AGV_Return_Flag = RESET;
uint8_t AGV_data_Falg = RESET;
uint32_t canu_zibe_rxtime = 0;



/**
函数功能：保存ZigBee数据
参    数: 无
返 回 值：无
*/
void Can_ZigBeeRx_Save(uint8_t res)
{

	if(Zigbee_Rx_flag == 0)
	{
		canu_zibe_rxtime = gt_get()+5;
		Zigbee_Rx_num =0;
		Zigb_Rx_Buf[Zigbee_Rx_num]=res;
		Zigbee_Rx_flag = 1;
	}
	else if(Zigbee_Rx_num < ZIGB_RX_MAX )	
	{
		Zigb_Rx_Buf[++Zigbee_Rx_num]=res;	 
	}
		
}

/**
函数功能：ZigBee数据监测
参    数：无
返 回 值：无
*/
void Can_ZigBeeRx_Check(void)
{
	if(Zigbee_Rx_flag)
	{
		if(gt_get_sub(canu_zibe_rxtime) == 0)
		{
			if(Zigb_Rx_Buf[1] == 0x03) 			// 道闸 
			{
			   Dao_Zha_CallBackFun(Zigb_Rx_Buf);
			} 
			else if(Zigb_Rx_Buf[1]==0x0c) 		// ETC
			{
				if(Zigb_Rx_Buf[4]==0x06)
					ETC_Open_Flag=1;
			}
			else if((Zigb_Rx_Buf[1]==0x0E)||(Zigb_Rx_Buf[2]==0x01))	//交通灯标志物					
			{
				Jiao_Tong_CallBackFun(Zigb_Rx_Buf);
			} 
			else if((Zigb_Rx_Buf[1]==0x0F)||(Zigb_Rx_Buf[2]==0x01))
			{
				Jiao_Tong_CallBackFun(Zigb_Rx_Buf);
			}
			else if((Zigb_Rx_Buf[1]==0x05)||(Zigb_Rx_Buf[1]==0x03))//立体车库标志物
			{
				Che_ku_CallBackFun(Zigb_Rx_Buf);
			}
			else if((Zigb_Rx_Buf[1]==0x0D)||(Zigb_Rx_Buf[1]==0x03)) 
			{
				Che_ku_CallBackFun(Zigb_Rx_Buf);
			}
			else if((Zigb_Rx_Buf[0] == 0x55)&&(Zigb_Rx_Buf[1] == 0x02))		//返回从车数据
			{		
					memcpy(Follower_Tab,Zigb_Rx_Buf,50);
					AGV_data_Falg = SET;
			}
			else if(Zigb_Rx_Buf[1]==0x06)	 //语音播报返回
			{
				if(Zigb_Rx_Buf[2]==0x01)
				{
						
				}			
			}
			Zigbee_Rx_flag = 0;
		}	
	}
}


/** 暂未使用
函数功能：设置循迹上传更新时间
参    数：无
返 回 值：无
*/
void Canuser_upTrackTime(void)
{
	static uint8_t run_mode = 0;
	
	if(gt_get() < 1000)
		run_mode = 0;
	else if(run_mode == 0)
	{
		run_mode = 1;
		Host_Set_UpTrack(50);
	}
}

#define UART_or_CAN    0   //  0---UART  1---CAN

/**
函数功能：CAN查询、发送接收检测
参    数：无
返 回 值：无
*/
void Canuser_main(void)
{
	CanP_Host_Main();
	//CanP_CanTx_Check();				//CAN总线发送数据监测
	CanP_CanTx_Check_fIrq();
}

//end file

