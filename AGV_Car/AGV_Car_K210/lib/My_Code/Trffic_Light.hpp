#ifndef _TRFFIC_LIGHT_HPP_
#define _TRFFIC_LIGHT_HPP_
#include <ExtSRAMInterface.h>
#include "Command.h"

class Trffic_Light
{
private:
   uint8_t Type;
   void Send_TO_Trffic(uint8_t f,uint8_t c1,uint8_t c2,uint8_t c3)
   {
       uint8_t data[]={0x55,Type,f,c1,c2,c3,0x00,0xBB};
       Command.Judgment(data);
       ExtSRAMInterface.ExMem_Write_Bytes(0x6008,data,8);
       delay(100);
       ExtSRAMInterface.ExMem_Write_Bytes(0x6008,data,8);
       delay(100);
   }
public:
    Trffic_Light(uint8_t AB){Type=AB;}
    ~Trffic_Light(){}
    void Start_Reco();
    void Choose_Color(uint8_t color);
    void Color_Red();
    void Color_Green();
    void Color_Yellow();
};

//进入识别模式
void Trffic_Light::Start_Reco()
{
    Send_TO_Trffic(0x01,0,0,0);
}

/*
0x01 “红色”识别结果请求确认
0x02 “绿色”识别结果请求确认
0x03 “黄色”识别结果请求确认
*/
void Trffic_Light::Choose_Color(uint8_t color)
{
    Send_TO_Trffic(0x02,color,0,0);
}

void Trffic_Light::Color_Red()
{
    Choose_Color(0x01);
}

void Trffic_Light::Color_Green()
{
    Choose_Color(0x02);
}

void Trffic_Light::Color_Yellow()
{
    Choose_Color(0x03);
}




#endif