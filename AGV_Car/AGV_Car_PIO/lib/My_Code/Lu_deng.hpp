#ifndef _LU_DENG_HPP_
#define _LU_DENG_HPP_
#include <Infrare.h>
#include <BH1750.h>

class Lu_deng
{
private:
   uint8_t add1[4]={0x00,0xff,0x0c,0xF3};
   uint8_t add2[4]={0x00,0xff,0x18,0xE7};
   uint8_t add3[4]={0x00,0xff,0x5e,0xA1};
public:
    Lu_deng(/* args */){}
    ~Lu_deng(){}
    void Light_add1();
    void Light_add2();
    void Light_add3();
    void Light_add(uint8_t value);
    uint8_t Set_light(uint8_t ger);
    uint8_t Get_light();

};

//档位+
void Lu_deng::Light_add(uint8_t value)
{
    switch (value)
    {
    case 1:
         Infrare.Transmition(add1,4);
        break;

    case 2:
         Infrare.Transmition(add2,4);
        break;

    case 3:
         Infrare.Transmition(add3,4);
        break;
    } 
}

//加1挡
void Lu_deng::Light_add1()
{
    Light_add(1);
}

//加2挡
void Lu_deng::Light_add2()
{
    Light_add(2);
}
//加三挡
void Lu_deng::Light_add3()
{
    Light_add(3);
}

//调光，返回档位
uint8_t Lu_deng::Set_light(uint8_t ger)
{
    uint32_t start_gear,new_gear;
    int i;
    start_gear=BH1750.ReadLightLevel();
    if(ger > 4)	ger = 4;
    for(i=0;i<5;i++)
    {
        Light_add1();
        delay(1000);
        new_gear=BH1750.ReadLightLevel();
        if(start_gear>new_gear)
        {
            break;
        }else{
            start_gear=new_gear;
        }
    }
    switch (ger)
    {
    case 2:
        Light_add1();
        break;
    case 3:
        Light_add1();
        break;
    case 4:
        Light_add1();
        break;
    }
    return 4-i;
}

uint8_t Lu_deng::Get_light()
{
    uint32_t start_gear,new_gear;
    int i;
    start_gear=BH1750.ReadLightLevel();
    for(i=0;i<5;i++)
    {
        Light_add1();
        delay(1000);
        new_gear=BH1750.ReadLightLevel();
        if(start_gear>new_gear)
        {
            break;
        }else{
            start_gear=new_gear;
        }
    }
    switch (4-i)
    {
    case 2:
        Light_add1();
        break;
    case 3:
        Light_add2();
        break;
    case 4:
        Light_add3();
        break;
    }
    return 4-i;
}






#endif