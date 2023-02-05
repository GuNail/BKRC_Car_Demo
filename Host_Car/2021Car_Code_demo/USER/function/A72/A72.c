#include "A72.h"
#include "main.h"
#include "Timer.h"
#include "tba.h"
#include "Fun_motor.h"
#include "Feng_huo.h"
#include "Lu_Deng.h"
#include <string.h>
#include "plan.h"

extern uint8_t Host_AGV_Return_Flag;
extern uint8_t AGV_data_Falg;

uint8_t Rx_Flag ;
uint8_t Infrared_Tab[6];			//�������ݴ������
uint8_t Wifi_Rx_Buf[WIFI_MAX_NUM];
uint8_t Wifi_Rx_num ;
uint8_t Wifi_Rx_flag ;  //������ɱ�־λ
uint32_t Wifi_rxtime = 0;


void A72_Wifi_Send(uint8_t *data)
{
    Send_WifiData_To_Fifo(data, 8);
}



void A72_Trffic_light_Task_A(void)
{
    Host_Car_Run_Flag = 0;
    uint8_t data[8] = {0x55, 0x72, 0x01, 0x0A, 0x00, 0x00, 0x00, 0xBB};
    A72_Wifi_Send(data);
}

void A72_Trffic_light_Task_B(void)
{
    Host_Car_Run_Flag = 0;
    uint8_t data[8] = {0x55, 0x72, 0x01, 0x0B, 0x00, 0x00, 0x00, 0xBB};
    A72_Wifi_Send(data);
}

void A72_QR_Code_Task(void)
{
    Host_Car_Run_Flag = 0;
    uint8_t data[8] = {0x55, 0x72, 0x02, 0x01, 0x00, 0x00, 0x00, 0xBB};
    A72_Wifi_Send(data);
}

void A72_Tx_Task_A(void)
{
    Host_Car_Run_Flag = 0;
    uint8_t data[8] = {0x55, 0x72, 0x03, 0x00, 0x00, 0x00, 0x00, 0xBB};
    A72_Wifi_Send(data);
}

void A72_Tx_Task_B(void)
{
    Host_Car_Run_Flag = 0;
    uint8_t data[8] = {0x55, 0x72, 0x04, 0x00, 0x00, 0x00, 0x00, 0xBB};
    A72_Wifi_Send(data);
}


void WifiRx_Save(uint8_t res)//���ݱ���
{
    if(Wifi_Rx_flag == 0)
    {
        Wifi_rxtime = gt_get() + 10;
        Wifi_Rx_num = 0;
        Wifi_Rx_Buf[Wifi_Rx_num] = res;
        Wifi_Rx_flag = 1;
    }
    else if(Wifi_Rx_num < WIFI_MAX_NUM )
    {
        Wifi_Rx_Buf[++Wifi_Rx_num] = res;
    }
}

void Normal_data(void)	 // ��������8�ֽڿ���ָ��
{
    u8 sum = 0;

    if(Wifi_Rx_Buf[7] == 0xbb)	 // �жϰ�β
    {
        //��ָ������λ��ָ�������У��
        //ע�⣺��������ʱӦ�öԺ���256ȡ�ࡣ
        sum = (Wifi_Rx_Buf[2] + Wifi_Rx_Buf[3] + Wifi_Rx_Buf[4] + Wifi_Rx_Buf[5]) % 256;

        if(sum == Wifi_Rx_Buf[6])
        {
            Rx_Flag = 1;
        }
        else Rx_Flag = 0;
    }
}

void Abnormal_data(void)	  //�����쳣����
{
    u8 i, j;
    u8 sum = 0;

    if(Wifi_Rx_num < 8)			// �쳣�����ֽ���С��8�ֽڲ�������
    {
        Rx_Flag = 0;
    }
    else
    {
        for(i = 0; i <= (Wifi_Rx_num - 7); i++)
        {
            if(Wifi_Rx_Buf[i] == 0x55)	 // Ѱ�Ұ�ͷ
            {
                if(Wifi_Rx_Buf[i + 7] == 0xbb)	// �жϰ�β
                {
                    sum = (Wifi_Rx_Buf[i + 2] + Wifi_Rx_Buf[i + 3] + Wifi_Rx_Buf[i + 4] + Wifi_Rx_Buf[i + 5]) % 256;

                    if(sum == Wifi_Rx_Buf[i + 6])	 // �ж����
                    {
                        for(j = 0; j < 8; j++)
                        {
                            Wifi_Rx_Buf[j] = Wifi_Rx_Buf[j + i];	 // ���ݰ���
                        }

                        Rx_Flag = 1;
                    }
                    else Rx_Flag = 0;
                }
            }
        }

    }
}

void WifiRx_Check(void)
{
    if(Wifi_Rx_flag)
    {
        if(gt_get_sub(Wifi_rxtime) == 0)
        {
            if(Wifi_Rx_Buf[0] == 0xFD)
            {
                Send_ZigbeeData_To_Fifo( Wifi_Rx_Buf, (Wifi_Rx_num + 1));
            }
            else if(Wifi_Rx_Buf[0] == 0x55)
            {
                Normal_data();
            }
            else
            {
                Abnormal_data();
            }

            Wifi_Rx_flag = 0;
        }
    }

    if(Rx_Flag == 1)
    {
        if(Wifi_Rx_Buf[1] == 0xAA)
        {
            switch(Wifi_Rx_Buf[2])
            {
                case 0x01:              //ֹͣ
                    Stop();
                    break;

                case 0x02:              //ǰ��
                    Go_Back(Wifi_Rx_Buf[3], (Wifi_Rx_Buf[5] << 8) | Wifi_Rx_Buf[4]);
                    break;

                case 0x03:              //����
                    Go_Back(Wifi_Rx_Buf[3], (Wifi_Rx_Buf[5] << 8) | Wifi_Rx_Buf[4]);
                    break;

                case 0x04:              //��ת
                    Track_wheel_L(100);
                    break;

                case 0x05:              //��ת
                    Track_wheel_R(100);
                    break;

                case 0x06:              //ѭ��
                    Track(100);
                    break;

                case 0x07:				//��������
                    CanHost_Mp = 0;
                    break;

                case 0x10:									//����ǰ��λ����
                    Infrared_Tab[0] = Wifi_Rx_Buf[3];
                    Infrared_Tab[1] = Wifi_Rx_Buf[4];
                    Infrared_Tab[2] = Wifi_Rx_Buf[5];
                    break;

                case 0x11:									//�������λ����
                    Infrared_Tab[3] = Wifi_Rx_Buf[3];
                    Infrared_Tab[4] = Wifi_Rx_Buf[4];
                    Infrared_Tab[5] = Wifi_Rx_Buf[5];
                    break;

                case 0x12:							//֪ͨС����Ƭ�����ͺ�����
                    Feng_huo_Open(Infrared_Tab);
                    break;

                case 0x20:							//ת��ƿ���
                    A72_Control_LED(((Wifi_Rx_Buf[3] << 4) | Wifi_Rx_Buf[4]));
                    break;

                case 0x30:
                    Set_tba_Beep(Wifi_Rx_Buf[3]);
                    break;

                case 0x40:										//��δʹ��

                    break;

                case 0x50:  				//������Ƭ�Ϸ�

                    break;

                case 0x51:    					//������Ƭ�·�

                    break;

                case 0x61:    					//���ⷢ����ƹ�Դǿ�ȵ�λ��1
                    Lu_Deng_Gears_1();
                    break;

                case 0x62:   	 				//���ⷢ����ƹ�Դǿ�ȵ�λ��2
                    Lu_Deng_Gears_2();
                    break;

                case 0x63:    					//���ⷢ����ƹ�Դǿ�ȵ�λ��3
                    Lu_Deng_Gears_3();
                    break;

                case 0x72:
                    Host_Car_Run_Flag = Wifi_Rx_Buf[3];
                    break;

                case 0x80:						//������������λ
                    Host_AGV_Return_Flag = Wifi_Rx_Buf[3];   //SET ���� / RESET ��ֹ
                    break;

                default:
                    break;
            }
        }
        else
        {
            Send_ZigbeeData_To_Fifo(Wifi_Rx_Buf, 8);
        }

        Rx_Flag = 0;
    }
}
