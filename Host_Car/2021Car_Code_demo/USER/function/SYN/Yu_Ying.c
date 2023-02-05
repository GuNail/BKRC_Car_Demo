#include "Yu_Ying.h"

void Yu_Ying_Send(u8 *Data)
{
    u8 data[8] = {0x55, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBB};
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

void Yu_Ying_Specify(_Yu_Ying_Specify Specify)
{
    u8 data[4] = {0x10, 0x00, 0x00, 0x00};
    data[1] = Specify;
    Yu_Ying_Send(data);
}

void Yu_Ying_Random(void)
{
    u8 data[4] = {0x20, 0x01, 0x00, 0x00};
    Yu_Ying_Send(data);
}

void Yu_Ying_Set_RTC_Date(uint8_t hex_year, uint8_t hex_month, uint8_t hex_day)
{
    u8 data[4] = {0x30, hex_year, hex_month, hex_day};
    Yu_Ying_Send(data);
}

void Yu_Ying_Get_RTC_Date(void)
{
    u8 data[4] = {0x31, 0x01, 0x00, 0x00};
    Yu_Ying_Send(data);
}

void Yu_Ying_Set_RTC_Time(uint8_t hex_hour, uint8_t hex_min, uint8_t hex_sec)
{
    u8 data[4] = {0x40, hex_hour, hex_min, hex_sec};
    Yu_Ying_Send(data);
}

void Yu_Ying_Get_RTC_Time(void)
{
    u8 data[4] = {0x41, 0x01, 0x00, 0x00};
    Yu_Ying_Send(data);
}

void Yu_Ying_Play(uint8_t* strPut)
{
    uint32_t len = strlen((char*)strPut) + 2;
    uint8_t Frame[5] = {0xFD, len >> 8, len & 0xFF, 0x01, 0x00};

    Send_ZigbeeData_To_Fifo(Frame, 5);
    Send_ZigbeeData_To_Fifo(strPut, len);
    delay_ms(100);
    Send_ZigbeeData_To_Fifo(Frame, 5);
    Send_ZigbeeData_To_Fifo(strPut, len);
    delay_ms(100);
}






