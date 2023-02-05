#include "Jiao_Tong.h"

void Jian_Tong_Send_A(u8 *data)
{
    u8 B_Data[8] = {0x55, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBB};
    B_Data[2] = data[0];
    B_Data[3] = data[1];
    B_Data[4] = data[2];
    B_Data[5] = data[3];
    _Checksum_(B_Data);
    Send_ZigbeeData_To_Fifo(B_Data, 8);
    delay_ms(300);
    Send_ZigbeeData_To_Fifo(B_Data, 8);
    delay_ms(300);
    Send_ZigbeeData_To_Fifo(B_Data, 8);
}

void Jiao_Tong_Reco_A(void)
{
    u8 data[] = {0x01, 0x00, 0x00, 0x00};
    Jian_Tong_Send_A(data);
}

void Jiao_Tong_Color_A(_Jiao_Tong_Color_ Color)
{
    u8 data[4] = {0x02, 0x00, 0x00, 0x00};
    data[1] = Color;
    Jian_Tong_Send_A(data);
}

void Jiao_Tong_Red_A(void)
{
    Jiao_Tong_Color_A(Red);
}

void Jiao_Tong_Green_A(void)
{
    Jiao_Tong_Color_A(Green);
}

void Jiao_Tong_Yellow_A(void)
{
    Jiao_Tong_Color_A(Yellow);
}

void Jian_Tong_Send_B(u8 *data)
{
    u8 B_Data[8] = {0x55, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBB};
    B_Data[2] = data[0];
    B_Data[3] = data[1];
    B_Data[4] = data[2];
    B_Data[5] = data[3];
    _Checksum_(B_Data);
    Send_ZigbeeData_To_Fifo(B_Data, 8);
    delay_ms(300);
    Send_ZigbeeData_To_Fifo(B_Data, 8);
    delay_ms(300);
    Send_ZigbeeData_To_Fifo(B_Data, 8);
}

void Jiao_Tong_Reco_B(void)
{
    u8 data[] = {0x01, 0x00, 0x00, 0x00};
    Jian_Tong_Send_B(data);
}

void Jiao_Tong_Color_B(_Jiao_Tong_Color_ Color)
{
    u8 data[4] = {0x02, 0x00, 0x00, 0x00};
    data[1] = Color;
    Jian_Tong_Send_B(data);
}

void Jiao_Tong_Red_B(void)
{
    Jiao_Tong_Color_B(Red);
}

void Jiao_Tong_Green_B(void)
{
    Jiao_Tong_Color_B(Green);
}

void Jiao_Tong_Yellow_B(void)
{
    Jiao_Tong_Color_B(Yellow);
}


void Jiao_Tong_CallBackFun(uint8_t *data)
{
    if(data[4] == 0x07)
    {

    }
    else if(data[4] == 0x08)
    {

    }
}

