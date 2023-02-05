#ifndef _OPENMV_HPP_
#define _OPENMV_HPP_
#include <DCMotor.h>
#include <ExtSRAMInterface.h>
#include <CoreBeep.h>
#include "data_fun.hpp"
#include <Command.h>

class _OpenMv
{
private:
    bool AGV_Run_Flag;
    uint8_t From_OpenMv[8];
    uint8_t OpenMv_QR_Return_Error_Num=0;
    uint8_t OpenMv_Trffic_Return_Error_Num=0;
    void OpenMV_QR_Return_Error_Check(uint8_t);
    uint8_t Opencv_data[50];
public:
    _OpenMv(bool *flag){AGV_Run_Flag=flag;}
    ~_OpenMv(){}
    void OpenMv_Data_Get();
    void OpenMv_QR_Start();
    void OpenMv_QR_Stop();
    void OpenMv_Trffic_Start();
    uint8_t QR_Code_Data[50];
    uint8_t Trffic_Color=0;
    bool OpenMv_Run_Flag=false;
};

void _OpenMv::OpenMv_Data_Get()
{
    uint32_t Error_Time=millis()+25000;
    while(OpenMv_Run_Flag==true)
    {
        if(Error_Time<=millis())
        {
            OpenMv_Run_Flag=false;
        }
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
                    AGV_Run_Flag=true;
                    OpenMv_Run_Flag=false;
                }
                else if(Data_Seate==0x02)
                {
                    CoreBeep.TurnOn();
                    delay(500);
                    CoreBeep.TurnOff(); 
                    delay(500);   
                    OpenMV_QR_Return_Error_Check(OpenMv_QR_Return_Error_Num++);
                }
                else if(Data_Seate==0x03)
                {
                    CoreBeep.TurnOn();
                    delay(1000);
                    CoreBeep.TurnOff();
                }
            }
            else if(Data_Type==0x91)
            {
                ExtSRAMInterface.ExMem_Read_Bytes(0x6038,From_OpenMv,8);
                Trffic_Color=From_OpenMv[3];
                if(Trffic_Color>3)
                    Trffic_Color=(rand()%3)+1;
                AGV_Run_Flag=true;
                OpenMv_Run_Flag=false;
            }
        }
    }
}

void _OpenMv::OpenMv_QR_Start()
{
    AGV_Run_Flag=false;
    OpenMv_Run_Flag=true;
    uint8_t data[]={0x55,0x02,0x92,0x01,0x00,0x00,0x00,0xBB};
    Command.Judgment(data);
    ExtSRAMInterface.ExMem_Write_Bytes(0x6008,data,8);
}

void _OpenMv::OpenMv_QR_Stop()
{
    AGV_Run_Flag=true;
    OpenMv_Run_Flag=false;
    uint8_t data[]={0x55,0x02,0x92,0x01,0x00,0x00,0x00,0xBB};
    Command.Judgment(data);
    ExtSRAMInterface.ExMem_Write_Bytes(0x6008,data,8);
}

void _OpenMv::OpenMv_Trffic_Start()
{
    AGV_Run_Flag=false;
    OpenMv_Run_Flag=true;
    uint8_t data[]={0x55,0x02,0x91,0x01,0x00,0x00,0x00,0xBB};
    Command.Judgment(data);
    ExtSRAMInterface.ExMem_Write_Bytes(0x6008,data,8);
}

void _OpenMv::OpenMV_QR_Return_Error_Check(uint8_t Error_Num)
{
    if(Error_Num == 0)
    {
        DCMotor.TurnRightAngle(90,20);    //右转
        OpenMv_QR_Start();
    }  
    else if(Error_Num == 1)
    {   
        DCMotor.TurnLeftAngle(90,40);   //左转  
        OpenMv_QR_Start();
    }
    else if(Error_Num == 2)
    {
        OpenMv_Run_Flag=false;
        AGV_Run_Flag=true;
    }
}



#endif