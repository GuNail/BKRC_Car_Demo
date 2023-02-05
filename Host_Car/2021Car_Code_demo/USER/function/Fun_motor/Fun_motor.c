#include "Fun_motor.h"
#include "roadway_check.h"
#include "Rc522.h"
#include <stdio.h>
#include "plan.h"
/**小车停止*/
void Stop(void)
{
    Send_UpMotor(0, 0);
    Roadway_Flag_clean();
}


/**寻迹指定距离（可被打断）*/
void Track_Mp(int sp, uint16_t mp)
{
    Stop();
    Roadway_mp_syn();	//码盘同步

    if(sp >= 0)
        Track_Go_Mp_Flag = 1;
    else
        Track_Back_Mp_Flag = 1;

    MP_Track_flag = 1;
    Car_Spend = sp;
    temp_MP = mp;
}

/**寻迹（可被打断）**/

void Track(int sp)
{
    Stop();

    if(sp >= 0)
        Track_Go_Flag = 1;
    else
        Track_Back_Flag = 1;

    Track_Flag = 1;
    Car_Spend = sp;
}
/**前进后退（可被打断）**/
void Go_Back(int sp, uint16_t mp)
{
    Stop();
    Roadway_mp_syn();	//码盘同步

    if(sp >= 0)
        Go_Flag = 1;
    else
        Back_Flag = 1 ;

    temp_MP = mp;
    Car_Spend = sp;
    Control(Car_Spend, Car_Spend);
}

void LeftX(uint8_t sp, uint16_t angle)
{
    Stop();
    Roadway_mp_syn();	//码盘同步
    wheel_Nav_Flag = 1;
    temp_Nav = angle * 10;
    Car_Spend = sp;
    Control(-Car_Spend, Car_Spend);
}

void RightX(uint8_t sp, uint16_t angle)
{
    Stop();
    Roadway_mp_syn();	//码盘同步
    wheel_Nav_Flag = 1;
    temp_Nav = angle * 10;
    Car_Spend = sp;
    Control(Car_Spend, -Car_Spend);
}

void Track_wheel_L(uint8_t sp)
{
    Stop();
    wheel_L_Flag = 1;
    Car_Spend = sp;
    Control(-Car_Spend, Car_Spend);
}

void Track_wheel_R(uint8_t sp)
{
    Stop();
    wheel_R_Flag = 1;
    Car_Spend = sp;
    Control(Car_Spend, -Car_Spend);
}





void Fun_Go_Back(int sp, uint16_t mp)
{
    Stop();
    Roadway_mp_syn();	//码盘同步

    if(sp >= 0)
        Go_Flag = 1;
    else
        Back_Flag = 1;

    temp_MP = mp;
    Car_Spend = sp;
    Control(Car_Spend, Car_Spend);

    while(Stop_Flag != 0x03);
}

void Fun_LeftX(uint8_t sp, uint16_t angle)
{
    Stop();
    Roadway_mp_syn();	//码盘同步
    wheel_Nav_Flag = 1;
    temp_Nav = angle * 10;
    Car_Spend = sp;
    Control(-Car_Spend, Car_Spend);

    while(Stop_Flag != 0x02);
}

void Fun_RightX(uint8_t sp, uint16_t angle)
{
    Stop();
    Roadway_mp_syn();	//码盘同步
    wheel_Nav_Flag = 1;
    temp_Nav = angle * 10;
    Car_Spend = sp;
    Control(Car_Spend, -Car_Spend);

    while(Stop_Flag != 0x02);
}

void Fun_Track_wheel_L(uint8_t sp)
{
    Stop();
    wheel_L_Flag = 1;
    Car_Spend = sp;
    Control(-Car_Spend, Car_Spend);

    while(Stop_Flag != 0x02);
}

void Fun_Track_wheel_R(uint8_t sp)
{
    Stop();
    wheel_R_Flag = 1;
    Car_Spend = sp;
    Control(Car_Spend, -Car_Spend);

    while(Stop_Flag != 0x02);
}

void Fun_Track(int sp)
{
    Stop();

    if(sp >= 0)
        Track_Go_Flag = 1;
    else
        Track_Back_Flag = 1;

    Track_Flag = 1;
    Car_Spend = sp;

    while((Stop_Flag != 0x01) && (Stop_Flag != 0x04));
}

void Fun_Track_Mp(int sp, uint16_t mp)
{
    Stop();
    Roadway_mp_syn();

    if(sp >= 0)
        Track_Go_Mp_Flag = 1;
    else
        Track_Back_Mp_Flag = 1;

    MP_Track_flag = 1;
    Car_Spend = sp;
    temp_MP = mp;

    while((Stop_Flag != 0x01) && (Stop_Flag != 0x03) && (Stop_Flag != 0x04));
}

void Fun_Card_Track(uint8_t sp, uint16_t mp)
{
    Stop();
    Roadway_mp_syn();
    Track_Card_Flag = 1;
    Car_Spend = sp;
    temp_MP = mp;

    while((Stop_Flag != 0x01) && (Stop_Flag != 0x03));
}

void Fun_Track_Go(void)
{
    Fun_Track(60);
    delay_ms(1);

    if(Stop_Flag == 0x01)
    {
        Fun_Go_Back(60, 380);
    }
}


void Find_Line()
{
    Stop();
    Find_Line_Track_Flag = 1;
    Car_Spend = 60;

    while(Find_Line_Track_Flag);
}

/***
寻迹修正
	i 速度上下限
	time 延时时间

	Go 前进值  二选一
	back 后退值

	cont 修正次数
*/

void Fun_Track_Amend(int i, uint8_t time, uint8_t Go, uint8_t back, uint8_t cont)
{
    for(cont = 0; cont < 3; cont++)
    {
        for(int sp = i; sp >= 0; sp--)
        {
            delay_ms(time + Go);
            Track(sp);
        }

        for(int sp = -i; sp <= 0; sp++)
        {
            delay_ms(time + back);
            Track(sp);
        }
    }

    Stop();
}




