#include "AGV_Car.hpp"


void Task_Thread(void);
_AGV_Car AGV_Car{Task_Thread};

uint32_t LED_BLinker=millis()+1000;

#define Get_Mp  350
#define Get_Sp  60
uint16_t mp[2];


/**
 按键处理函数
*/
void KEY_Handler(uint8_t k_value)
{
    switch(k_value)
    {
        case 1:
          AGV_Car.K210.Trffic_Start();
          break;

        case 2:

          break;

       case 3:

          break;

        case 4:

          break;
      }
 }

void Task_Thread(void)
{
  delay(100);
  switch (AGV_Car.Mark_Flag)
  {
    case 5:
      K210_Track.Track_Road_Start(); 
      AGV_Car.Mark_Flag=6;
    break;

    case 6:
      K210_Track.CarTrack_Go(50);
      AGV_Car.Mark_Flag=7;
    break;

    case 7:
      K210_Track.CarTrack_Go(50);
      AGV_Car.Mark_Flag=8;
    break;

    case 8:
      K210_Track.TurnLeft(90);
      AGV_Car.Mark_Flag=9;
    break;

    case 9:
      K210_Track.CarTrack_Go(50);
      AGV_Car.Mark_Flag=10;
    break;

    case 10:
      K210_Track.TurnRight(90);
      AGV_Car.Mark_Flag=11;
    break;

    case 11:
      K210_Track.CarTrack_Go(50);
      AGV_Car.Mark_Flag=12;
      
    break;

    case 12:
      K210_Track.Track_Road_Stop();
      AGV_Car.Mark_Flag=0;
      AGV_Car.Run_Flag=true;
    break;


  }
}

void setup() 
{
  AGV_Car.Init();//从车硬件初始化
  AGV_Car.Mark_Flag=0;
  AGV_Car.Run_Flag=false;                       
  while(!Serial);       //等待串口初始化完成  
}

void loop()      
{
    AGV_Car.run();
    CoreKEY.Kwhile(KEY_Handler);   
    if(AGV_Car.Get_time(LED_BLinker)==0)
    {
      LED_BLinker=millis()+500;
      PORTF =!PORTF;
    }
}



