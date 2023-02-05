#ifndef _K210_TTACK_HPP_
#define _K210_TTACK_HPP_
#include <DCMotor.h>
#include <ExtSRAMInterface.h>
#include <CoreBeep.h>
#include "data_fun.hpp"
#include <Command.h>

class _K210_Track
{
private:
    /* data */
public:
    _K210_Track(/* args */);
    ~_K210_Track();

    uint8_t From_K210[8];
    uint8_t Track_is_special=0;
    uint8_t Track_is_cross=0;
    uint8_t Track_is_car_state=0;
    uint8_t Track_data=0;

    uint8_t stop_Flag=0;
   
    void Track_Road_Start();
    void Track_Road_Stop();
    void sweep_Flag();

    void TurnLeft(uint8_t speed);
    void TurnRight(uint8_t speed);

    bool Read_Track_Data();

    void CarTrack(uint8_t speed);
    void CarTrack_Go(uint8_t speed);
};

_K210_Track::_K210_Track(/* args */)
{
}

_K210_Track::~_K210_Track()
{
}

void _K210_Track::CarTrack(uint8_t speed)
{
    int16_t LSpeed=0, RSpeed=0;

    while(!Read_Track_Data())
    {
    }

    while(true)
    {
       if(Read_Track_Data())
       {
            if(Track_is_car_state==0x2A)
            {
                LSpeed=speed;
                RSpeed=speed;
            }
            else if(Track_is_car_state==0x2B)
            {
                if(Track_data<20)
                {
                    LSpeed=speed-30;
                    RSpeed=speed+30;
                }
                else if(Track_data>=20 && Track_data<40)
                {
                    LSpeed=speed-40;
                    RSpeed=speed+40;
                }
                else if(Track_data>=40 && Track_data<60)
                {
                    LSpeed=speed-50;
                    RSpeed=speed+50;
                }
                else if(Track_data>=60&&Track_data<80)
                {
                    LSpeed = speed-60;
                    RSpeed = speed+60;	
                }
                else if(Track_data>=80&&Track_data<=100)
                {
                    LSpeed = speed-70;
                    RSpeed = speed+70;	
                }
                else if(Track_data>=100&&Track_data<=120)
                {
                    LSpeed = speed-80;
                    RSpeed = speed+80;	
                }
            }
            else if(Track_is_car_state==0x2D)
            {    
                if(Track_data<20)
                {
                    LSpeed=speed-30;
                    RSpeed=speed+30;
                }
                else if(Track_data>=20 && Track_data<40)
                {
                    LSpeed=speed-40;
                    RSpeed=speed+40;
                }
                else if(Track_data>=40 && Track_data<60)
                {
                    LSpeed=speed-50;
                    RSpeed=speed+50;
                }
                else if(Track_data>=60&&Track_data<80)
                {
                    LSpeed = speed-60;
                    RSpeed = speed+60;	
                }
                else if(Track_data>=80&&Track_data<=100)
                {
                    LSpeed = speed-70;
                    RSpeed = speed+70;	
                }
                else if(Track_data>=100&&Track_data<=120)
                {
                    LSpeed = speed-80;
                    RSpeed = speed+80;	
                }
            }

            if(Track_is_special==0xA1)
            {
                DCMotor.Stop();
                stop_Flag=0x02;
                break;
            }
            else if(Track_is_special==0xA2)
            {
                DCMotor.Stop();
                stop_Flag=0x03;
                break;
            }
            if(Track_is_cross==1)
            {
                DCMotor.Stop();
                stop_Flag=0x01;
                break;
            }
            DCMotor.SpeedCtr(LSpeed,RSpeed);
            sweep_Flag();	
       }
    }
}

void _K210_Track::TurnLeft(uint8_t speed)
{
    unsigned long t;
    while (DCMotor.ClearCodeDisc())
    {
    }
    
    while (!Read_Track_Data())
    {
    }

    DCMotor.SpeedCtr(speed*(-1), speed);

    do{
        if(Read_Track_Data())
        {
            if(Track_is_special==0xA1)
            {
                break;
            }
        }
    } while (true);

    t = millis();
    while ((!Read_Track_Data())|| ((millis() - t) > 10000))
    {
    }
    do{
		if(Read_Track_Data())
        {
            if ((Track_is_car_state==0x2B) || ((millis() - t) > 10000))
            {
                DCMotor.Stop();
                break;
            }
        }
	} while (true);
}

void _K210_Track::TurnRight(uint8_t speed)
{
        unsigned long t;
    while (DCMotor.ClearCodeDisc())
    {
    }
    
    while (!Read_Track_Data())
    {
    }

    DCMotor.SpeedCtr(speed, speed*(-1));

    do{
        if(Read_Track_Data())
        {
            if(Track_is_special==0xA1)
            {
                break;
            }
        }
    } while (true);

    t = millis();
    while ((!Read_Track_Data())|| ((millis() - t) > 10000))
    {
    }
    do{
		if(Read_Track_Data())
        {
            if ((Track_is_car_state==0x2D) || ((millis() - t) > 10000))
            {
                DCMotor.Stop();
                break;
            }
        }
	} while (true);
}

void _K210_Track::CarTrack_Go(uint8_t speed)
{
    CarTrack(speed);
    if(stop_Flag==0x01)
    {
        DCMotor.Go(50,450);
    }
}

void _K210_Track::Track_Road_Start()
{
    uint8_t data[]={0x55,0x02,0x91,0x01,0x00,0x00,0x00,0xBB};
    Command.Judgment(data);
    ExtSRAMInterface.ExMem_Write_Bytes(0x6008,data,8);
}

void _K210_Track::Track_Road_Stop()
{
    uint8_t data[]={0x55,0x02,0x91,0x02,0x00,0x00,0x00,0xBB};
    Command.Judgment(data);
    ExtSRAMInterface.ExMem_Write_Bytes(0x6008,data,8);
}

bool _K210_Track::Read_Track_Data()
{
    ExtSRAMInterface.ExMem_Read_Bytes(0x6038,From_K210,8);
    if(From_K210[0]==0x55&&From_K210[1]==0x02&&From_K210[2]==0x91)
    {
            ExtSRAMInterface.ExMem_Read_Bytes(0x6038,From_K210,8);
            Track_is_special=From_K210[3];
            Track_is_cross=From_K210[4];
            Track_is_car_state=From_K210[5];
            Track_data=From_K210[6];
            return true;
    }
    return false;
}

void _K210_Track::sweep_Flag()
{
    Track_is_special=0;
    Track_is_cross=0;
    Track_is_car_state=0;
    Track_data=0;
    memset(From_K210,0,8);
}

_K210_Track K210_Track;
extern _K210_Track K210_Track;
#endif
