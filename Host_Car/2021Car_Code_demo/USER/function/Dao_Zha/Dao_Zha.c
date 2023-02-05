#include "Dao_Zha.h"

void Dao_zha_Send(u8 *Data)
{
    u8 data[8] = {0x55, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBB};
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

void Dao_Zha_Open(void)
{
    u8 data[4] = {0x01, 0x01, 0x00, 0x00};
    Dao_zha_Send(data);
}

void Dao_Zha_close(void)
{
    u8 data[4] = {0x01, 0x02, 0x00, 0x00};
    Dao_zha_Send(data);
}

void Dao_Zha_Angle_Up(void)
{
    u8 data[4] = {0x09, 0x01, 0x00, 0x00};
    Dao_zha_Send(data);
}

void Dao_Zha_Angle_Down(void)
{
    u8 data[4] = {0x09, 0x02, 0x00, 0x00};
    Dao_zha_Send(data);
}

void Dao_Zha_CarID(u8 *CarID)
{
    u8 data[4] = {0x10, 0x00, 0x00, 0x00};
    data[1] = CarID[0];
    data[2] = CarID[1];
    data[3] = CarID[2];
    Dao_zha_Send(data);
    data[0] = 0x11;
    data[1] = CarID[3];
    data[2] = CarID[4];
    data[3] = CarID[5];
    Dao_zha_Send(data);
}

void Dao_Zha_Call(void)
{
    u8 data[4] = {0x20, 0x01, 0x00, 0x00};
    Dao_zha_Send(data);
}

void Dao_Zha_CallBackFun(uint8_t *data)
{
    if(data[4] == 0x05) //µÀÕ¢¿ªÆô
    {
        Dao_Zha_Flag = 1;
    }
}

void Dao_Zha_pass(u8 *CarID)
{
    while(Dao_Zha_Flag != 1)
    {
        Dao_Zha_CarID(CarID);
        delay_ms(100);
        Dao_Zha_Call();
        delay_ms(100);
    }

    Dao_Zha_Flag = 0;

}


