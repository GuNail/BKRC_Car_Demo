
/****************************************************************
---------------------------------------------------------------------------
**********<<<<<<<<<<任务板引脚定义，及所在C文件>>>>>>>>>>>*************

SDA                  bh1750.c        void SDA_OUT()
SCL                   bh1750.c        void BH1750_PortInit()
步骤（SDA与SCL引脚互换）
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;      6改成7
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;     15不变
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;      7改成6
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;      7改成6
bh1750.h
#define IIC_SCL    PBout(6) //SCL             6改成7
#define IIC_SDA    PBout(7) //SDA输出	        7改成6
#define READ_SDA   PBin(7)  //SDA输入        7改成6

ADDR               bh1750.c        void BH1750_PortInit()
---------------------------------------------------------------------------

INC      uitrasonic.c    
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOA,ENABLE);
INT0      uitrasonic.c    
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOA,ENABLE);
步骤(INC与INT0引脚互换)
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;   15改成4
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;    4改成15
GPIO_Init(GPIOA,&GPIO_InitStructure);        A改成B
GPIO_Init(GPIOB,&GPIO_InitStructure);        B改成A
---------------------------------------------------------------------------

RI_TXD             infrared.c       void Infrared_Init()
R_OUT             tba.c               void Tba_Photoresistance_Config(void)
步骤(RI_TXD：PF11  引脚改成  R_OUT：PG8)
  GPIO_Init(GPIOF,&GPIO_TypeDefStructure);   F改成G,,,,G改成F
  GPIO_Init(GPIOG,&GPIO_TypeDefStructure);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
  GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_8;        8改成11
  GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_11;       11改成8
RI_TXD=1;
//#define RI_TXD PFout(11)  改为  #define RI_TXD PGout(8) 


---------------------------------------------------------------------------
LER_R          tba.c          void Tba_WheelLED_Config(void)
LED_L          tba.c        void Tba_WheelLED_Config(void)
步骤（LED_R与LED_L引脚互换）
GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;   10改成11 / 11改成10
GPIO_SetBits(GPIOH,GPIO_Pin_10|GPIO_Pin_11);                10改
---------------------------------------------------------------------------



---------------------------------------------------------------------------


  Full_Terrain_Track(Get_Tra_sp()-20,1);
		mark = 115;
		break;
		case 115:
   Full_Go(Get_Go_sp(),150);	
    mark = 120;
		break;
		case 120:
  Full_Terrain_Track(Get_Tra_sp()-20,2);	     //地形标志物循迹
	
	
      <<地形标准物在十字路口>>    循迹到地形标志物后面的边停下，会停在地下标志物上
case 70:
     Full_Terrain_Track(Get_Tra_sp()-20,1);				
mark = 75;
break;
case 75:
      Full_Go(Get_Go_sp(),Get_Go_mp()-100);				
mark = 80;
break;
case 80:
      Full_Terrain_Track(Get_Tra_sp()-20,2);	     //地形标志物循迹
mark = 85;
break;
case 85:

     <<地形标志物在两十字路口中间>>
Full_Left(Get_RL_sp());                        //左转
 delay_ms(200);
       Full_Terrain_Track(Get_Tra_sp()-20,1);	     
mark = 160;
break;
case 160:
       Full_Go(Get_Go_sp(),Get_Go_mp()-100);		
mark = 165;
break;
case 165:
       Full_Terrain_Track(Get_Tra_sp()-20,2);	     
       delay_ms(200);
       Full_Track(Get_Tra_sp());             
       delay_ms(200);
       Full_Go(Get_Go_sp(),Get_Go_mp());
       delay_ms(200);
mark = 30;
break;
case 30:


	
	
---------------------------------------------------------------------------
  从车启动，主车等待
	  mark = 140;
		break;
		case 140:		
		  Full_Mp_Track(40,700);        //前进700码
		  delay_ms(500);	
		mark = 150;
		break;
		case 150:
			delay_ms(200);
		  Send_AGV_Interaction_Function(0x1a);   //从车启动
		  delay_ms(200);
		  Send_AGV_Interaction_Function(0x1a);
		  delay_ms(200);
		mark = 155;
		break;
		case 155:
		  Full_FC_Open_Flag = 1;                 //从车返回主车启动指令
		  Full_Run_Flag = 0;
	  	delay_ms(500);
     mark = 165;
		break;
		case 165:
       Full_Back(40,1700);                   //后退1700入库
       delay_ms(200); 

---------------------------------------------------------------------------

*****************************************************************/



/*****************************************************************
---------------------------------------------------------------------------
函数

Full_Run_Flag = 0;          // <<退出当前全制动>> 
full_Speech("开始识别二维码?", strlen("开始识别二维码?"));    // <<语音播报>> 


---------------------------------------------------------------------------
//---------------------------寻卡
---------------------------------------------------------------------------
Full_Find_cards2_3();               //巡卡模式2.3	
Full_Find_cards4_5();               //巡卡模式4.5
Full_Find_cards6_7();               //巡卡模式6.7
---------------------------------------------------------------------------
//---------------------------立体显示
---------------------------------------------------------------------------
				Stereo_Display(LTXS_CP);       //立体显示车牌	采用ASCII码值
				Revolve_Show(LTXS_JL);         //立体显示距离 采用ASCII码值
				Revolve_Show(LTXS_XZ)；        //立体显示形状
		  	    Revolve_Show(LTXS_YS)；        //立体显示颜色
		  	    Revolve_Show(LTXS_LK)；        //立体显示路况
---------------------------------------------------------------------------
//---------------------------超声波测距（立体显示 与 LED显示）
---------------------------------------------------------------------------				
			  LT_Distance();                 //超声波测距函数，保存在JL_Code[5]={0x11,0x33,0x37,0x00,0x00}; 0x33十位,0x37个位
			  Revolve_Show(JL_Code);         //立体显示距离 采用ASCII码值 		
			  LED_Distance();	              //  <<超声波测距LED显示>>注意把循迹速度调慢40  
			  delay_ms(200);             
---------------------------------------------------------------------------
//---------------------------LED显示
---------------------------------------------------------------------------
        LED_graphics(LED_Figure);     //LED显示第图形个数如A1B2C3  
        LED_graphics(SMG_JL);         //数码管显示距离 
        Control_LED_Nixie_light(1);   //数码管计时器控制(1)开(0)关(3)清零
---------------------------------------------------------------------------
//---------------------------语音播报
---------------------------------------------------------------------------
full_Speech("开始识别二维码?", strlen("开始识别二维码?"));    // <<语音播报>> 

SYN_7318_One_test(1,0);	
delay_ms(200);                                                //  <<语音识别>>

 SYN_7318_One_test(0,2);   	                                    //指定语音播报2美好生活3秀丽山河4追逐梦想5扬帆起航6齐头并进
---------------------------------------------------------------------------
//---------------------------无线充电
---------------------------------------------------------------------------
Wireless_charging();	                                         //  <<无线充电开起>>
---------------------------------------------------------------------------
//---------------------------TFT显示
---------------------------------------------------------------------------
     Control_TFT_show_Time(1);    //    TFT A  （1）开始计时（2）停止计时（3）清零
     Control_TFTB_show_Time(1);   //    TFT B  （1）开始计时（2）停止计时（3）清零
     Control_TFT_show_distance(TFT_JL);               //TFTA显示距离
     Control_TFTB_show_distance(TFTB_JL);             //TFTB显示距离
		 Control_TFT_show_data(TFT_CP);               //TFTA显示车牌
		 Control_TFTB_show_data(TFTB_CP);             //TFTB显示车牌
       TFT_turn_over(1);                                //A_TFT翻页  1上翻页2下翻页
	   TFTB_turn_over(1);                             //B_TFT翻页	 1上翻页2下翻页
		 
		 WIFI_Data[2] = 0X2A;                          
     A72_Send(WIFI_Data);                  
     Full_Run_Flag = 0;			    //  <<识别TFT标志物>>	

---------------------------------------------------------------------------
//---------------------------立体车库
---------------------------------------------------------------------------
Full_Stereo_Return_Flag= 1;	            
Full_Run_Flag = 0;                                    
delay_ms(500);                              //倒车入A库

Full_Btereo_Return_Flag= 1;	            
Full_Run_Flag = 0;                                    
delay_ms(500);                             //倒车入B库


SetStereo_garage(1);
	case 161:
//			delay_ms(200);
//			SetStereo_Barage(1);						//立体车B库到达第一层
//			delay_ms(200);
//			Full_Run_Flag = 0;
//			Full_Btereo_Return_Flag = 1;
//			mark = 165;
//			break;
//		case 165:
//			Full_Back(60,2000);
//			mark = 166;
//			break;
//		case 166:
//			delay_ms(200);
//			SetStereo_Barage(2);						//立体车B库到达第一层
//			delay_ms(200);
//			mark = 170;
//			break;
---------------------------------------------------------------------------
//---------------------------ETC标志物
---------------------------------------------------------------------------
delay_ms(500);
delay_ms(500);
delay_ms(500);
delay_ms(500);
Full_Run_Flag = 0;
Full_ETC_Open_Flag = 1;     //  <<经过ETC>>防止杠杆提前落下-先等2秒
delay_ms(500);
---------------------------------------------------------------------------
//---------------------------交通灯
---------------------------------------------------------------------------
delay_ms(500);
Full_Traffic_Light_Recognize_patterns();       //交通灯开启A
WIFI_Data[2] = 0X5A;                           
A72_Send(WIFI_Data);
Full_Run_Flag = 0;
delay_ms(500);
delay_ms(500);
delay_ms(500);

Full_Braffic_Light_Recognize_patterns();       //交通灯开启B
WIFI_Data[2] = 0X5B;                           
A72_Send(WIFI_Data);
Full_Run_Flag = 0;
delay_ms(500);
delay_ms(500);
---------------------------------------------------------------------------
//---------------------------烽火台
---------------------------------------------------------------------------
Infrared_Send(HW_G,6);	                               //烽火台关闭
Infrared_Send(HW_K,6);	                               //烽火台开起 
Modify_Beacon_tower_Infrared(XG_HW);   //写烽火台开启码
---------------------------------------------------------------------------
//---------------------------智能路灯
---------------------------------------------------------------------------
Full_Light_Goal(3);		                                 //控制路灯标志物  (目标档位)
Full_current_brightness();                             //获得灯光初始档位  存在LD_DW[0]={0x02};                         

---------------------------------------------------------------------------
//---------------------------道闸
---------------------------------------------------------------------------
Control_DZ_show_data(u8 *Data);                  // <<道闸显示车牌数据>>

Track_gate_Open();		
Full_Run_Flag = 0;           // <<开启道闸>> 
Full_DZ_Open_Flag  = 1;                        
delay_ms(500);   

---------------------------------------------------------------------------
//---------------------------WIFI通信
---------------------------------------------------------------------------
WIFI通信
Can_WifiRx_Check();             //wifi接收上层指令

WIFI_Data[2] = 0X2A;           //给上层一个2A
wifi_Send(WIFI_Data);

WIFI_Data[2] = 0X3A;
WIFI_Data[2] = LD_DW;
A72_Send(WIFI_Data);           //  <<识别二维码>>        
Full_Run_Flag = 0;			   

 ---------------------------------------------------------------------------
//---------------------------小车动作函数

Full_XJ_Track()   //寻迹函数(用这个)
---------------------------------------------------------------------------
Full_Right(Get_RL_sp());
delay_ms(300);               //右转弯（循迹）

Full_LefRigX(80,L,100);                             
delay_ms(300);               //左转100(当出现同时两个或以上右转时添加这个函数)

Full_Left(Get_RL_sp());                            
delay_ms(300);               //左转弯（循迹）

Full_LefRigX(80,R,100);                             
delay_ms(300);               //右转100(当出现同时两个或以上左转时添加这个函数)

Full_LefRigX(80,L,430);                             
delay_ms(300);               //左转45度（不循迹）（带上a72为430，不带为450码）；左转90度为800码

Full_LefRigX(80,R,500);                             
delay_ms(300);               //右转45度（不循迹）（带上a72为500，不带为450码）；右转90度为800码

Full_XJ_Track();             //寻迹函数               
delay_ms(500); 

Full_Go(Get_Go_sp(),Get_Go_mp());        
delay_ms(500);                //指定距离前进（不循迹）

Full_Mp_Track(40,400);                          
delay_ms(500);                //指定距离循迹（400码为半个车身，850为宽的1/2,1060为长的1/2）；半个车身14cm，长74.5cm，宽59.5cm

Full_Back(60,2000);
delay_ms(300);               //后退 
 ---------------------------------------------------------------------------
//---------------------------小车摆正车身准备入库
---------------------------------------------------------------------------
    Full_Mp_Track(60,500);       //寻迹500                   
    delay_ms(500);  
		Full_Back(60,500);           //后退500
    delay_ms(300);                
		Full_Mp_Track(60,500);                          
    delay_ms(500);  
		Full_Back(60,1500);
    delay_ms(300);   		
		
---------------------------------------------------------------------------
//----------用这个---------------//经过特殊地形------------------------------
---------------------------------------------------------------------------


	Full_Right(Get_RL_sp());
delay_ms(300);               //右转弯（循迹）
		Full_Back(60,200);
delay_ms(300);               //后退为了加长循迹距离

//	  mark = 100;
//		break;
//		case 100:
		
Full_Mp_Track(50,500); 

//	  mark = 105;
//		break;
//		case 105:			
		
  Full_Terrain_Track(Get_Tra_sp()-20,1);
	
//		mark = 115;
//		break;
//		case 115:
		
   Full_Go(Get_Go_sp(),200);	
	 
//   mark = 120;
//		break;
//		case 120:
		
			Full_Go(Get_Go_sp(),1100);
		  delay_ms(500);
			
//		mark = 125;
//		break;
//		case 125:
		
     Full_Track(50);             //循迹
	   delay_ms(500); 
		 Full_Go(60,360);
		 delay_ms(500);
		 
---------------------------------------------------------------------------
//--------------------------- 入库坐标判断
---------------------------------------------------------------------------
mark = 235;
		break;
		case 235:               //循迹          入库坐标判断
  for(u8 i=0;i<3;i++)
	{
		  if(RFID_Card_Position[0] == 0x1a)
		 {
			 Send_InfoData_To_Fifo("1\n",3);
			 
				mark = 260;
				break;
		 }
    else if(RFID_Card_Position[0]== 0x2a)
    {
			Send_InfoData_To_Fifo("2\n",3);	
      	mark = 240;
		  	break;
			
    }			
    else if(RFID_Card_Position[0] == 0x3a)
		 {
			 Send_InfoData_To_Fifo("3\n",3);	
				mark = 305;	
        break;			 
      		 
			}	
		 else 
    {
			Send_InfoData_To_Fifo("4\n",3);	
      	mark = 240;
			  break;
		}
	}
		break;	
		case 240:

*****************************************************************/