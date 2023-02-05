#ifndef _LANDMARK_HPP_
#define _LANDMARK_HPP_
#include <Arduino.h>
#include <DCMotor.h>             
#include <CoreLED.h>            
#include <CoreKEY.h>             
#include <CoreBeep.h>           
#include <ExtSRAMInterface.h>    
#include <LED.h>                  
#include <BH1750.h>               
#include <Command.h>              
#include <BEEP.h>                
#include <Infrare.h>               
#include <Ultrasonic.h>           
#include <SYN7318.h>              
#include <Chinese.h>  

#include "LED_DIS.hpp"
#include "Che_Ku.hpp"
#include "Dao_Zha.hpp"
#include "voice_Player.hpp"
#include "feng_Huo.hpp"
#include "TFT.hpp"
#include "Lu_deng.hpp"
#include "LI_Ti.hpp"
#include "ETC.hpp"
#include "Trffic_Light.hpp"

class _Landmark
{
private:
    
public: 
     _Landmark(/* args */);
    ~_Landmark();
    
    LED_DIS led_dis;
    Che_Ku Che_Ku_A{0x0D};
    Che_Ku Che_Ku_B{0x05};
    Dao_Zha dao_Zha;
    voice_Player Voice;
    Feng_Huo feng_huo;
    TFT tft_A{0x0B};
    TFT tft_B{0x08};
    Lu_deng lu_deng;
    Wu_Xian wu_xian;
    LI_Ti li_ti;
    Trffic_Light trffic_A{0x0E};
    Trffic_Light trffic_B{0x0F};
    _ETC ETC;
    TS_DX ts_dx;
};

_Landmark::_Landmark(/* args */)
{
   
}

_Landmark::~_Landmark()
{

}
_Landmark landMark;

#endif
