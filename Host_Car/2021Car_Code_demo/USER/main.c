/**
�������ƣ�2018�����ۺϳ���
�޸�ʱ�䣺2018.05.15
*/
#include "main.h"
#include <stdio.h>
#include "stm32f4xx.h"
#include "delay.h"
#include "infrared.h"
#include "cba.h"
#include "ultrasonic.h"
#include "canp_hostcom.h"
#include "hard_can.h"
#include "bh1750.h"
#include "syn7318.h"
#include "power_check.h"
#include "can_user.h"
#include "data_base.h"
#include "roadway_check.h"
#include "tba.h"
#include "data_base.h"
#include "swopt_drv.h"
#include "uart_a72.h"
#include "Can_check.h"
#include "delay.h"
#include "can_user.h"
#include "Timer.h"
#include "Rc522.h"
/*******************************/
#include "Fun_motor.h"
#include "plan.h"
#include "AGV_Car.h"
#include "arm_math.h"
#include "Dao_Zha.h"
#include "LED_DIS.h"
#include "TFT.h"
#include "WU_Xian.h"
#include "Feng_huo.h"
#include "Che_Ku.h"
#include "Lu_Deng.h"
#include "DEBUG.h"
#include "3D_DIS.h"
#include "A72.h"
#include "ETC.h"
#include "Yu_Ying.h"

RCC_ClocksTypeDef RCC_Clocks;

uint16_t main_cont;

/**
�������ܣ�Ӳ����ʼ��
��    ������
�� �� ֵ����
*/
void Hardware_Init()
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

    //UartA72_Init();
    Can_check_Init(83, 7);											//CAN���߶�ʱ����ʼ��
    roadway_check_TimInit(167, 1999);						//·�����
    Timer_Init(167, 999);										   //��������ͨѶʱ��֡
    //TIMx_Configuration();
    Readcard_daivce_Init();											//RFID��ʼ��
}

uint8_t RFID[16] = "123456789";
uint8_t open_road_buf[] = {0x36, 0x33, 0x31, 0x31, 0x30, 0x00, 0x0, 0x30};			//��բ����
uint8_t test_buf[] = {0xFD, 0x00, 0x06, 0x01, 0x01, 0xC4, 0xFA, 0xBA, 0xC3};			//�������������á�
uint8_t repo_buf[] = {0x03, 0x05, 0x14, 0x45, 0xDE, 0x92};							//�򿪺��ⱨ��
uint8_t seg[] = {0x55, 0xAA, 0x01};

/**
�������ܣ��������
��    ������
�� �� ֵ����
*/
void KEY_Check()
{
    if(S1 == 0)
    {
        LED1 = !LED1;

        while(!S1);

        Host_Car_Run_Flag = 1;
        Mark = 5;
    }

    if(S2 == 0)
    {
        LED2 = !LED2;
        while(!S2);
			
			Fun_Track(60);
			
			printf("0x%x", Get_Host_UpTrack(TRACK_H8));
			delay_ms(10);
			Fun_Go_Back(40,100);
			delay_ms(10);
    }

    if(S3 == 0)
    {
        LED3 = !LED3;

        while(!S3);

        Fun_Track_Go();
    }

    if(S4 == 0)
    {
        LED4 = !LED4;

        while(!S4);

        AGV_Car_Rode3();
        delay_ms(500);
        AGV_Car_Start();
    }
}


static uint32_t Power_check_times;		  //�����������
static uint32_t LED_twinkle_times;		  //LED��˸����
static uint32_t WIFI_Upload_data_times;   //ͨ��Wifi�ϴ���������
static uint32_t RFID_Init_Check_times;	//RFID�������

uint32_t  Host_Error_Check_times_seate = 1; //�����Զ���ʱ
static uint32_t Host_Error_Check_times; //�������Զ���ʱ
static uint32_t Host_Error_Flag;



int main(void)
{

    uint16_t Light_Value = 0;				//��ǿ��ֵ
    uint16_t CodedDisk_Value = 0;			//����
    uint16_t Nav_Value = 0;					//�Ƕ�

    Hardware_Init();						//Ӳ����ʼ��

    LED_twinkle_times =  gt_get() + 1000;
    Power_check_times =  gt_get() + 200;
    WIFI_Upload_data_times = gt_get() + 200;
    RFID_Init_Check_times = gt_get() + 200;
    Host_Error_Check_times = gt_get() + 1000;

    Principal_Tab[0] = 0x55;
    Principal_Tab[1] = 0xAA;

    Follower_Tab[0] = 0x55;
    Follower_Tab[1] = 0x02;

    Send_UpMotor(0, 0);
    Cba_Beep(50, 10, 10);

    while(1)
    {
        KEY_Check();									//�������
        Can_ZigBeeRx_Check();
        AGV_Car_Zigbee();
        WifiRx_Check();
       Stop_Flag_Cheack();

        #if 1

        if(Host_Car_Run_Flag == 1)
        {
            plan();
            Host_Error_Flag = 0;
        }
        else if(Host_Car_Run_Flag == 0 && Mark >= 5)
        {
            if(Host_Error_Check_times_seate)
            {
                if(gt_get_sub(Host_Error_Check_times) == 0)
                {
                    Host_Error_Check_times = gt_get() + 1000;
                    Host_Error_Flag++;

                    if(Host_Error_Flag >= 15)
                    {
                        Cba_Beep(50, 10, 10);
                        Host_Car_Run_Flag = 1;
                        Host_Error_Flag = 0;
                    }
                }
            }
        }

        #endif

        #if 1

        if(gt_get_sub(LED_twinkle_times) == 0)
        {
            LED_twinkle_times =  gt_get() + 1000;			//LED4״̬ȡ��
            LED4 = !LED4;
        }

        #endif

        if(gt_get_sub(Power_check_times) == 0)
        {
            Power_check_times =  gt_get() + 200;		//��ص������
            Power_Check();
        }

        #if 1

        if(gt_get_sub(RFID_Init_Check_times) == 0)
        {
            RFID_Init_Check_times =  gt_get() + 1000;	//RFID��ʼ�����

            if(Rc522_GetLinkFlag() == 0)
            {
                Readcard_daivce_Init();
            }
            else
            {
                LED4 = !LED4;
                Rc522_LinkTest();
            }
        }

        #endif

        #if 0

        if(gt_get_sub(WIFI_Upload_data_times) == 0)
        {
            WIFI_Upload_data_times =  gt_get() + 1000;

            if(Host_AGV_Return_Flag == RESET)
            {
                CodedDisk_Value = CanHost_Mp;	//����
                Principal_Tab[2] = Stop_Flag;				//����״̬
                Principal_Tab[3] = CodedDisk_Value % 0xFF;
                Principal_Tab[4] = CodedDisk_Value / 0xFF;
                Send_WifiData_To_Fifo(Principal_Tab, 12);
            }
            else if((Host_AGV_Return_Flag == SET) && (AGV_data_Falg == SET))
            {
                Send_WifiData_To_Fifo(Follower_Tab, 50);
                AGV_data_Falg = 0;
            }
        }

        #endif
    }
}

void TIMx_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    // �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    // ������ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    // ���������ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 9;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void TIM_Mode_Config(uint32_t period, uint16_t Prescaler)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    // ����TIMx_CLK,x[6,7]
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    /* �ۼ� TIM_Period�������һ�����»����ж�*/
    //����ʱ����0������4999����Ϊ5000�Σ�Ϊһ����ʱ����
    TIM_TimeBaseStructure.TIM_Period = period;
    // ͨ�ÿ��ƶ�ʱ��ʱ��ԴTIMxCLK = HCLK/2=84MHz
    // �趨��ʱ��Ƶ��Ϊ=TIMxCLK/(TIM_Prescaler+1)=10000Hz
    TIM_TimeBaseStructure.TIM_Prescaler = Prescaler;
    // ����ʱ�ӷ�Ƶ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    // ������ʽ
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    // ��ʼ����ʱ��TIMx, x[1,8]
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    // �����ʱ�������жϱ�־λ
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    // ������ʱ�������ж�
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    // ʹ�ܶ�ʱ��
    TIM_Cmd(TIM2, ENABLE);
}

void TIMx_Configuration(void)
{
    TIMx_NVIC_Configuration();
    TIM_Mode_Config(9999, 8399);
}

void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus( TIM2, TIM_IT_Update) != RESET )
    {
        Can_ZigBeeRx_Check();
        AGV_Car_Zigbee();
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}








