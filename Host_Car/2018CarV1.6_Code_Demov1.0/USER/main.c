/**
�������ƣ�2018�����ۺϳ���
�޸�ʱ�䣺2018.05.08
*/
#include "main.h"

RCC_ClocksTypeDef RCC_Clocks;
uint16_t main_cont;
extern uint8_t DZCP_data[6];
uint8_t RFID1[16]="123456789";
/**
�������ܣ��������
��    ������
�� �� ֵ����
*/
void KEY_Check()
{
	if(S1 == 0)
	{
			while(!S1);
			LED1 = !LED1;
			Full_Left(60);
	}
	if(S2 == 0)
	{
			while(!S2);
			LED2 = !LED2;
			PcdWrite(0x1,RFID1);
	}

	if(S3 == 0)
	{

			while(!S3); 
			LED3 = !LED3;
		Full_Find_card();
	}
	if(S4 == 0)
	{
		while(!S4);
		LED4 = !LED4;
		
	}
}
static uint32_t Power_check_times;		//�����������
static uint32_t LED_twinkle_times;		//LED��˸����
static uint32_t WIFI_Upload_data_times; //ͨ��Wifi�ϴ���������
static uint32_t Strero_Get_return_times;

static uint32_t Full_Run_Error = 0;

extern uint32_t Rs522_cmd_cnt;

uint32_t time_m_1s;

int main(void)
{
    uint16_t Light_Value = 0;				//��ǿ��ֵ
    uint16_t CodedDisk_Value = 0;			//����
    uint16_t Nav_Value = 0;					//�Ƕ�

    Rs522_cmd_cnt = 0;
    Hardware_Init();						//Ӳ����ʼ��

    LED_twinkle_times =  gt_get() + 200;     //
    Power_check_times =  gt_get() + 200;

    WIFI_Upload_data_times = gt_get() + 200;
    Strero_Get_return_times = gt_get() + 200;

    Principal_Tab[0] = 0x55;
    Principal_Tab[1] = 0xAA;

    Follower_Tab[0] = 0x55;
    Follower_Tab[1] = 0x02;

    Send_UpMotor(0, 0);

    Cba_Beep(50, 10, 10);

    time_m_1s = gt_get() + 1000;

    while(1)
    {
        KEY_Check();									//�������
        Can_WifiRx_Check();						//�Խ��յ���wifi���ݴ���
        Can_ZigBeeRx_Check();                    //�Խ��յ���ZigBee���ݴ���
        DX_Panduan();                           //��������Զ��жϺ��������������������RFID����
        TXDX_Panduan();                         //��������Զ��жϺ��������������������RFID����

        while(Full_Run_Flag)
        {
            if(gt_get_sub(LED_twinkle_times) == 0) //�Ƚϵ�ǰʱ��ֵ�Ƿ����Ŀ��ʱ��ֵ
            {
                LED_twinkle_times =  gt_get() + 500;			//LED4״̬ȡ������ȡĿ��ʱ��ֵ
                LED3 = !LED3;
								planning();
            }
            Full_Run_Error = 0;
        }

        if(gt_get_sub(Power_check_times) == 0)
        {
            Power_check_times =  gt_get() + 200;			//LED4״̬ȡ��
            Power_Check();									//�������

            if(Rc522_GetLinkFlag() == 0)
            {
                Readcard_daivce_Init();
                Cba_Beep(50, 10, 10);
            }
            else
            {
                LED4 = !LED4;
            }

            if(MLib_abs(mark) >= 5)	//abs(mark) >= 5)
            {
                Full_Run_Error++;
            }
        }
        if(Full_Run_Error == 150)//ȫ���ݴ�������ʱ��10S��200*100=20000ms
        {
            Cba_Beep(50, 10, 10);
            Full_Run_Flag = 1;
        }
			}
}


/**
�������ܣ�Ӳ����ʼ��
��    ������
�� �� ֵ����
*/
void Hardware_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);					//�жϷ���

    delay_init(168);
    Tba_Init();														//������ʼ��
    Infrared_Init();												//�����ʼ��
    Cba_Init();														//���İ��ʼ��
    Ultrasonic_Init();												//��������ʼ��
    Hard_Can_Init();												//CAN���߳�ʼ��
    BH1750_Configure();												//BH1750��ʼ������
    SYN7318_Init();													//����ʶ���ʼ��
    Electricity_Init();												//��������ʼ��
    UartA72_Init();
    Can_check_Init(83, 7);											//CAN���߶�ʱ����ʼ��
    roadway_check_TimInit(167, 1999);								//·�����
    Timer_Init(167, 999);										   //��������ͨѶʱ��֡
    Readcard_daivce_Init();
}

