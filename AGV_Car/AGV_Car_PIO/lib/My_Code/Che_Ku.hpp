#ifndef _CHE_KU_HPP_
#define _CHE_KU_HPP_
#include <ExtSRAMInterface.h>
#include <Command.h>

class Che_Ku
{
private:
    uint8_t Type;
    void Send_TO_Che_Ku(uint8_t first,uint8_t commd1,uint8_t commd2,uint8_t commd3);
public:
    Che_Ku(){Type=0x05;}
    Che_Ku(uint8_t CheKu);
    ~Che_Ku(){}
    uint8_t floor=0;
    uint8_t Infrare_seate=0;
    void Floor_1(void);
    void Floor_2(void);
    void Floor_3(void);
    void Floor_4(void);
    void Floor_Choose(uint8_t floor);
    void Get_Floor(void);
    void Get_Infrared(void);
};

Che_Ku::Che_Ku(uint8_t Cheku)
{
    Type=Cheku;
}

void Che_Ku::Send_TO_Che_Ku(uint8_t first,uint8_t commd1,uint8_t commd2,uint8_t commd3)
{
    uint8_t data[8]={0x55,Type,first,commd1,commd2,commd3,0x00,0xBB};
    Command.Judgment(data);
    ExtSRAMInterface.ExMem_Write_Bytes(0x6008,data,8);
    delay(100);
    ExtSRAMInterface.ExMem_Write_Bytes(0x6008,data,8);
    delay(100);
    ExtSRAMInterface.ExMem_Write_Bytes(0x6008,data,8);
}

void Che_Ku::Floor_1(void)
{
    Send_TO_Che_Ku(0x01,0x01,0x00,0x00);
}

void Che_Ku::Floor_2(void)
{
    Send_TO_Che_Ku(0x01,0x02,0x00,0x00);
}

void Che_Ku::Floor_3(void)
{
    Send_TO_Che_Ku(0x01,0x03,0x00,0x00);
}

void Che_Ku::Floor_4(void)
{
    Send_TO_Che_Ku(0x01,0x04,0x00,0x00);
}

void Che_Ku::Floor_Choose(uint8_t floor)
{
    Send_TO_Che_Ku(0x01,floor,0x00,0x00);
}

void Che_Ku::Get_Floor(void)
{
    Send_TO_Che_Ku(0x02,0x01,0x00,0x00);
}

void Che_Ku::Get_Infrared(void)
{
    Send_TO_Che_Ku(0x02,0x02,0x00,0x00); 
}








#endif