#ifndef _DAO_ZHA_HPP_
#define _DAO_ZHA_HPP_
#include <ExtSRAMInterface.h>
#include <Command.h>

class Dao_Zha
{
private:
    void Send_To_DaoZha(uint8_t first,uint8_t commd1,uint8_t commd2,uint8_t commd3)
    {      
        uint8_t data[8]={0x55,0x03,first,commd1,commd2,commd3,0x00,0xBB};
        Command.Judgment(data);
        ExtSRAMInterface.ExMem_Write_Bytes(0x6008,data,8);
        delay(100);
        ExtSRAMInterface.ExMem_Write_Bytes(0x6008,data,8);
        delay(100);
        ExtSRAMInterface.ExMem_Write_Bytes(0x6008,data,8);
    }
public:
    Dao_Zha(/* args */) {}
    ~Dao_Zha() {}
    bool Open_Flag=false;
    void Open(void);
    void Close(void);
    void CP_Open(String CP);
    void CP_Open(uint8_t *CP);
    void Get_Seate();
    void Set_Start_Angle(uint8_t);
};
void Dao_Zha::Close(void)
{
    Send_To_DaoZha(0x01,0x02,0x00,0x00);
}

void Dao_Zha::Open()
{
    Send_To_DaoZha(0x01,0x01,0,0);
}

void Dao_Zha::CP_Open(String CP)
{
    Send_To_DaoZha(0x10,CP[0],CP[1],CP[2]);
    Send_To_DaoZha(0x11,CP[3],CP[4],CP[5]);
}

void Dao_Zha::CP_Open(uint8_t *CP)
{
    Send_To_DaoZha(0x10,CP[0],CP[1],CP[2]);
    Send_To_DaoZha(0x11,CP[3],CP[4],CP[5]);
}

void Dao_Zha::Get_Seate()
{
    Send_To_DaoZha(0x20,0x01,0,0);
}

void Dao_Zha::Set_Start_Angle(uint8_t angle)
{
    Send_To_DaoZha(0x09,angle,0,0);
}





#endif