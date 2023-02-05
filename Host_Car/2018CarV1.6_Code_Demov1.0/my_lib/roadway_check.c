#include "stm32f4xx.h"
#include "CanP_Hostcom.h"
#include "delay.h"
#include "roadway_check.h"
#include "cba.h"
#include "Timer.h"
#include "Full.h"
#include "can_user.h"


uint8_t wheel_L_Flag =0,wheel_R_Flag = 0;
uint8_t wheel_Nav_Flag = 0;
uint8_t Go_Flag = 0,Back_Flag = 0;
uint8_t Track_Flag = 0;
uint8_t Back_Track_Flag = 0;				//����ѭ����־λ
uint8_t Terrain_Flag = 0;					//����α�־��ѭ����־λ
uint8_t Mp_Track_Flag = 0;					//ָ������ѭ��

uint8_t Line_Flag = 0;
uint16_t count = 0;

uint8_t Stop_Flag = 0;
int LSpeed = 0,RSpeed = 0;
int Car_Spend = 0;
uint16_t temp_MP = 0;
uint16_t temp_Nav = 0;

uint32_t Wheel_flag = 0;

void Track_Correct(uint8_t gd);
void Back_Track(uint8_t gd);
void Track_Ccorrect(uint8_t gd);   //ѭ������
//_________________________________________________________
int16_t Roadway_cmp;
extern int16_t CanHost_Mp;

/*
	����ͬ��
**/
void Roadway_mp_syn(void)
{
	Roadway_cmp = CanHost_Mp;
}

/*
	���̻�ȡ
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

//_______________________________________________________________



//_________________________________________________________
uint16_t Roadway_Navig;
extern uint16_t CanHost_Navig;

/*
	�Ƕ�ͬ��
**/
void Roadway_nav_syn(void)
{
	Roadway_Navig = CanHost_Navig;
}

/*
	��ȡ�ǶȲ�ֵ
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

//_______________________________________________________________

void Roadway_Flag_clean(void)
{
	wheel_L_Flag =0;wheel_R_Flag = 0;
	Go_Flag = 0;Back_Flag = 0;
	Track_Flag = 0;
	Stop_Flag = 0;
	temp_MP = 0;
}

/**
	ǰ�����
*/
void Go_and_Back_Check(void)
{	
	if(Go_Flag == 1)
	{
		if(temp_MP <= Roadway_mp_Get())
		{
			Go_Flag = 0;
			Stop_Flag = 3;
			Send_UpMotor(0,0);
		}
	} 
	else if(Back_Flag == 1)
	{
		if(temp_MP <= Roadway_mp_Get())
		{
			Back_Flag = 0;
			Stop_Flag = 3;
			Send_UpMotor(0,0);
		}
	}
}

uint8_t Roadway_GoBack_Check(void)
{
	return ((Go_Flag == 0)&&(Back_Flag == 0)&&(Track_Flag == 0)&&(wheel_L_Flag == 0)&&(wheel_R_Flag == 0))? 1:0;
}

/**
	����ת��
*/
void wheel_Nav_check(void)
{ 	
	uint16_t Mp_Value = 0;
	
	if(wheel_Nav_Flag)
	{
		Mp_Value = Roadway_mp_Get(); 
		if(Mp_Value >= temp_Nav)
		{
			wheel_Nav_Flag = 0;
			Stop_Flag = 2;
			Send_UpMotor(0,0);
		}
	}
}


/**
	����ѭ����ת��
*/
uint32_t Mp_Value = 0;
void wheel_Track_check(void)
{ 	
	uint16_t Track_Value = 0;
	
	if(wheel_L_Flag == 1)
	{
		Track_Value = Get_Host_UpTrack(TRACK_H8);
		{
			if(!(Track_Value &0X08)) //�ҵ�ѭ���ߣ�ֹͣ
			{	
				if(Wheel_flag > 50)
				{
					wheel_L_Flag = 0;
					Wheel_flag=0;
					Stop_Flag=2;
					Send_UpMotor(0,0);
				}
			}
			else if(Track_Value == 0Xff) 
				Wheel_flag++;
		}
	} 
	else if(wheel_R_Flag == 1)
	{
		Track_Value = Get_Host_UpTrack(TRACK_H8);
		{
		 if(!(Track_Value &0X10)) //�ҵ�ѭ���ߣ�ֹͣ
			{	
				if(Wheel_flag > 50)
				{
					wheel_R_Flag=0;
					Wheel_flag=0;
					Stop_Flag=2;
					Send_UpMotor(0,0);
				}
			}
			else if( Track_Value == 0Xff) 
			{
				Wheel_flag++;
			}
					
		}
	}
}




/**
	ѭ�����
*/
void Track_Check()
{	
	if(Track_Flag == 1)
	{
		Track_Ccorrect(Get_Host_UpTrack(TRACK_H8));  	//ѭ��
	}
	if(Track_Flag == 2)
	{
		Track_Correct(Get_Host_UpTrack(TRACK_H8));  	//ѭ��
	}
	if(Track_Flag == 3)
	{
		TXDT_Ccorrect(Get_Host_UpTrack(TRACK_H8));  	//ѭ��
	}
	if(Terrain_Flag == 1)								
	{
		Terrain_Track(Get_Host_UpTrack(TRACK_H8));
	}
	if(Terrain_Flag == 2)									//�����ѭ��
	{
		Teshudixing_Pd(Get_Host_UpTrack(TRACK_H8));
	}
	if(Terrain_Flag == 3)									//�����ѭ��
	{
		TXDX_Pd(Get_Host_UpTrack(TRACK_H8));
	}
	if(Mp_Track_Flag == 1)									//ָ������ѭ��
	{
		Mp_Track(Get_Host_UpTrack(TRACK_H8));				
	}
}

void Roadway_Check(void)
{
	Track_Check();
	Go_and_Back_Check();
	wheel_Track_check();
	wheel_Nav_check();
}
	

/***************************************************************
** ���ܣ�     ������ƺ���
** ������	  L_Spend����������ٶ�
**            R_Spend����������ٶ�
** ����ֵ��   ��	  
****************************************************************/
void Control(int L_Spend,int R_Spend)
{
	if(L_Spend>=0)
	{	
		if(L_Spend>100)L_Spend=100;if(L_Spend<5)L_Spend=5;		//�����ٶȲ���
	}
	else 
	{
		if(L_Spend<-100)L_Spend= -100;if(L_Spend>-5)L_Spend= -5;     //�����ٶȲ���
	}	
	if(R_Spend>=0)
	{	
		if(R_Spend>100)R_Spend=100;if(R_Spend<5)R_Spend=5;		//�����ٶȲ���
	}
	else
	{	
		if(R_Spend<-100)R_Spend= -100;if(R_Spend>-5)R_Spend= -5;		//�����ٶȲ���		
	}
	Send_UpMotor(L_Spend ,R_Spend);	
}

/***************************************************************
** ���ܣ�    Ѱ�� ѭ������
** ������	  �޲���
** ����ֵ��   ��
****************************************************************/
void Track_Correct(uint8_t gd)
{
	if(gd == 0x00)	//ѭ����ȫ�� ֹͣ
	{	
		Track_Flag = 0;
		Stop_Flag = 1;
		Send_UpMotor(0,0);
	}
	else if(gd == 0xFE||gd == 0xFC||gd == 0xF8)      //1111 1110  -  1111 1100 - 1111 1000     
	{								
		Track_Flag=0;
		Stop_Flag=1;
		Send_UpMotor(0,0);																	 		                
	}
	else if(gd == 0x7F||gd == 0x3F||gd == 0x1F)		//0111 1111  -  0011 1111 - 0001 1111
	{
		Track_Flag=0;
		Stop_Flag=1;
		Send_UpMotor(0,0);
	}
	else if(gd == 0x7E||gd == 0x3C)  				//0111 1110  -  0011 1100
	{
		Track_Flag=0;
		Stop_Flag=1;
		Send_UpMotor(0,0);
	}
	else if(gd == 0x7C)								//0111 1100  -  0111 1000
	{
		Track_Flag=0;
		Stop_Flag=1;
		Send_UpMotor(0,0);
	}
	else if(gd == 0x3E||gd == 0x1E) 				//0011 1110  -  0001 1110
	{
		Track_Flag=0;
		Stop_Flag=1;
		Send_UpMotor(0,0);
	}
	else
	{
	   	Stop_Flag=0;
		if(gd==0XE7)   //1���м�3/4��������⵽���ߣ�ȫ������
		{ 
			LSpeed=Car_Spend;
			RSpeed=Car_Spend;
		}
		
		if(gd==0XF7)		//11110111
		{
			LSpeed=Car_Spend+40;
			RSpeed=Car_Spend-50;
		}
		if(gd==0XEF)
		{
			LSpeed=Car_Spend-50;
			RSpeed=Car_Spend+40;
		}
		if(Line_Flag!=2)
		{		
			if(gd==0XF3||gd==0XFB) //2���м�4��3��������⵽���ߣ�΢�ҹ�	
			{ 
				LSpeed=Car_Spend+50;
				RSpeed=Car_Spend-60;
				Line_Flag=0;
			}
			else if(gd==0XF9||gd==0XFD)//3���м�3��2��������⵽���ߣ���΢�ҹ�		
			{ 
				 LSpeed=Car_Spend+60;
				 RSpeed=Car_Spend-70;				 
				 Line_Flag=0;
			}
			else if(gd==0XFC)//4���м�2��1��������⵽���ߣ�ǿ�ҹ�
			{ 
				LSpeed = Car_Spend+60;
				RSpeed = Car_Spend-90;
				 Line_Flag=0;
			}
			else if(gd==0XFE)//5�����ұ�1��������⵽���ߣ���ǿ�ҹ�			
			{ 
				 LSpeed = Car_Spend+80;
				 RSpeed = Car_Spend-120;			
				 Line_Flag=1;
			}
		}
		if(Line_Flag!=1)
		{
			if(gd==0XCF)//6���м�6��5��������⵽���ߣ�΢���			
			{ 
				 RSpeed = Car_Spend+50;
				 LSpeed = Car_Spend-60;
				 Line_Flag=0;
			} 
			else if(gd==0X9F||gd==0XDF)//7���м�7��6��������⵽���ߣ���΢���		 
			{ 
				 RSpeed = Car_Spend+60;
				 LSpeed = Car_Spend-70;
				 Line_Flag=0;
			} 			  
			else if(gd==0X3F||gd==0XBF)//8���м�8��7��������⵽���ߣ�ǿ���		 
			{ 
				 RSpeed = Car_Spend+70;
				 LSpeed = Car_Spend-90;
				 Line_Flag=0;
			} 
			else if(gd==0X7F)//9������8��������⵽���ߣ���ǿ���		 	
			{ 
				 RSpeed = Car_Spend+80;
				 LSpeed = Car_Spend-120;
				 Line_Flag=2;
			}			
		}
		if(gd==0xFF)   //ѭ����ȫ��
		{
			if(count > 5000)
			{
					count=0;
					Send_UpMotor(0,0);
					Track_Flag=0;
					if(Line_Flag ==0) 
						Stop_Flag=4;
			}	
			else
			{				
				count++;
				LSpeed = Car_Spend;
				RSpeed = Car_Spend;
			}				
		}
		else 
			count=0;
	}
	
	if(Track_Flag != 0)
	{
		Control(LSpeed,RSpeed);
	}
}

/***************************************************************
** ���ܣ�     ѭ������
** ������	  �޲���
** ����ֵ��   ��
****************************************************************/
void TXDT_Ccorrect(uint8_t gd)   //ѭ������
{
	
	if(gd == 0xFF)	//ѭ����ȫ�� ֹͣ
	{	 
		DX_Bzw200=1;
		Track_Flag = 0;
		Stop_Flag = 1;
		Send_UpMotor(0,0);
	}
	else if(gd == 0x18||gd == 0x08||gd == 0x0C||gd == 0x10||gd == 0x30)         
	{	
		
    DX_Bzw201=1;
		TXDX_Bzw=1;		
		Track_Flag=0;
		Stop_Flag=1;
		Send_UpMotor(0,0);																	 		                
	}
	else if(gd == 0x3F||gd == 0xFC||gd == 0x3C||gd == 0x78)		
	{
		DX_Bzw203=1;
		Track_Flag=0;
		Stop_Flag=1;
		Send_UpMotor(0,0);
	}
	else if(gd == 0x00||gd == 0x7E||gd == 0x7F||gd == 0xFE)		
	{
		DX_Bzw203=1;
		Track_Flag=0;
		Stop_Flag=1;
		Send_UpMotor(0,0);
	}
	else if(gd == 0x1E||gd == 0x1C||gd == 0x38)		
	{
		DX_Bzw201=1;
		TXDX_Bzw=1;
		Track_Flag=0;
		Stop_Flag=1;
		Send_UpMotor(0,0);
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
		if(Track_Flag != 0)
		{
				Control(LSpeed,RSpeed);
		}
	}
	
}

/***************************************************************
** ���ܣ�     ѭ������
** ������	  �޲���
** ����ֵ��   ��
****************************************************************/
void Track_Ccorrect(uint8_t gd)   //ѭ������
{
	
	if(gd == 0xFF)	//ѭ����ȫ�� ֹͣ
	{	 
		DX_Bzw100=1;
		Track_Flag = 0;
		Stop_Flag = 1;
		Send_UpMotor(0,0);
	}
	else if(gd == 0x18||gd == 0x08||gd == 0x0C||gd == 0x10||gd == 0x30)   //0001 1000��0000 1000��0000 1100��0000 1010��0011 0000 //       
	{	
		
        DX_Bzw101=1;		
		Track_Flag=0;
		Stop_Flag=1;
		Send_UpMotor(0,0);																	 		                
	}
	else if(gd == 0x3F||gd == 0xFC||gd == 0x3C||gd == 0x78)	//0011 1111��1111 1100��0011 1100��0111 1000	//
	{
		DX_Bzw103=1;
		Track_Flag=0;
		Stop_Flag=1;
		Send_UpMotor(0,0);
	}
	else if(gd == 0x00||gd == 0x7E||gd == 0x7F||gd == 0xFE)		//0000 0000��0111 1110��0111 1111��1111 1110//
	{
		DX_Bzw103=1;
		Track_Flag=0;
		Stop_Flag=1;
		Send_UpMotor(0,0);
	}
	else if(gd == 0x1E||gd == 0x1C||gd == 0x38)		         //0001 1110��0001 1100��0011 1000//
	{
		DX_Bzw103=1;
		Track_Flag=0;
		Stop_Flag=1;
		Send_UpMotor(0,0);
	}
		else
	{
		if(gd==0XE7)                                      //1110 0111//
		{ 
			LSpeed=Car_Spend;
			RSpeed=Car_Spend;
			
		}
		
	else	if(gd==0XF7)		                            //1111 0111//
		{
			LSpeed=Car_Spend+30;
			RSpeed=Car_Spend-40;
			
		}
	else	if(gd==0XEF)                               //1110 1111//
		{
			LSpeed=Car_Spend-40;
			RSpeed=Car_Spend+30;
			
		}
		 
	else	if(gd==0XF3||gd==0XFB) 	                 //1111 0011��1111 1011/
		{ 
			LSpeed=Car_Spend+30;
			RSpeed=Car_Spend-40;
			
		}
		else if(gd==0XF9||gd==0XFD)	                //1111 1001��1111 1101//
		{ 
			 LSpeed=Car_Spend+60;
			 RSpeed=Car_Spend-70;				 
			 
		}
		else if(gd==0XFC)                           //1111 1100//
		{ 
			LSpeed = Car_Spend+60;
			RSpeed = Car_Spend-90;
			
		}
	else if(gd==0XFE)		                         //1111 1110//
		{ 
			 LSpeed = Car_Spend+80;
			 RSpeed = Car_Spend-120;			
			
		} 
	else	if(gd==0XCF)	                         //1100 1111//
		{ 
			 RSpeed = Car_Spend+30;
			 LSpeed = Car_Spend-40;
			
		}  
		else if(gd==0X9F||gd==0XDF)		             //1001 1111��1101 1111//
		{ 
			 RSpeed = Car_Spend+60;
			 LSpeed = Car_Spend-70;
			 
		} 			  
		else if(gd==0X3F||gd==0XBF)		             //0011 1111��1011 1111//
		{ 
			 RSpeed = Car_Spend+70;
			 LSpeed = Car_Spend-90;
			 
		} 
		else if(gd==0X7F) 	                     //0111 1111//
		{  
			 RSpeed = Car_Spend+80;
			 LSpeed = Car_Spend-120;
			 
		}			
		if(Track_Flag != 0)
	{
//		static uint32_t gt_save = 0;
//		uint32_t gt_temp;
//		gt_temp = gt_get();
//		if((gt_save + 2) < gt_temp)
//		{
			Control(LSpeed,RSpeed);
//			gt_save = gt_temp;
//		}
	}
	}
	
}


/*�������ѭ��
**/
void Teshudixing_Pd(uint8_t gd)  //�������ѭ��
{

	
		if(gd==0X18)   //1���м�3/4��������⵽���ߣ�ȫ������     1110 0111
		{ 
			LSpeed=Car_Spend;
			RSpeed=Car_Spend;
		}
		

			else if(gd==0X08||gd==0X0C||gd==0X1C)//3���м�3��2��������⵽���ߣ���΢�ҹ�		
			{ 
				 LSpeed=Car_Spend+30;
				 RSpeed=Car_Spend-40;				 
				 
			}
			else if(gd==0X04||gd==0X06)//4���м�2��1��������⵽���ߣ�ǿ�ҹ�
			{ 
				LSpeed = Car_Spend+40;
				RSpeed = Car_Spend-60;
				 
			}
			else if(gd==0X10||gd==0X30||gd==0X38)//3���м�3��2��������⵽���ߣ���΢�ҹ�		
			{ 
				 LSpeed=Car_Spend-30;
				 RSpeed=Car_Spend+40;				 
				 
			}
			else if(gd==0X20||gd==0X60)//3���м�3��2��������⵽���ߣ���΢�ҹ�		
			{ 
				 LSpeed=Car_Spend-60;
				 RSpeed=Car_Spend+40;				 
				 
			}					
		else{ 
			LSpeed=Car_Spend;
			RSpeed=Car_Spend;
		}
	
	

	if(temp_MP <= Roadway_mp_Get())
	{
		
		Go_Flag = 0;
		Stop_Flag = 3;
		Mp_Track_Flag = 0;
		Send_UpMotor(0,0);
		DX_Bzw102=1;
		Terrain_Flag = 0;
  } 
	else
	{
	
		Control(LSpeed,RSpeed);
		
	 }

 
}
/*�������ѭ��
**/
void TXDX_Pd(uint8_t gd)  //�������ѭ��
{

	
		if(gd==0X18)   //1���м�3/4��������⵽���ߣ�ȫ������     1110 0111
		{ 
			LSpeed=Car_Spend;
			RSpeed=Car_Spend;
		}
		

			else if(gd==0X08||gd==0X0C||gd==0X1C)//3���м�3��2��������⵽���ߣ���΢�ҹ�		
			{ 
				 LSpeed=Car_Spend+30;
				 RSpeed=Car_Spend-40;				 
				 
			}
			else if(gd==0X04||gd==0X06)//4���м�2��1��������⵽���ߣ�ǿ�ҹ�
			{ 
				LSpeed = Car_Spend+40;
				RSpeed = Car_Spend-60;
				 
			}
			else if(gd==0X10||gd==0X30||gd==0X38)//3���м�3��2��������⵽���ߣ���΢�ҹ�		
			{ 
				 LSpeed=Car_Spend-30;
				 RSpeed=Car_Spend+40;				 
				 
			}
			else if(gd==0X20||gd==0X60)//3���м�3��2��������⵽���ߣ���΢�ҹ�		
			{ 
				 LSpeed=Car_Spend-60;
				 RSpeed=Car_Spend+40;				 
				 
			}					
		else{ 
			LSpeed=Car_Spend;
			RSpeed=Car_Spend;
		}
	
	

	if(temp_MP <= Roadway_mp_Get())
	{
		
		Go_Flag = 0;
		Stop_Flag = 3;
		Mp_Track_Flag = 0;
		Send_UpMotor(0,0);
		DX_Bzw202=1;
		Terrain_Flag = 0;
  } 
	else
	{
	
		Control(LSpeed,RSpeed);
		
	 }

 
}

void Mp_Track(uint8_t gd)
{

	if(gd==0X00)	//ѭ����ȫ�� ֹͣ
	{	
		Mp_Track_Flag = 0;
		//Stop_Flag = 1;
		Send_UpMotor(0,0);
	}
	else
	{
	   	Stop_Flag=0;
		if(gd==0XE7)   //1���м�3/4��������⵽���ߣ�ȫ������
		{ 
			LSpeed=Car_Spend;
			RSpeed=Car_Spend;
		}
		
		if(gd==0XF7)		//11110111
		{
			LSpeed=Car_Spend+40;
			RSpeed=Car_Spend-50;
		}
		if(gd==0XEF)
		{
			LSpeed=Car_Spend-50;
			RSpeed=Car_Spend+40;
		}
		if(Line_Flag!=2)
		{		
			if(gd==0XF3||gd==0XFB) //2���м�4��3��������⵽���ߣ�΢�ҹ�	
			{ 
				LSpeed=Car_Spend+50;
				RSpeed=Car_Spend-60;
				Line_Flag=0;
			}
			else if(gd==0XF9||gd==0XFD)//3���м�3��2��������⵽���ߣ���΢�ҹ�		
			{ 
				 LSpeed=Car_Spend+60;
				 RSpeed=Car_Spend-70;				 
				 Line_Flag=0;
			}
			else if(gd==0XFC)//4���м�2��1��������⵽���ߣ�ǿ�ҹ�
			{ 
				LSpeed = Car_Spend+60;
				RSpeed = Car_Spend-90;
				 Line_Flag=0;
			}
			else if(gd==0XFE)//5�����ұ�1��������⵽���ߣ���ǿ�ҹ�			
			{ 
				 LSpeed = Car_Spend+80;
				 RSpeed = Car_Spend-120;			
				 Line_Flag=1;
			}
		}
		if(Line_Flag!=1)
		{
			if(gd==0XCF)//6���м�6��5��������⵽���ߣ�΢���			
			{ 
				 RSpeed = Car_Spend+50;
				 LSpeed = Car_Spend-60;
				 Line_Flag=0;
			} 
			else if(gd==0X9F||gd==0XDF)//7���м�7��6��������⵽���ߣ���΢���		 
			{ 
				 RSpeed = Car_Spend+60;
				 LSpeed = Car_Spend-70;
				 Line_Flag=0;
			} 			  
			else if(gd==0X3F||gd==0XBF)//8���м�8��7��������⵽���ߣ�ǿ���		 
			{ 
				 RSpeed = Car_Spend+70;
				 LSpeed = Car_Spend-90;
				 Line_Flag=0;
			} 
			else if(gd==0X7F)//9������8��������⵽���ߣ���ǿ���		 	
			{ 
				 RSpeed = Car_Spend+80;
				 LSpeed = Car_Spend-120;
				 Line_Flag=2;
			}			
		}
		if(gd==0xFF)   //ѭ����ȫ��
		{
			if(count > 5000)
			{
					count=0;
					Send_UpMotor(0,0);
					Mp_Track_Flag = 0;
					if(Line_Flag ==0) 
						Stop_Flag=4;
			}	
			else
			{				
				count++;
				LSpeed = Car_Spend;
				RSpeed = Car_Spend;
			}				
		}
		else 
			count=0;
	}
	
	if(temp_MP <= Roadway_mp_Get())
	{
		Go_Flag = 0;
		Stop_Flag = 3;
		Mp_Track_Flag = 0;
		Send_UpMotor(0,0);
	} else {
	
		Control(LSpeed,RSpeed);
	}



}

/*
����ѭ�����
**/
void Terrain_Track(uint8_t gd)
{
	if(gd==0X00)	//ѭ����ȫ�� ֹͣ
	{	
		Terrain_Flag = 0;
		Send_UpMotor(0,0);
	}
	else
	{
	   	Stop_Flag=0;
		if(gd==0XE7)   //1���м�3/4��������⵽���ߣ�ȫ������     1110 0111
		{ 
			LSpeed=Car_Spend;
			RSpeed=Car_Spend;
		}
		
		if(gd==0XF7)		//11110111
		{
			LSpeed=Car_Spend+40;
			RSpeed=Car_Spend-50;
		}
		if(gd==0XEF)  //  1110 1111
		{
			LSpeed=Car_Spend-50;
			RSpeed=Car_Spend+40;
		}
		if(Line_Flag!=2)
		{		
			if(gd==0XF3||gd==0XFB) //2���м�4��3��������⵽���ߣ�΢�ҹ�	
			{ 
				LSpeed=Car_Spend+50;
				RSpeed=Car_Spend-60;
				Line_Flag=0;
			}
			else if(gd==0XF9||gd==0XFD)//3���м�3��2��������⵽���ߣ���΢�ҹ�		
			{ 
				 LSpeed=Car_Spend+60;
				 RSpeed=Car_Spend-70;				 
				 Line_Flag=0;
			}
			else if(gd==0XFC)//4���м�2��1��������⵽���ߣ�ǿ�ҹ�
			{ 
				LSpeed = Car_Spend+60;
				RSpeed = Car_Spend-90;
				 Line_Flag=0;
			}
			else if(gd==0XFE)//5�����ұ�1��������⵽���ߣ���ǿ�ҹ�			
			{ 
				 LSpeed = Car_Spend+80;
				 RSpeed = Car_Spend-120;			
				 Line_Flag=1;
			}
		}
		if(Line_Flag!=1)
		{
			if(gd==0XCF)//6���м�6��5��������⵽���ߣ�΢���			
			{ 
				 RSpeed = Car_Spend+50;
				 LSpeed = Car_Spend-60;
				 Line_Flag=0;
			} 
			else if(gd==0X9F||gd==0XDF)//7���м�7��6��������⵽���ߣ���΢���		 
			{ 
				 RSpeed = Car_Spend+60;
				 LSpeed = Car_Spend-70;
				 Line_Flag=0;
			} 			  
			else if(gd==0X3F||gd==0XBF)//8���м�8��7��������⵽���ߣ�ǿ���		 
			{ 
				 RSpeed = Car_Spend+70;
				 LSpeed = Car_Spend-90;
				 Line_Flag=0;
			} 
			else if(gd==0X7F)//9������8��������⵽���ߣ���ǿ���		 	
			{ 
				 RSpeed = Car_Spend+80;
				 LSpeed = Car_Spend-120;
				 Line_Flag=2;
			}			
		}
		if(gd==0xFF)   //ѭ����ȫ��
		
			Send_UpMotor(0,0);
			Terrain_Flag = 0;
			
		
	}
	

	if(temp_MP <= Roadway_mp_Get())
	{
		Go_Flag = 0;
		Stop_Flag = 3;
		Mp_Track_Flag = 0;
		Send_UpMotor(0,0);
	} else {
	
		Control(LSpeed,RSpeed);
	}

}



void Back_Track(uint8_t gd)
{
		if(gd == 0x00)
		{
			Back_Track_Flag = 0;
			Stop_Flag = 1;
			Send_UpMotor(0,0);
		}
		else if(gd == 0x77)		// 1110111		ȫ��
		{
			LSpeed = Car_Spend;
			RSpeed = Car_Spend;
		}
		else if((gd == 0x73) || (gd == 0x7B))		//1110011 && 1111011 ƫ��
		{
			LSpeed = Car_Spend+30;
			RSpeed = Car_Spend-30;
		}
		else if((gd == 0x79) || (gd == 0x7D))	    //1111001 && 1111101 ƫ��
		{
			LSpeed = Car_Spend+40;
			RSpeed = Car_Spend-40;
		}
		else if((gd == 0x7C) || (gd == 0x7E))		//1111100 && 1111110 ƫ��
		{
			LSpeed = Car_Spend+50;
			RSpeed = Car_Spend-50;
		}
		
		if((gd == 0x67) || (gd == 0x6F))		//1100111 && 1101111 ƫ��
		{
			LSpeed = Car_Spend-30;
			RSpeed = Car_Spend+30;
		}
		else if((gd == 0x5F) || (gd == 0x4F))		//1011111 && 1001111 ƫ��
		{
			LSpeed = Car_Spend-40;
			RSpeed = Car_Spend+40;
		}
		else if((gd == 0x1F) || (gd == 0x3F))		//0011111 && 0111111 ƫ��
		{
			LSpeed = Car_Spend-50;
			RSpeed = Car_Spend+50;
		}
		
		if(gd == 0xFF)   //ѭ����ȫ��
		{
			if(count > 5000)
			{
					count = 0;
					Send_UpMotor(0,0);
					Back_Track_Flag = 0;
					if(Line_Flag == 0) 
						Stop_Flag = 4;
			}	
			else count++;				
		}
		else count = 0;
	
		if(Back_Track_Flag != 0)
		{
			Control(LSpeed,RSpeed);
		}
}

//______________________________________________


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
		Roadway_Check();								//·�����
	}
	TIM_ClearITPendingBit(TIM9,TIM_IT_Update);
}













//end file



