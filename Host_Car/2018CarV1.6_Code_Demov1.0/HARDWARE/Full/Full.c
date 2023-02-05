#include "main.h"

u8 RFID_Card_Position[2]={0x00,0x00} ;				//RFID卡片位置标志位  默认位置为5 ：缺省在距离终点1/4(F3)
u8 sRXRFID[16];				  //存放RFID数据数组
u8 Infrared_Tab1[6];            //红外数据存放
extern uint8_t CP_data[6];           //车牌信息存放数组
extern uint8_t DZCP_data[6]; 
uint8_t RFID;
#define E 2.71828
uint8_t a[6] = {0x32,0x30,0x32,0x30,0x31,0x30};

/********************
函数功能：全自动前进
参    数：sp 速度 mp 前进距离
返 回 值：无
**********************/
void Full_Go(uint8_t sp,uint16_t mp)
{
	Roadway_mp_syn();	//码盘同步
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
前进
**/
void Go(uint8_t sp,uint16_t mp)
{
	Roadway_mp_syn();	//码盘同步
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
函数功能：全自动后退
参    数：sp 速度 mp 前进距离
返 回 值：无
**/
void Full_Back(uint8_t sp,uint16_t mp)
{
	Roadway_mp_syn();	//码盘同步
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
函数功能：全自动左转
参    数：sp 速度
返 回 值：无
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
函数功能：全自动右转
参    数：sp 速度
返 回 值：无
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
函数功能：全自动循迹
参    数：sp 速度
返 回 值：无
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
循迹
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
多地形循迹
**/
void Full_Terrain_Track(uint8_t sp,uint8_t Terrain_Type)
{
	Stop_Flag = 0; Go_Flag = 0; wheel_L_Flag = 0;wheel_R_Flag = 0;wheel_Nav_Flag = 0;
	Back_Flag = 0; Track_Flag = 0; Back_Track_Flag = 0; Terrain_Flag = Terrain_Type;
	Car_Spend = sp;
	while(Stop_Flag != 0x01);
}

/*
指定前进距多地形循迹
**/
void Full_Terrain_XG(uint8_t sp,uint16_t mp)
{
	Roadway_mp_syn();				//码盘同步
	Stop_Flag = 0; Go_Flag = 0; wheel_L_Flag = 0;wheel_R_Flag = 0;wheel_Nav_Flag = 0;
	Back_Flag = 0; Track_Flag = 0; Back_Track_Flag = 0; Terrain_Flag = 2;Mp_Track_Flag = 1;
	Car_Spend = sp;
	temp_MP = mp;
//	while(Stop_Flag != 0x03);
	
}

void Full_TXDX_XG(uint8_t sp,uint16_t mp)     //指定前进距多地形循迹 
{
	Roadway_mp_syn();				//码盘同步
	Stop_Flag = 0; Go_Flag = 0; wheel_L_Flag = 0;wheel_R_Flag = 0;wheel_Nav_Flag = 0;
	Back_Flag = 0; Track_Flag = 0; Back_Track_Flag = 0; Terrain_Flag = 3;Mp_Track_Flag = 1;
	Car_Spend = sp;
	temp_MP = mp;
//	while(Stop_Flag != 0x03);
	
}
/*
指定前进距离循迹
**/
void Full_Mp_Track(uint8_t sp,uint16_t mp)
{
	Roadway_mp_syn();				//码盘同步
	Stop_Flag = 0; Go_Flag = 0; wheel_L_Flag = 0;wheel_R_Flag = 0;wheel_Nav_Flag = 0;
	Back_Flag = 0; Track_Flag = 0; Back_Track_Flag = 0; Terrain_Flag = 0;Mp_Track_Flag = 1;
	Car_Spend = sp;
	temp_MP = mp;
	while(Stop_Flag != 0x03);
	
}

void Full_Stop(void)
{
	Send_UpMotor(0 ,0);
	Roadway_Flag_clean();	//清除标志位状态	
}

/*
函数功能：全自动任意角度左右转
参    数：sp 速度 
		  LorR = 1 左转 LorR = 2 
		  右转 angle 旋转角度
返 回 值：无
**/
void Full_LefRigX(uint8_t sp,uint8_t LorR,uint16_t angle)
{
	if(LorR == L)
	{
		Roadway_mp_syn();	//码盘同步
		Stop_Flag = 0; Go_Flag = 0; wheel_L_Flag = 0;wheel_R_Flag = 0;wheel_Nav_Flag = 1;
		Back_Flag = 0; Track_Flag = 0;
		temp_Nav = angle;
		Car_Spend = sp;			
		Control(-Car_Spend ,Car_Spend);	
	} 
	else if(LorR == R)
	{
		Roadway_mp_syn();	//码盘同步
		Stop_Flag = 0; Go_Flag = 0; wheel_L_Flag = 0;wheel_R_Flag = 0;wheel_Nav_Flag = 1;
		Back_Flag = 0; Track_Flag = 0;
		temp_Nav = angle;
		Car_Spend = sp;			
		Control(Car_Spend,-Car_Spend);
	}
	while(Stop_Flag != 0x02);
}

 
//寻迹函数在判断路线时退出全自动

void Full_XJ_Track()   //寻迹函数
{
	 Full_Track(Get_Go_sp);           
	 delay_ms(100);                //自动前进循迹
	 Full_Run_Flag = 0; 
		
}
void Full_TXXJ_Track()
{	
	Full_TXTrack(Get_Tra_sp); 	
	delay_ms(100);                //自动前进循迹
	Full_Run_Flag = 0; 
}

uint8_t TXRFID[16] = "F6F4D4B4B6B7";
uint8_t RXRFID[16];

uint8_t Section_Number = 1;		//扇区编号			红色个数代表扇区编号 默认1

uint8_t Block_Number = 2;		//数据块编号		菱形个数代表数据块编号 默认1


//根据扇区得到对应秘钥地址
uint8_t Get_RFID_SectionKey_Ades(void)
{
	return (4*Section_Number+3);   //0x3B;
}

//根据扇区和块计算块地址
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
 RFID数据处理
 *RxRFIDdata	待处理数据数组指针
 例  <-&Y&,/;[D->Rr]>
	Y = 0~99;
	Rr = C5,E5,D4,D6

	D4(1)
C5(2)   E5(3)
	D6(4)
**/


uint8_t Ascii_NumberTab[] = {'0','1','2','3','4','5','6','7','8','9'};
uint8_t Ascii_LOAATab[] = {'&','[','-','>',']','C','D','E'};

uint16_t RFID_Y = 0;
uint8_t  AGV_StartXY = 3;					//从车起始坐标

uint8_t Test[2];

uint16_t RFID_n = 0;
uint16_t RFID_x = 0;
uint8_t TestN[2];
uint8_t TestX[16];

uint8_t wifi_buf[12]={0x55,0xAA,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x20,0x40};
uint8_t fangxiang[1];
uint8_t P1[6],P2[4];

int p16(int n)//计算16进制的n次方
{
	int sum = 1, i;
	for (i = 0; i < n; i++)
		sum = sum * 16;
	return sum;
}
/****************************************************************
函数功能：把字符变成16进制，例如a1变成0xa1；
参	  数：*arr,要传入的字符,如果输入ABC123，则输出0xabc123
返回  值：16进制
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


void Full_Write_Card()   //写卡函数
{
	char status = 0;
	u8 CT[2];									//卡类型
	u8 SN[4]; 									//卡号
	u8 KEY[6]={0xff,0xff,0xff,0xff,0xff,0xff};  //密钥
	u8 s = 0x17;       							// #define   Status2Reg   0x08.   
	//这里s是定义的程序默认读卡的块地址0x01,结合文件"2017全自动程序\Readme.txt"
	u8 RXRFIDH[8];
	
	u8 i = 0, j = 0;

		status = PcdRequest(PICC_REQALL,CT);		//寻卡
		if(status == MI_OK)							//寻卡成功
		{
			status=MI_ERR;
			CT[0] = CT[0] + 0x30;
			CT[1] = CT[1] + 0x30;
			Send_Debug_Info(CT,2);					//卡类型
			Send_Debug_Info("\n",2);
			Send_Debug_Info("Findcard",8);
			Send_Debug_Info(" OK\n",5);
		
			status = PcdAnticoll(SN);				//防冲撞
			if(status == MI_OK)
			{
				status=MI_ERR;
//				Send_Debug_Info("a OK\n",6);
				delay_ms(100);
//				Send_Debug_Info("-collision",10);
//				Send_Debug_Info(" OK\n",5);
				
				status =PcdSelect(SN);				//选定此卡
				if(status == MI_OK)					//选定成功
				{
					status=MI_ERR;
//					Send_Debug_Info("P OK\n",6);
					delay_ms(100);
//					Send_Debug_Info(" card",5);	//提示："Pick a"有5个字符
//				  Send_Debug_Info(" OK\n",5);
//					//发送调试信息"Pick a card OK"
					status =PcdAuthState(0x60,s,KEY,SN);		//验证密钥:0x60 = 验证A密钥;0x61 = 验证B密钥。块地址0x03\KEY为密码\ SN卡序列号(或称卡号)
					if(status == MI_OK)		//RFID卡密码、卡号验证成功
					{
						
						status = MI_ERR;
//						
//						Send_Debug_Info("t OK\n",6);
//						Send_Debug_Info(" and",4);
//						Send_Debug_Info(" verify",7);
//						Send_Debug_Info(" OK\n",5);
						delay_ms(100);
						//发送调试信息"test and verify OK"
						
						status = PcdWrite(s,TXRFID);			//写卡
						
						if(status == MI_OK)
						{
							status = MI_ERR;
							Send_Debug_Info("write OK",10);
							Send_Debug_Info("\n",2);	
						}	
						delay_ms(500);
						
						status=PcdRead(s,sRXRFID);				//读卡：其中，s是该函数一开始定义的读卡默认块地址;RXREID为读出的数据，16字节
						if(status == MI_OK)
						{
							status = MI_ERR;
//							Send_Debug_Info("r OK\n",6);		//发送调试信息"read OK",即读卡成功
							delay_ms(100);
							for(i=0;i<8;i++)
							{
								RXRFIDH[i] = sRXRFID[i+8];		//实现数据搬移。不过，这里说明,读卡的最终数据存储在sRXRFID数组中
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
//读卡函数
//*******/
//void Full_Read_Card()
//{
//	 
//	char status = 0;
//	u8 CT[2];									//卡类型
//	u8 SN[4]; 									//卡号
//	u8 KEY[6]={0xff,0xff,0xff,0xff,0xff,0xff};  //密钥
//	u8 s ;

//	//这里s是定义的程序默认读卡的块地址0x01,结合文件"2017全自动程序\Readme.txt"
//	u8 RXRFIDH[8];
//	
//	u8 i = 0, j = 0;

////		status = PcdRequest(PICC_REQALL,CT);		//寻卡
////		if(status == MI_OK)							//寻卡成功
////		{
//			status=MI_ERR;
////			CT[0] = CT[0] + 0x30;
////			CT[1] = CT[1] + 0x30;
////			Send_Debug_Info(CT,2);					//卡类型
////			Send_Debug_Info("\n",2);
////			Send_Debug_Info("Findcard",8);
////			Send_Debug_Info(" OK\n",5);
//		
//			status = PcdAnticoll(SN);				//防冲撞
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
//				status =PcdSelect(SN);				//选定此卡
//				if(status == MI_OK)					//选定成功
//				{
//					status=MI_ERR;
//					Send_Debug_Info("P OK\n",6);
//					delay_ms(100);
////					Send_Debug_Info(" card",5);	//提示："Pick a"有5个字符
////				  Send_Debug_Info(" OK\n",5);
//					//发送调试信息"Pick a card OK"
//					status =PcdAuthState(0x60,s,KEY,SN);		//验证密钥:0x60 = 验证A密钥;0x61 = 验证B密钥。块地址0x03\KEY为密码\ SN卡序列号(或称卡号)
//					if(status == MI_OK)		//RFID卡密码、卡号验证成功
//					{
//						
//						status = MI_ERR;
//						
//						Send_Debug_Info("t OK\n",6);
////						Send_Debug_Info(" and",4);
////						Send_Debug_Info(" verify",7);
////						Send_Debug_Info(" OK\n",5);
//						delay_ms(100);
//						//发送调试信息"test and verify OK"
//						
////						status = PcdWrite(s,TXRFID);			//写卡
//						
////						if(status == MI_OK)
////						{
////							status = MI_ERR;
////							Send_Debug_Info("write OK",10);
////							Send_Debug_Info("\n",2);	
////						}	
////						delay_ms(500);
//						
//						status=PcdRead(s,sRXRFID);				//读卡：其中，s是该函数一开始定义的读卡默认块地址;RXREID为读出的数据，16字节
//						if(status == MI_OK)
//						{
//							status = MI_ERR;
////							BEEP = 0;
////							SYN_TTS("读卡成功");
//							Send_Debug_Info("r OK\n",6);		//发送调试信息"read OK",即读卡成功
//							delay_ms(100);
//							for(i=0;i<8;i++)
//							{
//								RXRFIDH[i] = sRXRFID[i+8];		//实现数据搬移。不过，这里说明,读卡的最终数据存储在sRXRFID数组中
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
读卡函数
*******/
void Full_Read_Card()
{
	char status = 0;
	u8 CT[2];									//卡类型
	u8 SN[4]; 									//卡号
	u8 KEY[6]={0xff,0xff,0xff,0xff,0xff,0xff};  //密钥
	u8 s = 0x18;       							// #define   Status2Reg   0x08.   
	//这里s是定义的程序默认读卡的块地址0x01,结合文件"2017全自动程序\Readme.txt"
	u8 RXRFIDH[8];
	u8 i = 0, j = 0;
				
		if(RFID_Card_Position[0] == 0xE2)
	{
//		s=0xe2%3+12;
		Send_Debug_Info("s OK\n",6);
		Send_Debug_Info("\n",2); 
//	  Send_Debug_Info(&s,1);
	}

//		status = PcdRequest(PICC_REQALL,CT);		//寻卡
//		if(status == MI_OK)							//寻卡成功
//		{
			status=MI_ERR;
//			CT[0] = CT[0] + 0x30;
//			CT[1] = CT[1] + 0x30;
//			Send_Debug_Info(CT,2);					//卡类型
//			Send_Debug_Info("\n",2);
//			Send_Debug_Info("Findcard",8);
//			Send_Debug_Info(" OK\n",5);
		
			status = PcdAnticoll(SN);				//防冲撞
			if(status == MI_OK)
			{
				status=MI_ERR;
				Send_Debug_Info("a OK\n",6);
				delay_ms(100);
//				Send_Debug_Info("-collision",10);
//				Send_Debug_Info(" OK\n",5);
				
				status =PcdSelect(SN);				//选定此卡
				if(status == MI_OK)					//选定成功
				{
					status=MI_ERR;
					Send_Debug_Info("P OK\n",6);
					delay_ms(100);
//					Send_Debug_Info(" card",5);	//提示："Pick a"有5个字符
//				  Send_Debug_Info(" OK\n",5);
					//发送调试信息"Pick a card OK"
					status =PcdAuthState(0x60,s,KEY,SN);		//验证密钥:0x60 = 验证A密钥;0x61 = 验证B密钥。块地址0x03\KEY为密码\ SN卡序列号(或称卡号)
					if(status == MI_OK)		//RFID卡密码、卡号验证成功
					{
						
						status = MI_ERR;
						
						Send_Debug_Info("t OK\n",6);
//						Send_Debug_Info(" and",4);
//						Send_Debug_Info(" verify",7);
//						Send_Debug_Info(" OK\n",5);
						delay_ms(100);
						//发送调试信息"test and verify OK"
						
//						status = PcdWrite(s,TXRFID);			//写卡
						
//						if(status == MI_OK)
//						{
//							status = MI_ERR;
//							Send_Debug_Info("write OK",10);
//							Send_Debug_Info("\n",2);	
//						}	
//						delay_ms(500);
						
						status=PcdRead(s,sRXRFID);				//读卡：其中，s是该函数一开始定义的读卡默认块地址;RXREID为读出的数据，16字节
						if(status == MI_OK)
						{
							status = MI_ERR;
							
//							BEEP = 0;
//							SYN_TTS("读卡成功");
							Send_Debug_Info("r OK\n",6);		//发送调试信息"read OK",即读卡成功
							delay_ms(100);
							RFID_DKWC_Flag = 1;
							delay_ms(100);
							for(i=0;i<8;i++)
							{
								RXRFIDH[i] = sRXRFID[i+8];		//实现数据搬移。不过，这里说明,读卡的最终数据存储在sRXRFID数组中
							}
							Send_Debug_Info(sRXRFID,8);
							Send_Debug_Info(RXRFIDH,8);
							Send_Debug_Info("\n",2);	
							delay_ms(100);
						}
						for(i=0;i<16;i++)
							{
								RXRFID[i] = sRXRFID[i];		//实现数据搬移。不过，这里说明,读卡的最终数据存储在sRXRFID数组中
							}
//						delay_ms(200);
//						BEEP = 1;
					}
				}
			}
//	}
				
}


/****************************************************************
函数功能：超声波测距 LED显示距离
参	  数：无
返回  值：无
****************************************************************/
void LED_Distance(void)
{	
	uint8_t LED_DisTab1[8] = {0x55,0x0B,0x50,0x00,0x00,0x00,0x00,0xBB};
	delay_ms(700);
	uint8_t time;
	for(time=0;time<5;time++)
	{
		Ultrasonic_Ranging();		//超声波数据
		delay_ms(100);
	}	
	LED_DisTab1[4]=dis/100;
	LED_DisTab1[5]=dis%100;
	//LED_DisTab1[4] = dis/100%10;                             //十位
	//LED_DisTab1[5] = ((dis/10%10)<<4) + dis%10;					 //个位 可显示两位
	Full_Car_CKS(LED_DisTab1);	
	Send_ZigbeeData_To_Fifo(LED_DisTab1,8);
}

/****************************************************************
函数功能：超声波测距 立体显示距离
参	  数：无
返回  值：无
****************************************************************/
void LT_Distance(void)
{
	uint8_t i = 0;
	delay_ms(500);
	delay_ms(500);
	for(i=0; i<8; i++)
	{
		Ultrasonic_Ranging();						//超声波数据
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
函数功能：立体显示 手动控制输入内容
参	  数：无
返回  值：无
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
函数功能：立体显示车牌
参	  数：*data 车牌数据指针
返回  值：无     
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
函数功能：打印数据显示
参	  数：vluae 传入数据 打印在debug屏幕上
返回  值：无
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
函数功能：计算校验和
参	  数：*srcdata 指针 指向传入数据数组
返回  值：无
*****************************************************************/
void Full_Car_CKS(uint8_t *srcdata)
{
	srcdata[6] = (srcdata[2]+srcdata[3]+srcdata[4]+srcdata[5])%256;
}

/****************************************************************
函数功能：计算校验和
参	  数：*srcdata 指针 指向传入数据数组
返回  值：无
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

//******************************从车控制*******************************	

uint8_t Full_ENABLE_AGV_Return_Flag = 0;			//允许从车返回标志位

/*
从车启动
参数	LightValue_X    当前档位
		N_AGV_Start		起始坐标
		RFID_AGVData_Y  RFID内相关数据
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
	AGV_Data[3] = N_AGV_Start;					//从车起始方向
	AGV_Data[4] = LightValue_X;					//当前档位
	AGV_Data[5] = N_AGVEnd;						//入库坐标
	Full_Car_CKS(AGV_Data);						//校验和
	Send_ZigbeeData_To_Fifo(AGV_Data,8);
	delay_ms(200);
	Send_ZigbeeData_To_Fifo(AGV_Data,8);
	delay_ms(200);
	Send_ZigbeeData_To_Fifo(AGV_Data,8);
}


/*
从车启动
**/
void AGV_Start2(void)
{
	uint8_t AGV_Data[8] = {0x55,0x02,0x00,0x00,0x00,0x00,0x00,0xBB};
	AGV_Data[2] = 0x1B;
	Full_Car_CKS(AGV_Data);					//校验和
	Send_ZigbeeData_To_Fifo(AGV_Data,8);
	delay_ms(200);
	Send_ZigbeeData_To_Fifo(AGV_Data,8);
	delay_ms(200);
	Send_ZigbeeData_To_Fifo(AGV_Data,8);
}


/****************************************************************
函数功能：主车向AGV发送运行状态
参	  数unm  运行状态 具体参考通讯协议
返回  值：无
*****************************************************************/
void Send_AGV_Interaction_Function(uint8_t nun_data)
{
	u8 i;
	uint8_t Interaction_data[] = {0x55,0x02,0x00,0x00,0x00,0x00,0x00,0xBB};
	Interaction_data[2] =  nun_data;
	Full_Car_CKS(Interaction_data);					//校验和
	for(i = 0;i < 2;i++)
	{
	Send_ZigbeeData_To_Fifo(Interaction_data,8);
   	delay_ms(200);
	}   	
}

/****************************************************************
函数功能：主车向AGV发送红外数据
参	  数：*Infrared_Data  AGV红外数据地址
返回  值：无
*****************************************************************/
void AGV_Control_Infrared(u8 *Infrared_Data)			//AGV_Control_Infrared(Caution_Text)；
{
	uint8_t Infrared_Temp[] = {0x55,0x02,0x00,0x00,0x00,0x00,0x00,0xbb};
	Infrared_Temp[2] = 0x10;
 	Infrared_Temp[3] = Infrared_Data[0];
	Infrared_Temp[4] = Infrared_Data[1];
	Infrared_Temp[5] = Infrared_Data[2];
	Full_Car_CKS(Infrared_Temp);					//计算校验和
	Send_ZigbeeData_To_Fifo(Infrared_Temp,8);
	Send_ZigbeeData_To_Fifo(Infrared_Temp,8);
    delay_ms(1000);
	Infrared_Temp[2] = 0x11;
	Infrared_Temp[3] = Infrared_Data[3];
	Infrared_Temp[4] = Infrared_Data[4];
	Infrared_Temp[5] = Infrared_Data[5];
	Full_Car_CKS(Infrared_Temp);					//计算校验和
	Send_ZigbeeData_To_Fifo(Infrared_Temp,8);
	Send_ZigbeeData_To_Fifo(Infrared_Temp,8);
	delay_ms(1000);
}
/****************************************************************
函数功能 向A72发送数据
参	  数Infrared_Data  需要发送的数据
返回  值：无
*****************************************************************/
void A72_Send(u8 *A72_Data)
{
	u8 wife_data[12]={0x55,0xaa,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x20,0x40};//给WIFI发数据一定要12字节一起发
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
	UartA72_TxClear();							 //清除发送区缓存                   //A72通信方式要1、UartA72_TxClear 2、UartA72_TxAddStr 3artA72_TxStart
	UartA72_TxAddStr(wife_data,12);  //把要发送的内容发送到缓存区
	UartA72_TxStart();               //开始发送
	delay_ms(500);
}
/****************************************************************
函数功能 向wifi模块发送数据
参	  数Infrared_Data  需要发送的数据
返回  值：无
*****************************************************************/
void wifi_Send(u8 *wifi_Data)
{
	u8 i;
	u8 wifi_temp[]={0x55,0xaa,0x10,0x00,0x00,0x00,0x00,0xbb};//给WIFI发数据一定要12字节一起发
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
函数功能:TFT显示车牌数据   A
参    数: Car_Text作为实际参数
返回值:无
*****************************************************************/
void Control_TFT_show_data(u8 *Data)
{
		u8 i;
	for(i=0;i<5;i++)
		{
		u8 TFT_Data[8] = {0x55,0x0B,0x00,0x00,0x00,0x00,0x00,0xBB};//参考通讯协议——平板电脑向LED显示标志物发送命令的数据结构
	/*******************
	**车牌数据前三位
	*******************/
		TFT_Data[2]=0x20;
		TFT_Data[3]=Data[0];
		TFT_Data[4]=Data[1];
		TFT_Data[5]=Data[2];
		Full_Car_CKS(TFT_Data);
	
			Send_ZigBee_Info(TFT_Data,8);
			delay_ms(100);	//这里的100可以调整小一点，看看有没有影响。如果没有影响，尽量小一点，或者直接删掉delay_ms(100)

	/******************
	**车牌数据后三位
	********************/
		TFT_Data[2]=0x21;
		TFT_Data[3]=Data[3];
		TFT_Data[4]=Data[4];
		TFT_Data[5]=Data[5];
		Full_Car_CKS(TFT_Data);

			Send_ZigBee_Info(TFT_Data,8);
			delay_ms(100);	//这里的100可以调整小一点，看看有没有影响。如果没有影响，尽量小一点，或者直接删掉delay_ms(100)
		}	

}
/****************************************************************
函数功能:TFT显示车牌数据  B
参    数: Car_Text作为实际参数
返回值:无
*****************************************************************/
void Control_TFTB_show_data(u8 *Data)
{
		u8 i;
			
	for(i=0;i<5;i++)
		{

    u8 TFTB_Data[8] = {0x55,0x08,0x00,0x00,0x00,0x00,0x00,0xBB};//参考通讯协议——平板电脑向LED显示标志物发送命令的数据结构
	/*******************
	**车牌数据前三位
	*******************/
		TFTB_Data[2]=0x20;
		TFTB_Data[3]=Data[0];
		TFTB_Data[4]=Data[1];
		TFTB_Data[5]=Data[2];
		Full_Car_CKS(TFTB_Data);
		
			Send_ZigBee_Info(TFTB_Data,8);
			Send_ZigBee_Info(TFTB_Data,8);
			delay_ms(100);	//这里的100可以调整小一点，看看有没有影响。如果没有影响，尽量小一点，或者直接删掉delay_ms(100)
		

	/******************
	**车牌数据后三位
	********************/
		TFTB_Data[2]=0x21;
		TFTB_Data[3]=Data[3];
		TFTB_Data[4]=Data[4];
		TFTB_Data[5]=Data[5];
		Full_Car_CKS(TFTB_Data);
	
			Send_ZigBee_Info(TFTB_Data,8);
			Send_ZigBee_Info(TFTB_Data,8);
			delay_ms(100);	//这里的100可以调整小一点，看看有没有影响。如果没有影响，尽量小一点，或者直接删掉delay_ms(100)
		
	}
}

/****************************************************************
函数功能:TFTHEX
参    数:Principal_Tab作为实参
返回值 :无
*****************************************************************/
void Control_TFT_HEX_distance(u8 *Data)
{
  u8 i;
	u8 TFT_Dis_Data[8] = {0x55,0x0B,0x40,0x00,0x00,0x00,0x00,0xBB};//参考通讯协议——平板电脑向LED显示标志物发送命令的数据结构

	TFT_Dis_Data[3] = Data[0];
    TFT_Dis_Data[4] = Data[1];
    TFT_Dis_Data[5] = Data[2];
    Full_Car_CKS(TFT_Dis_Data);
  
	
  for(i=0;i<3;i++)
	{              
		Send_ZigbeeData_To_Fifo(TFT_Dis_Data,8);
		delay_ms(100);	//这里的100可以调整小一点，看看有没有影响。如果没有影响，尽量小一点，或者直接删掉delay_ms(100)
	}	
	}

/****************************************************************
函数功能:TFT显示距离    A
参    数:Principal_Tab作为实参
返回值 :无
*****************************************************************/
void Control_TFT_show_distance(u8 *Data)
{
  u8 i;
	u8 TFT_Dis_Data[8] = {0x55,0x0B,0x50,0x00,0x00,0x00,0x00,0xBB};//参考通讯协议——平板电脑向LED显示标志物发送命令的数据结构


  TFT_Dis_Data[4] = (u8)(Data[0]&0x0F);
  TFT_Dis_Data[5] = (u8)(((Data[1]&0x0F) << 4)|(Data[2]&0x0F));
  Full_Car_CKS(TFT_Dis_Data);
  
	
  for(i=0;i<3;i++)
	{              
		Send_ZigbeeData_To_Fifo(TFT_Dis_Data,8);
		delay_ms(100);	//这里的100可以调整小一点，看看有没有影响。如果没有影响，尽量小一点，或者直接删掉delay_ms(100)
	}	
	}
/****************************************************************
函数功能:TFT显示距离    B
参    数:Principal_Tab作为实参
返回值 :无
*****************************************************************/
void Control_TFTB_show_distance(u8 *Data)
{
  u8 i;
	u8 TFTB_Dis_Data[8] = {0x55,0x08,0x50,0x00,0x00,0x00,0x00,0xBB};//参考通讯协议——平板电脑向LED显示标志物发送命令的数据结构


  TFTB_Dis_Data[4] = (u8)(Data[0]&0x0F);
  TFTB_Dis_Data[5] = (u8)(((Data[1]&0x0F) << 4)|(Data[2]&0x0F));
  Full_Car_CKS(TFTB_Dis_Data);
  
	
  for(i=0;i<3;i++)
	{              
		Send_ZigbeeData_To_Fifo(TFTB_Dis_Data,8);
		delay_ms(100);	//这里的100可以调整小一点，看看有没有影响。如果没有影响，尽量小一点，或者直接删掉delay_ms(100)
	}	
	}

/****************************************************************
函数功能:TFT定时控制    A
参    数:Mode_num=1 定时器开 Mode_num=2 定时器关
返回值:无
*****************************************************************/
void Control_TFT_show_Time(u8 Mode_num)
{
	if(Mode_num == 1)
		{
		Send_ZigBee_Info(TFT_JSK,8);		//控制定时器开
	  } 
		else if(Mode_num == 2)
		{
		Send_ZigBee_Info(TFT_JSG,8);		//控制定时器关
	  }
		else if(Mode_num==3)
		{
		Send_ZigBee_Info(TFT_JSClear,8);		//控制定时器清零
		}
}
/****************************************************************
函数功能:TFT定时控制    B
参    数:Mode_num=1 定时器开 Mode_num=2 定时器关
返回值:无
*****************************************************************/
void Control_TFTB_show_Time(u8 Mode_num)
{
	if(Mode_num == 1)
		{
		Send_ZigBee_Info(TFTB_JSK,8);		//控制定时器开
	  } 
		else if(Mode_num == 2)
		{
		Send_ZigBee_Info(TFTB_JSG,8);		//控制定时器关
	  }
		else if(Mode_num==3)
		{
		Send_ZigBee_Info(TFTB_JSClear,8);		//控制定时器清零
		}
}
/*******************************************
函数功能：TFT翻页控制 A
参	  数：num = 1 控制向上进行翻页 num = 2 控制向下进行翻页
返回  值：无
*******************************************/
void TFT_turn_over(u8 num)   
{
	 if(num == 1)//控制向上进行翻页
	 {
		Send_ZigBee_Info(TFT_UP,8);
	 } 
	 else if(num == 2) //控制向下进行翻页
   {
		Send_ZigBee_Info(TFT_down,8);
	 }
}
/*******************************************
函数功能：TFT翻页控制 B
参	  数：num = 1 控制向上进行翻页 num = 2 控制向下进行翻页
返回  值：无
*******************************************/
void TFTB_turn_over(u8 nm)
{
	 if(nm == 1)//控制向上进行翻页
	 {
		Send_ZigBee_Info(TFTB_UP,8);
	 } 
	 else if(nm == 2) //控制向下进行翻页
   {
		Send_ZigBee_Info(TFTB_down,8);
	 }
}
/****************************************************************
函数功能：开启道闸
mode 1 固定指令开启
mode 2 指定车牌开启(显示车牌数据)
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
函数功能：道闸状态回传
****************************************************************/
void daozha_zhuantai(void)
{
	u8 DZ_ZT_Data[8] = {0x55,0x03,0x20,0x01,0x00,0x00,(0x20+0x01+0x00+0x00)%256,0xBB};
	Send_ZigbeeData_To_Fifo(DZ_ZT_Data,8);
	delay_ms(100);
}

/****************************************************************
函数功能：开启交通灯识别A
参	  数: 无
返回  值：无
****************************************************************/
void Full_Traffic_Light_Recognize_patterns()
{
	u8 i;
	u8 JTD_Data[8] = {0x55,0x0E,0x01,0x00,0x00,0x00,0x00,0xBB};
  Full_Car_CKS(JTD_Data);
	for(i=0;i<3;i++)
	{              
		Send_ZigbeeData_To_Fifo(JTD_Data,8);
		delay_ms(100);	//这里的100可以调整小一点，看看有没有影响。如果没有影响，尽量小一点，或者直接删掉delay_ms(100)
	}	
}
/****************************************************************
函数功能:交通灯识别结果比对A
参	  数: mode_data[3]   0x01(红色) 0x02(绿色) 0x03(黄色)   识别结果请求确认
返回  值：无
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
		delay_ms(50);	//这里的100可以调整小一点，看看有没有影响。如果没有影响，尽量小一点，或者直接删掉delay_ms(100)
	}	


}
/****************************************************************
函数功能：开启交通灯识别B
参	  数: 无
返回  值：无
****************************************************************/
void Full_Braffic_Light_Recognize_patterns()
{
	u8 i;
	u8 TD_Data[8] = {0x55,0x0F,0x01,0x00,0x00,0x00,0x00,0xBB};
  Full_Car_CKS(TD_Data);
	for(i=0;i<2;i++)
	{              
		Send_ZigbeeData_To_Fifo(TD_Data,8);
		delay_ms(100);	//这里的100可以调整小一点，看看有没有影响。如果没有影响，尽量小一点，或者直接删掉delay_ms(100)
	}	
}
/****************************************************************
函数功能:交通灯识别结果比对B
参	  数: mode_data[3]   0x01(红色) 0x02(绿色) 0x03(黄色)   识别结果请求确认
返回  值：无
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
		delay_ms(50);	//这里的100可以调整小一点，看看有没有影响。如果没有影响，尽量小一点，或者直接删掉delay_ms(100)
	}	


}
/****************************************************************
函数功能:道闸显示车牌数据
参    数: *Data指针，指向传入数据数组
返回值:无
*****************************************************************/
void Control_DZ_show_data(u8 *Data)
{
		u8 i;
		u8 DZ_Data[8] = {0x55,0x03,0x00,0x00,0x00,0x00,0x00,0xBB};//参考通讯协议——平板电脑向LED显示标志物发送命令的数据结构
	/*******************
	**车牌数据前三位
	*******************/
		DZ_Data[2]=0x10;
		DZ_Data[3]=Data[0];
		DZ_Data[4]=Data[1];
		DZ_Data[5]=Data[2];
		Full_Car_CKS(DZ_Data);
		for(i=0;i<3;i++)
		{
			Send_ZigBee_Info(DZ_Data,8);
//			delay_ms(100);	//这里的100可以调整小一点，看看有没有影响。如果没有影响，尽量小一点，或者直接删掉delay_ms(100)
		}

	/******************
	**车牌数据后三位
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
//			delay_ms(100);	//这里的100可以调整小一点，看看有没有影响。如果没有影响，尽量小一点，或者直接删掉delay_ms(100)
		}	

}

/****************************************************************
函数功能：全自动语音播报
参	  数：*s_data 数据指针 length  数据长度
返回  值：无
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
函数功能：LED数码管控制
参	  数：Mode_num=1 打开计时  Mode_num=2 关闭计时
返回  值：无
*****************************************************************/
void Control_LED_Nixie_light(u8 Mode_num)
{
	if(Mode_num == 1)
	{
		Send_ZigBee_Info(SMG_JSK,8);		//计时开启
	} 
	else if(Mode_num == 2)
	{
		Send_ZigBee_Info(SMG_JSG,8);		//计时关闭
	}
	else if(Mode_num == 3)
	{
		Send_ZigBee_Info(SMG_JSQ,8);		//计时清零
	}
		
}
/****************************************************************
函数功能：LED数码管显示数据
参	  数：*data指针数组  x =1数据显示在第一行  x=2数据显示在第二行
返回  值：无
*****************************************************************/
void Control_LED_show_data(u8 X,u8 *Data)
{
  u8 LED_Data[8] = {0x55,0x04,0x00,0x00,0x00,0x00,0x00,0xBB};//参考"通讯协议——平板电脑向LED显示标志物发送命令的数据结构"
  if(X == 1)					//第一排
  {
	  LED_Data[2] = 0x01;  
  } 
  else if( X == 2 ) //第二排 
	{	 		
	  LED_Data[2] = 0x02;  
  }
  LED_Data[3] = (u8)((Data[0]&0x0F) << 4)|(Data[1]&0x0F);
  LED_Data[4] = (u8)((Data[2]&0x0F) << 4)|(Data[3]&0x0F);
  LED_Data[5] = (u8)((Data[4]&0x0F) << 4)|(Data[5]&0x0F);
	Full_Car_CKS(LED_Data);					//计算校验和
  Send_ZigBee_Info(LED_Data,8);
}

/****************************************************************
函数功能：LED显示
参	  数：无
返回  值：无
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
	Full_Car_CKS(LED_DisTab);					//计算校验和
	Send_ZigbeeData_To_Fifo(LED_DisTab,8);
	delay_ms(200);
	Send_ZigbeeData_To_Fifo(LED_DisTab,8);
}

/****************************************************************
函数功能：副车立体车库倒车入库控制函数
参	  数：无Full_Fucheruku_Return_Flag ==1:A库   2：B库
返回  值：无
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
函数功能：立体车库控制A
参	  数：无
返回  值：无
****************************************************************/
void SetStereo_garage(uint8_t mode)
{
	uint8_t Stereo_Tab[] = {0x55,0x0D,0x01,0x00,0x00,0x00,0x00,0xBB};
	switch(mode)
	{
		case 1:				//到达第一层
			Stereo_Tab[3] = 0x01;
			break;
		case 2:				//到达第二层
			Stereo_Tab[3] = 0x02;
			break;
		case 3:				//到达第三层
			Stereo_Tab[3] = 0x03;
			break;
		case 4:				//到达第四层
			Stereo_Tab[3] = 0x04;
			break;
	}
	Full_Car_CKS(Stereo_Tab);					//计算校验和
	delay_ms(200);
	Send_ZigbeeData_To_Fifo(Stereo_Tab,8);
	delay_ms(200);
	Send_ZigbeeData_To_Fifo(Stereo_Tab,8);
	delay_ms(200);
	Send_ZigbeeData_To_Fifo(Stereo_Tab,8);
}
/****************************************************************
函数功能：发送立体车库返回状态指令A
参	  数：Query_mode = 1 请求返回当前层数 Query_mode = 2 请求返回当前红外状态
返回  值：无
****************************************************************/
void Send_Get_Stereo_Return(u8 Query_mode)
{
	uint8_t Stereo_Tab[] = {0x55,0x0D,0x02,0x00,0x00,0x00,0x00,0xBB};
	if(Query_mode == 1)     //请求返回当前层数
	{
		Stereo_Tab[3] = 0x01;
	}
	if(Query_mode == 2)     //请求返回当前红外状态
	{
		Stereo_Tab[3] = 0x02;
	}
	  Full_Car_CKS(Stereo_Tab);					//计算校验和
	  Send_ZigbeeData_To_Fifo(Stereo_Tab,8);
}

/****************************************************************
函数功能：立体车库倒车入库控制函数A
参	  数：无
返回  值：无
****************************************************************/
void Full_astern_garage_Control(void)
{
		u8 i;
		if(Full_astern_garage_Flag ==0)
		{
			SetStereo_garage(1);
			delay_ms(100);
//			while(Stop_Flag != 0x09)	//等待到达第一层
//			{
//				Set_tba_Beep(SET);
//			}	
//			 Set_tba_Beep(RESET);
			for(i=10;i>0;i--)
			{
				Send_Get_Stereo_Return(1);
				delay_ms(100);
				if((Zigb_Rx_Buf[3] == 0x01)&&(Zigb_Rx_Buf[4]==0x01))//车库当前位于第一层
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
函数功能：立体车库倒车入库控制函数B
参	  数：无
返回  值：无
****************************************************************/
void Full_Bstern_garage_Control(void)
{
		u8 i;
		if(Full_Bstern_garage_Flag ==0)
		{
			SetStereo_Barage(1);
			delay_ms(100);
//			while(Stop_Flag != 0x09)	//等待到达第一层
//			{
//				Set_tba_Beep(SET);
//			}	
//			 Set_tba_Beep(RESET);
			for(i=10;i>0;i--)
			{
				Send_Get_Btereo_Return(1);
				delay_ms(100);
				if((Zigb_Rx_Buf[3] == 0x01)&&(Zigb_Rx_Buf[4]==0x01))//车库当前位于第一层
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
函数功能：发送立体车库返回状态指令B
参	  数：Query_mode = 1 请求返回当前层数 Query_mode = 2 请求返回当前红外状态
返回  值：无
****************************************************************/
void Send_Get_Btereo_Return(u8 Query_mode)
{
	uint8_t Stereo_Tab[] = {0x55,0x05,0x02,0x00,0x00,0x00,0x00,0xBB};
	if(Query_mode == 1)     //请求返回当前层数
	{
		Stereo_Tab[3] = 0x01;
	}
	if(Query_mode == 2)     //请求返回当前红外状态
	{
		Stereo_Tab[3] = 0x02;
	}
	  Full_Car_CKS(Stereo_Tab);					//计算校验和
	  Send_ZigbeeData_To_Fifo(Stereo_Tab,8);
}
/****************************************************************
函数功能：立体车库控制B
参	  数：无
返回  值：无
****************************************************************/
void SetStereo_Barage(uint8_t mode)
{
	uint8_t Stereo_Tab[] = {0x55,0x05,0x01,0x00,0x00,0x00,0x00,0xBB};
	switch(mode)
	{
		case 1:				//到达第一层
			Stereo_Tab[3] = 0x01;
			break;
		case 2:				//到达第二层
			Stereo_Tab[3] = 0x02;
			break;
		case 3:				//到达第三层
			Stereo_Tab[3] = 0x03;
			break;
		case 4:				//到达第四层
			Stereo_Tab[3] = 0x04;
			break;
	}
	Full_Car_CKS(Stereo_Tab);					//计算校验和
	delay_ms(200);
	Send_ZigbeeData_To_Fifo(Stereo_Tab,8);
	delay_ms(200);
	Send_ZigbeeData_To_Fifo(Stereo_Tab,8);
	delay_ms(200);
	Send_ZigbeeData_To_Fifo(Stereo_Tab,8);
}

/****************************************************************
函数功能：开启无线充电
参	  数：无
返回  值：无
****************************************************************/
void Wireless_charging(void)
{
	uint8_t Stereo_Tab[] = {0x55,0x0A,0x01,0x01,0x00,0x00,0x00,0xBB};
	Full_Car_CKS(Stereo_Tab);					//计算校验和
	delay_ms(200);
	Send_ZigbeeData_To_Fifo(Stereo_Tab,8);
	delay_ms(200);
	Send_ZigbeeData_To_Fifo(Stereo_Tab,8);
	delay_ms(200);
	Send_ZigbeeData_To_Fifo(Stereo_Tab,8);
}
/****************************************************************
函数功能：烽火台红外开起
参	  数：*红外开起数据
返回  值：无
*****************************************************************/
void fht_frared(u8 *Data)
{
	u8 i;
  uint8_t  HW_Tab[8] = {0x55,0x07,0x00,0x00,0x00,0x00,0x00,0xBB};
	/*******************
	红外数据前三位
	*******************/
  HW_Tab[2]=0x10;
  HW_Tab[3]=Data[0];
  HW_Tab[4]=Data[1];
  HW_Tab[5]=Data[2];
  Full_Car_CKS(HW_Tab);					//计算校验和
	for(i=0;i<3;i++)
	{
	Infrared_Send(HW_Tab,8);
delay_ms(100);		
	}

	/*******************
	红外数据后三位
	*******************/
	delay_ms(200);
	HW_Tab[2]=0x11;
  HW_Tab[3]=Data[3];
  HW_Tab[4]=Data[4];
  HW_Tab[5]=Data[5];
  Full_Car_CKS(HW_Tab);					//计算校验和	
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
函数功能：修改烽火台红外开起数据
参	  数：*Beacon_tower_Data 修改红外开起数据
返回  值：无
*****************************************************************/
void Modify_Beacon_tower_Infrared(u8 *Beacon_tower_Data)			//AGV_Control_Infrared(Caution_Text)；
{
	uint8_t Infrared_Temp[] = {0x55,0x07,0x00,0x00,0x00,0x00,0x00,0xbb};
	Infrared_Temp[2] = 0x10;
 	Infrared_Temp[3] = Beacon_tower_Data[0];
	Infrared_Temp[4] = Beacon_tower_Data[1];
	Infrared_Temp[5] = Beacon_tower_Data[2];
//	Full_Car_CKS(Infrared_Temp);					//计算校验和
	Infrared_Temp[6] = (Infrared_Temp[2]+Infrared_Temp[3]+Infrared_Temp[4]+Infrared_Temp[5])%256;
	Send_ZigbeeData_To_Fifo(Infrared_Temp,8);
  delay_ms(300);
	Infrared_Temp[2] = 0x11;
	Infrared_Temp[3] = Beacon_tower_Data[3];
	Infrared_Temp[4] = Beacon_tower_Data[4];
	Infrared_Temp[5] = Beacon_tower_Data[5];
	Infrared_Temp[6] = (Infrared_Temp[2]+Infrared_Temp[3]+Infrared_Temp[4]+Infrared_Temp[5])%256;
//	Full_Car_CKS(Infrared_Temp);					//计算校验和
	Send_ZigbeeData_To_Fifo(Infrared_Temp,8);
	delay_ms(300);
}

//******************************从车控制Code End***********************

int mark = 0; 
int Full_Run_Flag = 0;            //全自动运行状态标志位
int Full_ETC_Open_Flag = 0;				//ETC允许返回标志位
int Full_FC_Open_Flag = 0;			//等待副车入库标志位
int Full_DZ_Open_Flag = 0;				//DZ允许返回标志位
int Full_Stereo_Return_Flag = 0;		//立体车库允许返回标志位A 
int Full_astern_garage_Flag = 0;		//立体车库允许倒车标志位A
int Full_Btereo_Return_Flag = 0;		//立体车库允许返回标志位B
int Full_Bstern_garage_Flag = 0;		//立体车库允许倒车标志位B
int Full_AGV_Return_Flag1 = 0;			//全自动从车返回标志位1
int Full_AGV_Return_Flag2 = 0;			//全自动从车返回标志位2
int Full_Fucheruku_Return_Flag = 0;		//副车立体车库标志位A 
int RFID_DKWC_Flag = 0;


uint8_t RFID_S50_Address = 0x01;
uint8_t AGV_Return = 0x1B;			//默认接收返回到达道闸标志位位置状态 

uint8_t WIFI_Data[8] = {0x55,0xAA,0x00,0x00,0x00,0x00,0x00,0xBB};

//                        三角形 圆 矩形 五角星 菱形
//						             1    2    3    4     5   6
uint8_t Shape_Number[] = {0x31,0x32,0x33,0x34,0x35,0x36};
// 						           红色、绿色、蓝色、黄色、紫色、青色、黑色
uint8_t Color_Number[] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07};
//车牌			
uint8_t Plate_Number[] = {0xA1,0x31,0x42,0x32,0x43,0x33};   //副车识别车牌结果

uint8_t Qr_Code[] = {0x55,0x01,0x02,0x03,0x04,0xFF};   //识别二维码结果--报警器控制码

uint8_t Speech_recognition_data[] = {0xAF,0x06,0x00,0x02,0x00,0x00,0x01,0xBF}; //语音识别正确标志数据

uint8_t Qr_Code_1[] = {0x47,0x37,0x02,0x03,0x04,0xFF};   //识别二维码结果--报警器控制码

uint8_t LTXS_YY[] = {0x11,0x30,0x00,0x00,0x00};      //立体显示 语音播报值
//

void Send_AVG_data(uint8_t *data)
{
	uint8_t  AVG_data[] = {0x55,0x02,0x00,0x00,0x00,0x00,0x00,0xbb};
	AVG_data[2] = 0xA1;
	AVG_data[3] = data[0];
	Full_Car_CKS(AVG_data);					//计算校验和
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



void Full_Find_card(void)     //寻卡
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
					Track_Value[0] = Get_Host_UpTrack(TRACK_H8);	//路况检测				
					if((Track_Value[0]&0x3C)==0x3C)
					{	
						  Send_Debug_Info("（2）\n",4);			
					  	Full_Stop();
					    delay_ms(500);
						  Track_Value[1] = Get_Host_UpTrack(TRACK_H8);
						  Track_Value[2] = Get_Host_UpTrack(TRACK_Q7);//如果卡在线上Track_Value[2]==0x7F，如果在十字路口Track_Value[2]!=0xF7 		
						  cont[0]=((BitCount(Track_Value[1]))+(BitCount(Track_Value[2])));
						  delay_ms(500);
						  Full_Go(70,70);
						  Track_Value[0] = Get_Host_UpTrack(TRACK_Q7);
						  delay_ms(500);
						if(Track_Value[0]==0x18||Track_Value[0]==0x08||Track_Value[0]==0x10||Track_Value[0]==0x18||Track_Value[0]==0x0c||Track_Value[0]==0x30)
						{
							Send_Debug_Info("（2-1）\n",6);			
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
								RFID_Card_Position[0] =RFID;		//RFID 位置坐标为 4——E2：这里应该是C2吧？
								delay_ms(100);
								Full_Read_Card();	//读卡操作
								delay_ms(500);
								delay_ms(500);
								Send_Debug_Info("（3）\n",4);
								delay_ms(500);
								delay_ms(500);
								Set_tba_Beep(SET);//任务板蜂鸣器开
								delay_ms(500);
								delay_ms(500);
								Set_tba_Beep(RESET);//任务板蜂鸣器关
								RFID_Data_Handler(sRXRFID);//对RFID内数据进行处理：通过Full_Read_Card()函数进行读卡操作后,读取的RFID卡的数据存储在RXREID这个数组中		
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
		 if(cont[0]<=0x02)//在线上
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
	   else if(cont[0]>=0x03)//十字路口
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



void TFT_GD(int mode,int num)     /********TFT固定图片*********/
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


//mode==1 获取档位，mode==2调光
int Full_Light(uint8_t gear,int mode)
{
	uint8_t i;
	uint8_t gear_init = 0;	// 初始挡位值
	uint16_t array[2];		//缓存自学习的光档位数组
	
	if((gear > 0) && (gear < 5))
	{
		delay_ms(100);
		array[0] = Get_Bh_Value();		//光强度传感器	
		for(i=0; i<4; i++)
		{
			gear_init++;
			Infrared_Send(H_1,4);
			delay_ms(500);
			delay_ms(500);
			delay_ms(500);
			array[1] = Get_Bh_Value();		//光强度传感器
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
				Infrared_Send(H_1,4);	//光源档位加1
			}
			else if(gear_init==3)
			{
				Infrared_Send(H_2,4);	//光源档位加2
			}
			else if(gear_init==4)
			{
				Infrared_Send(H_3,4);	//光源档位加3
			}
		}
		if(mode==2)
		{
			if(gear==2)
			{
				Infrared_Send(H_1,4);	//光源档位加1
			}
			else if(gear==3)
			{
				Infrared_Send(H_2,4);	//光源档位加2
			}
			else if(gear==4)
			{
				Infrared_Send(H_3,4);	//光源档位加3
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
		Ultrasonic_Ranging();		//超声波数据
		delay_ms(100);
	}	
	LED_DisTab[4]=dis/100;
	LED_DisTab[5]=dis%100;
	delay_ms(500);
	Full_Car_CKS(LED_DisTab);					//计算校验和
	Send_ZigbeeData_To_Fifo(LED_DisTab,8);
	delay_ms(200);
	Send_ZigbeeData_To_Fifo(LED_DisTab,8);
	delay_ms(300);
	JL_Code[0]=(dis/100)+0x30;
	JL_Code[1]=((dis%100/10))+0x30;
	JL_Code[2]=(dis%10)+0x30;
	Send_Debug_Info(JL_Code,3);  //debug 毫米返回数据
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
	u8 CT[2];									//卡类型
	u8 SN[4]; 									//卡号
	u8 KEY[6]={0xff,0xff,0xff,0xff,0xff,0xff}; //密钥		
	int a=0;
		status=PcdRequest(PICC_REQALL,CT);	
		if(status==MI_OK)
		{
			status = MI_ERR;
			Send_Debug_Info("f OK\n",6);
			status = PcdAnticoll(SN);				//防冲撞
			if(status == MI_OK)
			{
				status=MI_ERR;
				Send_Debug_Info("a OK\n",6);
				delay_ms(100);			
				status =PcdSelect(SN);				//选定此卡
				if(status == MI_OK)					//选定成功
				{
					status=MI_ERR;
					Send_Debug_Info("P OK\n",6);
					delay_ms(100);
					status =PcdAuthState(0x60,Addr,KEY,SN);		//验证密钥:0x60 = 验证A密钥;0x61 = 验证B密钥。块地址0x03\KEY为密码\ SN卡序列号(或称卡号)
					if(status == MI_OK)		//RFID卡密码、卡号验证成功
					{					
						status = MI_ERR;					
						Send_Debug_Info("t OK\n",6);
						delay_ms(100);
						status=PcdRead(Addr,data);				//读卡：其中，s是该函数一开始定义的读卡默认块地址data为读出的数据，16字节
						if(status == MI_OK)
						{
							a=1;
							status = MI_ERR;							
							Send_Debug_Info("r OK\n",6);		//发送调试信息"read OK",即读卡成功
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



