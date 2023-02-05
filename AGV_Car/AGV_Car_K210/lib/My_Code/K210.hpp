#ifndef _K210_HPP_
#define _K210_HPP_
#include <DCMotor.h>
#include <ExtSRAMInterface.h>
#include <CoreBeep.h>
#include "data_fun.hpp"
#include <Command.h>

class _K210
{
private:
    uint8_t From_K210[8];
    uint8_t QR_Return_Error_Num=0;
    uint8_t Trffic_Return_Error_Num=0;
    void QR_Return_Error_Check(uint8_t);
    uint8_t Opencv_data[50];
public:
    _K210(){}
    ~_K210(){}
    void Get_Data();
    void QR_Start();
    void QR_Stop();

    void Trffic_Start();
    uint8_t QR_Code_Data[50];
    uint8_t Trffic_Color=0;
    bool Run_Flag=false;

    uint8_t Track_is_special=0;
    uint8_t Track_is_cross=0;
    uint8_t Track_is_car_state=0;
    uint8_t Track_data=0;
};

void _K210::Get_Data()
{
    if(ExtSRAMInterface.ExMem_Read(0x6038)==0x55&&ExtSRAMInterface.ExMem_Read(0x6039)==0x02)
    {
        uint8_t Data_Type =ExtSRAMInterface.ExMem_Read(0x603A);
        uint8_t Data_Seate=ExtSRAMInterface.ExMem_Read(0x603B);
        uint8_t Data_Len  =ExtSRAMInterface.ExMem_Read(0x603C);
        if(Data_Type==0x92)
        {
            ExtSRAMInterface.ExMem_Read_Bytes(0x6038,Opencv_data,Data_Len+6);
            if(Data_Seate==0x01)
            {
                for(int i=5;i<Data_Len+5;i++)
                {
                    QR_Code_Data[i-5]=Opencv_data[i];
                    Serial.println(QR_Code_Data[i-5],HEX);
                }
            }
        }
        else if(Data_Type==0x93)
        {
            ExtSRAMInterface.ExMem_Read_Bytes(0x6038,From_K210,8);
            Trffic_Color=From_K210[3];
            if(Trffic_Color>3)
                Trffic_Color=(rand()%3)+1;
        }
    }
    
}

void _K210::QR_Start()
{
    uint8_t data[]={0x55,0x02,0x92,0x01,0x01,0x00,0x00,0xBB};
    Command.Judgment(data);
    ExtSRAMInterface.ExMem_Write_Bytes(0x6008,data,8);
}

void _K210::QR_Stop()
{
    uint8_t data[]={0x55,0x02,0x92,0x01,0x02,0x00,0x00,0xBB};
    Command.Judgment(data);
    ExtSRAMInterface.ExMem_Write_Bytes(0x6008,data,8);
}

void _K210::Trffic_Start()
{
    uint8_t data[]={0x55,0x02,0x92,0x02,0x01,0x00,0x00,0xBB};
    Command.Judgment(data);
    ExtSRAMInterface.ExMem_Write_Bytes(0x6008,data,8);
}



#endif