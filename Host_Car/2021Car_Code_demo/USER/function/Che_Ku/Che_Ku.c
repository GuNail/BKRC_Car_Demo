#include "Che_Ku.h"
#include "Fun_motor.h"

void Che_Ku_Send_A(u8 *data)
{
    u8 B_data[8] = {0x55, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBB};
    B_data[2] = data[0];
    B_data[3] = data[1];
    B_data[4] = data[2];
    B_data[5] = data[3];
    _Checksum_(B_data);
    Send_ZigbeeData_To_Fifo(B_data, 8);
    delay_ms(300);
    Send_ZigbeeData_To_Fifo(B_data, 8);
    delay_ms(300);
    Send_ZigbeeData_To_Fifo(B_data, 8);
}

void Che_Ku_Floor_Choose_A(uint8_t Floor)
{
    u8 data[4] = {0x01, 0x00, 0x00, 0x00};
    data[1] = 0x01;
    Che_Ku_Send_A(data);
}

void Che_Ku_CallBack_Floor_A(void)
{
    u8 data[] = {0x55, 0x0D, 0x02, 0x01, 0x00, 0x00, 0x03, 0xBB};
    Send_ZigbeeData_To_Fifo(data, 8);
}

void Che_Ku_CallBack_IrF_A(void)
{
    u8 data[] = {0x55, 0x0D, 0x02, 0x02, 0x00, 0x00, 0x04, 0xBB};
    Send_ZigbeeData_To_Fifo(data, 8);
}

void Che_Ku_Floor_1_A(void)
{
    Che_Ku_Floor_Choose_A(Floor_1);
}

void Che_Ku_Floor_2_A(void)
{
    Che_Ku_Floor_Choose_A(Floor_2);
}

void Che_Ku_Floor_3_A(void)
{
    Che_Ku_Floor_Choose_A(Floor_3);
}

void Che_Ku_Floor_4_A(void)
{
    Che_Ku_Floor_Choose_A(Floor_4);
}

void Che_Ku_Send_B(u8 *data)
{
    u8 B_data[8] = {0x55, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBB};
    B_data[2] = data[0];
    B_data[3] = data[1];
    B_data[4] = data[2];
    B_data[5] = data[3];
    _Checksum_(B_data);
    Send_ZigbeeData_To_Fifo(B_data, 8);
    delay_ms(300);
    Send_ZigbeeData_To_Fifo(B_data, 8);
    delay_ms(300);
    Send_ZigbeeData_To_Fifo(B_data, 8);
}

void Che_Ku_Floor_Choose_B(_Floor_Typedef Floor )
{
    u8 data[4] = {0x01, 0x00, 0x00, 0x00};
    data[1] = Floor;
    Che_Ku_Send_B(data);
}

void Che_Ku_Floor_1_B(void)
{
    Che_Ku_Floor_Choose_B(Floor_1);
}

void Che_Ku_Floor_2_B(void)
{
    Che_Ku_Floor_Choose_B(Floor_2);
}

void Che_Ku_Floor_3_B(void)
{
    Che_Ku_Floor_Choose_B(Floor_3);
}

void Che_Ku_Floor_4_B(void)
{
    Che_Ku_Floor_Choose_B(Floor_4);
}

void Che_Ku_CallBack_Floor_B(void)
{
    u8 data[] = {0x55, 0x05, 0x02, 0x01, 0x00, 0x00, 0x03, 0xBB};
    Send_ZigbeeData_To_Fifo(data, 8);
}

void Che_Ku_CallBack_IrF_B(void)
{
    u8 data[] = {0x55, 0x05, 0x02, 0x02, 0x00, 0x00, 0x04, 0xBB};
    Send_ZigbeeData_To_Fifo(data, 8);
}

void Che_ku_CallBackFun(u8 *data)
{
    if(data[3] == 0x01)							//获取立体车库当前层数
    {
        switch(data[4])
        {
            case 1:
                Che_Ku_Floor_Flag = 1;
                break;

            case 2:
                Che_Ku_Floor_Flag = 2;
                break;

            case 3:
                Che_Ku_Floor_Flag = 3;
                break;

            case 4:
                Che_Ku_Floor_Flag = 4;
                break;
        }
    }
    else if(data[3] == 0x02)
    {
        if(data[5] == 0x01)
        {
            if(Che_Ku_Floor_Flag == 1)
                Che_Ku_IRD_Flag = 1;
        }
    }
}

void Che_ku_wait_Floor_A(_Floor_Typedef Floor)
{
    while(Che_Ku_Floor_Flag != Floor)
    {
        delay_ms(500);
        Che_Ku_Floor_Choose_A(Floor);
        delay_ms(500);
        delay_ms(500);
        Che_Ku_CallBack_Floor_A();
        delay_ms(500);
        delay_ms(500);
    }
}

void Che_ku_wait_Floor_B(_Floor_Typedef Floor)
{
    while(Che_Ku_Floor_Flag != Floor)
    {
        delay_ms(500);
        Che_Ku_Floor_Choose_B(Floor);
        delay_ms(500);
        delay_ms(500);
        Che_Ku_CallBack_Floor_B();
        delay_ms(500);
        delay_ms(500);
    }
}

void Che_Ku_wait_Ird_A(void)
{
    Che_Ku_CallBack_IrF_A();

    while(Che_Ku_IRD_Flag != 1)
    {
        delay_ms(500);
        Fun_Go_Back(-20, 100);
        delay_ms(500);
        delay_ms(500);
        Che_Ku_CallBack_IrF_A();
        delay_ms(500);
    }

    Che_Ku_IRD_Flag = 0;
    Che_Ku_Floor_Flag = 0;
}

void Che_Ku_wait_Ird_B(void)
{
    Che_Ku_CallBack_IrF_B();

    while(Che_Ku_IRD_Flag != 1)
    {
        delay_ms(500);
        Fun_Go_Back(-20, 100);
        delay_ms(500);
        delay_ms(500);
        Che_Ku_CallBack_IrF_B();
        delay_ms(500);
    }

    Che_Ku_IRD_Flag = 0;
    Che_Ku_Floor_Flag = 0;
}


