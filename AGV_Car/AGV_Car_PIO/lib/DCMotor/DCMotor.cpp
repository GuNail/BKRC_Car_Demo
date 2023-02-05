#include <DCMotor.h>
#include <Command.h>
#include "wiring_private.h"
#include <ExtSRAMInterface.h>
#include <Metro.h>

_DCMotor DCMotor;
Metro DCMotorMetro(20);

_DCMotor::_DCMotor()
{
	ExtSRAMInterface.Initialization();
}

_DCMotor::~_DCMotor()
{
}


/************************************************************************************************************
【函 数 名】：	Initialization	直流电机初始化
【参数说明】：	fHz		：		初始化PWM输出频率，单位：Hz
【返 回 值】：	无
【简    例】：	Initialization(8000);
************************************************************************************************************/
void _DCMotor::Initialization(uint32_t fHz)
{
	pinMode(L_CONTROL_PIN, OUTPUT);
	pinMode(R_CONTROL_PIN, OUTPUT);
	pinMode(R_F_M_PIN, OUTPUT);
	pinMode(R_B_M_PIN, OUTPUT);
	pinMode(L_F_M_PIN, OUTPUT);
	pinMode(L_B_M_PIN, OUTPUT);

	TCCR4A = 0x00;
	TCCR4B = 0x00;
	TCCR3A = 0x00;
	TCCR3B = 0x00;

	//选择工作模式,模式14--fast PWM
	TCCR4A |= _BV(WGM41);
	TCCR4B |= _BV(WGM42) | _BV(WGM43);
	TCCR3A |= _BV(WGM31);
	TCCR3B |= _BV(WGM32) | _BV(WGM33);

	//设置PWM波的频率
	MotorFrequency(fHz);

	//设置PWM波的占空比
	//SpeedCtr(0, 0);
	//MotorSpeed(MOVE_RUNMODE_STOP,_Fspeed, _Bspeed);
	//LeftMotorSpeed(_Fspeed, _Bspeed);
	Stop();
	//设置输出通道
	//TCCR4A |= _BV(COM4C1) | _BV(COM4B1) | _BV(COM4C0) | _BV(COM4B0);
	//TCCR3A |= _BV(COM3C1) | _BV(COM3B1) | _BV(COM3C0) | _BV(COM3B0);
	TCCR4A |= _BV(COM4C1) | _BV(COM4B1);
	TCCR3A |= _BV(COM3C1) | _BV(COM3B1);
	ParameterInit();
	Stop();
}

void _DCMotor::ParameterInit(void)
{
	for (uint8_t i = 0; i < 101; i++)	//占空比初始化
	{
		speed[i] = fHz * i / 100;
	}
}

void _DCMotor::SpeedSetOne(int16_t s, uint8_t *c1, uint8_t *c2)
{
	uint8_t t;
	t = (s >= 0) ? s : s*(-1);
	if (t > 100)
		t = 100;
	if (t < 5)
		t = 5;
	if (s == 0)
	{
		*c1 = speed[100];		//100;
		*c2 = speed[100];		//100;
	}
	else if (s > 0)
	{
		*c1 = speed[t];
		*c2 = speed[0];
	}
	else
	{
		*c1 = speed[0];
		*c2 = speed[t];
	}
}

void _DCMotor::SpeedCtr(int16_t L_speed, int16_t R_speed)
{
	uint8_t ocr3b, ocr3c, ocr4b, ocr4c;

	SpeedSetOne(L_speed, &ocr4c, &ocr4b);
	SpeedSetOne(R_speed, &ocr3b, &ocr3c);

	(ocr3b == 0) ? (TCCR3A |= _BV(COM3B0), ocr3b = fHz) : (TCCR3A &= ~_BV(COM3B0));
	(ocr3c == 0) ? (TCCR3A |= _BV(COM3C0), ocr3c = fHz) : (TCCR3A &= ~_BV(COM3C0));
	(ocr4b == 0) ? (TCCR4A |= _BV(COM4B0), ocr4b = fHz) : (TCCR4A &= ~_BV(COM4B0));
	(ocr4c == 0) ? (TCCR4A |= _BV(COM4C0), ocr4c = fHz) : (TCCR4A &= ~_BV(COM4C0));

	OCR4C = ocr4c;
	OCR4B = ocr4b;
	OCR3C = ocr3c;
	OCR3B = ocr3b;
}

bool _DCMotor::ClearCodeDisc(void)
{
	uint16_t distance;
	unsigned long t;
	Command.Judgment(Command.command01);
	for (size_t i = 0; i < 8; i++)
	{
		ExtSRAMInterface.ExMem_JudgeWrite(WRITEADDRESS + i, Command.command01[i]);
	}
	DCMotorMetro.interval(20);
	for (size_t i = 0; i < 5; i++)
	{
		if (DCMotorMetro.check() == 1)
		{
			distance = uint16_t(ExtSRAMInterface.ExMem_Read(BASEADDRESS + CODEOFFSET) + (ExtSRAMInterface.ExMem_Read(BASEADDRESS + CODEOFFSET + 1) << 8));
			if (distance == 0x0000)
			{
				return false;
			}
		}
	}
	return true;
}

/************************************************************************************************************
小车动作的相关函数
************************************************************************************************************/
/************************************************************************************************************
【函 数 名】：  Go		小车前进函数
【参数说明】：	speed	：设置速度
distance: 设置前进的距离
【返 回 值】：	无
【简    例】：	Go(70);	小车动作：前进，前进速度：70
************************************************************************************************************/
void _DCMotor::Go(uint8_t speed)
{
	SpeedCtr(speed, speed);
	StartUp();
}

uint16_t _DCMotor::Go(uint8_t speed, uint16_t _distance)
{
	unsigned long t;
	uint16_t distance;
	while (ClearCodeDisc());
	SpeedCtr(speed, speed);
	StartUp();

	t = millis();
	do
	{
		distance = uint16_t(ExtSRAMInterface.ExMem_Read(BASEADDRESS + CODEOFFSET) + (ExtSRAMInterface.ExMem_Read(BASEADDRESS + CODEOFFSET + 1) << 8));
		if ((65516 > distance) && (distance > 20))
		{
			if ((distance >= _distance) || ((millis() - t) >= 30000))
			{
				Stop();
				delay(50);
				distance = uint16_t(ExtSRAMInterface.ExMem_Read(BASEADDRESS + CODEOFFSET) + (ExtSRAMInterface.ExMem_Read(BASEADDRESS + CODEOFFSET + 1) << 8));
				break;
			}
		}
		delay(1);
	} while (true);
	return distance;
}

/************************************************************************************************************
【函 数 名】：  Back		小车后退函数
【参数说明】：	speed	:	设置速度
distance:	设置后退的距离
【返 回 值】：	无
【简    例】：	Back(70);	小车动作：后退，后退速度：70
************************************************************************************************************/
void _DCMotor::Back(uint8_t speed)
{
	SpeedCtr(speed*(-1), speed*(-1));
	StartUp();
}
uint16_t _DCMotor::Back(uint8_t speed, uint16_t _distance)
{
	unsigned long t;
	uint16_t distance;
	while (ClearCodeDisc());
	Back(speed);
	t = millis();
	do
	{
		distance = uint16_t(ExtSRAMInterface.ExMem_Read(BASEADDRESS + CODEOFFSET) + (ExtSRAMInterface.ExMem_Read(BASEADDRESS + CODEOFFSET + 1) << 8));
		if ((65516 > distance) && (distance > 20))
		{
			if (((65536 - distance) >= _distance) || ((millis() - t) > 30000))
			{
				Stop();
				delay(50);
				distance = uint16_t(ExtSRAMInterface.ExMem_Read(BASEADDRESS + CODEOFFSET) + (ExtSRAMInterface.ExMem_Read(BASEADDRESS + CODEOFFSET + 1) << 8));
				break;
			}
		}
		delay(10);
	} while (true);
	return (65536 - distance);
}

/************************************************************************************************************
【函 数 名】：  TurnLeft	小车左转函数,Lspeed <= Rspeed
【参数说明】：	Lspeed	：	设置左轮速度
Rspeed	：	设置右轮速度
【返 回 值】：	无
【简    例】：	TurnLeft(70);	小车动作：左转，左转速度：70
************************************************************************************************************/
void _DCMotor::TurnLeft(int8_t Lspeed, int8_t Rspeed)
{
	SpeedCtr(Lspeed*(-1), Rspeed);
	StartUp();
}
void _DCMotor::TurnLeft(int8_t speed)
{
	uint8_t tgd, tp;
	unsigned long t;
	uint8_t  trackval;
	while (ClearCodeDisc());
	SpeedCtr(speed*(-1), speed);
	StartUp();
	do
	{
		tgd = ExtSRAMInterface.ExMem_Read(BASEADDRESS + TRACKOFFSET);
		tp = SearchBit(1,tgd);
		if (tp <= 0x04)	break;
	} while (true);

	t = millis();
	do
	{
		trackval = ExtSRAMInterface.ExMem_Read(BASEADDRESS + TRACKOFFSET);
		if ((!(trackval & 0x10)) || ((millis() - t) > 10000))
		{
			Stop();
			break;
		}
	} while (true);
}


/************************************************************************************************************
【函 数 名】：  TurnRight	小车右转函数,Rspeed <= Lspeed
【参数说明】：	Lspeed	：	设置左轮速度
Rspeed	：	设置右轮速度
【返 回 值】：	无
【简    例】：	TurnRight(70);	小车动作：右转，右转速度：70
************************************************************************************************************/
void _DCMotor::TurnRight(int8_t Lspeed, int8_t Rspeed)
{
	SpeedCtr(Lspeed, Rspeed*(-1));
	StartUp();
}

void _DCMotor::TurnRight(int8_t speed)
{
	uint8_t tgd, tp;
	unsigned long t;
	uint8_t  trackval;
	while (ClearCodeDisc());
	SpeedCtr(speed, speed*(-1));
	StartUp();
	do{
		tgd = ExtSRAMInterface.ExMem_Read(BASEADDRESS + TRACKOFFSET);
		tp = SearchBit(0,tgd);
		if ((tp >= 0x20))	break;
	} while (true);

	t = millis();
	do
	{
		trackval = ExtSRAMInterface.ExMem_Read(BASEADDRESS + TRACKOFFSET);
		if (((!(trackval & 0x08)) || ((millis() - t) > 10000)))
		{
			Stop();
			break;
		}
	} while (true);
}
/*
左转任意角度
speed 速度
_distance 角度
**/
uint16_t _DCMotor::TurnLeftAngle(uint8_t speed, uint16_t _distance)
{
	unsigned long t;
	uint16_t distance;
	while (ClearCodeDisc());
	TurnLeft(speed, speed);
	t = millis();
	do
	{
		distance = uint16_t(ExtSRAMInterface.ExMem_Read(BASEADDRESS + CODEOFFSET) + (ExtSRAMInterface.ExMem_Read(BASEADDRESS + CODEOFFSET + 1) << 8));
		if ((65516 > distance) && (distance > 20))
		{
			if (((65536 - distance) >= _distance) || ((millis() - t) > 30000))
			{
				Stop();

				delay(50);
				distance = uint16_t(ExtSRAMInterface.ExMem_Read(BASEADDRESS + CODEOFFSET) + (ExtSRAMInterface.ExMem_Read(BASEADDRESS + CODEOFFSET + 1) << 8));
				break;
			}
		}
		delay(10);
	} while (true);
	return (65536 - distance);
}


/*
右转任意角度
speed 速度
_distance 角度
**/
uint16_t _DCMotor::TurnRightAngle(uint8_t speed, uint16_t _distance)
{
	unsigned long t;
	uint16_t distance;
	while (ClearCodeDisc())
	{

	}
	TurnRight(speed, speed);

	t = millis();
	do
	{
		distance = uint16_t(ExtSRAMInterface.ExMem_Read(BASEADDRESS + CODEOFFSET) + (ExtSRAMInterface.ExMem_Read(BASEADDRESS + CODEOFFSET + 1) << 8));
		if ((65516 > distance) && (distance > 20))
		{
			if ((distance >= _distance) || ((millis() - t) >= 30000))
			{
				Stop();
				delay(50);
				distance = uint16_t(ExtSRAMInterface.ExMem_Read(BASEADDRESS + CODEOFFSET) + (ExtSRAMInterface.ExMem_Read(BASEADDRESS + CODEOFFSET + 1) << 8));
				break;
			}
		}
		delay(1);
	} while (true);
	return distance;
}



/************************************************************************************************************
【函 数 名】：  Stop	小车停止函数
【参数说明】：	无
【返 回 值】：	无
【简    例】：	Stop();	小车动作：停止
************************************************************************************************************/
void _DCMotor::Stop(void)
{
	/*这两段代码都有电机停止功能，但停止的效果不一样*/

	/**********电机不锁死******************/
	//PORTE &= ~_BV(PE3);
	//PORTH &= ~_BV(PH3);
	//SpeedCtr(0, 0);
	/***********END**************************/

	//电机瞬间锁死
	SpeedCtr(0, 0);
	PORTE |= _BV(PE3);
	PORTH |= _BV(PH3);
	/**********END************/
}

/************************************************************************************************************
【函 数 名】：  StartUp		小车启动函数
【参数说明】：	无
【返 回 值】：	无
【简    例】：	StartUp();	小车动作：启动
************************************************************************************************************/
void _DCMotor::StartUp(void)
{
	PORTE |= _BV(PE3);
	PORTH |= _BV(PH3);
	//digitalWrite(L_CONTROL_PIN, HIGH);
	//digitalWrite(R_CONTROL_PIN, HIGH);
}

/************************************************************************************************************
【函 数 名】：  ShutDown	小车关闭函数
【参数说明】：	无
【返 回 值】：	无
【简    例】：	ShutDown();	小车动作：关闭
************************************************************************************************************/
void _DCMotor::ShutDown(void)
{
	SpeedCtr(100, 100);
	PORTE |= _BV(PE3);
	PORTH |= _BV(PH3);
}
/*************************************************************END***********************************************/

/*
获取寻迹板数据
0 前7位
1 后8位
2 全部
*/
uint16_t _DCMotor::Get_Track_broad(uint8_t value)
{
	uint16_t gd;
	switch (value)
	{
	case 0:
		gd=ExtSRAMInterface.ExMem_Read(TRACK_Q7);
		break;
	case 1:
		gd=ExtSRAMInterface.ExMem_Read(TRACK_H8);
		break;
	case 2:
		gd=(ExtSRAMInterface.ExMem_Read(TRACK_H8)<<8)|ExtSRAMInterface.ExMem_Read(TRACK_Q7);
		break;
	}
	return gd;
}

/************************************************************************************************************
【函 数 名】：  CarTrack	寻迹函数
【参数说明】：	Car_Spend	速度
【返 回 值】：	无
【简    例】：	
************************************************************************************************************/
void _DCMotor::CarTrack(uint8_t Car_Spend)
{
    uint8_t H8_gd = 0;
	uint8_t Q7_gd = 0;
	uint16_t All_gd = 0;
	while (true)
	{
		Q7_gd = Get_Track_broad(0);
		H8_gd = Get_Track_broad(1);
		All_gd = Get_Track_broad(2);
		uint16_t LSpeed, RSpeed;		
		if(Q7_gd==0x00)   
		{
			Stop();
			Stop_Flag=0x01;
			break;
		}
		else if(All_gd== 0xFF7F)
		{	
			Stop();
			Stop_Flag=0x04;
			break;
		}
		else
		{
			if(H8_gd==0xE7||H8_gd==0xFF)
			{ 		
				LSpeed=Car_Spend;
				RSpeed=Car_Spend;	
			}
			else if(H8_gd==0XF7)		
			{
				LSpeed=Car_Spend+30;
				RSpeed=Car_Spend-40;
			}
			else if(H8_gd==0XEF)
			{
				LSpeed=Car_Spend-40;
				RSpeed=Car_Spend+30;	
			}	
			else if(H8_gd==0XF3||H8_gd==0XFB)
			{ 
				LSpeed=Car_Spend+30;
				RSpeed=Car_Spend-40;	
			}
			else if(H8_gd==0XF9||H8_gd==0XFD)
			{ 
				LSpeed=Car_Spend+60;
				RSpeed=Car_Spend-70;	
			}
			else if(H8_gd==0XFC)
			{ 
				LSpeed = Car_Spend+60;
				RSpeed = Car_Spend-90;	
			}
			else if(H8_gd==0XFE)		
			{ 
				LSpeed = Car_Spend+80;
				RSpeed = Car_Spend-120;			
			}
			else if(H8_gd==0XCF)
			{ 
				RSpeed = Car_Spend+30;
				LSpeed = Car_Spend-40;	
			} 
			else if(H8_gd==0X9F||H8_gd==0XDF) 
			{ 
				RSpeed = Car_Spend+60;
				LSpeed = Car_Spend-70; 
			} 			  
			else if(H8_gd==0X3F||H8_gd==0XBF)
			{ 
				RSpeed = Car_Spend+70;
				LSpeed = Car_Spend-90;	 
			} 
			else if(H8_gd==0X7F) 	
			{ 
				RSpeed = Car_Spend+80;
				LSpeed = Car_Spend-120;	 
			}
			SpeedCtr(LSpeed,RSpeed);	
		}
	}
}
/************************************************************************************************************
【函 数 名】：  CarTrack	寻迹函数指定距离
【参数说明】：	Car_Spend	速度		_distance 码盘
【返 回 值】：	无
【简    例】：	
************************************************************************************************************/
void _DCMotor::CarTrack(uint8_t Car_Spend,uint32_t _distance)
{
	uint32_t distance;
	while (ClearCodeDisc());
	uint32_t time=millis();
	uint8_t H8_gd = 0;
	uint8_t Q7_gd = 0;
	uint16_t All_gd = 0;
	while (true)
	{
		distance=uint32_t(ExtSRAMInterface.ExMem_Read(BASEADDRESS + CODEOFFSET) + (ExtSRAMInterface.ExMem_Read(BASEADDRESS + CODEOFFSET + 1) << 8));
		if((distance>=_distance)||(millis()-time)>=30000)
		{
			Stop();
			break;
		}
		else
		{
			Q7_gd = Get_Track_broad(0);
			H8_gd = Get_Track_broad(1);
			All_gd = Get_Track_broad(2);
			uint16_t LSpeed, RSpeed;		
			if(Q7_gd==0x00)   
			{
				Stop();
				Stop_Flag=0x01;
				break;
			}
			else if(All_gd== 0xFF7F)
			{	
				Stop();
				Stop_Flag=0x04;
				break;
			}
			else
			{
				if(H8_gd==0xE7||H8_gd==0xFF)
				{ 		
					LSpeed=Car_Spend;
					RSpeed=Car_Spend;	
				}
				else if(H8_gd==0XF7)		
				{
					LSpeed=Car_Spend+30;
					RSpeed=Car_Spend-40;
				}
				else if(H8_gd==0XEF)
				{
					LSpeed=Car_Spend-40;
					RSpeed=Car_Spend+30;	
				}	
				else if(H8_gd==0XF3||H8_gd==0XFB)
				{ 
					LSpeed=Car_Spend+30;
					RSpeed=Car_Spend-40;	
				}
				else if(H8_gd==0XF9||H8_gd==0XFD)
				{ 
					LSpeed=Car_Spend+60;
					RSpeed=Car_Spend-70;	
				}
				else if(H8_gd==0XFC)
				{ 
					LSpeed = Car_Spend+60;
					RSpeed = Car_Spend-90;	
				}
				else if(H8_gd==0XFE)		
				{ 
					LSpeed = Car_Spend+80;
					RSpeed = Car_Spend-120;			
				}
				else if(H8_gd==0XCF)
				{ 
					RSpeed = Car_Spend+30;
					LSpeed = Car_Spend-40;	
				} 
				else if(H8_gd==0X9F||H8_gd==0XDF) 
				{ 
					RSpeed = Car_Spend+60;
					LSpeed = Car_Spend-70; 
				} 			  
				else if(H8_gd==0X3F||H8_gd==0XBF)
				{ 
					RSpeed = Car_Spend+70;
					LSpeed = Car_Spend-90;	 
				} 
				else if(H8_gd==0X7F) 	
				{ 
					RSpeed = Car_Spend+80;
					LSpeed = Car_Spend-120;	 
				}
				SpeedCtr(LSpeed,RSpeed);
			}
		}	
	}
}


/************************************************************************************************************
【函 数 名】： Find_Line	寻线函数
【参数说明】：	无
【返 回 值】：	无
【简    例】：	
************************************************************************************************************/
void _DCMotor::Find_Line(uint8_t Car_Spend)
{
	uint8_t H8_gd = 0;
	while (true)
	{
		H8_gd = Get_Track_broad(1);	
		if(H8_gd==0XE7) 
		{ 		
			Stop();
			break;
		}
		else if(H8_gd==0XF7)		
		{
			Stop();
			break;
		}
		else if(H8_gd==0XEF)
		{
			Stop();
			break;
		}	
		else if(H8_gd==0XF3||H8_gd==0XFB)
		{ 
			Stop();
			break;
		}
		else if(H8_gd==0XF9||H8_gd==0XFD)
		{ 
			Stop();
			break;	 
		}
		else if(H8_gd==0XCF)
		{ 
			Stop();
			break;
		} 
		else if(H8_gd==0X9F||H8_gd==0XDF) 
		{ 
			Stop();
			break;
		} 			  
		else
		{
			SpeedCtr(Car_Spend,Car_Spend);
		}
	}			
}

/************************************************************************************************************
【函 数 名】：  CarTrack_G	寻迹函数 带前进一小步
【参数说明】：	Car_Spend  速度
【返 回 值】：	无
【简    例】：	
************************************************************************************************************/
void _DCMotor::CarTrack_Go(uint8_t Car_Spend)
{
	CarTrack(Car_Spend);
	delay(5);
	if(Stop_Flag==0x01)
	{
		Stop_Flag=0x00;
		Go(80,350);
	}
}


uint8_t _DCMotor::SearchBit(uint8_t mode,uint8_t s)
{
	if (mode == 1)
	{
		for (size_t i = 0x80; i > 0x00; i >>= 1)
		{
			if ((s & i) == 0)	return i;
		}
		return 0;
	}
	else
	{
		for (size_t i = 0x01; i < 0x100; i <<= 1)
		{
			if ((s & i) == 0)	return i;
		}
		return 0xff;
	}
}



/************************************************************************************************************
【函 数 名】：  RightMotorSpeed	设置PWM波的占空比，频率保持最后一次设置值  通道A-----pin6，通道B-----pin7
【参数说明】：	_Fduty	：设置通道 A 输出PWM波的占空比
				_Bduty	：设置通道 B 输出PWM波的占空比
【返 回 值】：	无
【简    例】：	RightMotorSpeed(70,0);
************************************************************************************************************/
void _DCMotor::MotorSpeed(uint8_t runmode, int8_t l_speed, int8_t r_speed)
{
	// sbi(TCCR4A, COM4C1);
	// sbi(TCCR4A, COM4B1);
	// sbi(TCCR3A, COM3C1);
	// sbi(TCCR3A, COM3B1);
	switch (runmode)
	{
	case MOVE_RUNMODE_STOP:
		PORTE &= ~(_BV(PE4) | _BV(PE5));
		PORTH &= ~(_BV(PH4) | _BV(PH5));
		OCR4C = 0x0000;
		OCR4B = 0x0000;
		OCR3C = 0x0000;
		OCR3B = 0x0000;
		break;
	case MOVE_RUNMODE_FF:
		OCR4C = 0;
		OCR4B = fHz * l_speed / 100;
		OCR3C = 0;
		OCR3B = fHz * r_speed / 100;
		break;
	case MOVE_RUNMODE_BB:
		OCR4B = 0;
		OCR4C = fHz * l_speed / 100;
		OCR3B = 0;
		OCR3C = fHz * r_speed / 100;
		break;
	case MOVE_RUNMODE_LIFT:
		OCR4C = 0;
		OCR4B = fHz * l_speed / 100;
		OCR3B = 0;
		OCR3C = fHz * r_speed / 100;
		break;
	case MOVE_RUNMODE_RIGHT:
		OCR4B = 0;
		OCR4C = fHz * l_speed / 100;
		OCR3C = 0;
		OCR3B = fHz * r_speed / 100;
		break;
	case MOVE_RUNMODE_TRACK:

		break;
	}
}

/************************************************************************************************************
【函 数 名】：  RightMotorFrequency	设置PWM波的频率，占空比保持最后一次设置值  通道A-----pin6，通道B-----pin7
【参数说明】：	_fHz	：设置通道 A/B 输出PWM波的频率
【返 回 值】：	无
【简    例】：	RightMotorFrequency(7000);	//频率范围为245Hz～8MHz
************************************************************************************************************/
void _DCMotor::MotorFrequency(uint32_t _fHz)
{
	fHz = 16000000 / _fHz;
	TCCR4B &= ~(_BV(CS40) | _BV(CS41) | _BV(CS42));
	TCCR4B |= _BV(CS40);
	TCCR3B &= ~(_BV(CS30) | _BV(CS31) | _BV(CS32));
	TCCR3B |= _BV(CS30);

	ICR4 = fHz;
	ICR3 = fHz;
}

/***************************************************END*****************************************************/


/***************************Timer5设置Fast PWM工作模式，以下是一些设置函数**********************************/

/************************************************************************************************************
【函 数 名】：  LeftMotorSpeed	设置PWM波的占空比，频率保持最后一次设置值  通道A-----pin46，通道B-----pin45
【参数说明】：	_front_speed	：设置通道 A 输出PWM波的占空比
				_behind_speed	：设置通道 B 输出PWM波的占空比
【返 回 值】：	无
【简    例】：	LeftMotorSpeed(70,50);
************************************************************************************************************/
void _DCMotor::LeftMotorSpeed(uint8_t _front_speed, uint8_t _behind_speed)
{
	//LfHz = TCNT3;
	LFSpeed = _front_speed;
	LBSpeed = _behind_speed;
	if ((_front_speed == 100) && (_behind_speed == 100))
	{
		sbi(TCCR3A, COM3C1);
		sbi(TCCR3A, COM3B1);
		OCR3C = 0xffff;// LfHz;
		OCR3B = 0xffff;// LfHz;
	}
	else
	{
		if (_front_speed >= 100)
		{
			sbi(TCCR3A, COM3C1);
			OCR3C = 0xffff;
		}
		else if (_front_speed <= 0)
		{
			sbi(TCCR3A, COM3C1);
			OCR3C = 0;
		}
		else
		{
			sbi(TCCR3A, COM3C1);
			OCR3C = LfHz * _front_speed / 100;
		}

		if (_behind_speed >= 100)
		{
			sbi(TCCR3A, COM3B1);
			OCR3B = 0xffff;

		}
		else if(_behind_speed <= 0)
		{
			sbi(TCCR3A, COM3B1);
			OCR3B = 0;

		}
		else
		{
			sbi(TCCR3A, COM3B1);
			OCR3B = LfHz * _behind_speed / 100;
		}

	}


}

/************************************************************************************************************
【函 数 名】：  LeftMotorFrequency	设置PWM波的频率，占空比保持最后一次设置值  通道A-----pin46，通道B-----pin45
【参数说明】：	_fHz	：设置通道 A/B 输出PWM波的频率
【返 回 值】：	无
【简    例】：	LeftMotorFrequency(7000);
************************************************************************************************************/
void _DCMotor::LeftMotorFrequency(uint32_t _fHz)
{
	LfHz = _fHz;

	if (_fHz > 1000000)	//Ƶ�����Ϊ1MHz
	{
		TCCR3B &= ~(_BV(CS30) | _BV(CS31) | _BV(CS32));
		TCCR3B |= _BV(CS30);
		ICR3 = 16;
		LfHz = 16;
		LeftMotorSpeed(LFSpeed, LBSpeed);
	}
	else if (_fHz < 30)	//Ƶ����СΪ30Hz
	{
		TCCR3B &= ~(_BV(CS30) | _BV(CS31) | _BV(CS32));
		TCCR3B |= _BV(CS31);
		ICR3 = 0xffff;
		LfHz = 0xffff;
		LeftMotorSpeed(LFSpeed, LBSpeed);
	}
	else				//Ƶ�ʷ�ΧΪ30Hz~1MHz
	{
		LfHz = 16000000 / _fHz;
		if (LfHz >= 0xffff)
		{
			TCCR3B &= ~(_BV(CS30) | _BV(CS31) | _BV(CS32));
			TCCR3B |= _BV(CS31);
			LfHz = 2000000 / _fHz;
			ICR3 = LfHz;
			LeftMotorSpeed(LFSpeed, LBSpeed);
		}
		else
		{
			TCCR3B &= ~(_BV(CS30) | _BV(CS31) | _BV(CS32));
			TCCR3B |= _BV(CS30);
			/*LfHz = 16000000 / _fHz;*/
			ICR3 = LfHz;
			LeftMotorSpeed(LFSpeed, LBSpeed);
		}
	}
}


uint8_t _DCMotor::ShiftBitAdd(uint8_t n)
{
	uint8_t bit = 0;
	for (uint8_t i = 0; i < 8; i++)
	{
		/*if (((n >> i) & 0x01) == 0x01)	bit ++;*/
		bit += bitRead(n, i);
	}
	return bit;
}

uint8_t _DCMotor::JudgeAround(uint8_t n)
{
	for (uint8_t i = 0; i < 4; i++)
	{
		if ((((n >> 3) & 0x01) == 0x00) && (((n >> 4) & 0x01) == 0x00))
		{
			return 8;
		}
		else if (((n >> (3 - i)) & 0x01) == 0x00)
		{
			return (3 - i);
		}
		else if (((n >> (4 + i)) & 0x01) == 0x00)
		{
			return (4 + i);
		}
	}
	return 9;
}
