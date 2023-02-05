#include "AGV_Car.h"
#include "plan.h"
#include "data_base.h"
#include <stdio.h>


void Send_To_AGV_Car(uint8_t first, uint8_t commd1, uint8_t commd2, uint8_t commd3)
{
    uint8_t data[] = {0x55, 0x02, first, commd1, commd2, commd3, 0x00, 0xBB};
    Send_ZigbeeData_To_Fifo(data, 8);
    delay_ms(300);
    Send_ZigbeeData_To_Fifo(data, 8);
    delay_ms(300);
    Send_ZigbeeData_To_Fifo(data, 8);
    delay_ms(300);
}

void AGV_Car_Start(void)
{
    Send_To_AGV_Car(0x1A, 0, 0, 0);
}

void AGV_Car_Start_Rode(void)
{
    Send_To_AGV_Car(0x1B, 0, 0, 0);
}

void AGV_Car_Rode1(void)
{
    Send_To_AGV_Car(0x1C, 0, 0, 0);
}

void AGV_Car_Rode2(void)
{
    Send_To_AGV_Car(0x2C, 0, 0, 0);
}

void AGV_Car_Rode3(void)
{
    Send_To_AGV_Car(0x3C, 0, 0, 0);
}

void AGV_Car_Zigbee()
{
    if(AGV_data_Falg == SET)
    {
        AGV_data_Falg = RESET;

        switch (Follower_Tab[2])
        {
            case 0xA1:
                AGV_Car_Rode1();
                delay_ms(500);
                AGV_Car_Start();
                delay_ms(500);
                break;

            case 0xA2:
                Host_Car_Run_Flag = 1; //Ö÷³µÆô¶¯
                printf("Test\n");
                break;
        }

    }
}


