#include "main.h"

u8 RFID_Card_Position[2]={0x00,0x00} ;				//RFID��Ƭλ�ñ�־λ  Ĭ��λ��Ϊ5 ��ȱʡ�ھ����յ�1/4(F3)
u8 sRXRFID[16];				  //���RFID��������
u8 Infrared_Tab1[6];            //�������ݴ��
extern uint8_t CP_data[6];           //������Ϣ�������
extern uint8_t DZCP_data[6]; 
uint8_t RFID;
#define E 2.71828
uint8_t a[6] = {0x32,0x30,0x32,0x30,0x31,0x30};

/********************
�������ܣ�ȫ�Զ�ǰ��
��    ����sp �ٶ� mp ǰ������
�� �� ֵ����
**********************/
void Full_Go(uint8_t sp,uint16_t mp)
{
	Roadway_mp_syn();	//����ͬ��
	Stop_Flag = 0; 
	Go_Flag = 1; 
	wheel_L_Flag = 0;
	wheel_R_Flag = 0;
	wheel_Nav_Flag = 0;
	Back_Flag = 0; 
	Track_Flag = 0;
	temp_MP = mp;
	Car_Spend = sp;
	Control(Car_Spend ,Car_Spend);
	while(Stop_Flag != 0x03);
}

/*
ǰ��
**/
void Go(uint8_t sp,uint16_t mp)
{
	Roadway_mp_syn();	//����ͬ��
	Stop_Flag = 0; 
	Go_Flag = 1; 
	wheel_L_Flag = 0;
	wheel_R_Flag = 0;
	wheel_Nav_Flag = 0;
	Back_Flag = 0; 
	Track_Flag = 0;//Mp_Track_Flag = 1;
	temp_MP = mp;
	Car_Spend = sp;
	Control(Car_Spend ,Car_Spend);
}

/*
�������ܣ�ȫ�Զ�����
��    ����sp �ٶ� mp ǰ������
�� �� ֵ����
**/
void Full_Back(uint8_t sp,uint16_t mp)
{
	Roadway_mp_syn();	//����ͬ��
	Stop_Flag = 0;
	Go_Flag = 0; 
	wheel_L_Flag = 0;
	wheel_R_Flag = 0;
	wheel_Nav_Flag = 0;
	Back_Flag = 1; 
	Track_Flag = 0;
	temp_MP = mp;
	Car_Spend = sp;
	Control(-Car_Spend ,-Car_Spend);
	while(Stop_Flag != 0x03);	
}

/*
�������ܣ�ȫ�Զ���ת
��    ����sp �ٶ�
�� �� ֵ����
**/
void Full_Left(uint8_t sp)
{
	Stop_Flag = 0;
	Go_Flag = 0; 
	wheel_L_Flag = 1;
	wheel_R_Flag = 0;
	wheel_Nav_Flag = 0;
	Back_Flag = 0; 
	Track_Flag = 0;
	Car_Spend = sp;				
	Control(-Car_Spend ,Car_Spend);
	while(Stop_Flag != 0x02);
}

/*
�������ܣ�ȫ�Զ���ת
��    ����sp �ٶ�
�� �� ֵ����
**/
void Full_Right(uint8_t sp)
{
	Stop_Flag = 0; Go_Flag = 0; wheel_L_Flag = 0;wheel_R_Flag = 1;wheel_Nav_Flag = 0;
	Back_Flag = 0; Track_Flag = 0;
	Car_Spend = sp;
	Control(Car_Spend,-Car_Spend);
	while(Stop_Flag != 0x02);
}

/*
�������ܣ�ȫ�Զ�ѭ��
��    ����sp �ٶ�
�� �� ֵ����
**/
void Full_Track(uint8_t sp)
{
	Stop_Flag = 0;
	Go_Flag = 0; 
	wheel_L_Flag = 0;
	wheel_R_Flag = 0;
	wheel_Nav_Flag = 0;
	Back_Flag = 0;
	Track_Flag = 1;
	Car_Spend = sp;
	while(Stop_Flag != 0x01);
}

void Full_TXTrack(uint8_t sp)
{
	Stop_Flag = 0;
	Go_Flag = 0;
	wheel_L_Flag = 0;
	wheel_R_Flag = 0;
	wheel_Nav_Flag = 0;
	Back_Flag = 0; 
	Track_Flag = 3;
	Car_Spend = sp;
	while(Stop_Flag != 0x01);
}
/*
ѭ��
**/
void Track(uint8_t sp)
{
	Stop_Flag = 0; Go_Flag = 0; wheel_L_Flag = 0;wheel_R_Flag = 0;wheel_Nav_Flag = 0;
	Back_Flag = 0; Track_Flag = 2;
	Car_Spend = sp;
}

/***/
void Full_Back_Track(uint8_t sp)
{
	Stop_Flag = 0; Go_Flag = 0; wheel_L_Flag = 0;wheel_R_Flag = 0;wheel_Nav_Flag = 0;
	Back_Flag = 0; Track_Flag = 0; Back_Track_Flag = 1;
	Car_Spend = sp;
	while(Stop_Flag != 0x01);
}

/*
�����ѭ��
**/
void Full_Terrain_Track(uint8_t sp,uint8_t Terrain_Type)
{
	Stop_Flag = 0; Go_Flag = 0; wheel_L_Flag = 0;wheel_R_Flag = 0;wheel_Nav_Flag = 0;
	Back_Flag = 0; Track_Flag = 0; Back_Track_Flag = 0; Terrain_Flag = Terrain_Type;
	Car_Spend = sp;
	while(Stop_Flag != 0x01);
}

/*
ָ��ǰ��������ѭ��
**/
void Full_Terrain_XG(uint8_t sp,uint16_t mp)
{
	Roadway_mp_syn();				//����ͬ��
	Stop_Flag = 0; Go_Flag = 0; wheel_L_Flag = 0;wheel_R_Flag = 0;wheel_Nav_Flag = 0;
	Back_Flag = 0; Track_Flag = 0; Back_Track_Flag = 0; Terrain_Flag = 2;Mp_Track_Flag = 1;
	Car_Spend = sp;
	temp_MP = mp;
//	while(Stop_Flag != 0x03);
	
}

void Full_TXDX_XG(uint8_t sp,uint16_t mp)     //ָ��ǰ��������ѭ�� 
{
	Roadway_mp_syn();				//����ͬ��
	Stop_Flag = 0; Go_Flag = 0; wheel_L_Flag = 0;wheel_R_Flag = 0;wheel_Nav_Flag = 0;
	Back_Flag = 0; Track_Flag = 0; Back_Track_Flag = 0; Terrain_Flag = 3;Mp_Track_Flag = 1;
	Car_Spend = sp;
	temp_MP = mp;
//	while(Stop_Flag != 0x03);
	
}
/*
ָ��ǰ������ѭ��
**/
void Full_Mp_Track(uint8_t sp,uint16_t mp)
{
	Roadway_mp_syn();				//����ͬ��
	Stop_Flag = 0; Go_Flag = 0; wheel_L_Flag = 0;wheel_R_Flag = 0;wheel_Nav_Flag = 0;
	Back_Flag = 0; Track_Flag = 0; Back_Track_Flag = 0; Terrain_Flag = 0;Mp_Track_Flag = 1;
	Car_Spend = sp;
	temp_MP = mp;
	while(Stop_Flag != 0x03);
	
}

void Full_Stop(void)
{
	Send_UpMotor(0 ,0);
	Roadway_Flag_clean();	//�����־λ״̬	
}

/*
�������ܣ�ȫ�Զ�����Ƕ�����ת
��    ����sp �ٶ� 
		  LorR = 1 ��ת LorR = 2 
		  ��ת angle ��ת�Ƕ�
�� �� ֵ����
**/
void Full_LefRigX(uint8_t sp,uint8_t LorR,uint16_t angle)
{
	if(LorR == L)
	{
		Roadway_mp_syn();	//����ͬ��
		Stop_Flag = 0; Go_Flag = 0; wheel_L_Flag = 0;wheel_R_Flag = 0;wheel_Nav_Flag = 1;
		Back_Flag = 0; Track_Flag = 0;
		temp_Nav = angle;
		Car_Spend = sp;			
		Control(-Car_Spend ,Car_Spend);	
	} 
	else if(LorR == R)
	{
		Roadway_mp_syn();	//����ͬ��
		Stop_Flag = 0; Go_Flag = 0; wheel_L_Flag = 0;wheel_R_Flag = 0;wheel_Nav_Flag = 1;
		Back_Flag = 0; Track_Flag = 0;
		temp_Nav = angle;
		Car_Spend = sp;			
		Control(Car_Spend,-Car_Spend);
	}
	while(Stop_Flag != 0x02);
}

 
//Ѱ���������ж�·��ʱ�˳�ȫ�Զ�

void Full_XJ_Track()   //Ѱ������
{
	 Full_Track(Get_Go_sp);           
	 delay_ms(100);                //�Զ�ǰ��ѭ��
	 Full_Run_Flag = 0; 
		
}
void Full_TXXJ_Track()
{	
	Full_TXTrack(Get_Tra_sp); 	
	delay_ms(100);                //�Զ�ǰ��ѭ��
	Full_Run_Flag = 0; 
}

uint8_t TXRFID[16] = "F6F4D4B4B6B7";
uint8_t RXRFID[16];

uint8_t Section_Number = 1;		//�������			��ɫ��������������� Ĭ��1

uint8_t Block_Number = 2;		//���ݿ���		���θ����������ݿ��� Ĭ��1


//���������õ���Ӧ��Կ��ַ
uint8_t Get_RFID_SectionKey_Ades(void)
{
	return (4*Section_Number+3);   //0x3B;
}

//���������Ϳ������ַ
uint8_t Get_RFID_Block_Ades(void)
{
	if((Section_Number == 0) && (Block_Number == 0))
	{
		return 1;
	} 
	else 
	{
		return (4*Section_Number+Block_Number+0x30);  //0x39;
	}
}

/*
 RFID���ݴ���
 *RxRFIDdata	��������������ָ��
 ��  <-&Y&,/;[D->Rr]>
	Y = 0~99;
	Rr = C5,E5,D4,D6

	D4(1)
C5(2)   E5(3)
	D6(4)
**/


uint8_t Ascii_NumberTab[] = {'0','1','2','3','4','5','6','7','8','9'};
uint8_t Ascii_LOAATab[] = {'&','[','-','>',']','C','D','E'};

uint16_t RFID_Y = 0;
uint8_t  AGV_StartXY = 3;					//�ӳ���ʼ����

uint8_t Test[2];

uint16_t RFID_n = 0;
uint16_t RFID_x = 0;
uint8_t TestN[2];
uint8_t TestX[16];

uint8_t wifi_buf[12]={0x55,0xAA,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x20,0x40};
uint8_t fangxiang[1];
uint8_t P1[6],P2[4];

int p16(int n)//����16���Ƶ�n�η�
{
	int sum = 1, i;
	for (i = 0; i < n; i++)
		sum = sum * 16;
	return sum;
}
/****************************************************************
�������ܣ����ַ����16���ƣ�����a1���0xa1��
��	  ����*arr,Ҫ������ַ�,�������ABC123�������0xabc123
����  ֵ��16����
*****************************************************************/
int data_Handler(char* arr)
{
	int a=0,x=0,str=0,len = strlen(arr);
	for (int i=len-1; i>=0; i--)
	{
		if(arr[i]>='0'&&arr[i]<='9')
		{
			a = arr[i] - 0x30;
		}
		else if (arr[i] >= 'a' && arr[i] <= 'f')
		{
			a = arr[i] -'a'+10;
		}
		else if (arr[i] >= 'A' && arr[i] <= 'F')
		{
			a = arr[i] - 'A'+10;
		}
		str = str + a * p16(x);
		x++;
	}
	return str;
}


void RFID_Data_Handler(u8 *RFIDBuf)
{
	int len=0,cont;
	char data_west[2];
	char dat[16];
	memcpy(dat,RFIDBuf,16);
	len=strlen(dat);
	if(len==6)
	{
		for(cont=0;cont<len;cont++)
		{
			P1[cont]=dat[cont];
		}
		wifi_buf[3]=P1[0];
		wifi_buf[4]=P1[1];
		wifi_buf[5]=P1[2];
		wifi_buf[6]=P1[3];
		wifi_buf[7]=P1[4];
		wifi_buf[8]=P1[5];
	}
	else if(len==4)
	{
		for(cont=0;cont<len;cont++)
		{
			P2[cont]=RFIDBuf[cont];
		}
		data_west[0]=P2[0];
		data_west[1]=P2[1];
		fangxiang[0]=data_Handler(data_west);
		falot=P2[3]-0x30;
	}
	
}


void Full_Write_Card()   //д������
{
	char status = 0;
	u8 CT[2];									//������
	u8 SN[4]; 									//����
	u8 KEY[6]={0xff,0xff,0xff,0xff,0xff,0xff};  //��Կ
	u8 s = 0x17;       							// #define   Status2Reg   0x08.   
	//����s�Ƕ���ĳ���Ĭ�϶����Ŀ��ַ0x01,����ļ�"2017ȫ�Զ�����\Readme.txt"
	u8 RXRFIDH[8];
	
	u8 i = 0, j = 0;

		status = PcdRequest(PICC_REQALL,CT);		//Ѱ��
		if(status == MI_OK)							//Ѱ���ɹ�
		{
			status=MI_ERR;
			CT[0] = CT[0] + 0x30;
			CT[1] = CT[1] + 0x30;
			Send_Debug_Info(CT,2);					//������
			Send_Debug_Info("\n",2);
			Send_Debug_Info("Findcard",8);
			Send_Debug_Info(" OK\n",5);
		
			status = PcdAnticoll(SN);				//����ײ
			if(status == MI_OK)
			{
				status=MI_ERR;
//				Send_Debug_Info("a OK\n",6);
				delay_ms(100);
//				Send_Debug_Info("-collision",10);
//				Send_Debug_Info(" OK\n",5);
				
				status =PcdSelect(SN);				//ѡ���˿�
				if(status == MI_OK)					//ѡ���ɹ�
				{
					status=MI_ERR;
//					Send_Debug_Info("P OK\n",6);
					delay_ms(100);
//					Send_Debug_Info(" card",5);	//��ʾ��"Pick a"��5���ַ�
//				  Send_Debug_Info(" OK\n",5);
//					//���͵�����Ϣ"Pick a card OK"
					status =PcdAuthState(0x60,s,KEY,SN);		//��֤��Կ:0x60 = ��֤A��Կ;0x61 = ��֤B��Կ�����ַ0x03\KEYΪ����\ SN�����к�(��ƿ���)
					if(status == MI_OK)		//RFID�����롢������֤�ɹ�
					{
						
						status = MI_ERR;
//						
//						Send_Debug_Info("t OK\n",6);
//						Send_Debug_Info(" and",4);
//						Send_Debug_Info(" verify",7);
//						Send_Debug_Info(" OK\n",5);
						delay_ms(100);
						//���͵�����Ϣ"test and verify OK"
						
						status = PcdWrite(s,TXRFID);			//д��
						
						if(status == MI_OK)
						{
							status = MI_ERR;
							Send_Debug_Info("write OK",10);
							Send_Debug_Info("\n",2);	
						}	
						delay_ms(500);
						
						status=PcdRead(s,sRXRFID);				//���������У�s�Ǹú���һ��ʼ����Ķ���Ĭ�Ͽ��ַ;RXREIDΪ���������ݣ�16�ֽ�
						if(status == MI_OK)
						{
							status = MI_ERR;
//							Send_Debug_Info("r OK\n",6);		//���͵�����Ϣ"read OK",�������ɹ�
							delay_ms(100);
							for(i=0;i<8;i++)
							{
								RXRFIDH[i] = sRXRFID[i+8];		//ʵ�����ݰ��ơ�����������˵��,�������������ݴ洢��sRXRFID������
							}
							Send_Debug_Info(sRXRFID,8);
							Send_Debug_Info(RXRFIDH,8);
							Send_Debug_Info("\n",2);							
						}
						delay_ms(200);
					}
				}
			}
	}
}


///*******
//��������
//*******/
//void Full_Read_Card()
//{
//	 
//	char status = 0;
//	u8 CT[2];									//������
//	u8 SN[4]; 									//����
//	u8 KEY[6]={0xff,0xff,0xff,0xff,0xff,0xff};  //��Կ
//	u8 s ;

//	//����s�Ƕ���ĳ���Ĭ�϶����Ŀ��ַ0x01,����ļ�"2017ȫ�Զ�����\Readme.txt"
//	u8 RXRFIDH[8];
//	
//	u8 i = 0, j = 0;

////		status = PcdRequest(PICC_REQALL,CT);		//Ѱ��
////		if(status == MI_OK)							//Ѱ���ɹ�
////		{
//			status=MI_ERR;
////			CT[0] = CT[0] + 0x30;
////			CT[1] = CT[1] + 0x30;
////			Send_Debug_Info(CT,2);					//������
////			Send_Debug_Info("\n",2);
////			Send_Debug_Info("Findcard",8);
////			Send_Debug_Info(" OK\n",5);
//		
//			status = PcdAnticoll(SN);				//����ײ
//			if(status == MI_OK)
//			{
//				status=MI_ERR;
//				Send_Debug_Info("a OK\n",6);
//				delay_ms(100);
//				Send_InfoData_To_Fifo(&RFID_Card_Position+0x30,3); 	
////				if(RFID_Card_Position == 1)
////	{
////		s=0xD2%3+12;
////		Send_Debug_Info("s OK\n",6);
////		Send_InfoData_To_Fifo("\n",2); 
////	  Send_InfoData_To_Fifo(&s,1);
////	}
//				
////				Send_Debug_Info("-collision",10);
////				Send_Debug_Info(" OK\n",5);
//				
//				status =PcdSelect(SN);				//ѡ���˿�
//				if(status == MI_OK)					//ѡ���ɹ�
//				{
//					status=MI_ERR;
//					Send_Debug_Info("P OK\n",6);
//					delay_ms(100);
////					Send_Debug_Info(" card",5);	//��ʾ��"Pick a"��5���ַ�
////				  Send_Debug_Info(" OK\n",5);
//					//���͵�����Ϣ"Pick a card OK"
//					status =PcdAuthState(0x60,s,KEY,SN);		//��֤��Կ:0x60 = ��֤A��Կ;0x61 = ��֤B��Կ�����ַ0x03\KEYΪ����\ SN�����к�(��ƿ���)
//					if(status == MI_OK)		//RFID�����롢������֤�ɹ�
//					{
//						
//						status = MI_ERR;
//						
//						Send_Debug_Info("t OK\n",6);
////						Send_Debug_Info(" and",4);
////						Send_Debug_Info(" verify",7);
////						Send_Debug_Info(" OK\n",5);
//						delay_ms(100);
//						//���͵�����Ϣ"test and verify OK"
//						
////						status = PcdWrite(s,TXRFID);			//д��
//						
////						if(status == MI_OK)
////						{
////							status = MI_ERR;
////							Send_Debug_Info("write OK",10);
////							Send_Debug_Info("\n",2);	
////						}	
////						delay_ms(500);
//						
//						status=PcdRead(s,sRXRFID);				//���������У�s�Ǹú���һ��ʼ����Ķ���Ĭ�Ͽ��ַ;RXREIDΪ���������ݣ�16�ֽ�
//						if(status == MI_OK)
//						{
//							status = MI_ERR;
////							BEEP = 0;
////							SYN_TTS("�����ɹ�");
//							Send_Debug_Info("r OK\n",6);		//���͵�����Ϣ"read OK",�������ɹ�
//							delay_ms(100);
//							for(i=0;i<8;i++)
//							{
//								RXRFIDH[i] = sRXRFID[i+8];		//ʵ�����ݰ��ơ�����������˵��,�������������ݴ洢��sRXRFID������
//							}
//							Send_Debug_Info(sRXRFID,8);
//							Send_Debug_Info(RXRFIDH,8);
//							Send_Debug_Info("\n",2);							
//						}
////						delay_ms(200);
////						BEEP = 1;
//					}
//				}
//			}
////	}
//}
/*******
��������
*******/
void Full_Read_Card()
{
	char status = 0;
	u8 CT[2];									//������
	u8 SN[4]; 									//����
	u8 KEY[6]={0xff,0xff,0xff,0xff,0xff,0xff};  //��Կ
	u8 s = 0x18;       							// #define   Status2Reg   0x08.   
	//����s�Ƕ���ĳ���Ĭ�϶����Ŀ��ַ0x01,����ļ�"2017ȫ�Զ�����\Readme.txt"
	u8 RXRFIDH[8];
	u8 i = 0, j = 0;
				
		if(RFID_Card_Position[0] == 0xE2)
	{
//		s=0xe2%3+12;
		Send_Debug_Info("s OK\n",6);
		Send_Debug_Info("\n",2); 
//	  Send_Debug_Info(&s,1);
	}

//		status = PcdRequest(PICC_REQALL,CT);		//Ѱ��
//		if(status == MI_OK)							//Ѱ���ɹ�
//		{
			status=MI_ERR;
//			CT[0] = CT[0] + 0x30;
//			CT[1] = CT[1] + 0x30;
//			Send_Debug_Info(CT,2);					//������
//			Send_Debug_Info("\n",2);
//			Send_Debug_Info("Findcard",8);
//			Send_Debug_Info(" OK\n",5);
		
			status = PcdAnticoll(SN);				//����ײ
			if(status == MI_OK)
			{
				status=MI_ERR;
				Send_Debug_Info("a OK\n",6);
				delay_ms(100);
//				Send_Debug_Info("-collision",10);
//				Send_Debug_Info(" OK\n",5);
				
				status =PcdSelect(SN);				//ѡ���˿�
				if(status == MI_OK)					//ѡ���ɹ�
				{
					status=MI_ERR;
					Send_Debug_Info("P OK\n",6);
					delay_ms(100);
//					Send_Debug_Info(" card",5);	//��ʾ��"Pick a"��5���ַ�
//				  Send_Debug_Info(" OK\n",5);
					//���͵�����Ϣ"Pick a card OK"
					status =PcdAuthState(0x60,s,KEY,SN);		//��֤��Կ:0x60 = ��֤A��Կ;0x61 = ��֤B��Կ�����ַ0x03\KEYΪ����\ SN�����к�(��ƿ���)
					if(status == MI_OK)		//RFID�����롢������֤�ɹ�
					{
						
						status = MI_ERR;
						
						Send_Debug_Info("t OK\n",6);
//						Send_Debug_Info(" and",4);
//						Send_Debug_Info(" verify",7);
//						Send_Debug_Info(" OK\n",5);
						delay_ms(100);
						//���͵�����Ϣ"test and verify OK"
						
//						status = PcdWrite(s,TXRFID);			//д��
						
//						if(status == MI_OK)
//						{
//							status = MI_ERR;
//							Send_Debug_Info("write OK",10);
//							Send_Debug_Info("\n",2);	
//						}	
//						delay_ms(500);
						
						status=PcdRead(s,sRXRFID);				//���������У�s�Ǹú���һ��ʼ����Ķ���Ĭ�Ͽ��ַ;RXREIDΪ���������ݣ�16�ֽ�
						if(status == MI_OK)
						{
							status = MI_ERR;
							
//							BEEP = 0;
//							SYN_TTS("�����ɹ�");
							Send_Debug_Info("r OK\n",6);		//���͵�����Ϣ"read OK",�������ɹ�
							delay_ms(100);
							RFID_DKWC_Flag = 1;
							delay_ms(100);
							for(i=0;i<8;i++)
							{
								RXRFIDH[i] = sRXRFID[i+8];		//ʵ�����ݰ��ơ�����������˵��,�������������ݴ洢��sRXRFID������
							}
							Send_Debug_Info(sRXRFID,8);
							Send_Debug_Info(RXRFIDH,8);
							Send_Debug_Info("\n",2);	
							delay_ms(100);
						}
						for(i=0;i<16;i++)
							{
								RXRFID[i] = sRXRFID[i];		//ʵ�����ݰ��ơ�����������˵��,�������������ݴ洢��sRXRFID������
							}
//						delay_ms(200);
//						BEEP = 1;
					}
				}
			}
//	}
				
}


/****************************************************************
�������ܣ���������� LED��ʾ����
��	  ������
����  ֵ����
****************************************************************/
void LED_Distance(void)
{	
	uint8_t LED_DisTab1[8] = {0x55,0x0B,0x50,0x00,0x00,0x00,0x00,0xBB};
	delay_ms(700);
	uint8_t time;
	for(time=0;time<5;time++)
	{
		Ultrasonic_Ranging();		//����������
		delay_ms(100);
	}	
	LED_DisTab1[4]=dis/100;
	LED_DisTab1[5]=dis%100;
	//LED_DisTab1[4] = dis/100%10;                             //ʮλ
	//LED_DisTab1[5] = ((dis/10%10)<<4) + dis%10;					 //��λ ����ʾ��λ
	Full_Car_CKS(LED_DisTab1);	
	Send_ZigbeeData_To_Fifo(LED_DisTab1,8);
}

/****************************************************************
�������ܣ���������� ������ʾ����
��	  ������
����  ֵ����
****************************************************************/
void LT_Distance(void)
{
	uint8_t i = 0;
	delay_ms(500);
	delay_ms(500);
	for(i=0; i<8; i++)
	{
		Ultrasonic_Ranging();						//����������
		delay_ms(200);
	}
	
	JL_Code[2]= ((dis%100/10)<<4)+(dis%10);
  JL_Code[1] =  (dis/100);
	
	JL_Code[2]=(JL_Code[2]>>4)+0x30;
	JL_Code[1]=JL_Code[1]+0x30;
	delay_ms(200);
	Set_tba_Beep(SET);
	delay_ms(100);
	Set_tba_Beep(RESET);

}

/****************************************************************
�������ܣ�������ʾ �ֶ�������������
��	  ������
����  ֵ����
*****************************************************************/
void Revolve_Show(u8 *data)
{
	uint8_t RevoTab[6] = {0xFF,0x00,0x00,0x00,0x00,0x00};
	
	RevoTab[1] = data[0];
	RevoTab[2] = data[1];
	RevoTab[3] = data[2];
	RevoTab[4] = data[3];
	RevoTab[5] = data[4];
	Infrared_Send(RevoTab,6);
	delay_ms(200);
	Infrared_Send(RevoTab,6);
	delay_ms(200);

}

/****************************************************************
�������ܣ�������ʾ����
��	  ����*data ��������ָ��
����  ֵ����     
*****************************************************************/
void Stereo_Display(u8 *data)
{
	u8 Car_data[6] = {0xFF,0x00,0x00,0x00,0x00,0x00};
	u8 i = 0;

	Car_data[1] = 0x20;
	Car_data[2] = data[0];
	Car_data[3] = data[1];
	Car_data[4] = data[2];
	Car_data[5] = data[3];
	Infrared_Send(Car_data,6);
	
	delay_ms(500);

	Car_data[1] = 0x10;
	Car_data[2] = data[4];
	Car_data[3] = data[5];
  Car_data[4] = data[6];
	Car_data[5] = data[7];

	Infrared_Send(Car_data,6);
}

/****************************************************************
�������ܣ���ӡ������ʾ
��	  ����vluae �������� ��ӡ��debug��Ļ��
����  ֵ����
*****************************************************************/
void Full_Diplay(uint32_t vluae)
{
	uint8_t num[10];
	num[0] = vluae/10000+0x30;
	num[1] = vluae%10000/1000+0x30;
	num[3] = vluae%1000/100+0x30;
	num[4] = vluae%100/10+0x30;
	num[5] = vluae%10+0x30;
	Send_InfoData_To_Fifo(num,6);
	Send_InfoData_To_Fifo("\n",2);
}

/****************************************************************
�������ܣ�����У���
��	  ����*srcdata ָ�� ָ������������
����  ֵ����
*****************************************************************/
void Full_Car_CKS(uint8_t *srcdata)
{
	srcdata[6] = (srcdata[2]+srcdata[3]+srcdata[4]+srcdata[5])%256;
}

/****************************************************************
�������ܣ�����У���
��	  ����*srcdata ָ�� ָ������������
����  ֵ����
*****************************************************************/
void Pan_duan_ru_ku(uint8_t *ruku)
{
	 
	if((ruku[0] == 'D') && (ruku[1] == '7'))
						{
							AGV_StartXY = 1;
							Send_InfoData_To_Fifo("D4\n",4);
						}
				else
						{
							AGV_StartXY = 2;
							Send_InfoData_To_Fifo("C5\n",4);
						}
}

//******************************�ӳ�����*******************************	

uint8_t Full_ENABLE_AGV_Return_Flag = 0;			//����ӳ����ر�־λ

/*
�ӳ�����
����	LightValue_X    ��ǰ��λ
		N_AGV_Start		��ʼ����
		RFID_AGVData_Y  RFID���������
**/
void AGV_Start1(uint8_t LightValue_X,uint8_t N_AGV_Start,uint8_t RFID_AGVData_Y)
{
	//((X*3-1)*Y+1)%4=N
	
	// N_AGVEnd = 1		B1
	// N_AGVEnd = 2		D1
	// N_AGVEnd = 3		F1
	// N_AGVEnd = 4		G2
	LightValue_X += 1; 
	uint8_t N_AGVEnd = ((((LightValue_X*3)-1)*RFID_AGVData_Y)%4)+1;
	Full_Diplay(N_AGVEnd);
	delay_ms(300);
	uint8_t AGV_Data[8] = {0x55,0x02,0x00,0x00,0x00,0x00,0x00,0xBB};
	AGV_Data[2] = 0x1A;
	AGV_Data[3] = N_AGV_Start;					//�ӳ���ʼ����
	AGV_Data[4] = LightValue_X;					//��ǰ��λ
	AGV_Data[5] = N_AGVEnd;						//�������
	Full_Car_CKS(AGV_Data);						//У���
	Send_ZigbeeData_To_Fifo(AGV_Data,8);
	delay_ms(200);
	Send_ZigbeeData_To_Fifo(AGV_Data,8);
	delay_ms(200);
	Send_ZigbeeData_To_Fifo(AGV_Data,8);
}


/*
�ӳ�����
**/
void AGV_Start2(void)
{
	uint8_t AGV_Data[8] = {0x55,0x02,0x00,0x00,0x00,0x00,0x00,0xBB};
	AGV_Data[2] = 0x1B;
	Full_Car_CKS(AGV_Data);					//У���
	Send_ZigbeeData_To_Fifo(AGV_Data,8);
	delay_ms(200);
	Send_ZigbeeData_To_Fifo(AGV_Data,8);
	delay_ms(200);
	Send_ZigbeeData_To_Fifo(AGV_Data,8);
}


/****************************************************************
�������ܣ�������AGV��������״̬
��	  ���nunm  ����״̬ ����ο�ͨѶЭ��
����  ֵ����
*****************************************************************/
void Send_AGV_Interaction_Function(uint8_t nun_data)
{
	u8 i;
	uint8_t Interaction_data[] = {0x55,0x02,0x00,0x00,0x00,0x00,0x00,0xBB};
	Interaction_data[2] =  nun_data;
	Full_Car_CKS(Interaction_data);					//У���
	for(i = 0;i < 2;i++)
	{
	Send_ZigbeeData_To_Fifo(Interaction_data,8);
   	delay_ms(200);
	}   	
}

/****************************************************************
�������ܣ�������AGV���ͺ�������
��	  ����*Infrared_Data  AGV�������ݵ�ַ
����  ֵ����
*****************************************************************/
void AGV_Control_Infrared(u8 *Infrared_Data)			//AGV_Control_Infrared(Caution_Text)��
{
	uint8_t Infrared_Temp[] = {0x55,0x02,0x00,0x00,0x00,0x00,0x00,0xbb};
	Infrared_Temp[2] = 0x10;
 	Infrared_Temp[3] = Infrared_Data[0];
	Infrared_Temp[4] = Infrared_Data[1];
	Infrared_Temp[5] = Infrared_Data[2];
	Full_Car_CKS(Infrared_Temp);					//����У���
	Send_ZigbeeData_To_Fifo(Infrared_Temp,8);
	Send_ZigbeeData_To_Fifo(Infrared_Temp,8);
    delay_ms(1000);
	Infrared_Temp[2] = 0x11;
	Infrared_Temp[3] = Infrared_Data[3];
	Infrared_Temp[4] = Infrared_Data[4];
	Infrared_Temp[5] = Infrared_Data[5];
	Full_Car_CKS(Infrared_Temp);					//����У���
	Send_ZigbeeData_To_Fifo(Infrared_Temp,8);
	Send_ZigbeeData_To_Fifo(Infrared_Temp,8);
	delay_ms(1000);
}
/****************************************************************
�������� ��A72��������
��	  ���IInfrared_Data  ��Ҫ���͵�����
����  ֵ����
*****************************************************************/
void A72_Send(u8 *A72_Data)
{
	u8 wife_data[12]={0x55,0xaa,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x20,0x40};//��WIFI������һ��Ҫ12�ֽ�һ��
	wife_data[2] = A72_Data[2];
	wife_data[3] = A72_Data[3];
	wife_data[4] = A72_Data[4];
	wife_data[5] = A72_Data[5];
	wife_data[6] = A72_Data[6];
	wife_data[7] = A72_Data[7];
	wife_data[8] = A72_Data[8];
	wife_data[9] = A72_Data[9];
	wife_data[10] = A72_Data[10];
	wife_data[11] = A72_Data[11];
	UartA72_TxClear();							 //�������������                   //A72ͨ�ŷ�ʽҪ1��UartA72_TxClear 2��UartA72_TxAddStr 3�UartA72_TxStart
	UartA72_TxAddStr(wife_data,12);  //��Ҫ���͵����ݷ��͵�������
	UartA72_TxStart();               //��ʼ����
	delay_ms(500);
}
/****************************************************************
�������� ��wifiģ�鷢������
��	  ���IInfrared_Data  ��Ҫ���͵�����
����  ֵ����
*****************************************************************/
void wifi_Send(u8 *wifi_Data)
{
	u8 i;
	u8 wifi_temp[]={0x55,0xaa,0x10,0x00,0x00,0x00,0x00,0xbb};//��WIFI������һ��Ҫ12�ֽ�һ��
	wifi_temp[2] = wifi_Data[2];
	wifi_temp[3] = wifi_Data[3];
	wifi_temp[4] = wifi_Data[4];
	wifi_temp[5] = wifi_Data[5];
	Full_Car_CKS(wifi_temp);
	for(i=0;i<3;i++)
	{
	    Send_WifiData_To_Fifo(wifi_temp,8);
			delay_ms(100);	
	}
}
/****************************************************************
��������:TFT��ʾ��������   A
��    ��: Car_Text��Ϊʵ�ʲ���
����ֵ:��
*****************************************************************/
void Control_TFT_show_data(u8 *Data)
{
		u8 i;
	for(i=0;i<5;i++)
		{
		u8 TFT_Data[8] = {0x55,0x0B,0x00,0x00,0x00,0x00,0x00,0xBB};//�ο�ͨѶЭ�顪��ƽ�������LED��ʾ��־�﷢����������ݽṹ
	/*******************
	**��������ǰ��λ
	*******************/
		TFT_Data[2]=0x20;
		TFT_Data[3]=Data[0];
		TFT_Data[4]=Data[1];
		TFT_Data[5]=Data[2];
		Full_Car_CKS(TFT_Data);
	
			Send_ZigBee_Info(TFT_Data,8);
			delay_ms(100);	//�����100���Ե���Сһ�㣬������û��Ӱ�졣���û��Ӱ�죬����Сһ�㣬����ֱ��ɾ��delay_ms(100)

	/******************
	**�������ݺ���λ
	********************/
		TFT_Data[2]=0x21;
		TFT_Data[3]=Data[3];
		TFT_Data[4]=Data[4];
		TFT_Data[5]=Data[5];
		Full_Car_CKS(TFT_Data);

			Send_ZigBee_Info(TFT_Data,8);
			delay_ms(100);	//�����100���Ե���Сһ�㣬������û��Ӱ�졣���û��Ӱ�죬����Сһ�㣬����ֱ��ɾ��delay_ms(100)
		}	

}
/****************************************************************
��������:TFT��ʾ��������  B
��    ��: Car_Text��Ϊʵ�ʲ���
����ֵ:��
*****************************************************************/
void Control_TFTB_show_data(u8 *Data)
{
		u8 i;
			
	for(i=0;i<5;i++)
		{

    u8 TFTB_Data[8] = {0x55,0x08,0x00,0x00,0x00,0x00,0x00,0xBB};//�ο�ͨѶЭ�顪��ƽ�������LED��ʾ��־�﷢����������ݽṹ
	/*******************
	**��������ǰ��λ
	*******************/
		TFTB_Data[2]=0x20;
		TFTB_Data[3]=Data[0];
		TFTB_Data[4]=Data[1];
		TFTB_Data[5]=Data[2];
		Full_Car_CKS(TFTB_Data);
		
			Send_ZigBee_Info(TFTB_Data,8);
			Send_ZigBee_Info(TFTB_Data,8);
			delay_ms(100);	//�����100���Ե���Сһ�㣬������û��Ӱ�졣���û��Ӱ�죬����Сһ�㣬����ֱ��ɾ��delay_ms(100)
		

	/******************
	**�������ݺ���λ
	********************/
		TFTB_Data[2]=0x21;
		TFTB_Data[3]=Data[3];
		TFTB_Data[4]=Data[4];
		TFTB_Data[5]=Data[5];
		Full_Car_CKS(TFTB_Data);
	
			Send_ZigBee_Info(TFTB_Data,8);
			Send_ZigBee_Info(TFTB_Data,8);
			delay_ms(100);	//�����100���Ե���Сһ�㣬������û��Ӱ�졣���û��Ӱ�죬����Сһ�㣬����ֱ��ɾ��delay_ms(100)
		
	}
}

/****************************************************************
��������:TFTHEX
��    ��:Principal_Tab��Ϊʵ��
����ֵ :��
*****************************************************************/
void Control_TFT_HEX_distance(u8 *Data)
{
  u8 i;
	u8 TFT_Dis_Data[8] = {0x55,0x0B,0x40,0x00,0x00,0x00,0x00,0xBB};//�ο�ͨѶЭ�顪��ƽ�������LED��ʾ��־�﷢����������ݽṹ

	TFT_Dis_Data[3] = Data[0];
    TFT_Dis_Data[4] = Data[1];
    TFT_Dis_Data[5] = Data[2];
    Full_Car_CKS(TFT_Dis_Data);
  
	
  for(i=0;i<3;i++)
	{              
		Send_ZigbeeData_To_Fifo(TFT_Dis_Data,8);
		delay_ms(100);	//�����100���Ե���Сһ�㣬������û��Ӱ�졣���û��Ӱ�죬����Сһ�㣬����ֱ��ɾ��delay_ms(100)
	}	
	}

/****************************************************************
��������:TFT��ʾ����    A
��    ��:Principal_Tab��Ϊʵ��
����ֵ :��
*****************************************************************/
void Control_TFT_show_distance(u8 *Data)
{
  u8 i;
	u8 TFT_Dis_Data[8] = {0x55,0x0B,0x50,0x00,0x00,0x00,0x00,0xBB};//�ο�ͨѶЭ�顪��ƽ�������LED��ʾ��־�﷢����������ݽṹ


  TFT_Dis_Data[4] = (u8)(Data[0]&0x0F);
  TFT_Dis_Data[5] = (u8)(((Data[1]&0x0F) << 4)|(Data[2]&0x0F));
  Full_Car_CKS(TFT_Dis_Data);
  
	
  for(i=0;i<3;i++)
	{              
		Send_ZigbeeData_To_Fifo(TFT_Dis_Data,8);
		delay_ms(100);	//�����100���Ե���Сһ�㣬������û��Ӱ�졣���û��Ӱ�죬����Сһ�㣬����ֱ��ɾ��delay_ms(100)
	}	
	}
/****************************************************************
��������:TFT��ʾ����    B
��    ��:Principal_Tab��Ϊʵ��
����ֵ :��
*****************************************************************/
void Control_TFTB_show_distance(u8 *Data)
{
  u8 i;
	u8 TFTB_Dis_Data[8] = {0x55,0x08,0x50,0x00,0x00,0x00,0x00,0xBB};//�ο�ͨѶЭ�顪��ƽ�������LED��ʾ��־�﷢����������ݽṹ


  TFTB_Dis_Data[4] = (u8)(Data[0]&0x0F);
  TFTB_Dis_Data[5] = (u8)(((Data[1]&0x0F) << 4)|(Data[2]&0x0F));
  Full_Car_CKS(TFTB_Dis_Data);
  
	
  for(i=0;i<3;i++)
	{              
		Send_ZigbeeData_To_Fifo(TFTB_Dis_Data,8);
		delay_ms(100);	//�����100���Ե���Сһ�㣬������û��Ӱ�졣���û��Ӱ�죬����Сһ�㣬����ֱ��ɾ��delay_ms(100)
	}	
	}

/****************************************************************
��������:TFT��ʱ����    A
��    ��:Mode_num=1 ��ʱ���� Mode_num=2 ��ʱ����
����ֵ:��
*****************************************************************/
void Control_TFT_show_Time(u8 Mode_num)
{
	if(Mode_num == 1)
		{
		Send_ZigBee_Info(TFT_JSK,8);		//���ƶ�ʱ����
	  } 
		else if(Mode_num == 2)
		{
		Send_ZigBee_Info(TFT_JSG,8);		//���ƶ�ʱ����
	  }
		else if(Mode_num==3)
		{
		Send_ZigBee_Info(TFT_JSClear,8);		//���ƶ�ʱ������
		}
}
/****************************************************************
��������:TFT��ʱ����    B
��    ��:Mode_num=1 ��ʱ���� Mode_num=2 ��ʱ����
����ֵ:��
*****************************************************************/
void Control_TFTB_show_Time(u8 Mode_num)
{
	if(Mode_num == 1)
		{
		Send_ZigBee_Info(TFTB_JSK,8);		//���ƶ�ʱ����
	  } 
		else if(Mode_num == 2)
		{
		Send_ZigBee_Info(TFTB_JSG,8);		//���ƶ�ʱ����
	  }
		else if(Mode_num==3)
		{
		Send_ZigBee_Info(TFTB_JSClear,8);		//���ƶ�ʱ������
		}
}
/*******************************************
�������ܣ�TFT��ҳ���� A
��	  ����num = 1 �������Ͻ��з�ҳ num = 2 �������½��з�ҳ
����  ֵ����
*******************************************/
void TFT_turn_over(u8 num)   
{
	 if(num == 1)//�������Ͻ��з�ҳ
	 {
		Send_ZigBee_Info(TFT_UP,8);
	 } 
	 else if(num == 2) //�������½��з�ҳ
   {
		Send_ZigBee_Info(TFT_down,8);
	 }
}
/*******************************************
�������ܣ�TFT��ҳ���� B
��	  ����num = 1 �������Ͻ��з�ҳ num = 2 �������½��з�ҳ
����  ֵ����
*******************************************/
void TFTB_turn_over(u8 nm)
{
	 if(nm == 1)//�������Ͻ��з�ҳ
	 {
		Send_ZigBee_Info(TFTB_UP,8);
	 } 
	 else if(nm == 2) //�������½��з�ҳ
   {
		Send_ZigBee_Info(TFTB_down,8);
	 }
}
/****************************************************************
�������ܣ�������բ
mode 1 �̶�ָ���
mode 2 ָ�����ƿ���(��ʾ��������)
****************************************************************/
void Track_gate_Open(int mode,u8 *data)
{
	uint8_t DZ_data[8]={0x55,0x03,0x00,0x00,0x00,0x00,0x00,0xbb};
	if(mode == 1)
	{
	 Send_ZigbeeData_To_Fifo(DZ_data,8);
	}
	else if(mode == 2)
	{
	DZ_data[2]=0x10;
	DZ_data[3]=data[0];
	DZ_data[4]=data[1];
	DZ_data[5]=data[2];
	delay_ms(500);
	Full_Car_CKS(DZ_data);
	Send_ZigbeeData_To_Fifo(DZ_data,8);
		Send_ZigbeeData_To_Fifo(DZ_data,8);
    DZ_data[2]=0x11;
	DZ_data[3]=data[3];
	DZ_data[4]=data[4];
	DZ_data[5]=data[5];
	Full_Car_CKS(DZ_data);
	Send_ZigbeeData_To_Fifo(DZ_data,8);
		Send_ZigbeeData_To_Fifo(DZ_data,8);
	}
}
/****************************************************************
�������ܣ���բ״̬�ش�
****************************************************************/
void daozha_zhuantai(void)
{
	u8 DZ_ZT_Data[8] = {0x55,0x03,0x20,0x01,0x00,0x00,(0x20+0x01+0x00+0x00)%256,0xBB};
	Send_ZigbeeData_To_Fifo(DZ_ZT_Data,8);
	delay_ms(100);
}

/****************************************************************
�������ܣ�������ͨ��ʶ��A
��	  ��: ��
����  ֵ����
****************************************************************/
void Full_Traffic_Light_Recognize_patterns()
{
	u8 i;
	u8 JTD_Data[8] = {0x55,0x0E,0x01,0x00,0x00,0x00,0x00,0xBB};
  Full_Car_CKS(JTD_Data);
	for(i=0;i<3;i++)
	{              
		Send_ZigbeeData_To_Fifo(JTD_Data,8);
		delay_ms(100);	//�����100���Ե���Сһ�㣬������û��Ӱ�졣���û��Ӱ�죬����Сһ�㣬����ֱ��ɾ��delay_ms(100)
	}	
}
/****************************************************************
��������:��ͨ��ʶ�����ȶ�A
��	  ��: mode_data[3]   0x01(��ɫ) 0x02(��ɫ) 0x03(��ɫ)   ʶ��������ȷ��
����  ֵ����
****************************************************************/
void Full_Identification_result_comparison(u8 mode_data)
{
  u8 i;
	u8 D_Data[8] = {0x55,0x0E,0x02,0x00,0x00,0x00,0x00,0xBB};
	D_Data[3] = mode_data;
  Full_Car_CKS(D_Data);
	for(i=0;i<2;i++)
	{              
		Send_ZigbeeData_To_Fifo(D_Data,8);
		delay_ms(50);	//�����100���Ե���Сһ�㣬������û��Ӱ�졣���û��Ӱ�죬����Сһ�㣬����ֱ��ɾ��delay_ms(100)
	}	


}
/****************************************************************
�������ܣ�������ͨ��ʶ��B
��	  ��: ��
����  ֵ����
****************************************************************/
void Full_Braffic_Light_Recognize_patterns()
{
	u8 i;
	u8 TD_Data[8] = {0x55,0x0F,0x01,0x00,0x00,0x00,0x00,0xBB};
  Full_Car_CKS(TD_Data);
	for(i=0;i<2;i++)
	{              
		Send_ZigbeeData_To_Fifo(TD_Data,8);
		delay_ms(100);	//�����100���Ե���Сһ�㣬������û��Ӱ�졣���û��Ӱ�죬����Сһ�㣬����ֱ��ɾ��delay_ms(100)
	}	
}
/****************************************************************
��������:��ͨ��ʶ�����ȶ�B
��	  ��: mode_data[3]   0x01(��ɫ) 0x02(��ɫ) 0x03(��ɫ)   ʶ��������ȷ��
����  ֵ����
****************************************************************/
void Full_Bdentification_result_comparison(u8 Bo_data)
{
  u8 i;
	u8 BD_Data[8] = {0x55,0x0F,0x02,0x00,0x00,0x00,0x00,0xBB};
	BD_Data[3] = Bo_data;
  Full_Car_CKS(BD_Data);
	for(i=0;i<2;i++)
	{              
		Send_ZigbeeData_To_Fifo(BD_Data,8);
		delay_ms(50);	//�����100���Ե���Сһ�㣬������û��Ӱ�졣���û��Ӱ�죬����Сһ�㣬����ֱ��ɾ��delay_ms(100)
	}	


}
/****************************************************************
��������:��բ��ʾ��������
��    ��: *Dataָ�룬ָ������������
����ֵ:��
*****************************************************************/
void Control_DZ_show_data(u8 *Data)
{
		u8 i;
		u8 DZ_Data[8] = {0x55,0x03,0x00,0x00,0x00,0x00,0x00,0xBB};//�ο�ͨѶЭ�顪��ƽ�������LED��ʾ��־�﷢����������ݽṹ
	/*******************
	**��������ǰ��λ
	*******************/
		DZ_Data[2]=0x10;
		DZ_Data[3]=Data[0];
		DZ_Data[4]=Data[1];
		DZ_Data[5]=Data[2];
		Full_Car_CKS(DZ_Data);
		for(i=0;i<3;i++)
		{
			Send_ZigBee_Info(DZ_Data,8);
//			delay_ms(100);	//�����100���Ե���Сһ�㣬������û��Ӱ�졣���û��Ӱ�죬����Сһ�㣬����ֱ��ɾ��delay_ms(100)
		}

	/******************
	**�������ݺ���λ
	********************/
		DZ_Data[2]=0x11;
		DZ_Data[3]=Data[3];
		DZ_Data[4]=Data[4];
		DZ_Data[5]=Data[5];
		Full_Car_CKS(DZ_Data);
		delay_ms(200);
		for(i=0;i<3;i++)
		{
			Send_ZigBee_Info(DZ_Data,8);
//			delay_ms(100);	//�����100���Ե���Сһ�㣬������û��Ӱ�졣���û��Ӱ�죬����Сһ�㣬����ֱ��ɾ��delay_ms(100)
		}	

}

/****************************************************************
�������ܣ�ȫ�Զ���������
��	  ����*s_data ����ָ�� length  ���ݳ���
����  ֵ����
*****************************************************************/
void full_Speech(u8 *s_data,u16 lenght)
{   
	u16 i = 0;
	u8 Speech[200];
	Speech[0] = 0xFD;
	Speech[1] = (lenght+2)/256;
	Speech[2] = (lenght+2)%256;
	Speech[3] = 0x01;
	Speech[4] = 0x01;
	for(i=0;i<lenght;i++)
	{
		Speech[i+5] = s_data[i];
	}
	delay_ms(350);
	Send_ZigbeeData_To_Fifo(Speech,lenght+5);
	delay_ms(350);
}


/****************************************************************
�������ܣ�LED����ܿ���
��	  ����Mode_num=1 �򿪼�ʱ  Mode_num=2 �رռ�ʱ
����  ֵ����
*****************************************************************/
void Control_LED_Nixie_light(u8 Mode_num)
{
	if(Mode_num == 1)
	{
		Send_ZigBee_Info(SMG_JSK,8);		//��ʱ����
	} 
	else if(Mode_num == 2)
	{
		Send_ZigBee_Info(SMG_JSG,8);		//��ʱ�ر�
	}
	else if(Mode_num == 3)
	{
		Send_ZigBee_Info(SMG_JSQ,8);		//��ʱ����
	}
		
}
/****************************************************************
�������ܣ�LED�������ʾ����
��	  ����*dataָ������  x =1������ʾ�ڵ�һ��  x=2������ʾ�ڵڶ���
����  ֵ����
*****************************************************************/
void Control_LED_show_data(u8 X,u8 *Data)
{
  u8 LED_Data[8] = {0x55,0x04,0x00,0x00,0x00,0x00,0x00,0xBB};//�ο�"ͨѶЭ�顪��ƽ�������LED��ʾ��־�﷢����������ݽṹ"
  if(X == 1)					//��һ��
  {
	  LED_Data[2] = 0x01;  
  } 
  else if( X == 2 ) //�ڶ��� 
	{	 		
	  LED_Data[2] = 0x02;  
  }
  LED_Data[3] = (u8)((Data[0]&0x0F) << 4)|(Data[1]&0x0F);
  LED_Data[4] = (u8)((Data[2]&0x0F) << 4)|(Data[3]&0x0F);
  LED_Data[5] = (u8)((Data[4]&0x0F) << 4)|(Data[5]&0x0F);
	Full_Car_CKS(LED_Data);					//����У���
  Send_ZigBee_Info(LED_Data,8);
}

/****************************************************************
�������ܣ�LED��ʾ
��	  ������
����  ֵ����
****************************************************************/
void LED_graphics(u8 *Data)
{
	
	uint8_t i = 0;
	uint8_t LED_DisTab[] = {0x55,0x04,0x02,0xA1,0xB1,0xC3,0x00,0xBB};
	delay_ms(500);
	delay_ms(500);
	delay_ms(500); 
    LED_DisTab[2]= Data[0];
    LED_DisTab[3]= Data[1];
    LED_DisTab[4]= Data[2];
	LED_DisTab[5]= Data[3];
	Full_Car_CKS(LED_DisTab);					//����У���
	Send_ZigbeeData_To_Fifo(LED_DisTab,8);
	delay_ms(200);
	Send_ZigbeeData_To_Fifo(LED_DisTab,8);
}

/****************************************************************
�������ܣ��������峵�⵹�������ƺ���
��	  ������Full_Fucheruku_Return_Flag ==1:A��   2��B��
����  ֵ����
****************************************************************/
void Full_Fuche_garage_Control(void)
{	
		u8 i;
		if(Full_Fucheruku_Return_Flag ==1)
		{
			SetStereo_garage(1);
			delay_ms(500);
      delay_ms(500);
			delay_ms(500);
			delay_ms(500);
			delay_ms(500);
			 for(u8 i=0;i<40;i++)
	   {
			 delay_ms(500);
			 delay_ms(500);
		    uint8_t Stereo_ab[] = {0x55,0x0D,0x02,0x01,0x00,0x00,0x03,0xBB};
	              Send_ZigbeeData_To_Fifo(Stereo_ab,8);
          if((Zigb_Rx_Buf[3] == 0x01) && (Zigb_Rx_Buf[4] == 0x01))
					{
						 Send_InfoData_To_Fifo("H\n",3);
						i=20;
						break;
					}
		 	}
		 }
		
		if(Full_Fucheruku_Return_Flag ==2)
		{
			SetStereo_Barage(1);
			delay_ms(500);
      delay_ms(500);
			delay_ms(500);
			delay_ms(500);
			delay_ms(500);
			 for(u8 i=0;i<40;i++)
	   {
			 delay_ms(500);
			 delay_ms(500);
		    uint8_t Stereo_ab[] = {0x55,0x05,0x02,0x01,0x00,0x00,0x03,0xBB};
	              Send_ZigbeeData_To_Fifo(Stereo_ab,8);
          if((Zigb_Rx_Buf[3] == 0x01) && (Zigb_Rx_Buf[4] == 0x01))
					{
						i=20;
						break;
					}
		 	}
		 }

}


/****************************************************************
�������ܣ����峵�����A
��	  ������
����  ֵ����
****************************************************************/
void SetStereo_garage(uint8_t mode)
{
	uint8_t Stereo_Tab[] = {0x55,0x0D,0x01,0x00,0x00,0x00,0x00,0xBB};
	switch(mode)
	{
		case 1:				//�����һ��
			Stereo_Tab[3] = 0x01;
			break;
		case 2:				//����ڶ���
			Stereo_Tab[3] = 0x02;
			break;
		case 3:				//���������
			Stereo_Tab[3] = 0x03;
			break;
		case 4:				//������Ĳ�
			Stereo_Tab[3] = 0x04;
			break;
	}
	Full_Car_CKS(Stereo_Tab);					//����У���
	delay_ms(200);
	Send_ZigbeeData_To_Fifo(Stereo_Tab,8);
	delay_ms(200);
	Send_ZigbeeData_To_Fifo(Stereo_Tab,8);
	delay_ms(200);
	Send_ZigbeeData_To_Fifo(Stereo_Tab,8);
}
/****************************************************************
�������ܣ��������峵�ⷵ��״ָ̬��A
��	  ����Query_mode = 1 ���󷵻ص�ǰ���� Query_mode = 2 ���󷵻ص�ǰ����״̬
����  ֵ����
****************************************************************/
void Send_Get_Stereo_Return(u8 Query_mode)
{
	uint8_t Stereo_Tab[] = {0x55,0x0D,0x02,0x00,0x00,0x00,0x00,0xBB};
	if(Query_mode == 1)     //���󷵻ص�ǰ����
	{
		Stereo_Tab[3] = 0x01;
	}
	if(Query_mode == 2)     //���󷵻ص�ǰ����״̬
	{
		Stereo_Tab[3] = 0x02;
	}
	  Full_Car_CKS(Stereo_Tab);					//����У���
	  Send_ZigbeeData_To_Fifo(Stereo_Tab,8);
}

/****************************************************************
�������ܣ����峵�⵹�������ƺ���A
��	  ������
����  ֵ����
****************************************************************/
void Full_astern_garage_Control(void)
{
		u8 i;
		if(Full_astern_garage_Flag ==0)
		{
			SetStereo_garage(1);
			delay_ms(100);
//			while(Stop_Flag != 0x09)	//�ȴ������һ��
//			{
//				Set_tba_Beep(SET);
//			}	
//			 Set_tba_Beep(RESET);
			for(i=10;i>0;i--)
			{
				Send_Get_Stereo_Return(1);
				delay_ms(100);
				if((Zigb_Rx_Buf[3] == 0x01)&&(Zigb_Rx_Buf[4]==0x01))//���⵱ǰλ�ڵ�һ��
				{
					Full_astern_garage_Flag = 1;
					i = 0;
					break;
				}
			}
		 }
		else
		{
			Send_Get_Stereo_Return(2);
			delay_ms(100);
		}
}
/****************************************************************
�������ܣ����峵�⵹�������ƺ���B
��	  ������
����  ֵ����
****************************************************************/
void Full_Bstern_garage_Control(void)
{
		u8 i;
		if(Full_Bstern_garage_Flag ==0)
		{
			SetStereo_Barage(1);
			delay_ms(100);
//			while(Stop_Flag != 0x09)	//�ȴ������һ��
//			{
//				Set_tba_Beep(SET);
//			}	
//			 Set_tba_Beep(RESET);
			for(i=10;i>0;i--)
			{
				Send_Get_Btereo_Return(1);
				delay_ms(100);
				if((Zigb_Rx_Buf[3] == 0x01)&&(Zigb_Rx_Buf[4]==0x01))//���⵱ǰλ�ڵ�һ��
				{
					Full_Bstern_garage_Flag = 1;
					i = 0;
					break;
				}
			}
		 }
		else
		{
			Send_Get_Btereo_Return(2);
			delay_ms(100);
		}
}
/****************************************************************
�������ܣ��������峵�ⷵ��״ָ̬��B
��	  ����Query_mode = 1 ���󷵻ص�ǰ���� Query_mode = 2 ���󷵻ص�ǰ����״̬
����  ֵ����
****************************************************************/
void Send_Get_Btereo_Return(u8 Query_mode)
{
	uint8_t Stereo_Tab[] = {0x55,0x05,0x02,0x00,0x00,0x00,0x00,0xBB};
	if(Query_mode == 1)     //���󷵻ص�ǰ����
	{
		Stereo_Tab[3] = 0x01;
	}
	if(Query_mode == 2)     //���󷵻ص�ǰ����״̬
	{
		Stereo_Tab[3] = 0x02;
	}
	  Full_Car_CKS(Stereo_Tab);					//����У���
	  Send_ZigbeeData_To_Fifo(Stereo_Tab,8);
}
/****************************************************************
�������ܣ����峵�����B
��	  ������
����  ֵ����
****************************************************************/
void SetStereo_Barage(uint8_t mode)
{
	uint8_t Stereo_Tab[] = {0x55,0x05,0x01,0x00,0x00,0x00,0x00,0xBB};
	switch(mode)
	{
		case 1:				//�����һ��
			Stereo_Tab[3] = 0x01;
			break;
		case 2:				//����ڶ���
			Stereo_Tab[3] = 0x02;
			break;
		case 3:				//���������
			Stereo_Tab[3] = 0x03;
			break;
		case 4:				//������Ĳ�
			Stereo_Tab[3] = 0x04;
			break;
	}
	Full_Car_CKS(Stereo_Tab);					//����У���
	delay_ms(200);
	Send_ZigbeeData_To_Fifo(Stereo_Tab,8);
	delay_ms(200);
	Send_ZigbeeData_To_Fifo(Stereo_Tab,8);
	delay_ms(200);
	Send_ZigbeeData_To_Fifo(Stereo_Tab,8);
}

/****************************************************************
�������ܣ��������߳��
��	  ������
����  ֵ����
****************************************************************/
void Wireless_charging(void)
{
	uint8_t Stereo_Tab[] = {0x55,0x0A,0x01,0x01,0x00,0x00,0x00,0xBB};
	Full_Car_CKS(Stereo_Tab);					//����У���
	delay_ms(200);
	Send_ZigbeeData_To_Fifo(Stereo_Tab,8);
	delay_ms(200);
	Send_ZigbeeData_To_Fifo(Stereo_Tab,8);
	delay_ms(200);
	Send_ZigbeeData_To_Fifo(Stereo_Tab,8);
}
/****************************************************************
�������ܣ����̨���⿪��
��	  ����*���⿪������
����  ֵ����
*****************************************************************/
void fht_frared(u8 *Data)
{
	u8 i;
  uint8_t  HW_Tab[8] = {0x55,0x07,0x00,0x00,0x00,0x00,0x00,0xBB};
	/*******************
	��������ǰ��λ
	*******************/
  HW_Tab[2]=0x10;
  HW_Tab[3]=Data[0];
  HW_Tab[4]=Data[1];
  HW_Tab[5]=Data[2];
  Full_Car_CKS(HW_Tab);					//����У���
	for(i=0;i<3;i++)
	{
	Infrared_Send(HW_Tab,8);
delay_ms(100);		
	}

	/*******************
	�������ݺ���λ
	*******************/
	delay_ms(200);
	HW_Tab[2]=0x11;
  HW_Tab[3]=Data[3];
  HW_Tab[4]=Data[4];
  HW_Tab[5]=Data[5];
  Full_Car_CKS(HW_Tab);					//����У���	
	for(i=0;i<3;i++)
	{
	Infrared_Send(HW_Tab,8);	
		delay_ms(100);
	}
		HW_Tab[3]=HW_Tab[3]+0X30;
	 Send_InfoData_To_Fifo(&HW_Tab[3],1);
  delay_ms(100);
}


/****************************************************************
�������ܣ��޸ķ��̨���⿪������
��	  ����*Beacon_tower_Data �޸ĺ��⿪������
����  ֵ����
*****************************************************************/
void Modify_Beacon_tower_Infrared(u8 *Beacon_tower_Data)			//AGV_Control_Infrared(Caution_Text)��
{
	uint8_t Infrared_Temp[] = {0x55,0x07,0x00,0x00,0x00,0x00,0x00,0xbb};
	Infrared_Temp[2] = 0x10;
 	Infrared_Temp[3] = Beacon_tower_Data[0];
	Infrared_Temp[4] = Beacon_tower_Data[1];
	Infrared_Temp[5] = Beacon_tower_Data[2];
//	Full_Car_CKS(Infrared_Temp);					//����У���
	Infrared_Temp[6] = (Infrared_Temp[2]+Infrared_Temp[3]+Infrared_Temp[4]+Infrared_Temp[5])%256;
	Send_ZigbeeData_To_Fifo(Infrared_Temp,8);
  delay_ms(300);
	Infrared_Temp[2] = 0x11;
	Infrared_Temp[3] = Beacon_tower_Data[3];
	Infrared_Temp[4] = Beacon_tower_Data[4];
	Infrared_Temp[5] = Beacon_tower_Data[5];
	Infrared_Temp[6] = (Infrared_Temp[2]+Infrared_Temp[3]+Infrared_Temp[4]+Infrared_Temp[5])%256;
//	Full_Car_CKS(Infrared_Temp);					//����У���
	Send_ZigbeeData_To_Fifo(Infrared_Temp,8);
	delay_ms(300);
}

//******************************�ӳ�����Code End***********************

int mark = 0; 
int Full_Run_Flag = 0;            //ȫ�Զ�����״̬��־λ
int Full_ETC_Open_Flag = 0;				//ETC�����ر�־λ
int Full_FC_Open_Flag = 0;			//�ȴ���������־λ
int Full_DZ_Open_Flag = 0;				//DZ�����ر�־λ
int Full_Stereo_Return_Flag = 0;		//���峵�������ر�־λA 
int Full_astern_garage_Flag = 0;		//���峵����������־λA
int Full_Btereo_Return_Flag = 0;		//���峵�������ر�־λB
int Full_Bstern_garage_Flag = 0;		//���峵����������־λB
int Full_AGV_Return_Flag1 = 0;			//ȫ�Զ��ӳ����ر�־λ1
int Full_AGV_Return_Flag2 = 0;			//ȫ�Զ��ӳ����ر�־λ2
int Full_Fucheruku_Return_Flag = 0;		//�������峵���־λA 
int RFID_DKWC_Flag = 0;


uint8_t RFID_S50_Address = 0x01;
uint8_t AGV_Return = 0x1B;			//Ĭ�Ͻ��շ��ص����բ��־λλ��״̬ 

uint8_t WIFI_Data[8] = {0x55,0xAA,0x00,0x00,0x00,0x00,0x00,0xBB};

//                        ������ Բ ���� ����� ����
//						             1    2    3    4     5   6
uint8_t Shape_Number[] = {0x31,0x32,0x33,0x34,0x35,0x36};
// 						           ��ɫ����ɫ����ɫ����ɫ����ɫ����ɫ����ɫ
uint8_t Color_Number[] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07};
//����			
uint8_t Plate_Number[] = {0xA1,0x31,0x42,0x32,0x43,0x33};   //����ʶ���ƽ��

uint8_t Qr_Code[] = {0x55,0x01,0x02,0x03,0x04,0xFF};   //ʶ���ά����--������������

uint8_t Speech_recognition_data[] = {0xAF,0x06,0x00,0x02,0x00,0x00,0x01,0xBF}; //����ʶ����ȷ��־����

uint8_t Qr_Code_1[] = {0x47,0x37,0x02,0x03,0x04,0xFF};   //ʶ���ά����--������������

uint8_t LTXS_YY[] = {0x11,0x30,0x00,0x00,0x00};      //������ʾ ��������ֵ
//

void Send_AVG_data(uint8_t *data)
{
	uint8_t  AVG_data[] = {0x55,0x02,0x00,0x00,0x00,0x00,0x00,0xbb};
	AVG_data[2] = 0xA1;
	AVG_data[3] = data[0];
	Full_Car_CKS(AVG_data);					//����У���
	Send_ZigbeeData_To_Fifo(AVG_data,8);
	Send_ZigbeeData_To_Fifo(AVG_data,8);
	delay_ms(1000);
}

int BitCount(unsigned int n)
{
    unsigned int c =0 ;
    while (n)
	{
		if (n & 1)
		{
			c++;
		}
		n = n >> 1;		
	}
	
   return 8-c ;
}



void Full_Find_card(void)     //Ѱ��
{
	u8 Track_Value[3]={0};
	uint8_t cont[1]={0};
	char status = MI_ERR;
	uint8_t CT[2];	
	int a=20;
	uint8_t Go=1,Flag=0;	 
	Track(60);	
	Send_Debug_Info((u8*)"(1)\n",4);      
	while(Go)
	{				
					Track_Value[0] = Get_Host_UpTrack(TRACK_H8);	//·�����				
					if((Track_Value[0]&0x3C)==0x3C)
					{	
						  Send_Debug_Info("��2��\n",4);			
					  	Full_Stop();
					    delay_ms(500);
						  Track_Value[1] = Get_Host_UpTrack(TRACK_H8);
						  Track_Value[2] = Get_Host_UpTrack(TRACK_Q7);//�����������Track_Value[2]==0x7F�������ʮ��·��Track_Value[2]!=0xF7 		
						  cont[0]=((BitCount(Track_Value[1]))+(BitCount(Track_Value[2])));
						  delay_ms(500);
						  Full_Go(70,70);
						  Track_Value[0] = Get_Host_UpTrack(TRACK_Q7);
						  delay_ms(500);
						if(Track_Value[0]==0x18||Track_Value[0]==0x08||Track_Value[0]==0x10||Track_Value[0]==0x18||Track_Value[0]==0x0c||Track_Value[0]==0x30)
						{
							Send_Debug_Info("��2-1��\n",6);			
							Flag=1;
							Go=0;
							Full_Go(60,1300);
						}
						else if((Track_Value[0]&0x3C)==0x3C)
						{
							delay_ms(500);
							Full_Go(40,300);						
						    delay_ms(500);				 		
							delay_ms(500);
						while(1)
						{
							status = PcdRequest(PICC_REQALL,CT);													
							if(status==MI_OK)
							{			
								RFID_Card_Position[0] =RFID;		//RFID λ������Ϊ 4����E2������Ӧ����C2�ɣ�
								delay_ms(100);
								Full_Read_Card();	//��������
								delay_ms(500);
								delay_ms(500);
								Send_Debug_Info("��3��\n",4);
								delay_ms(500);
								delay_ms(500);
								Set_tba_Beep(SET);//������������
								delay_ms(500);
								delay_ms(500);
								Set_tba_Beep(RESET);//������������
								RFID_Data_Handler(sRXRFID);//��RFID�����ݽ��д���ͨ��Full_Read_Card()�������ж���������,��ȡ��RFID�������ݴ洢��RXREID���������		
								Send_Debug_Info("ok\n",3);
								delay_ms(500);							
								Go=0;
								break;
							}
							else
							{
								Full_Back(80,50);
							}
						}
					 }
					}				
				if(Track_Value[0]==0x00|Track_Value[0]==0x01||Track_Value[0]==0x80)
				{
					Full_Stop();
					delay_ms(500);
					delay_ms(500);
					Send_Debug_Info("NULL\n",5);					
					Full_Go(60,380);
					Go=0;			
				}
	}
	 if(status==MI_OK)
	 {
		 if(cont[0]<=0x02)//������
			{
				Send_Debug_Info("line\n",5);
				//Track_Value[0]=Get_Host_UpTrack(TRACK_H8);			
				//if(Track_Value[0]==0xEF||Track_Value[0]==0xE7||Track_Value[0]==0xF7)
				{
					Full_Find_card();
					delay_ms(500);
				}	
					//Full_Go(Get_Go_sp(),Get_Go_mp());	
				//Send_ZigBee_Info(Track_Value,8);
			}
	   else if(cont[0]>=0x03)//ʮ��·��
			{
				RFID_Card_Position[0] =0xD2;
				Send_Debug_Info("TEN\n",4);
				Full_Stop();
			}
			Send_ZigbeeData_To_Fifo(cont,2);
  }
	 if(Flag==1)
	 {
		 Flag=0;
		 Send_Debug_Info("findcard\n",9);
		 Full_Find_card();
	 }
}



void TFT_GD(int mode,int num)     /********TFT�̶�ͼƬ*********/
{
uint8_t TFT_GD_data[8]={0x55,0x00,0x10,0x00,0x00,0x00,0x00,0xBB};
uint8_t a[12] = {0x55,0xaa,0x2A,'A','7','*','#','Z','-',0x00,0x00,0xbb};
  switch(mode)
{ 
		case 1:
			TFT_GD_data[1]=0x0B;
		 break;

		case 2: 
	        TFT_GD_data[1]=0x08;
		break;
}
	switch(num)
	{
		case 1:
			TFT_GD_data[4]=0x01;
		  break;
		
		case 2:
			TFT_GD_data[4]=0x20;
		  break;
	}
	Full_Car_CKS(TFT_GD_data);
	Send_ZigBee_Info(TFT_GD_data,8);
	delay_ms(500);
	Send_ZigBee_Info(TFT_GD_data,8);
}


//mode==1 ��ȡ��λ��mode==2����
int Full_Light(uint8_t gear,int mode)
{
	uint8_t i;
	uint8_t gear_init = 0;	// ��ʼ��λֵ
	uint16_t array[2];		//������ѧϰ�Ĺ⵵λ����
	
	if((gear > 0) && (gear < 5))
	{
		delay_ms(100);
		array[0] = Get_Bh_Value();		//��ǿ�ȴ�����	
		for(i=0; i<4; i++)
		{
			gear_init++;
			Infrared_Send(H_1,4);
			delay_ms(500);
			delay_ms(500);
			delay_ms(500);
			array[1] = Get_Bh_Value();		//��ǿ�ȴ�����
			if (array[0] < array[1]) 
			{
				array[0] = array[1];
				array[1] = 0;
			}
			else
			{
				gear_init = 5 - gear_init;
				break;
			}
		}
		if(mode==1)
		{
			 if(gear_init==2)
			{
				Infrared_Send(H_1,4);	//��Դ��λ��1
			}
			else if(gear_init==3)
			{
				Infrared_Send(H_2,4);	//��Դ��λ��2
			}
			else if(gear_init==4)
			{
				Infrared_Send(H_3,4);	//��Դ��λ��3
			}
		}
		if(mode==2)
		{
			if(gear==2)
			{
				Infrared_Send(H_1,4);	//��Դ��λ��1
			}
			else if(gear==3)
			{
				Infrared_Send(H_2,4);	//��Դ��λ��2
			}
			else if(gear==4)
			{
				Infrared_Send(H_3,4);	//��Դ��λ��3
			}
		}
	}
	return gear_init;
}
void Control_TFT_show_distance1()
{
 uint16_t time = 0;
	uint8_t LED_DisTab[] = {0x55,0x0B,0x50,0x00,0x00,0x00,0x00,0xBB};
	delay_ms(500);
	delay_ms(500);
	delay_ms(500);
	for(time=0;time<5;time++)
	{
		Ultrasonic_Ranging();		//����������
		delay_ms(100);
	}	
	LED_DisTab[4]=dis/100;
	LED_DisTab[5]=dis%100;
	delay_ms(500);
	Full_Car_CKS(LED_DisTab);					//����У���
	Send_ZigbeeData_To_Fifo(LED_DisTab,8);
	delay_ms(200);
	Send_ZigbeeData_To_Fifo(LED_DisTab,8);
	delay_ms(300);
	JL_Code[0]=(dis/100)+0x30;
	JL_Code[1]=((dis%100/10))+0x30;
	JL_Code[2]=(dis%10)+0x30;
	Send_Debug_Info(JL_Code,3);  //debug ���׷�������
	Send_Debug_Info("mm\n",3);
}


void More_Read_card(char *Sector)
{
	int len=strlen(Sector),cont=0;
	u8 data[16];
	while(cont!=len)
	{
		if(Full_Read1_Card(Sector[cont],data))
		{
			cont++;
			RFID_Data_Handler(data);
		}
	}
}


int Full_Read1_Card(u8 Addr,u8 *data)
{
	char status = MI_ERR;
	u8 CT[2];									//������
	u8 SN[4]; 									//����
	u8 KEY[6]={0xff,0xff,0xff,0xff,0xff,0xff}; //��Կ		
	int a=0;
		status=PcdRequest(PICC_REQALL,CT);	
		if(status==MI_OK)
		{
			status = MI_ERR;
			Send_Debug_Info("f OK\n",6);
			status = PcdAnticoll(SN);				//����ײ
			if(status == MI_OK)
			{
				status=MI_ERR;
				Send_Debug_Info("a OK\n",6);
				delay_ms(100);			
				status =PcdSelect(SN);				//ѡ���˿�
				if(status == MI_OK)					//ѡ���ɹ�
				{
					status=MI_ERR;
					Send_Debug_Info("P OK\n",6);
					delay_ms(100);
					status =PcdAuthState(0x60,Addr,KEY,SN);		//��֤��Կ:0x60 = ��֤A��Կ;0x61 = ��֤B��Կ�����ַ0x03\KEYΪ����\ SN�����к�(��ƿ���)
					if(status == MI_OK)		//RFID�����롢������֤�ɹ�
					{					
						status = MI_ERR;					
						Send_Debug_Info("t OK\n",6);
						delay_ms(100);
						status=PcdRead(Addr,data);				//���������У�s�Ǹú���һ��ʼ����Ķ���Ĭ�Ͽ��ַdataΪ���������ݣ�16�ֽ�
						if(status == MI_OK)
						{
							a=1;
							status = MI_ERR;							
							Send_Debug_Info("r OK\n",6);		//���͵�����Ϣ"read OK",�������ɹ�
							delay_ms(100);
							Send_InfoData_To_Fifo(data,16);
							return a;
						}				
					}
				}
			}	
		}
		else
		{
			return a;
		}			
}



