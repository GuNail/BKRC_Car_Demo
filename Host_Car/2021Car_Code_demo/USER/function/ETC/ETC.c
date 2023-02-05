#include "ETC.h"
#include "Fun_motor.h"
#include <stdio.h>

void ETC_Send(u8 *Data)
{
    u8 data[8] = {0x55, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBB};
    data[2] = Data[0];
    data[3] = Data[1];
    data[4] = Data[2];
    data[5] = Data[3];
    _Checksum_(data);
    Send_ZigbeeData_To_Fifo(data, 8);
    delay_ms(300);
    Send_ZigbeeData_To_Fifo(data, 8);
    delay_ms(300);
    Send_ZigbeeData_To_Fifo(data, 8);
}

void ETC_Start_Angle(_ETC_Typedef Left, _ETC_Typedef Right)
{
    u8 data[] = {0x08, Left, Right, 0x00};
    ETC_Send(data);
}

void ETC_CallBackFun(uint8_t *data)
{
    if(data[4] == 0x06) //ETC ¿ªÆô
    {
        ETC_Flag = 1;
    }
}

void ETC_Pass(void)
{
    while(ETC_Flag != 1)
    {
        for(int sp = 50; sp >= 0; sp--)
        {
            delay_ms(10);
            Track(sp);
        }

        for(int sp = -50; sp <= 0; sp++)
        {
            delay_ms(10);
            Track(sp);
        }
    }

    ETC_Flag = 0;
}
