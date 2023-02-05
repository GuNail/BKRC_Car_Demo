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
          landMark.trffic_A.Start_Reco();
          delay(1000);
          AGV_Car.OpenMv.OpenMv_Trffic_Start();
          AGV_Car.OpenMv.OpenMv_Data_Get();
          landMark.trffic_A.Choose_Color(AGV_Car.OpenMv.Trffic_Color);
          break;

        case 2:
          AGV_Car.sendTo_Host_Car(0xA2,0,0,0);
          
          break;

       case 3:
          AGV_Car.Mark_Flag=5;
          AGV_Car.Run_Flag=true;
          break;

        case 4:
          DCMotor.CarTrack_Go(60);
         
          break;
      }
 }

void Task_Thread(void)
{
  delay(100);
  switch (AGV_Car.Mark_Flag)
  {
    case 5:
      DCMotor.CarTrack_Go(60);
      AGV_Car.Mark_Flag=6;
    break;

    case 6:
      DCMotor.TurnRightAngle(90,450);
      AGV_Car.Mark_Flag=7;
    break;

    case 7:
      AGV_Car.OpenMv.OpenMv_QR_Start();
      AGV_Car.OpenMv.OpenMv_Data_Get();
      AGV_Car.Mark_Flag=8;
    break;

    case 8:
      DCMotor.TurnRight(90);
      AGV_Car.Mark_Flag=9;
    break;

    case 9:
      landMark.trffic_B.Start_Reco();
      delay(1000);
      AGV_Car.OpenMv.OpenMv_Trffic_Start();
      landMark.trffic_B.Choose_Color(AGV_Car.OpenMv.Trffic_Color);
      AGV_Car.Mark_Flag=10;
    break;

    case 10:
    
      AGV_Car.Mark_Flag=11;
    break;

    case 11:
      
      AGV_Car.Mark_Flag=12;
    break;

    case 12:
      
      AGV_Car.Mark_Flag=13;
    break;

    case 13:
      DCMotor.CarTrack_Go(60);
      AGV_Car.Mark_Flag=14;
    break;

    case 14:
      DCMotor.CarTrack_Go(60);
      AGV_Car.Mark_Flag=15;
    break;

    case 15:
      DCMotor.TurnLeft(90);
      AGV_Car.Mark_Flag=16;
    break;

    case 16:
      DCMotor.TurnLeftAngle(90,450);
      AGV_Car.Mark_Flag=17;
    break;

    case 17:
      landMark.feng_huo.open();
      AGV_Car.Mark_Flag=18;
    break;

    case 18:
       DCMotor.TurnLeft(90);
      AGV_Car.Mark_Flag=19;
    break;

    case 19:
      AGV_Car.Mark_Flag=0;
      AGV_Car.Run_Flag=false;
    break;

    //路线1
    case 100:
      DCMotor.CarTrack_Go(60);
      AGV_Car.Mark_Flag=101;
    break;

    case 101:
      DCMotor.CarTrack_Go(60);
      AGV_Car.Mark_Flag=102;
    break;

    case 102:
      DCMotor.TurnRight(90);
      AGV_Car.Mark_Flag=103;
    break;

    case 103:
      DCMotor.CarTrack_Go(60);
      AGV_Car.Mark_Flag=104;
    break;

    case 104:
      DCMotor.TurnRight(90);
      AGV_Car.Mark_Flag=105;
    break;

    case 105:
      DCMotor.CarTrack(60,500);
      AGV_Car.Mark_Flag=106;
    break;

    case 106:
      DCMotor.Back(40,500);
      AGV_Car.Mark_Flag=107;
    break;

    case 107:
      DCMotor.CarTrack(60,500);
      AGV_Car.Mark_Flag=108;
    break;

    case 108:
      DCMotor.Back(40,1200);
      AGV_Car.Mark_Flag=109;
    break;

    case 109:
      AGV_Car.Mark_Flag=0;
      AGV_Car.Run_Flag=false;
    break;

    //路线2
    case 200:
      DCMotor.CarTrack_Go(60);
      AGV_Car.Mark_Flag=201;
    break;

    case 201:
      DCMotor.TurnRight(90);
      AGV_Car.Mark_Flag=202;
    break;

    case 202:
      
      AGV_Car.Mark_Flag=203;
    break;

    case 203:
      
      AGV_Car.Mark_Flag=204;
    break;

    case 204:
      DCMotor.CarTrack_Go(60);
      AGV_Car.Mark_Flag=205;
    break;

    case 205:
      DCMotor.TurnLeft(90);
      AGV_Car.Mark_Flag=206;
    break;

    case 206:
      DCMotor.CarTrack_Go(60);
      AGV_Car.Mark_Flag=207;
    break;

    case 207:
      DCMotor.TurnLeft(90);
      AGV_Car.Mark_Flag=208;
    break;

    case 208:
      
      AGV_Car.Mark_Flag=209;
    break;

    case 209:
      DCMotor.TurnLeft(90);
      AGV_Car.Mark_Flag=210;
    break;

    case 210:
      
      AGV_Car.Mark_Flag=211;
    break;

    case 211:
      
      AGV_Car.Mark_Flag=212;
    break;

    case 212:
      
      AGV_Car.Mark_Flag=213;
    break;

    case 213:
      
      AGV_Car.Mark_Flag=214;
    break;

    case 214:
      
      AGV_Car.Mark_Flag=215;
    break;

    case 215:
      AGV_Car.Mark_Flag=0;
      AGV_Car.Run_Flag=false;
    break;

    //路线3
    case 300:
      DCMotor.TurnRight(90);
      AGV_Car.Mark_Flag=301;
    break;

    case 301:
      DCMotor.CarTrack_Go(60);
      AGV_Car.Mark_Flag=302;
    break;

    case 302:
      DCMotor.CarTrack_Go(60);
      AGV_Car.Mark_Flag=303;
    break;

    case 303:
      DCMotor.TurnLeft(90);
      AGV_Car.Mark_Flag=304;
    break;

    case 304:
      
      AGV_Car.Mark_Flag=305;
    break;

    case 305:
      DCMotor.TurnLeft(90);
      AGV_Car.Mark_Flag=306;
    break;

    case 306:
      DCMotor.CarTrack(60,500);
      AGV_Car.Mark_Flag=307;
    break;

    case 307:
      DCMotor.Back(60,500);
      AGV_Car.Mark_Flag=308;
    break;

    case 308:
      DCMotor.CarTrack(60,500);
      AGV_Car.Mark_Flag=309;
    break;

    case 309:
      DCMotor.Back(60,1500);
      AGV_Car.Mark_Flag=310;
    break;

    case 310:
      AGV_Car.Mark_Flag=0;
      AGV_Car.Run_Flag=false;
    break;


  }
}

void setup() 
{
  AGV_Car.Init();//从车硬件初始化
  AGV_Car.Mark_Flag=0;
  AGV_Car.Run_Flag=false;        
  Serial.begin(115200);  //串口初始化               
  while(!Serial);       //等待串口初始化完成  
}

void loop()      
{
    AGV_Car.run();
    CoreKEY.Kwhile(KEY_Handler);   
    if(AGV_Car.Get_time(LED_BLinker)==0)
    {
      LED_BLinker=millis()+1000;
      PORTF =!PORTF;
    }
}



