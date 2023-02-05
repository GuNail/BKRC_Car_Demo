#include <stdint.h>
#include "stm32f4xx.h"
#include "canP_HostCom.h"
#include "delay.h"
#include "data_channel.h"
#include "roadway_check.h"
#include "SEG.h"
#include "can_user.h"
#include "tba.h"
#include "data_base.h"
#include "cba.h"
#include "infrared.h"
#include "syn7318.h"
#include <string.h>
#include "Timer.h"
#include "Full.h"

#define __CAN_USER_C__
uint8_t Wifi_Rx_Buf[ WIFI_MAX_NUM ];
uint8_t Zigb_Rx_Buf[ ZIGB_RX_MAX ];
uint8_t Wifi_Rx_num ;
uint8_t Wifi_Rx_flag ;  //接收完成标志位
uint8_t Zigbee_Rx_num ;
uint8_t Zigbee_Rx_flag ;  //接收完成标志位
uint8_t DX_Bzw100=0 ;  //特殊地形判断标志位
uint8_t DX_Bzw101=0 ;  //特殊地形判断标志位
uint8_t DX_Bzw102=0 ;  //特殊地形判断标志位
uint8_t DX_Bzw103=0 ;  //特殊地形判断标志位
uint8_t DX_Bzw200=0 ;  //特殊地形判断标志位
uint8_t DX_Bzw201=0 ;  //特殊地形判断标志位
uint8_t DX_Bzw202=0 ;  //特殊地形判断标志位
uint8_t DX_Bzw203=0 ;  //特殊地形判断标志位
uint8_t DX_Bzw204=0 ;  //特殊地形判断标志位
uint8_t DX_Bzw   =0 ;
uint8_t TXDX_Bzw =0 ; 
uint8_t CK_Bzw   =3 ;   //车库标志位


uint8_t Host_AGV_Return_Flag = RESET;
uint8_t AGV_data_Falg = RESET;
uint8_t  LED_DisTab[] = {0x55,0x04,0x04,0x00,0x09,0x12,0x00,0xBB};


uint32_t canu_wifi_rxtime = 0;
uint32_t canu_zibe_rxtime = 0;

void Can_WifiRx_Save(uint8_t res)
{
	if(Wifi_Rx_flag == 0)
	{
		canu_wifi_rxtime = gt_get()+10;
		Wifi_Rx_num =0;
		Wifi_Rx_Buf[Wifi_Rx_num]=res;
		Wifi_Rx_flag = 1;
	}
	else if(Wifi_Rx_num < WIFI_MAX_NUM )	
	{
		Wifi_Rx_Buf[++Wifi_Rx_num]=res;	 
	}
}

uint8_t Rx_Flag ;
void TXDX_RFID()
{
		Send_Debug_Info("(6)\n",5);
	if(TXDX_Bzw==1&&RFID_DKWC_Flag==1)
	{
		Send_Debug_Info("(11)\n",5);
	}
	else if(TXDX_Bzw==0&&RFID_DKWC_Flag==1)   //E2
	{
		Send_Debug_Info("(22)\n",5);
		Full_TXXJ_Track();
//		CK_Bzw=2;
	}
	else if(TXDX_Bzw==1&&RFID_DKWC_Flag==0)  //D2
	{
		Send_Debug_Info("(33)\n",5);
		Full_TXXJ_Track();
//		CK_Bzw=4;
	}
	else if(TXDX_Bzw==0&&RFID_DKWC_Flag==0)
	{
		Send_Debug_Info("(44)\n",5);
		Full_TXXJ_Track();
	}
}

void TXDX_Panduan()   //特殊地形自动判断函数（可无视特殊地形与RFID卡）
{
	if(DX_Bzw200 == 1)
	{
		delay_ms(300);
		delay_ms(300);
        DX_Bzw200=0;	
		delay_ms(500); 
        Full_TXTrack(Get_Tra_sp);      //自动前进循迹 
        delay_ms(100);               
	}
    if(DX_Bzw201 == 1)
	{ 
		delay_ms(300);
		DX_Bzw201=0;
		delay_ms(100); 
        Full_TXDX_XG(60,1000);//指定前进距多地形循迹  
		TXDX_Bzw=1;
        delay_ms(100);  
   
              
	} 
	if(DX_Bzw202 == 1)
	{
	   delay_ms(100); 
		//Full_Go(70,50);    //指定距离前进（不循迹）          
    delay_ms(100); 
    DX_Bzw202=0;		
		delay_ms(100); 
		Full_TXTrack(Get_Tra_sp);      //自动前进循迹 
    delay_ms(100);                
    	
	} 

	if(DX_Bzw203 == 1)
	{ 
    DX_Bzw203=0;		
		delay_ms(500);
		Full_Go(60,350);    //指定距离前进（不循迹）  		
   	Full_Run_Flag = 1; 
    delay_ms(500);               
	} 

		
}

/**
函数功能：特殊地形自动判断函数（可无视特殊地形与RFID卡）
参    数: 无  	Full_Find_cards1();					//读卡操作	
返 回 值：无
*/
void DX_Panduan()   //特殊地形自动判断函数（可无视特殊地形与RFID卡）
{
	if(DX_Bzw100 == 1)
	{
		delay_ms(100); 
		Full_Go(70,50);    //指定距离前进（不循迹）          
		delay_ms(300);
    DX_Bzw100=0;	
		delay_ms(500); 
   	Full_Track(Get_Tra_sp);      //自动前进循迹 
    delay_ms(100);               
	} 
if(DX_Bzw101 == 1)
	{ 
		delay_ms(300);
		DX_Bzw101=0;
		delay_ms(100); 
    Full_Terrain_XG(70,1300);//指定前进距多地形循迹     
    delay_ms(100);  
   
              
	} 
	if(DX_Bzw102 == 1)
	{
	   delay_ms(100); 
		//Full_Go(70,50);    //指定距离前进（不循迹）          
    delay_ms(100); 
    DX_Bzw102=0;		
		delay_ms(100); 
   	Full_Track(Get_Tra_sp);      //自动前进循迹 
    delay_ms(100);                
    	
	} 

	if(DX_Bzw103 == 1)
	{

    DX_Bzw103=0;		
		delay_ms(100); 
   	Full_Run_Flag = 1; 
    delay_ms(100);               
	} 

		
}


void Normal_data(void)	  // 正常接收8字节控制指令
{
	u8 sum=0;

	if(Wifi_Rx_Buf[7]==0xbb)	 // 判断包尾
	 {									  
		//主指令与三位副指令左求和校验
		//注意：在求和溢出时应该对和做256取余。
		 sum=(Wifi_Rx_Buf[2]+Wifi_Rx_Buf[3]+Wifi_Rx_Buf[4]+Wifi_Rx_Buf[5])%256;
		if(sum == Wifi_Rx_Buf[6])
		{
			   Rx_Flag =1;
		}
		else Rx_Flag =0;
	}
}

void Abnormal_data(void)	  //数据异常处理
{
	u8 i,j;
	u8 sum=0;
	
	if(Wifi_Rx_num <8)			// 异常数据字节数小于8字节不做处理
	{
	   Rx_Flag =0;
	}
	else{
		for(i=0;i<=(Wifi_Rx_num -7);i++)  
		{
			if(Wifi_Rx_Buf[i]==0x55)	   // 寻找包头
			{
			   if(Wifi_Rx_Buf[i+7]==0xbb)	// 判断包尾
			   {
			   	    sum=(Wifi_Rx_Buf[i+2]+Wifi_Rx_Buf[i+3]+Wifi_Rx_Buf[i+4]+Wifi_Rx_Buf[i+5])%256;

		           if(sum==Wifi_Rx_Buf[i+6])	 // 判断求和
	              {
			         for(j=0;j<8;j++)
					 {
					 	 Wifi_Rx_Buf[j]=Wifi_Rx_Buf[j+i];	 // 数据搬移
					 }
					    Rx_Flag =1;
		          }
	  	         else Rx_Flag =0;
		       }
			}
	    }

	}
}

uint8_t Infrared_Tab[6];			//红外数据存放数组
uint8_t CP_data[6];           		//车牌信息存放数组
uint8_t Color;						//交通灯
uint8_t DZCP_data[6];  			//道闸数据存放数组
void Can_WifiRx_Check(void)
{
	if(Wifi_Rx_flag)
	{	
		if(gt_get_sub(canu_wifi_rxtime) == 0)
		{
			if(Wifi_Rx_Buf[0]==0xFD)  
			{			
				Send_ZigbeeData_To_Fifo( Wifi_Rx_Buf , (Wifi_Rx_num +1)); 
			}
			else if(Wifi_Rx_Buf[0]==0x55)  
			{              
				Normal_data();  
			}
			else
			{
				Abnormal_data();      
			} 	
			Wifi_Rx_flag = 0;
		}
	}
	if(Rx_Flag ==1)	
	{  
		if(Wifi_Rx_Buf[1] == 0xAA) 	   
		{	
			switch(Wifi_Rx_Buf[2])
			{
//			case 0x01:              	//停止
//				Send_UpMotor(0 ,0);
//				Roadway_Flag_clean();	//清除标志位状态	
			//	break;
//			case 0x02:              //前进
//				Roadway_mp_syn();	//码盘同步
//				Stop_Flag = 0; Go_Flag = 1; wheel_L_Flag = 0;wheel_R_Flag = 0;wheel_Nav_Flag = 0;
//				Back_Flag = 0; Track_Flag = 0;
//				temp_MP = Wifi_Rx_Buf[5];
//				temp_MP <<=  8;
//				temp_MP |= Wifi_Rx_Buf[4];
//				Car_Spend = Wifi_Rx_Buf[3];
//				//set_Test_Times();
//				Control(Car_Spend ,Car_Spend);
//				break;
//			case 0x03:              //后退
//				Roadway_mp_syn();	//码盘同步
//				Stop_Flag = 0; Go_Flag = 0; wheel_L_Flag = 0;wheel_R_Flag = 0;wheel_Nav_Flag = 0;
//				Back_Flag = 1; Track_Flag = 0;
//				temp_MP = Wifi_Rx_Buf[5];
//				temp_MP <<=  8;
//				temp_MP |= Wifi_Rx_Buf[4];
//				Car_Spend = Wifi_Rx_Buf[3];
//				Control(-Car_Spend ,-Car_Spend);					
//				break;
//			case 0x04:              //左转
//				Stop_Flag = 0; Go_Flag = 0; wheel_L_Flag = 1;wheel_R_Flag = 0;wheel_Nav_Flag = 0;
//				Back_Flag = 0; Track_Flag = 0;
//				Car_Spend = Wifi_Rx_Buf[3];				
//				Control(-Car_Spend ,Car_Spend);
//				break;
//			case 0x05:              //右转
//				Stop_Flag = 0; Go_Flag = 0; wheel_L_Flag = 0;wheel_R_Flag = 1;wheel_Nav_Flag = 0;
//				Back_Flag = 0; Track_Flag = 0;
//				Car_Spend = Wifi_Rx_Buf[3];
//				Control(Car_Spend,-Car_Spend);
//				break;
//			case 0x06:              //循迹
//				Stop_Flag = 0; Go_Flag = 0; wheel_L_Flag = 0;wheel_R_Flag = 0;wheel_Nav_Flag = 0;
//				Back_Flag = 0; Track_Flag = 1;
//				Car_Spend = Wifi_Rx_Buf[3];
//				//set_Test_Times();
//				break;
//			case 0x07:				//码盘清零
//				
//				break;
//			case 0x08:				//左转弯--角度											
//				Roadway_nav_syn();	//角度同步
//				Roadway_mp_syn();	//码盘同步
//				Stop_Flag = 0; Go_Flag = 0; wheel_L_Flag = 0;wheel_R_Flag = 0;wheel_Nav_Flag = 1;
//				Back_Flag = 0; Track_Flag = 0;
//				temp_Nav = Wifi_Rx_Buf[5];
//				temp_Nav <<=  8;
//				temp_Nav |= Wifi_Rx_Buf[4];
//				Car_Spend = Wifi_Rx_Buf[3];				
//				Send_UpMotor(-Car_Spend ,Car_Spend);					 
//				break;
//			case 0x09:				//右转弯--角度			
//				Roadway_nav_syn();	//角度同步
//				Roadway_mp_syn();	//码盘同步
//				Stop_Flag = 0; Go_Flag = 0; wheel_L_Flag = 0;wheel_R_Flag = 0;wheel_Nav_Flag = 1;
//				Back_Flag = 0; Track_Flag = 0;
//				temp_Nav = Wifi_Rx_Buf[5];
//				temp_Nav <<=  8;
//				temp_Nav |= Wifi_Rx_Buf[4];
//				Car_Spend = Wifi_Rx_Buf[3];
//				Send_UpMotor(Car_Spend,-Car_Spend);
//				break;
			case 0x10:										//红外前三位数据
				Infrared_Tab[0]=Wifi_Rx_Buf[3];
				Infrared_Tab[1]=Wifi_Rx_Buf[4];
				Infrared_Tab[2]=Wifi_Rx_Buf[5];
				break;
			case 0x11:										//红外后三位数据
				Infrared_Tab[3]=Wifi_Rx_Buf[3];//数据第四位
				Infrared_Tab[4]=Wifi_Rx_Buf[4];//低位校验码
				Infrared_Tab[5]=Wifi_Rx_Buf[5];//高位校验码
				//AGV_Control_Infrared(Infrared_Tab);
				Full_Run_Flag=1;
				break;
			case 0x12:				//通知小车单片机发送红外线

				break;									
//			case 0x20:	//转向灯控制
//				Set_tba_WheelLED(L_LED,Wifi_Rx_Buf[3]);
//				Set_tba_WheelLED(R_LED,Wifi_Rx_Buf[4]);
//				break;					
//			case 0x30:
//				Set_tba_Beep(Wifi_Rx_Buf[3]);				//蜂鸣器
//				break;
			case 0x40:										//暂未使用
				
				break;
			case 0x50:  				//红外发射控制相片上翻 
				Infrared_Send(H_S,4);
				break;
			case 0x51:    					//红外发射控制相片下翻 
				Infrared_Send(H_X,4);
				break;
			case 0x61:    					//红外发射控制光源强度档位加1 
				Infrared_Send(H_1,4);				
				break;
			case 0x62:   	 				//红外发射控制光源强度档位加2 
				Infrared_Send(H_2,4);
				break;
			case 0x63:    					//红外发射控制光源强度档位加3 
				Infrared_Send(H_3,4);
				break;
		   case 0x80:						//运动标志物数据返回允许位
			    Host_AGV_Return_Flag = Wifi_Rx_Buf[3];   //SET 允许 / RESET 禁止
				break;
		   case 0x1A:						//全自动启动
				mark = 5;
			    Full_Run_Flag = 1;
				Cba_Beep(80,10,5);
			   break;
		   case 0x1B:						//得到图形图像识别结果
			   Shape_Number[0] = (Wifi_Rx_Buf[3]>>4 & 0x0F);	//三角形
			   Shape_Number[1] = (Wifi_Rx_Buf[3] & 0x0F);		//圆形
			   Shape_Number[2] = (Wifi_Rx_Buf[4]>>4 & 0x0F);	//矩形
			   Shape_Number[3] = (Wifi_Rx_Buf[4] & 0x0F);		//五角星  
			   Shape_Number[4] = (Wifi_Rx_Buf[5]>>4 & 0x0F);	//菱形
			   Block_Number =  (Wifi_Rx_Buf[5]>>4 & 0x0F);	//得到RFID数据块编号
			   Cba_Beep(80,10,5);
			   break;
		   case 0x1C:						//颜色识别结果
			   Section_Number = (Wifi_Rx_Buf[3]>>4 & 0x0F);	//得到扇区编号
			   Color_Number[0] = (Wifi_Rx_Buf[3]>>4 & 0x0F);	//红色
		       Color_Number[1] = (Wifi_Rx_Buf[3] & 0x0F);		//绿色
			   Color_Number[2] = (Wifi_Rx_Buf[4]>>4 & 0x0F);	//蓝色
			   Color_Number[3] = (Wifi_Rx_Buf[4] & 0x0F);		//黄色
			   Color_Number[4] = (Wifi_Rx_Buf[5]>>4 & 0x0F);	//紫色
			   Color_Number[5] = (Wifi_Rx_Buf[5] & 0x0F);		//青色
			   Cba_Beep(80,10,5);
			   break;
			 case 0x02:
				   Color_Number[0] = Wifi_Rx_Buf[3];
//			     Color_Number[0]=Color_Number[0]+0X30;
			  Send_InfoData_To_Fifo(&Color_Number[0],1);
			 
				  break;
		   case 0x1D:
//			 Color_Number[6] = (Wifi_Rx_Buf[3]>>4 & 0x0F);	//黑色
			   Cba_Beep(80,10,5);
			   Full_Run_Flag = 1;
			   break;
		   case 0x1E:						//车牌识别结果
			//   memcpy(Plate_Number,Infrared_Tab,6);			//保存车牌结果
			   Cba_Beep(80,10,5);
			   Full_Run_Flag = 1;
			   break;
//		   case 0x2B:											//交通灯识别完成
//			   Cba_Beep(80,10,5);
//			   Full_Run_Flag = 1;
			   break;
			 case 0x19:
              Full_Identification_result_comparison(Wifi_Rx_Buf[3]);				 //交通灯A识别比对
		    //(Wifi_Rx_Buf[3]);
//			   Cba_Beep(80,10,5);
			  delay_ms(300);
			  delay_ms(300);
			   Full_Run_Flag = 1;
		     	break;
			 case 0x3B:	         //二维码结果
//				 Infrared_Tab[0]=Wifi_Rx_Buf[3];   //红外前三位数据
//				 Infrared_Tab[1]=Wifi_Rx_Buf[4];
//				 Infrared_Tab[2]=Wifi_Rx_Buf[5];
			    break;
			 case 0x3D:					 //二维码结果    //红外后三位数据
//				Infrared_Tab[3]=Wifi_Rx_Buf[3];//数据第四位
//				Infrared_Tab[4]=Wifi_Rx_Buf[4];//低位校验码
//				Infrared_Tab[5]=Wifi_Rx_Buf[5];//高位校验码
//			  Send_Debug_Info(Infrared_Tab,6);
			  Cba_Beep(80,10,5);
			  delay_ms(800);
				Full_Run_Flag = 1;
				break;
			 case 0x4B:						//二维码结果
//				memcpy(Qr_Code,Infrared_Tab,6);					//保存二维码结果
				Cba_Beep(80,10,5);
				Full_Run_Flag = 1;
				break;
			 case 0x29:											              //交通灯B识别比对
		    Full_Bdentification_result_comparison(Wifi_Rx_Buf[3]);
//			   Cba_Beep(80,10,5);
				 delay_ms(300);
				 delay_ms(300);
			   Full_Run_Flag = 1;
			   break;
		   default:
			   break;
			}
		}
	else if(Wifi_Rx_Buf[1]==0x0B||Wifi_Rx_Buf[1]==0x08)		    /*******识别车牌信息******/
{
	switch(Wifi_Rx_Buf[2])
	{
		case 0x10:
			Send_ZigbeeData_To_Fifo(Wifi_Rx_Buf,8); 
		break;
		
		case 0x20:
			CP_data[0]=Wifi_Rx_Buf[3];
			CP_data[1]=Wifi_Rx_Buf[4];
	  	    CP_data[2]=Wifi_Rx_Buf[5];
		    Send_ZigbeeData_To_Fifo(Wifi_Rx_Buf,8); 
		break;
		
		case 0x21:
			CP_data[3]=Wifi_Rx_Buf[3];
			CP_data[4]=Wifi_Rx_Buf[4];
	  	    CP_data[5]=Wifi_Rx_Buf[5]; 
		    Send_ZigbeeData_To_Fifo(Wifi_Rx_Buf,8); 
		    Full_Run_Flag=1;
		break;
		
		case 0x40:
			Send_ZigbeeData_To_Fifo(Wifi_Rx_Buf,8); 
			Full_Run_Flag=1;
		break;
		
	}
}
     else if(Wifi_Rx_Buf[1]==0x04 &&Wifi_Rx_Buf[2]==0x02)       /********识别图形********/
	 {
	      Send_ZigbeeData_To_Fifo(Wifi_Rx_Buf,8); 
	      delay_ms(500);
		  Full_Run_Flag=1;		
 	 }
	 else if(Wifi_Rx_Buf[1]==0x0E||Wifi_Rx_Buf[1]==0x0F)     /********识别交通灯*******/
	 {
	        if(Wifi_Rx_Buf[2]==0x01)
			{
			   Send_ZigbeeData_To_Fifo(Wifi_Rx_Buf,8); 
			}
			else if(Wifi_Rx_Buf[2]==0x02)
			{
			  Color=Wifi_Rx_Buf[3];
			  Send_ZigbeeData_To_Fifo(Wifi_Rx_Buf,8);
				delay_ms(500);
				delay_ms(500);
				delay_ms(500);
				delay_ms(500);
			  Full_Run_Flag = 1;
			}
	 }
		else  
		{
			Send_ZigbeeData_To_Fifo(Wifi_Rx_Buf,8); 
		}
		Rx_Flag = 0;
	}
}




/**
函数功能：保存ZigBee数据
参    数: 无
返 回 值：无
*/
void Can_ZigBeeRx_Save(uint8_t res)
{
/*	if(Zigbee_Rx_flag == 0)
	{
		Zigb_Rx_Buf[Zigbee_Rx_num]=res;
		Zigbee_Rx_num++;
		if(Zigbee_Rx_num > ZIGB_RX_MAX )	
		{
			Zigbee_Rx_num = 0;
			Zigbee_Rx_flag = 1;
		} 
	} */

	if(Zigbee_Rx_flag == 0)
	{
		canu_zibe_rxtime = gt_get()+10;
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
			if((Zigb_Rx_Buf[0] == 0x55)&&(Zigb_Rx_Buf[1] == 0x02))		//返回从车数据
			{		
			if(Zigb_Rx_Buf[2]==0xA1)
			{
				Full_Run_Flag = 1;
			}							
			else if(Zigb_Rx_Buf[2]==0x92&&Zigb_Rx_Buf[3]==0x01)   /*****主车接受从车扫码数据****/
			{ 
					
					DZCP_data[0]=Zigb_Rx_Buf[5];
					DZCP_data[1]=Zigb_Rx_Buf[6];
					DZCP_data[2]=Zigb_Rx_Buf[7];
					DZCP_data[3]=Zigb_Rx_Buf[8];
					DZCP_data[4]=Zigb_Rx_Buf[9];
					DZCP_data[5]=Zigb_Rx_Buf[10];
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
	CanP_CanTx_Check();				//CAN总线发送数据监测
}



//end file

