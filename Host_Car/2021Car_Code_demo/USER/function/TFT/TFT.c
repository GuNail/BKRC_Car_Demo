#include "TFT.h"
#include <stdio.h>

void TFT_Send_A(u8 *Data)
{
    u8 data[8] = {0x55, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBB};
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

void TFT_Choose_Photo_A(_TFT_Photo_typedef Photo)
{
    u8 data[4] = {0x10, 0x00, 0x00, 0x00};
    data[2] = Photo;
    TFT_Send_A(data);
}

void TFT_Next_Photo_A(void)
{
    u8 data[4] = {0x10, 0x01, 0x00, 0x00};
    TFT_Send_A(data);
}

void TFT_Last_Photo_A(void)
{
    u8 data[4] = {0x10, 0x02, 0x00, 0x00};
    TFT_Send_A(data);
}

void TFT_Next_Photo_aout_A(void)
{
    u8 data[4] = {0x10, 0x03, 0x00, 0x00,};
    TFT_Send_A(data);
}

void TFT_CarID_A(u8 *CarID)
{
    u8 data[4] = {0x20, 0x00, 0x00, 0x00};
    data[1] = CarID[0];
    data[2] = CarID[1];
    data[3] = CarID[2];
    TFT_Send_A(data);
    data[0] = 0x21;
    data[1] = CarID[3];
    data[2] = CarID[4];
    data[3] = CarID[5];
    TFT_Send_A(data);
}

void TFT_TimeStop_A(void)
{
    u8 data[4] = {0x30, 0x00, 0x00, 0x00,};
    TFT_Send_A(data);
}

void TFT_TimeStart_A(void)
{
    u8 data[4] = {0x30, 0x01, 0x00, 0x00,};
    TFT_Send_A(data);
}

void TFT_TimeClean_A(void)
{
    u8 data[4] = {0x30, 0x02, 0x00, 0x00,};
    TFT_Send_A(data);
}

void TFT_Display_HEX_A(u8 *Hex)
{
    u8 data[4] = {0x40, 0x00, 0x00, 0x00};
    data[1] = Hex[0];
    data[2] = Hex[1];
    data[3] = Hex[2];
    TFT_Send_A(data);
}

void TFT_Distan_A(u8 *Distan)
{
    u8 data[4] = {0x50, 0x00, 0x00, 0x00};
    data[2] = Distan[0];
    data[3] = Distan[1];
    TFT_Send_A(data);
}

void TFT_Distan_num_A(uint16_t data)
{
    u8 Data[4] = {0x50, 0x00, 0x00, 0x00};
    char num[10];
    int f_num;

    sprintf(num, "0x%d", data);
    sscanf(num, "%x", &f_num);


    Data[2] = f_num >> 8;
    Data[3] = f_num & 0xFF;
    TFT_Send_A(Data);
}

void TFT_Trffic_Mode_A(_TFT_Trffic_Typedef mode)
{
    u8 data[] = {0x60, mode, 0x00, 0x00};
    TFT_Send_A(data);
}


void TFT_ultrasonic_A(void)
{
    for(int i = 0; i < 5; i++)
    {
        Ultrasonic_Ranging();
        delay_ms(100);
    }

    TFT_Distan_num_A(dis);
    printf("%d\n", dis);
}


void TFT_Send_B(u8 *Data)
{
    u8 data[8] = {0x55, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBB};
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

void TFT_Choose_Photo_B(_TFT_Photo_typedef Photo)
{
    u8 data[4] = {0x10, 0x00, 0x00, 0x00};
    data[2] = Photo;
    TFT_Send_B(data);
}

void TFT_Next_Photo_B(void)
{
    u8 data[4] = {0x10, 0x01, 0x00, 0x00};
    TFT_Send_B(data);
}

void TFT_Last_Photo_B(void)
{
    u8 data[4] = {0x10, 0x02, 0x00, 0x00};
    TFT_Send_B(data);
}

void TFT_Next_Photo_aout_B(void)
{
    u8 data[4] = {0x10, 0x03, 0x00, 0x00,};
    TFT_Send_B(data);
}

void TFT_CarID_B(u8 *CarID)
{
    u8 data[4] = {0x20, 0x00, 0x00, 0x00};
    data[1] = CarID[0];
    data[2] = CarID[1];
    data[3] = CarID[2];
    TFT_Send_B(data);
    data[0] = 0x21;
    data[1] = CarID[3];
    data[2] = CarID[4];
    data[3] = CarID[5];
    TFT_Send_B(data);
}

void TFT_TimeStop_B(void)
{
    u8 data[4] = {0x30, 0x00, 0x00, 0x00,};
    TFT_Send_B(data);
}

void TFT_TimeStart_B(void)
{
    u8 data[4] = {0x30, 0x01, 0x00, 0x00,};
    TFT_Send_B(data);
}

void TFT_TimeClean_B(void)
{
    u8 data[4] = {0x30, 0x02, 0x00, 0x00,};
    TFT_Send_B(data);
}

void TFT_Display_HEX_B(u8 *Hex)
{
    u8 data[4] = {0x40, 0x00, 0x00, 0x00};
    data[1] = Hex[0];
    data[2] = Hex[1];
    data[3] = Hex[2];
    TFT_Send_B(data);
}

void TFT_Distan_B(u8 *Distan)
{
    u8 data[4] = {0x50, 0x00, 0x00, 0x00};
    data[2] = Distan[0];
    data[3] = Distan[1];
    TFT_Send_B(data);
}

void TFT_Distan_num_B(uint16_t data)
{
    u8 Data[4] = {0x50, 0x00, 0x00, 0x00};
    char num[10];
    int f_num;

    sprintf(num, "0x%d", data);
    sscanf(num, "%x", &f_num);

    Data[2] = f_num >> 8;
    Data[3] = f_num & 0xFF;
    TFT_Send_B(Data);
}

void TFT_Trffic_Mode_B(_TFT_Trffic_Typedef mode)
{
    u8 data[] = {0x60, mode, 0x00, 0x00};
    TFT_Send_A(data);
}

void TFT_ultrasonic_B(void)
{
    for(int i = 0; i < 5; i++)
    {
        Ultrasonic_Ranging();
        delay_ms(100);
    }

    TFT_Distan_num_B(dis);
    DEBUG_LOG("%d mm", dis);
}




