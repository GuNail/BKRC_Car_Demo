
/****************************************************************
---------------------------------------------------------------------------
**********<<<<<<<<<<��������Ŷ��壬������C�ļ�>>>>>>>>>>>*************

SDA                  bh1750.c        void SDA_OUT()
SCL                   bh1750.c        void BH1750_PortInit()
���裨SDA��SCL���Ż�����
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;      6�ĳ�7
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;     15����
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;      7�ĳ�6
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;      7�ĳ�6
bh1750.h
#define IIC_SCL    PBout(6) //SCL             6�ĳ�7
#define IIC_SDA    PBout(7) //SDA���	        7�ĳ�6
#define READ_SDA   PBin(7)  //SDA����        7�ĳ�6

ADDR               bh1750.c        void BH1750_PortInit()
---------------------------------------------------------------------------

INC      uitrasonic.c    
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOA,ENABLE);
INT0      uitrasonic.c    
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOA,ENABLE);
����(INC��INT0���Ż���)
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;   15�ĳ�4
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;    4�ĳ�15
GPIO_Init(GPIOA,&GPIO_InitStructure);        A�ĳ�B
GPIO_Init(GPIOB,&GPIO_InitStructure);        B�ĳ�A
---------------------------------------------------------------------------

RI_TXD             infrared.c       void Infrared_Init()
R_OUT             tba.c               void Tba_Photoresistance_Config(void)
����(RI_TXD��PF11  ���Ÿĳ�  R_OUT��PG8)
  GPIO_Init(GPIOF,&GPIO_TypeDefStructure);   F�ĳ�G,,,,G�ĳ�F
  GPIO_Init(GPIOG,&GPIO_TypeDefStructure);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
  GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_8;        8�ĳ�11
  GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_11;       11�ĳ�8
RI_TXD=1;
//#define RI_TXD PFout(11)  ��Ϊ  #define RI_TXD PGout(8) 


---------------------------------------------------------------------------
LER_R          tba.c          void Tba_WheelLED_Config(void)
LED_L          tba.c        void Tba_WheelLED_Config(void)
���裨LED_R��LED_L���Ż�����
GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;   10�ĳ�11 / 11�ĳ�10
GPIO_SetBits(GPIOH,GPIO_Pin_10|GPIO_Pin_11);                10��
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
  Full_Terrain_Track(Get_Tra_sp()-20,2);	     //���α�־��ѭ��
	
	
      <<���α�׼����ʮ��·��>>    ѭ�������α�־�����ı�ͣ�£���ͣ�ڵ��±�־����
case 70:
     Full_Terrain_Track(Get_Tra_sp()-20,1);				
mark = 75;
break;
case 75:
      Full_Go(Get_Go_sp(),Get_Go_mp()-100);				
mark = 80;
break;
case 80:
      Full_Terrain_Track(Get_Tra_sp()-20,2);	     //���α�־��ѭ��
mark = 85;
break;
case 85:

     <<���α�־������ʮ��·���м�>>
Full_Left(Get_RL_sp());                        //��ת
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
  �ӳ������������ȴ�
	  mark = 140;
		break;
		case 140:		
		  Full_Mp_Track(40,700);        //ǰ��700��
		  delay_ms(500);	
		mark = 150;
		break;
		case 150:
			delay_ms(200);
		  Send_AGV_Interaction_Function(0x1a);   //�ӳ�����
		  delay_ms(200);
		  Send_AGV_Interaction_Function(0x1a);
		  delay_ms(200);
		mark = 155;
		break;
		case 155:
		  Full_FC_Open_Flag = 1;                 //�ӳ�������������ָ��
		  Full_Run_Flag = 0;
	  	delay_ms(500);
     mark = 165;
		break;
		case 165:
       Full_Back(40,1700);                   //����1700���
       delay_ms(200); 

---------------------------------------------------------------------------

*****************************************************************/



/*****************************************************************
---------------------------------------------------------------------------
����

Full_Run_Flag = 0;          // <<�˳���ǰȫ�ƶ�>> 
full_Speech("��ʼʶ���ά��?", strlen("��ʼʶ���ά��?"));    // <<��������>> 


---------------------------------------------------------------------------
//---------------------------Ѱ��
---------------------------------------------------------------------------
Full_Find_cards2_3();               //Ѳ��ģʽ2.3	
Full_Find_cards4_5();               //Ѳ��ģʽ4.5
Full_Find_cards6_7();               //Ѳ��ģʽ6.7
---------------------------------------------------------------------------
//---------------------------������ʾ
---------------------------------------------------------------------------
				Stereo_Display(LTXS_CP);       //������ʾ����	����ASCII��ֵ
				Revolve_Show(LTXS_JL);         //������ʾ���� ����ASCII��ֵ
				Revolve_Show(LTXS_XZ)��        //������ʾ��״
		  	    Revolve_Show(LTXS_YS)��        //������ʾ��ɫ
		  	    Revolve_Show(LTXS_LK)��        //������ʾ·��
---------------------------------------------------------------------------
//---------------------------��������ࣨ������ʾ �� LED��ʾ��
---------------------------------------------------------------------------				
			  LT_Distance();                 //��������ຯ����������JL_Code[5]={0x11,0x33,0x37,0x00,0x00}; 0x33ʮλ,0x37��λ
			  Revolve_Show(JL_Code);         //������ʾ���� ����ASCII��ֵ 		
			  LED_Distance();	              //  <<���������LED��ʾ>>ע���ѭ���ٶȵ���40  
			  delay_ms(200);             
---------------------------------------------------------------------------
//---------------------------LED��ʾ
---------------------------------------------------------------------------
        LED_graphics(LED_Figure);     //LED��ʾ��ͼ�θ�����A1B2C3  
        LED_graphics(SMG_JL);         //�������ʾ���� 
        Control_LED_Nixie_light(1);   //����ܼ�ʱ������(1)��(0)��(3)����
---------------------------------------------------------------------------
//---------------------------��������
---------------------------------------------------------------------------
full_Speech("��ʼʶ���ά��?", strlen("��ʼʶ���ά��?"));    // <<��������>> 

SYN_7318_One_test(1,0);	
delay_ms(200);                                                //  <<����ʶ��>>

 SYN_7318_One_test(0,2);   	                                    //ָ����������2��������3����ɽ��4׷������5�﷫��6��ͷ����
---------------------------------------------------------------------------
//---------------------------���߳��
---------------------------------------------------------------------------
Wireless_charging();	                                         //  <<���߳�翪��>>
---------------------------------------------------------------------------
//---------------------------TFT��ʾ
---------------------------------------------------------------------------
     Control_TFT_show_Time(1);    //    TFT A  ��1����ʼ��ʱ��2��ֹͣ��ʱ��3������
     Control_TFTB_show_Time(1);   //    TFT B  ��1����ʼ��ʱ��2��ֹͣ��ʱ��3������
     Control_TFT_show_distance(TFT_JL);               //TFTA��ʾ����
     Control_TFTB_show_distance(TFTB_JL);             //TFTB��ʾ����
		 Control_TFT_show_data(TFT_CP);               //TFTA��ʾ����
		 Control_TFTB_show_data(TFTB_CP);             //TFTB��ʾ����
       TFT_turn_over(1);                                //A_TFT��ҳ  1�Ϸ�ҳ2�·�ҳ
	   TFTB_turn_over(1);                             //B_TFT��ҳ	 1�Ϸ�ҳ2�·�ҳ
		 
		 WIFI_Data[2] = 0X2A;                          
     A72_Send(WIFI_Data);                  
     Full_Run_Flag = 0;			    //  <<ʶ��TFT��־��>>	

---------------------------------------------------------------------------
//---------------------------���峵��
---------------------------------------------------------------------------
Full_Stereo_Return_Flag= 1;	            
Full_Run_Flag = 0;                                    
delay_ms(500);                              //������A��

Full_Btereo_Return_Flag= 1;	            
Full_Run_Flag = 0;                                    
delay_ms(500);                             //������B��


SetStereo_garage(1);
	case 161:
//			delay_ms(200);
//			SetStereo_Barage(1);						//���峵B�⵽���һ��
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
//			SetStereo_Barage(2);						//���峵B�⵽���һ��
//			delay_ms(200);
//			mark = 170;
//			break;
---------------------------------------------------------------------------
//---------------------------ETC��־��
---------------------------------------------------------------------------
delay_ms(500);
delay_ms(500);
delay_ms(500);
delay_ms(500);
Full_Run_Flag = 0;
Full_ETC_Open_Flag = 1;     //  <<����ETC>>��ֹ�ܸ���ǰ����-�ȵ�2��
delay_ms(500);
---------------------------------------------------------------------------
//---------------------------��ͨ��
---------------------------------------------------------------------------
delay_ms(500);
Full_Traffic_Light_Recognize_patterns();       //��ͨ�ƿ���A
WIFI_Data[2] = 0X5A;                           
A72_Send(WIFI_Data);
Full_Run_Flag = 0;
delay_ms(500);
delay_ms(500);
delay_ms(500);

Full_Braffic_Light_Recognize_patterns();       //��ͨ�ƿ���B
WIFI_Data[2] = 0X5B;                           
A72_Send(WIFI_Data);
Full_Run_Flag = 0;
delay_ms(500);
delay_ms(500);
---------------------------------------------------------------------------
//---------------------------���̨
---------------------------------------------------------------------------
Infrared_Send(HW_G,6);	                               //���̨�ر�
Infrared_Send(HW_K,6);	                               //���̨���� 
Modify_Beacon_tower_Infrared(XG_HW);   //д���̨������
---------------------------------------------------------------------------
//---------------------------����·��
---------------------------------------------------------------------------
Full_Light_Goal(3);		                                 //����·�Ʊ�־��  (Ŀ�굵λ)
Full_current_brightness();                             //��õƹ��ʼ��λ  ����LD_DW[0]={0x02};                         

---------------------------------------------------------------------------
//---------------------------��բ
---------------------------------------------------------------------------
Control_DZ_show_data(u8 *Data);                  // <<��բ��ʾ��������>>

Track_gate_Open();		
Full_Run_Flag = 0;           // <<������բ>> 
Full_DZ_Open_Flag  = 1;                        
delay_ms(500);   

---------------------------------------------------------------------------
//---------------------------WIFIͨ��
---------------------------------------------------------------------------
WIFIͨ��
Can_WifiRx_Check();             //wifi�����ϲ�ָ��

WIFI_Data[2] = 0X2A;           //���ϲ�һ��2A
wifi_Send(WIFI_Data);

WIFI_Data[2] = 0X3A;
WIFI_Data[2] = LD_DW;
A72_Send(WIFI_Data);           //  <<ʶ���ά��>>        
Full_Run_Flag = 0;			   

 ---------------------------------------------------------------------------
//---------------------------С����������

Full_XJ_Track()   //Ѱ������(�����)
---------------------------------------------------------------------------
Full_Right(Get_RL_sp());
delay_ms(300);               //��ת�䣨ѭ����

Full_LefRigX(80,L,100);                             
delay_ms(300);               //��ת100(������ͬʱ������������תʱ����������)

Full_Left(Get_RL_sp());                            
delay_ms(300);               //��ת�䣨ѭ����

Full_LefRigX(80,R,100);                             
delay_ms(300);               //��ת100(������ͬʱ������������תʱ����������)

Full_LefRigX(80,L,430);                             
delay_ms(300);               //��ת45�ȣ���ѭ����������a72Ϊ430������Ϊ450�룩����ת90��Ϊ800��

Full_LefRigX(80,R,500);                             
delay_ms(300);               //��ת45�ȣ���ѭ����������a72Ϊ500������Ϊ450�룩����ת90��Ϊ800��

Full_XJ_Track();             //Ѱ������               
delay_ms(500); 

Full_Go(Get_Go_sp(),Get_Go_mp());        
delay_ms(500);                //ָ������ǰ������ѭ����

Full_Mp_Track(40,400);                          
delay_ms(500);                //ָ������ѭ����400��Ϊ�������850Ϊ���1/2,1060Ϊ����1/2�����������14cm����74.5cm����59.5cm

Full_Back(60,2000);
delay_ms(300);               //���� 
 ---------------------------------------------------------------------------
//---------------------------С����������׼�����
---------------------------------------------------------------------------
    Full_Mp_Track(60,500);       //Ѱ��500                   
    delay_ms(500);  
		Full_Back(60,500);           //����500
    delay_ms(300);                
		Full_Mp_Track(60,500);                          
    delay_ms(500);  
		Full_Back(60,1500);
    delay_ms(300);   		
		
---------------------------------------------------------------------------
//----------�����---------------//�����������------------------------------
---------------------------------------------------------------------------


	Full_Right(Get_RL_sp());
delay_ms(300);               //��ת�䣨ѭ����
		Full_Back(60,200);
delay_ms(300);               //����Ϊ�˼ӳ�ѭ������

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
		
     Full_Track(50);             //ѭ��
	   delay_ms(500); 
		 Full_Go(60,360);
		 delay_ms(500);
		 
---------------------------------------------------------------------------
//--------------------------- ��������ж�
---------------------------------------------------------------------------
mark = 235;
		break;
		case 235:               //ѭ��          ��������ж�
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