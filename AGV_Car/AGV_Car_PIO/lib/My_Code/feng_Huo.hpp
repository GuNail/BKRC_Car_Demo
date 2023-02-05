#ifndef _FENG_HUO_HPP_
#define _FENG_HUO_HPP_
#include <ExtSRAMInterface.h>
#include <Infrare.h>
#include "Command.h"
#include <data_fun.hpp>

class Wu_Xian
{
private:
    /* data */
public:
    Wu_Xian(/* args */){}
    ~Wu_Xian(){}
    void Open();
};

void Wu_Xian::Open()
{
    uint8_t data[]={0x55,0x0A,0x01,0x01,0x00,0x00,0x00,0xBB};
    Command.Judgment(data);
    ExtSRAMInterface.ExMem_Write_Bytes(0x6008,data,8);
    delay(300);
    ExtSRAMInterface.ExMem_Write_Bytes(0x6008,data,8);
}

class Feng_Huo
{
private:
    /* data */
    void Send_TO_fengHuo(uint8_t first,uint8_t commd1,uint8_t commd2,uint8_t commd3)
    {
        uint8_t data[8]={0x55,0x07,first,commd1,commd2,commd3,0x00,0xBB};
        Command.Judgment(data);
        ExtSRAMInterface.ExMem_Write_Bytes(0x6008,data,8);
        delay(100);
        ExtSRAMInterface.ExMem_Write_Bytes(0x6008,data,8);
        delay(100);
    }
public:
    Feng_Huo(/* args */){}
    ~Feng_Huo(){}
    uint8_t Infrare_com[6];//红外数据存储数组
    void revise(uint8_t f1,uint8_t f2,uint8_t f3,uint8_t h1,uint8_t h2,uint8_t h3);
    void revise(); 
    void open();
    void open(uint8_t *data);
    void open(uint8_t f1,uint8_t f2,uint8_t f3,uint8_t h1,uint8_t h2,uint8_t h3);
};

void Feng_Huo::revise(uint8_t f1,uint8_t f2,uint8_t f3,uint8_t h1,uint8_t h2,uint8_t h3)
{
    Send_TO_fengHuo(0x10,f1,f2,f3);
    Send_TO_fengHuo(0x11,h1,h2,h3);
}

void Feng_Huo::revise()
{
    Send_TO_fengHuo(0x10,0x03,0x05,0x14);
    Send_TO_fengHuo(0x11,0x45,0xDE,0x92);
}

void Feng_Huo::open(uint8_t *data)
{
    Infrare.Transmition(data,6);
}

void Feng_Huo::open(uint8_t f1,uint8_t f2,uint8_t f3,uint8_t h1,uint8_t h2,uint8_t h3)
{
    uint8_t data[]={f1,f2,f3,h1,h2,h3};
    Infrare.Transmition(data,6);
}

void Feng_Huo::open()
{
    uint8_t data[]={0x03,0x05,0x14,0x45,0xDE,0x92};
    Infrare.Transmition(data,6);    
}





#endif
