#include "Lu_Deng.h"
#include "bh1750.h"

void Lu_Deng_Gears_1(void)
{
    u8 data[4] = {0x00, 0xFF, 0x0C, ~(0x0c)};
    Infrared_Send(data, 4);
}

void Lu_Deng_Gears_2(void)
{
    u8 data[4] = {0x00, 0xFF, 0x18, ~(0x18)};
    Infrared_Send(data, 4);
}

void Lu_Deng_Gears_3(void)
{
    u8 data[4] = {0x00, 0xFF, 0x5E, ~(0x5E)};
    Infrared_Send(data, 4);
}

uint8_t Lu_Deng_set_light(uint8_t ger)
{
    uint32_t start_gear = 0;
    uint32_t new_gear = 0;
    int i;
    start_gear = Get_Bh_Value();

    if(ger > 4)	ger = 4;

    for(i = 0; i < 5; i++)
    {
        Lu_Deng_Gears_1();
        delay_ms(500);
        delay_ms(500);
        new_gear = Get_Bh_Value();

        if(start_gear > new_gear)
        {
            break;
        }
        else
        {
            start_gear = new_gear;
        }
    }

    switch(ger)
    {
        case 2:
            Lu_Deng_Gears_1();
            break;

        case 3:
            Lu_Deng_Gears_2();
            break;

        case 4:
            Lu_Deng_Gears_3();
            break;
    }

    return 4 - i;
}

