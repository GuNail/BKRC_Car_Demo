#include "plan.h"
#include "arm_math.h"
#include <stdio.h>
#include "stm32f4xx.h"
#include "delay.h"
#include "infrared.h"
#include "cba.h"
#include "ultrasonic.h"
#include "canp_hostcom.h"
#include "hard_can.h"
#include "bh1750.h"
#include "syn7318.h"
#include "power_check.h"
#include "can_user.h"
#include "data_base.h"
#include "roadway_check.h"
#include "tba.h"
#include "data_base.h"
#include "swopt_drv.h"
#include "uart_a72.h"
#include "Can_check.h"
#include "delay.h"
#include "can_user.h"
#include "Timer.h"
#include "Rc522.h"
/*******************************/
#include "Dao_Zha.h"
#include "LED_DIS.h"
#include "TFT.h"
#include "WU_Xian.h"
#include "Feng_huo.h"
#include "Che_Ku.h"
#include "Lu_Deng.h"
#include "DEBUG.h"
#include "3D_DIS.h"
#include "A72.h"
#include "ETC.h"
/*****************************/
#include "Fun_motor.h"
#include "AGV_Car.h"
#include "data_base.h"

volatile uint16_t Mark = 0;
volatile uint8_t  Host_Car_Run_Flag = 0;

uint8_t LED_Dis[] = {0xFF, 0xFF, 0x00};

extern uint8_t cheku;
extern uint8_t Voice_Flag;
int16_t Temp_mp[2];
uint8_t start_Gaer = 0;
uint8_t mGaer = 0;
int cont;
void plan()
{
    delay_ms(100);

    switch(Mark)
    {
        case 5:

            Fun_Track_Go();
            Mark = 6;
            break;

        case 6:
            Fun_Track_wheel_L(90);
            Mark = 7;
            break;

        case 7:

            Mark = 8;
            break;

        case 8:

            Mark = 9;
            break;

        case 9:
            Fun_Track(60);
            Mark = 10;
            break;

        case 10:

            Mark = 11;
            break;

        case 11:
            Fun_Go_Back(60, 380);
            Mark = 12;
            break;

        case 12:

            Mark = 13;
            break;

        case 13:
            Fun_Track_wheel_R(90);
            Mark = 14;
            break;

        case 14:
            Fun_Track_Go();
            Mark = 15;
            break;

        case 15:
            Fun_Track_wheel_R(90);
            Mark = 16;
            break;

        case 16:
            Fun_Track_Mp(60, 500);
            Mark = 17;
            break;

        case 17:
            Fun_Go_Back(-40, 500);
            Mark = 18;
            break;

        case 18:

            Mark = 19;
            break;

        case 19:
            Fun_Track_Go();

            Mark = 20;

            break;

        case 20:

            Mark = 21;
            break;

        case 21:

            Mark = 22;
            break;

        case 22:
            Fun_Track_Go();
            Mark = 23;
            break;

        case 23:

            Mark = 24;
            break;

        case 24:
            Fun_Track_wheel_L(90);
            Mark = 25;
            break;

        case 25:
            Fun_Track_Go();
            Mark = 26;
            break;

        case 26:
            Fun_Track_wheel_R(90);
            Mark = 27;
            break;

        case 27:
            Mark = 28;
            break;

        case 28:
            Fun_Track_wheel_L(90);
            Mark = 29;
            break;

        case 29:

            Mark = 30;
            break;

        case 30:
            Fun_Track_wheel_L(90);
            Mark = 31;
            break;

        case 31:

            Mark = 32;
            break;

        case 32:

            Mark = 33;
            break;

        case 33:

            Mark = 34;
            break;

        case 34:
            Fun_Track_Go();
            Mark = 35;
            break;

        case 35:
            Fun_Track_Go();
            Mark = 36;
            break;

        case 36:

            Mark = 37;
            break;

        case 37:

            Mark = 38;
            break;

        case 38:
            Fun_Track_wheel_R(90);
            Mark = 39;
            break;

        case 39:
            Mark = 40;
            break;

        case 40:
            Fun_Track_wheel_R(90);
            Mark = 41;
            break;

        case 41:
            Fun_RightX(90, 45);
            Mark = 42;
            break;

        case 42:
						Fun_RightX(90, 45);
            Mark = 43;
            break;

        case 43:
            
            Mark = 44;
            break;

        case 44:

            Mark = 45;
            break;

        case 45:
            Fun_Track_Go();
            Mark = 46;
            break;

        case 46:
            Fun_Track_Go();
            Mark = 47;
            break;

        case 47:
            Fun_Track_wheel_L(90);
            Mark = 48;
            break;

        case 48:

            Mark = 49;
            break;

        case 49:
            Fun_Track_wheel_L(90);
            Mark = 50;
            break;

        case 50:
            Fun_Track_Mp(60, 500);
            Mark = 51;
            break;

        case 51:
            Fun_Go_Back(-40, 500);
            Mark = 52;
            break;

        case 52:
            Fun_Track_Mp(60, 500);
            Mark = 53;
            break;

        case 53:
            Fun_Go_Back(-40, 1800);
            Mark = 54;
            break;

        case 54:

            Mark = 55;
            break;

        case 55:

            Mark = 56;
            break;

        case 56:

            Mark = 57;
            break;

        case 57:
            Mark = 0;
            Host_Car_Run_Flag = 0;
            break;
    }
}



















