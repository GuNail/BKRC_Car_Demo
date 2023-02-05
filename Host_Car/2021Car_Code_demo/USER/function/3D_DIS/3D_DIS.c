#include "3D_DIS.h"

void _3D_DIS_Send(u8 *Data)
{
    u8 data[6] = {0xFF, 0x00, 0x00, 0x00, 0x00, 0x00};
    data[1] = Data[0];
    data[2] = Data[1];
    data[3] = Data[2];
    data[4] = Data[3];
    data[5] = Data[4];
    delay_ms(1);
    Infrared_Send(data, 6);
    delay_ms(300);
    Infrared_Send(data, 6);
    delay_ms(300);
    Infrared_Send(data, 6);
}

void _3D_DIS_CarID(u8 *CarID)
{
    u8 Data[5] = {0x20, 0x00, 0x00, 0x00, 0x00};
    Data[1] = CarID[0];
    Data[2] = CarID[1];
    Data[3] = CarID[2];
    Data[4] = CarID[3];
    _3D_DIS_Send(Data);
    Data[0] = 0x10;
    Data[1] = CarID[4];
    Data[2] = CarID[5];
//	Data[3]=CarID[6];
//	Data[4]=CarID[7];
    Data[3] = 0;
    Data[4] = 0;
    _3D_DIS_Send(Data);
}

void _3D_DIS_Distan(u8 *distan)
{
    u8 Data[5] = {0x11, 0x00, 0x00, 0x00, 0x00};
    Data[1] = distan[0];
    Data[2] = distan[1];
    _3D_DIS_Send(Data);
}
/*
立体显示距离 单位毫米
*/
void _3D_DIS_Distan_num(uint16_t mm)
{
    u8 Data[5] = {0x11, 0x00, 0x00, 0x00, 0x00};
    char num[2];
    num[0] = (mm / 10) + 0x30;
    num[1] = (mm % 10) + 0x30;

    Data[1] = num[0];
    Data[2] = num[1];
    _3D_DIS_Send(Data);
}

void _3D_DIS_ultrasonic(void)
{
    for(int i = 0; i < 5; i++)
    {
        Ultrasonic_Ranging();
        delay_ms(100);
    }

    _3D_DIS_Distan_num(dis);
    DEBUG_LOG("%d mm", dis);
}

void _3D_DIS_Graph(_3D_DIS_Graph_MODE Graph)
{
    u8 Data[5] = {0x12, 0x00, 0x00, 0x00, 0x00};
    Data[1] = Graph;
    _3D_DIS_Send(Data);
}

void _3D_DIS_Color(uint8_t Color)
{
    u8 Data[5] = {0x13, 0x00, 0x00, 0x00, 0x00};
    Data[1] = Color;
    _3D_DIS_Send(Data);
}

void _3D_DIS_Trffic_Warning(uint8_t mode)
{
    u8 Data[5] = {0x14, mode, 0x00, 0x00, 0x00};
    _3D_DIS_Send(Data);
}

void _3D_DIS_Trffic_Tips(uint8_t mode)
{
    u8 Data[5] = {0x15, mode, 0x00, 0x00, 0x00};
    _3D_DIS_Send(Data);
}

void _3D_DIS_default(void)
{
    u8 Data[5] = {0x16, 0x01, 0x00, 0x00, 0x00};
    _3D_DIS_Send(Data);
}

void _3d_DIS_Text_Color(uint8_t hex_R, uint8_t hex_G, uint8_t hex_B)
{
    u8 Data[5] = {0x17, 0x01, hex_R, hex_G, hex_B};
    _3D_DIS_Send(Data);
}




