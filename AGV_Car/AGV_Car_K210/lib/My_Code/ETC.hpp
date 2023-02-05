#ifndef _ETC_HPP_
#define _ETC_HPP_
#include <ExtSRAMInterface.h>
#include "Command.h"

class _ETC
{
private:
    
public:
    _ETC(/* args */){}
    ~_ETC(){}
    bool Open_Flag=false;
    void Set_Satrt_Angle(uint8_t left,uint8_t right)
    {
        uint8_t data[]={0x55,0x0C,0x08,left,right,0x00,0x00,0xBB};
        Command.Judgment(data);
        ExtSRAMInterface.ExMem_Write_Bytes(0x6008,data,8);
    }
};

class TS_DX
{
private:
    /* data */
public:
    TS_DX(/* args */);
    ~TS_DX();
    uint8_t staet = 0x00;
    uint8_t Flag=0;
    void Cheack()
    {
        uint8_t data[]={0x55,0x10,0x10,0x01,0x00,0x00,0x00,0xBB};
        Command.Judgment(data);
        ExtSRAMInterface.ExMem_Write_Bytes(0x6008,data,8);
        delay(100);
        ExtSRAMInterface.ExMem_Write_Bytes(0x6008,data,8);
        delay(100);
    }
};
 TS_DX::TS_DX(/* args */)
{
}
 TS_DX::~TS_DX()
{
}









#endif