#ifndef _AGV_CAR_HPP_
#define _AGV_CAR_HPP_
#include "Landmark.hpp"
#include "K210.hpp"
#include "K210_Track.hpp"


class _AGV_Car
{
private:
    uint32_t Timer=0;
    uint32_t Up_Timer=0;
    uint32_t Run_Overtime=5;
    bool Updata_flag=false;
    void (*Full)();
    uint8_t From_Host_Car[8];
    uint8_t *From_Landmark;
    uint8_t ZigBee_back[16]={0x55,0x02,0x01,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    void AGV_Car_Run_Handler(uint8_t);
    void AGV_Car_ZigBeeRx_Handler(uint8_t);
    void Host_Car_Callback();
    void LandMark_CallBack();
    void Thread_CallBack();
    void Update_Msg();
    void ZigbeeRx_Handle(uint8_t);
    void Stop_Flag_Cheack(void);
    
public:
    _AGV_Car(void(*Thread)())
    {
        Full=Thread;
    }
    ~_AGV_Car(){}
    void Init();                        //副车硬件初始化
    void sendTo_Host_Car(uint8_t*data); //发送数据给主车
    void run();                         //副车全自动
    uint32_t Get_time(uint32_t);
    bool Run_Flag=false;                //副车全自动标志位
    uint16_t Mark_Flag=0;                //
    _K210 K210;
    void sendTo_Host_Car(uint8_t frist,uint8_t Com1,uint8_t Com2,uint8_t Com3);
};

void _AGV_Car::Thread_CallBack()
{
    if(Run_Flag)
    {
        Full();
        Run_Overtime=0;
    }
    else
    {
        if(Mark_Flag>=5||Run_Flag==true)
        { 
            if(Get_time(Timer)==0)
            {
                Timer=millis()+100;
                Run_Overtime++;
                if(Run_Overtime>150)
                {
                    CoreBeep.TurnOn();
                    delay(500);
                    CoreBeep.TurnOff();
                    Run_Overtime=0;
                    Run_Flag=true;
                }
            }
        }
    }
}

void _AGV_Car::Stop_Flag_Cheack()
{   
   if(K210_Track.stop_Flag==0x02)
   {
        DCMotor.Go(50,200);
        delay(50);
        K210_Track.CarTrack_Go(50);
   }
   else if(K210_Track.stop_Flag==0x03)
   {

   }
}


uint32_t _AGV_Car::Get_time(uint32_t c)
{
    if(c>millis())
    {
        return (c-millis());
    }else{
        return 0;
    }
}

void _AGV_Car::Host_Car_Callback()
{
    if(ExtSRAMInterface.ExMem_Read(0x6100)==0x55)
    {
        ExtSRAMInterface.ExMem_Read_Bytes(0x6100,From_Host_Car,8);
        if(From_Host_Car[0]==0x55&&From_Host_Car[1]==0x02&&From_Host_Car[7]==0xBB)
        {
            ZigbeeRx_Handle(From_Host_Car[2]);
        }
    }
}

void _AGV_Car::LandMark_CallBack()
{
    if(ExtSRAMInterface.ExMem_Read(0x6180)==0x55)
    {
        ExtSRAMInterface.ExMem_Read_Bytes(0x6180,From_Landmark,8);
        if(From_Landmark[0]==0x55&&From_Landmark[7]==0xBB)
        {
            if(From_Landmark[1]==0x03&&From_Landmark[2]==0x01&&From_Landmark[4]==0x05)//道闸
            {
                landMark.dao_Zha.Open_Flag=true;
            }
            else if(From_Landmark[1]==0x0D&&From_Landmark[2]==0x03&&From_Landmark[3]==0x01)//车库A层数
            {
                landMark.Che_Ku_A.floor=From_Landmark[4];
            }
            else if(From_Landmark[1]==0x05&&From_Landmark[2]==0x03&&From_Landmark[3]==0x01)//车库B层数
            {
                landMark.Che_Ku_B.floor=From_Landmark[4];
            }
            else if(From_Landmark[1]==0x0D&&From_Landmark[2]==0x03&&From_Landmark[3]==0x02)//车库A红外
            {
                landMark.Che_Ku_A.Infrare_seate=(From_Landmark[4]<<4)|From_Landmark[5];
            }
            else if(From_Landmark[1]==0x05&&From_Landmark[2]==0x03&&From_Landmark[3]==0x02)//车库B红外
            {
                landMark.Che_Ku_B.Infrare_seate=(From_Landmark[4]<<4)|From_Landmark[5];
            }
            else if(From_Landmark[1]==0x10&&From_Landmark[2]==0x10&&From_Landmark[3]==0x01)//特殊地形
            {
                landMark.ts_dx.Flag=From_Landmark[4];
            }
        }
    }
}

void _AGV_Car::Update_Msg()
{
    if(Get_time(Up_Timer)==0&&Updata_flag==true)
    {
        Up_Timer=millis()+5000;
        uint16_t tp = (uint16_t)(Ultrasonic.Ranging(CM) * 10.0);     
        ZigBee_back[5] = (tp >> 8) & 0xff;     
        ZigBee_back[4] = tp & 0xff;            
        tp = BH1750.ReadLightLevel();           
        ZigBee_back[7] = (tp >> 8) & 0xff;      
        ZigBee_back[6] = tp & 0xff;             
        ZigBee_back[9] = (uint8_t)ExtSRAMInterface.ExMem_Read(0x6003);      
        if (ZigBee_back[9] >= 0x80)	ZigBee_back[9] = 0xff - ZigBee_back[9];  
            ZigBee_back[8] = (uint8_t)ExtSRAMInterface.ExMem_Read(0x6002);       
        ExtSRAMInterface.ExMem_Write_Bytes(0x6080,ZigBee_back,16);   
    }  		                                               
}

void _AGV_Car::ZigbeeRx_Handle(uint8_t com)
{
    switch (com)
      {
        case 0x01:                                       //停止
            DCMotor.Stop();
            break;
        case 0x02:                                        //前进
            DCMotor.Go(From_Host_Car[3], (From_Host_Car[4] + (From_Host_Car[5] << 8)));
            ZigBee_back[2] = 0x03;
            break;
        case 0x03:                                        //后退
            DCMotor.Back(From_Host_Car[3], (From_Host_Car[4] + (From_Host_Car[5] << 8)));
            ZigBee_back[2] = 0x03;
            break;
        case 0x04:                                        //左转
            DCMotor.TurnLeft(From_Host_Car[3]);
            ZigBee_back[2] = 0x02;
            break;
        case 0x05:                                        //右转
            DCMotor.TurnRight(From_Host_Car[3]);
            ZigBee_back[2] = 0x02;
            break;
        case 0x06:                                        //循迹
            DCMotor.CarTrack(From_Host_Car[3]);
            ZigBee_back[2] = 0x00;
            break;
        case 0x07:                                        //码盘清零
            Command.Judgment(Command.command01);                        //计算校验和                
            ExtSRAMInterface.ExMem_Write_Bytes(Command.command01, 8);   //码盘清零
            break;
        case 0x08:                                 //ETC 系统闸门初始角度调节     
            landMark.ETC.Set_Satrt_Angle(From_Host_Car[3],From_Host_Car[4]);
            break;
        case 0x09:
            landMark.dao_Zha.Set_Start_Angle(From_Host_Car[3]);
            break;
        case 0x10:                                       //保存红外数据
            landMark.feng_huo.Infrare_com[0] = From_Host_Car[3];
            landMark.feng_huo.Infrare_com[1] = From_Host_Car[4];
            landMark.feng_huo.Infrare_com[2] = From_Host_Car[5];
            break;
        case 0x11:                                       //保存红外数据
            landMark.feng_huo.Infrare_com[3] = From_Host_Car[3];
            landMark.feng_huo.Infrare_com[4] = From_Host_Car[4];
            landMark.feng_huo.Infrare_com[5] = From_Host_Car[5];
            break;
        case 0x12:                                        //发送红外数据
            landMark.feng_huo.open(landMark.feng_huo.Infrare_com);
            break;
        case 0x20:                                        //左右转向灯
            if(From_Host_Car[3] == 0x01) 
                LED.LeftTurnOn();
            else   
                LED.LeftTurnOff();
            if(From_Host_Car[4] == 0x01)  
                LED.RightTurnOn();
            else       
                LED.RightTurnOff();
            break;
        case 0x30:                                        //蜂鸣器
            if (From_Host_Car[3] == 0x01)
                BEEP.TurnOn();
            else     
                BEEP.TurnOff();
            break;
        case 0x61:                                        //光源加一档
            landMark.lu_deng.Light_add1();
            break;
        case 0x62:                                        //光源加二挡
            landMark.lu_deng.Light_add2();
            break;
        case 0x63:                                        //光源加三挡
            landMark.lu_deng.Light_add3();
            break;
        case 0x80:
            Updata_flag=From_Host_Car[3];
            break;
        // case 0x92:                                        //OpenMv二维码识别
        //     if(From_Host_Car[3] == 0x01)
        //     {
        //         OpenMv.OpenMv_QR_Start();
        //     } else if(From_Host_Car[3] == 0x02)
        //     {
        //         OpenMv.OpenMv_QR_Stop();
        //     }
        //     break;   
        case 0x1A:
             Run_Flag = true;    //从车启动
             Serial.println("S");
        break;

        case 0x1B:         //从车初始路线         
            Mark_Flag = 5;
            break;

        case 0x1C: //从车路线 1        
            Mark_Flag = 100;
            Serial.println("1");
            break;

        case 0x2C:
            Mark_Flag = 200;     //主车返回的从车入库位置C
            Serial.println("2");
            break;

        case 0x3C:
            Mark_Flag = 200;   //主车返回的从车入库位置D
            Serial.println("3");
            break;

        default:
        break;
      }
}

//从车全自动
void _AGV_Car::run()
{
    Host_Car_Callback();
    LandMark_CallBack();
    K210.Get_Data();
    Update_Msg(); 
    Stop_Flag_Cheack();
    Thread_CallBack();
}

//发送数据给主车
void _AGV_Car::sendTo_Host_Car(uint8_t*data)
{
    ExtSRAMInterface.ExMem_Write_Bytes(0x6180,data,strlen((char*)data));
    delay(100);
    ExtSRAMInterface.ExMem_Write_Bytes(0x6180,data,strlen((char*)data));
    delay(100);
}

void _AGV_Car::sendTo_Host_Car(uint8_t frist,uint8_t Com1,uint8_t Com2,uint8_t Com3)
{
    uint8_t data[]={0x55,0x02,frist,Com1,Com2,Com3,0x00,0xBB};
    Command.Judgment(data);
    ExtSRAMInterface.ExMem_Write_Bytes(0x6080,data,8);
    delay(300);
    ExtSRAMInterface.ExMem_Write_Bytes(0x6080,data,8);
    delay(300);
    ExtSRAMInterface.ExMem_Write_Bytes(0x6080,data,8);
    delay(300);
    ExtSRAMInterface.ExMem_Write_Bytes(0x6080,data,8);
}

//硬件初始化
void _AGV_Car::Init()
{
    Serial.begin(115200);
	CoreLED.Initialization();
	CoreKEY.Initialization();
	CoreBeep.Initialization();
	ExtSRAMInterface.Initialization();
	LED.Initialization();
	BH1750.Initialization();
	BEEP.Initialization();
	Infrare.Initialization();
	Ultrasonic.Initialization();
	//DCMotor.Initialization(30000);
	DCMotor.Initialization(72000);
    SYN7318.Initialization();  

    DCMotor.Stop();
}



#endif