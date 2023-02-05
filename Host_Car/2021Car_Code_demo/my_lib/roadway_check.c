#include "stm32f4xx.h"
#include "CanP_Hostcom.h"
#include "delay.h"
#include "roadway_check.h"
#include "cba.h"
#include "Timer.h"
#include "DEBUG.h"
#include "Fun_motor.h"
#include "rc522.h"

#include "data_base.h"
#include "A72.h"

#include <stdio.h>

volatile uint8_t Find_Line_Track_Flag=0;
volatile uint8_t Track_Go_Flag=0;
volatile uint8_t Track_Back_Flag=0;
volatile uint8_t Track_Back_Mp_Flag=0;
volatile uint8_t Track_Go_Mp_Flag=0;
volatile uint8_t Track_Card_Flag=0;
volatile uint8_t wheel_L_Flag =0;
volatile uint8_t wheel_R_Flag = 0;
volatile uint8_t wheel_Nav_Flag = 0;
volatile uint8_t Go_Flag = 0;
volatile uint8_t Back_Flag = 0;
volatile uint8_t Track_Flag = 0;
volatile uint8_t MP_Track_flag=0;
volatile uint8_t Stop_Flag = 0;
volatile int LSpeed = 0;
volatile int RSpeed = 0;
volatile int Car_Spend = 0;

volatile uint16_t temp_MP = 0;
volatile uint16_t temp_Nav = 0;

volatile uint8_t Line_Flag = 0;
volatile uint16_t count = 0;
volatile uint32_t Wheel_flag = 0;



void Track_Correct(uint8_t gd);
void Back_Track(uint8_t gd);

//_________________________________________________________
int16_t Roadway_cmp;
extern int16_t CanHost_Mp;

uint8_t CT[2];

/*
	码盘同步
**/
void Roadway_mp_syn(void)
{
	Roadway_cmp = CanHost_Mp;
}

/*
	码盘获取
**/
uint16_t Roadway_mp_Get(void)
{
	uint32_t ct;
	if(CanHost_Mp > Roadway_cmp)
		ct = CanHost_Mp - Roadway_cmp;
	else
		ct = Roadway_cmp - CanHost_Mp;
	if(ct > 0x8000)
		ct = 0xffff - ct;
	return ct;	
}
//________________________________________________________
uint16_t Roadway_Navig;
extern uint16_t CanHost_Navig;
/*
	角度同步
**/
void Roadway_nav_syn(void)
{
	Roadway_Navig = CanHost_Navig;
}

/*
	获取角度差值
**/
uint16_t Roadway_nav_Get(void)
{
	uint16_t ct;
	if(CanHost_Navig > Roadway_Navig)
		ct = CanHost_Navig - Roadway_Navig;
	else
		ct = Roadway_Navig - CanHost_Navig;
	while(ct >= 36000)
		ct -= 36000;
	return ct;
}
//清除标志位
void Roadway_Flag_clean(void)
{
	Track_Back_Mp_Flag=0;
	Track_Go_Mp_Flag=0;
	MP_Track_flag=0;
	Stop_Flag = 0;
	Back_Flag = 0;
	Go_Flag = 0; 
	wheel_L_Flag = 0;
	wheel_R_Flag = 0;
	wheel_Nav_Flag = 0;
	Track_Flag = 0;
	Track_Back_Flag=0;
	Track_Go_Flag=0;
	Car_Spend = 0;
	temp_MP = 0;
}

uint8_t Roadway_GoBack_Check(void)
{
	return ((Go_Flag == 0)&&(Back_Flag == 0)&&(Track_Flag == 0)&&(wheel_L_Flag == 0)&&(wheel_R_Flag == 0))? 1:0;
}
/**
	前进监测
*/
void Go_and_Back_Check(void)
{	
	if(Go_Flag == 1)
	{
		if(temp_MP <= Roadway_mp_Get())
		{
			Go_Flag = 0;
			Stop_Flag = 0x03;
			Send_UpMotor(0,0);
			return;
		}
	} 
	else if(Back_Flag == 1)
	{
		if(temp_MP <= Roadway_mp_Get())
		{
			Back_Flag = 0;
			Stop_Flag=0x03;
			Send_UpMotor(0,0);
			return;
		}
	}
}

/**码盘转弯*/
void wheel_Nav_check(void)
{ 	
	uint16_t Mp_Value = 0;
	
	if(wheel_Nav_Flag)
	{
		Mp_Value = Roadway_mp_Get(); 
		if(Mp_Value >= temp_Nav)
		{
			wheel_Nav_Flag = 0;
			Stop_Flag = 0x02;
			Send_UpMotor(0,0);
			return;
		}
	}
}

/**根据循迹线转弯*/
uint32_t Mp_Value = 0;
void wheel_Track_check(void)
{ 	
	uint16_t Track_Value = 0;
	
	if(wheel_L_Flag == 1)
	{
		Track_Value = Get_Host_UpTrack(TRACK_H8);
		if(!(Track_Value &0X08)) //找到循迹线，停止
		{	
			if(Wheel_flag > 50)
			{
				wheel_L_Flag = 0;
				Wheel_flag=0;
				Stop_Flag=0x02;
				Send_UpMotor(0,0);
				return;
			}
		}
		else if(Track_Value == 0Xff)
		{			
			Wheel_flag++;
		}
	} 
	else if(wheel_R_Flag == 1)
	{
		Track_Value = Get_Host_UpTrack(TRACK_H8);
		 if(!(Track_Value &0X10)) //找到循迹线，停止
			{	
				if(Wheel_flag > 50)
				{
					wheel_R_Flag=0;
					Wheel_flag=0;
					Stop_Flag=0x02;
					Send_UpMotor(0,0);
					return;
				}
			}
			else if( Track_Value == 0Xff) 
			{				
				Wheel_flag++;
			}
	}
}

/***************************************************************
** 功能：    官方循迹函数，修线不稳一般不使用
** 参数：	 gd 寻迹板值
** 返回值：   无
****************************************************************/
void Track_Correct_official(uint8_t gd)
{
	if(Get_Host_UpTrack(TRACK_Q7) == 0x00)	//循迹灯全灭 停止
	{	
		Track_Flag = 0;
		Stop_Flag = 1;
		Track_Back_Flag=0;
		Track_Go_Flag=0;
		Send_UpMotor(0,0);
		return;
	}
	else
	{
	  Stop_Flag=0;
		if(gd==0XE7||gd==0XF7||gd==0XEF)//1、中间3/4传感器检测到黑线，全速运行
		{ 
			LSpeed=Car_Spend;
			RSpeed=Car_Spend;
		}
		if(Line_Flag!=2)
		{		

			if(gd==0XF3||gd==0XFB) //2、中间4、3传感器检测到黑线，微右拐	
			{ 
				LSpeed=Car_Spend+30;
				RSpeed=Car_Spend-30;
				Line_Flag=0;
			}
			else if(gd==0XF9||gd==0XFD)//3、中间3、2传感器检测到黑线，再微右拐		
			{ 
				 LSpeed=Car_Spend+40;
				 RSpeed=Car_Spend-60;				 
				 Line_Flag=0;
			}
			else if(gd==0XFC)//4、中间2、1传感器检测到黑线，强右拐
			{ 
				LSpeed = Car_Spend+50;
				RSpeed = Car_Spend-90;
				Line_Flag=0;
			}
			else if(gd==0XFE)//5、最右边1传感器检测到黑线，再强右拐			
			{ 
				 LSpeed = Car_Spend+60;
				 RSpeed = Car_Spend-120;			
				 Line_Flag=1;
			}
		}
		if(Line_Flag!=1)
		{
			if(gd==0XCF)//6、中间6、5传感器检测到黑线，微左拐			
			{ 
				 RSpeed = Car_Spend+30;
				 LSpeed = Car_Spend-30;
				 Line_Flag=0;
			} 
			else if(gd==0X9F||gd==0XDF)//7、中间7、6传感器检测到黑线，再微左拐		 
			{ 
				 RSpeed = Car_Spend+40;
				 LSpeed = Car_Spend-60;
				 Line_Flag=0;
			} 			  
			else if(gd==0X3F||gd==0XBF)//8、中间8、7传感器检测到黑线，强左拐		 
			{ 
				 RSpeed = Car_Spend+50;
				 LSpeed = Car_Spend-90;
				 Line_Flag=0;
			} 
			else if(gd==0X7F)//9、最左8传感器检测到黑线，再强左拐		 	
			{ 
				 RSpeed = Car_Spend+60;
				 LSpeed = Car_Spend-120;
				 Line_Flag=2;
			}			
		}
		if(Get_Host_UpTrack(TRACK_ALL)==0xFF7F)   //循迹灯全亮
		{
			if(count > Track_MaxError)
			{
				count=0;
				Send_UpMotor(0,0);
				Track_Flag=0;
				Track_Back_Flag=0;
				Track_Go_Flag=0;
				if(Line_Flag ==0) 
				Stop_Flag=0x02;
				return;
			}	
			else 
				count++;				
		}
		else 
			count=0;
	}
	if(Track_Flag != 0)
	{
			Control(LSpeed,RSpeed);
	}
}

/*寻迹控制——前进**/
void Track_Control_Go(uint8_t gd)
{
	uint16_t All_gd=Get_Host_UpTrack(TRACK_ALL);
	if(Get_Host_UpTrack(TRACK_Q7) == 0x00)	//循迹灯全灭 停止
	{	
		Track_Flag = 0;
		Track_Go_Flag=0;
		Stop_Flag = 0x01;
		Send_UpMotor(0,0);
		return;
	}
	else
	{
	  if(gd==0XE7) 
		{ 		
			LSpeed=Car_Spend;
			RSpeed=Car_Spend;	
		}
		else	if(gd==0XF7)		
		{
			LSpeed=Car_Spend+30;
			RSpeed=Car_Spend-40;	
		}
		else	if(gd==0XEF)
		{
			LSpeed=Car_Spend-40;
			RSpeed=Car_Spend+30;	
		}	
		else	if(gd==0XF3||gd==0XFB)
		{ 
			LSpeed=Car_Spend+30;
			RSpeed=Car_Spend-40;	
		}
		else if(gd==0XF9||gd==0XFD)
		{ 
			 LSpeed=Car_Spend+60;
			 RSpeed=Car_Spend-70;				 
		}
		else if(gd==0XFC)
		{ 
			LSpeed = Car_Spend+60;
			RSpeed = Car_Spend-90;	
		}
		else if(gd==0XFE)		
		{ 
			 LSpeed = Car_Spend+80;
			 RSpeed = Car_Spend-120;					
		}
		else if(gd==0XCF)
		{ 
			 RSpeed = Car_Spend+30;
			 LSpeed = Car_Spend-40;	
		} 
		else if(gd==0X9F||gd==0XDF) 
		{ 
			 RSpeed = Car_Spend+60;
			 LSpeed = Car_Spend-70; 
		} 			  
		else if(gd==0X3F||gd==0XBF)
		{ 
			 RSpeed = Car_Spend+70;
			 LSpeed = Car_Spend-90;	 
		} 
		else if(gd==0X7F) 	
		{ 
			 RSpeed = Car_Spend+80;
			 LSpeed = Car_Spend-120;	 
		}	
		else if(All_gd==0xFF7F)
		{
			if(count >= Track_MaxError)
			{
				count=0;
				Track_Go_Flag=0;
				Send_UpMotor(0,0);
				Track_Flag=0;
				Stop_Flag=0x04;
				return;
			}	
			else 
			{
				count++;			
			}		
		}
		else 
		{
			count=0;
		}	
	}
	if(Track_Flag != 0)
	{
		Control(LSpeed,RSpeed);
	}
}
/***寻迹控制——后退**/
void Track_Control_Back(uint8_t gd)
{
	uint16_t All_gd=Get_Host_UpTrack(TRACK_ALL);
	if(Get_Host_UpTrack(TRACK_Q7) == 0x00)	//循迹灯全灭 停止
	{	
		Track_Flag = 0;
		Track_Back_Flag=0;
		Stop_Flag = 0x01;
		Send_UpMotor(0,0);
		return;
	}
	else
	{
	  if(gd==0XE7) 
		{ 		
			LSpeed=Car_Spend;
			RSpeed=Car_Spend;	
		}
		else	if(gd==0XF7)		
		{
			LSpeed=Car_Spend+30;
			RSpeed=Car_Spend-40;	
		}
		else	if(gd==0XEF)
		{
			LSpeed=Car_Spend-40;
			RSpeed=Car_Spend+30;	
		}	
		else	if(gd==0XF3||gd==0XFB)
		{ 
			LSpeed=Car_Spend+30;
			RSpeed=Car_Spend-40;	
		}
		else if(gd==0XF9||gd==0XFD)
		{ 
			 LSpeed=Car_Spend+60;
			 RSpeed=Car_Spend-70;				 
		}
		else if(gd==0XFC)
		{ 
			LSpeed = Car_Spend+60;
			RSpeed = Car_Spend-90;	
		}
		else if(gd==0XFE)		
		{ 
			 LSpeed = Car_Spend+80;
			 RSpeed = Car_Spend-120;					
		}
		else	if(gd==0XCF)
		{ 
			 RSpeed = Car_Spend+30;
			 LSpeed = Car_Spend-40;	
		} 
		else if(gd==0X9F||gd==0XDF) 
		{ 
			 RSpeed = Car_Spend+60;
			 LSpeed = Car_Spend-70; 
		} 			  
		else if(gd==0X3F||gd==0XBF)
		{ 
			 RSpeed = Car_Spend+70;
			 LSpeed = Car_Spend-90;	 
		} 
		else if(gd==0X7F) 	
		{ 
			 RSpeed = Car_Spend+80;
			 LSpeed = Car_Spend-120;	 
		}	
		else if(All_gd==0xFF7F)
		{
			if(count >= Track_MaxError)
			{
				count=0;
				Track_Back_Flag=0;
				Send_UpMotor(0,0);
				Track_Flag=0;
				Stop_Flag=0x04;
				return;
			}	
			else 
			{
				count++;			
			}		
		}
		else 
		{
			count=0;
		}	
	}
	if(Track_Flag != 0)
	{
		Control(LSpeed,RSpeed);
	}
}

/**前进指定码盘寻迹**/
void Track_Control_Go_Mp(uint8_t gd)
{
	if(Get_Host_UpTrack(TRACK_Q7) == 0x00)	//循迹灯全灭 停止
	{	
		MP_Track_flag = 0;
		Stop_Flag = 0x01;
		Track_Go_Mp_Flag=0;
		Send_UpMotor(0,0);
		return;
	}
	else
	{
	  if(gd==0XE7)   
		{ 		
			LSpeed=Car_Spend;
			RSpeed=Car_Spend;	
		}
		else	if(gd==0XF7)		
		{
			LSpeed=Car_Spend+30;
			RSpeed=Car_Spend-40;	
		}
		else	if(gd==0XEF)
		{
			LSpeed=Car_Spend-40;
			RSpeed=Car_Spend+30;	
		}	
		else	if(gd==0XF3||gd==0XFB) 	
		{ 
			LSpeed=Car_Spend+30;
			RSpeed=Car_Spend-40;	
		}
		else if(gd==0XF9||gd==0XFD)	
		{ 
			 LSpeed=Car_Spend+60;
			 RSpeed=Car_Spend-70;				 
		}
		else if(gd==0XFC)
		{ 
			LSpeed = Car_Spend+60;
			RSpeed = Car_Spend-90;	
		}
		else if(gd==0XFE)		
		{ 
			 LSpeed = Car_Spend+80;
			 RSpeed = Car_Spend-120;					
		}
		else	if(gd==0XCF)	
		{ 
			 RSpeed = Car_Spend+30;
			 LSpeed = Car_Spend-40;	
		} 
		else if(gd==0X9F||gd==0XDF)		 
		{ 
			 RSpeed = Car_Spend+60;
			 LSpeed = Car_Spend-70; 
		} 			  
		else if(gd==0X3F||gd==0XBF)		 
		{ 
			 RSpeed = Car_Spend+70;
			 LSpeed = Car_Spend-90;	 
		} 
		else if(gd==0X7F) 	
		{ 
			 RSpeed = Car_Spend+80;
			 LSpeed = Car_Spend-120;	 
		}		
		else if(gd==0xFF)   //循迹灯全亮
		{
			if(count > 2000)
			{
				count=0;
				MP_Track_flag=0;
				Stop_Flag=0x04;
				Track_Go_Mp_Flag=0;
				Send_UpMotor(0,0);
				return;
			}	
			else 
				count++;				
		}
		else 
			count=0;
	}
	if(temp_MP <= Roadway_mp_Get())
	{
		Stop_Flag = 0x03;
		Track_Go_Mp_Flag=0;
		MP_Track_flag = 0;
		Send_UpMotor(0,0);
		return;
	}
	if(MP_Track_flag!=0)
	{
		Control(LSpeed,RSpeed);
	}
}

/***后退指定码盘寻迹****/
void Track_Control_Back_Mp(uint8_t gd)
{
	if(gd == 0x00)	//循迹灯全灭 停止
	{	
		Stop_Flag = 0x01;
		Track_Back_Mp_Flag=0;
		MP_Track_flag = 0;
		Send_UpMotor(0,0);
		return;
	}
	else
	{
	  if(gd==0XE7)   
		{ 		
			LSpeed=Car_Spend;
			RSpeed=Car_Spend;	
		}
		else	if(gd==0XF7)		
		{
			LSpeed=Car_Spend+30;
			RSpeed=Car_Spend-40;
		}
		else	if(gd==0XEF)
		{
			LSpeed=Car_Spend-40;
			RSpeed=Car_Spend-30;
		}	
		else	if(gd==0XF3||gd==0XFB) 	
		{ 
			LSpeed=Car_Spend+30;
			RSpeed=Car_Spend-40;	
		}
		else if(gd==0XF9||gd==0XFD)	
		{ 
			 LSpeed=Car_Spend+60;
			 RSpeed=Car_Spend-70;				 
		}
		else if(gd==0XFC)
		{ 
			LSpeed = Car_Spend+60;
			RSpeed = Car_Spend-90;	
		}
		else if(gd==0XFE)		
		{ 
			 LSpeed = Car_Spend+80;
			 RSpeed = Car_Spend-120;					
		}
		else	if(gd==0XCF)	
		{ 
			 LSpeed = Car_Spend+30;
			 RSpeed = Car_Spend-40;	
		} 
		else if(gd==0X9F||gd==0XDF)		 
		{ 
			 LSpeed = Car_Spend+60;
			 RSpeed = Car_Spend-70; 
		} 			  
		else if(gd==0X3F||gd==0XBF)		 
		{ 
			 LSpeed = Car_Spend+70;
			 RSpeed = Car_Spend-90;	 
		} 
		else if(gd==0X7F) 	
		{ 
			 LSpeed = Car_Spend+80;
			 RSpeed = Car_Spend-120;	 
		}		
		else if(gd==0xFF)   //循迹灯全亮
		{
			if(count > Track_MaxError)
			{
				count=0;
				Track_Flag=0;
				Track_Back_Mp_Flag=0;
				Stop_Flag=0x04;
				Send_UpMotor(0,0);
				return;
			}	
			else 
				count++;				
		}
		else 
			count=0;
	}
	if(temp_MP <= Roadway_mp_Get())
	{
		Stop_Flag = 0x03;
		Track_Go_Mp_Flag=0;
		Track_Back_Mp_Flag=0;
		MP_Track_flag = 0;
		Send_UpMotor(0,0);
		return;
	} 
	if(MP_Track_flag!=0)
	{
			Control(LSpeed,RSpeed);
	}
}

void Find_Line_Track(uint8_t gd)
{
	  if(gd==0XE7) 
		{ 		
			Find_Line_Track_Flag=0;
			Send_UpMotor(0, 0);
			return;
		}
		else	if(gd==0XF7)		
		{
			Find_Line_Track_Flag=0;
			Send_UpMotor(0, 0);
			return;
		}
		else	if(gd==0XEF)
		{
			Find_Line_Track_Flag=0;
			Send_UpMotor(0, 0);
			return;
		}	
		else	if(gd==0XF3||gd==0XFB)
		{ 
			Find_Line_Track_Flag=0;
			Send_UpMotor(0, 0);
			return;
		}
		else if(gd==0XF9||gd==0XFD)
		{ 
			Find_Line_Track_Flag=0;
			Send_UpMotor(0, 0);
			return;
		}
		else if(gd==0XCF)
		{ 
			Find_Line_Track_Flag=0;
			Send_UpMotor(0, 0);
			return;
		} 
		else if(gd==0X9F||gd==0XDF) 
		{ 
			Find_Line_Track_Flag=0;
			Send_UpMotor(0, 0);
			return;
		} 			  

	if(Find_Line_Track_Flag != 0)
	{
		Control(Car_Spend,Car_Spend);
	}
}



void Card_Track(uint8_t gd)
{
	if(gd==0XE7)   
	{ 		
		LSpeed=Car_Spend;
		RSpeed=Car_Spend;	
	}
	else	if(gd==0XF7)		
	{
		LSpeed=Car_Spend+30;
		RSpeed=Car_Spend-40;	
	}
	else	if(gd==0XEF)
	{
		LSpeed=Car_Spend-40;
		RSpeed=Car_Spend+30;	
	}	
	else	if(gd==0XF3||gd==0XFB) 	
	{ 
		LSpeed=Car_Spend+30;
		RSpeed=Car_Spend-40;	
	}
	else if(gd==0XF9||gd==0XFD)	
	{ 
		 LSpeed=Car_Spend+60;
		 RSpeed=Car_Spend-70;				 
	}
	else if(gd==0XFC)
	{ 
		LSpeed = Car_Spend+60;
		RSpeed = Car_Spend-90;	
	}
	else if(gd==0XFE)		
	{ 
		 LSpeed = Car_Spend+80;
		 RSpeed = Car_Spend-120;					
	}
	else	if(gd==0XCF)	
	{ 
		 RSpeed = Car_Spend+30;
		 LSpeed = Car_Spend-40;	
	} 
	else if(gd==0X9F||gd==0XDF)		 
	{ 
		 RSpeed = Car_Spend+60;
		 LSpeed = Car_Spend-70; 
	} 			  
	else if(gd==0X3F||gd==0XBF)		 
	{ 
		 RSpeed = Car_Spend+70;
		 LSpeed = Car_Spend-90;	 
	} 
	else if(gd==0X7F) 	
	{ 
		 RSpeed = Car_Spend+80;
		 LSpeed = Car_Spend-120;	 
	}		
	if(temp_MP <= Roadway_mp_Get())
	{
		Stop_Flag = 0x03;
		Track_Go_Mp_Flag=0;
		MP_Track_flag = 0;
		Send_UpMotor(0,0);
		return;
	}
	if(MP_Track_flag!=0)
	{
		Control(LSpeed,RSpeed);
	}
}

void Stop_Flag_Cheack()
{
	delay_ms(1);
	if(Stop_Flag==0x04)
	{
		RFID_Cheack();
	}
}


void RFID_Cheack()
{
	uint16_t Track_Value[2];
	uint8_t res=0;
	Track_Value[0] = Get_Host_UpTrack(TRACK_ALL);
	uint16_t cont=BitCount(Track_Value[0]);
	delay_ms(5);
	Fun_Go_Back(40,100);
	if(Get_Host_UpTrack(TRACK_H8)==0xFF)
	{
		res=Read_Card(0x42,RFID_Card);
	}
	else
	{
			Fun_Go_Back(60,1800);
	}
	
	Fun_Track_Go();
		
}



//void RFID_Cheack(uint8_t *Flag)
//{
//	int8_t status=0;
//	uint16_t Track_Value[2];
//	delay_ms(5);
//	Track_Value[1] = Get_Host_UpTrack(TRACK_ALL);
//	uint16_t cont=BitCount(Track_Value[1]);
//	Fun_Go_Back(60,70);
//	delay_ms(5);
//	Track_Value[0] = Get_Host_UpTrack(TRACK_Q7);
//	delay_ms(5);
//	if((Track_Value[0]&0x1C)==0x1C)
//	{
//		Fun_Go_Back(60,300);
//		Read_Card(0x01,RFID_Card);
//		delay_ms(5);
//		if(cont<=2)
//		{
//			Fun_Track(60);
//			delay_ms(5);
//			Fun_Go_Back(60,410);
//			return;
//		}
//		else
//		{
//			return;
//		}
//	}
//	else
//	{
//		Find_Line();
//		delay_ms(5);
//		Fun_Track(60);
//		delay_ms(5);
//		Fun_Go_Back(60,410);
//		return;
//	}
//}

/**循迹监测*/
void Track_Check(void)
{	
	if(Track_Flag == 1)
	{
		if(Track_Go_Flag==1)
		{
#if official 
			Track_Correct_official(Get_Host_UpTrack(TRACK_H8));
#else
			Track_Control_Go(Get_Host_UpTrack(TRACK_H8));
#endif
		}
		else if(Track_Back_Flag==1)
		{
#if official
			Track_Correct_official(reverse_bit(Get_Host_UpTrack(TRACK_H8)));
#else
			Track_Control_Back(reverse_bit(Get_Host_UpTrack(TRACK_H8)));
#endif
		}
	}
	else if(MP_Track_flag==1)
	{
		if(Track_Go_Mp_Flag==1)
		{
			Track_Control_Go_Mp(Get_Host_UpTrack(TRACK_H8));
		}
		else if(Track_Back_Mp_Flag==1)
		{
			Track_Control_Back_Mp(reverse_bit(Get_Host_UpTrack(TRACK_H8)));
		}
	}
	else if(Find_Line_Track_Flag==1)
	{
		Find_Line_Track(Get_Host_UpTrack(TRACK_H8));
	}
	else if(Track_Card_Flag==1)
	{
		Card_Track(~(Get_Host_UpTrack(TRACK_H8)));
	}
	
}

void Roadway_Check(void)
{
	Go_and_Back_Check();
	wheel_Track_check();
	wheel_Nav_check();
	Track_Check();
}
	
/***************************************************************
** 功能：     电机控制函数
** 参数：	  L_Spend：电机左轮速度
**            R_Spend：电机右轮速度
** 返回值：   无	  
****************************************************************/
void Control(int L_Spend,int R_Spend)
{
	if(L_Spend>=0)
	{	
		if(L_Spend>100)L_Spend=100;if(L_Spend<5)L_Spend=5;		//限制速度参数
	}
	else 
	{
		if(L_Spend<-100)L_Spend= -100;if(L_Spend>-5)L_Spend= -5;     //限制速度参数
	}	
	if(R_Spend>=0)
	{	
		if(R_Spend>100)R_Spend=100;if(R_Spend<5)R_Spend=5;		//限制速度参数
	}
	else
	{	
		if(R_Spend<-100)R_Spend= -100;if(R_Spend>-5)R_Spend= -5;		//限制速度参数		
	}
	Send_UpMotor(L_Spend ,R_Spend);	
}


void roadway_check_TimInit(uint16_t arr,uint16_t psc)
{
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);
	
	TIM_InitStructure.TIM_Period = arr;
	TIM_InitStructure.TIM_Prescaler = psc;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM9,&TIM_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ITConfig(TIM9,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM9, ENABLE);
}

void TIM1_BRK_TIM9_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM9,TIM_IT_Update) == SET)
	{
		Roadway_Check();				//路况检测
	}
	TIM_ClearITPendingBit(TIM9,TIM_IT_Update);
}



//end file


