#include "LED_DIS.h"
#include "ultrasonic.h"
#include "DEBUG.h"
#include <stdio.h>


/*
LED发送数据
*/
void LED_DIS_Send(u8 *Data)
{
    u8 data[8] = {0x55, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBB};
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

/*
LED数据写入第一排数码管
data[3]={0xA1,0xB2,0xC3}
则显示 A1B2C3
*/

void LED_DIS_1seg(u8 *data)
{
    u8 Data[4] = {0x01, 0x00, 0x00, 0x00};
    Data[1] = data[0];
    Data[2] = data[1];
    Data[3] = data[2];
    LED_DIS_Send(Data);
}


/*
LED数据写入第二排数码管
data[3]={0xA1,0xB2,0xC3}
则显示 A1B2C3
*/

void LED_DIS_2seg(u8 *data)
{
    u8 Data[4] = {0x02, 0x00, 0x00, 0x00};
    Data[1] = data[0];
    Data[2] = data[1];
    Data[3] = data[2];
    LED_DIS_Send(Data);
}

/*
LED停止计时
*/

void LED_DIS_TimeStop(void)
{
    u8 Data[4] = {0x03, 0x00, 0x00, 0x00};
    LED_DIS_Send(Data);
}

/*
LED开始计时
*/

void LED_DIS_TimeStart(void)
{
    u8 Data[4] = {0x03, 0x01, 0x00, 0x00};
    LED_DIS_Send(Data);
}

/*
LED计时清零
*/

void LED_DIS_TimeClear(void)
{
    u8 Data[4] = {0x03, 0x02, 0x00, 0x00};
    LED_DIS_Send(Data);
}


/*
LED显示距离，data参数为长度为二的数组
如data[2]={0x1,0x23};
则显示距离为123
*/
void LED_DIS_Distan(u8 *data)
{
    u8 Data[4] = {0x04, 0x00, 0x00, 0x00};
    Data[2] = data[0];
    Data[3] = data[1];
    LED_DIS_Send(Data);
}

/*
LED显示距离，data参数为0-999的数字
*/
void LED_DIS_Distan_num(uint16_t data)
{
    u8 Data[4] = {0x04, 0x00, 0x00, 0x00};
    char num[3];
    int f_num;

    if(data > 999) data = 999;

    num[0] = (data / 100) + 0x30;
    num[1] = ((data / 10) % 10) + 0x30;
    num[2] = (data % 10) + 0x30;

    f_num = Char_To_int(num);
    Data[2] = f_num >> 8;
    Data[3] = f_num & 0xFF;
    LED_DIS_Send(Data);
}

/*

超声波测距LED显示

*/
void LED_DIS_ultrasonic(void)
{
    for(int i = 0; i < 5; i++)
    {
        Ultrasonic_Ranging();
        delay_ms(100);
    }

    LED_DIS_Distan_num(dis);
}


