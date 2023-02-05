#ifndef __FULL_H
#define __FULL_H

#define L 1
#define R 2

#define Get_Go_sp		60
#define Get_Go_mp		410
#define Get_RL_sp		60
#define Get_Tra_sp	60


extern int Full_Run_Flag;				//ȫ�Զ�����״̬��־λ
extern int Full_ETC_Open_Flag;			//�������ETC������־λ
extern int Full_FC_Open_Flag;	  //�ȴ���������־λ
extern int Full_DZ_Open_Flag;				//ETC�����ر�־λ
extern int Full_Stereo_Return_Flag;		//���峵�������ر�־λA
extern int Full_astern_garage_Flag;		//���峵����������־λA
extern int Full_Btereo_Return_Flag;		//���峵�������ر�־λB
extern int Full_Bstern_garage_Flag;		//���峵����������־λB
extern int Full_AGV_Return_Flag1;	    //����ӳ����ر�־λ1
extern int Full_AGV_Return_Flag2;	    //����ӳ����ر�־λ2
extern int Full_Fucheruku_Return_Flag;		//�������峵���־λA 
extern int mark;
extern int RFID_DKWC_Flag; //RFID������ɱ�־λ

extern uint8_t Start_Light_dear;			//��ǰ��λֵ
extern uint8_t  AGV_StartXY;					//�ӳ���ʼ����
extern uint16_t RFID_Y;						    //RFID����Ч��Ϣ

extern uint8_t Shape_Number[];
extern uint8_t Color_Number[];
extern uint8_t Plate_Number[];
extern uint8_t Qr_Code[];
extern uint8_t AGV_Return;
extern uint8_t Section_Number;		//�������
extern uint8_t Block_Number;		 //���ݿ���
extern uint8_t Speech_recognition_data[];
extern uint8_t LTXS_YY[];
extern uint8_t WIFI_Data[8];


/*******************************************************************************************
ȫ�Զ�С�����ж�������
*******************************************************************************************/
void Full_Stop(void);                                         //ȫ�Զ�ֹͣ����
void Track(uint8_t sp);                                       //ѭ������
void Full_Left(uint8_t sp);                                   //ȫ�Զ���ת����
void Full_Right(uint8_t sp);                                  //ȫ�Զ���ת����
void Back_Track(uint8_t gd);                                  //����ѭ������
void Full_Track(uint8_t sp);                                  //ȫ�Զ�ѭ������
void Go(uint8_t sp,uint16_t mp);                              //ǰ������
void Full_Back_Track(uint8_t sp);                             //�Զ�����ѭ������
void Full_Go(uint8_t sp,uint16_t mp);                         //�Զ�ǰ������
void Full_Back(uint8_t sp,uint16_t mp);                       //�Զ����˺���
void Full_Mp_Track(uint8_t sp,uint16_t mp);                   //�Զ�ָ������ѭ��
void Full_LefRigX(uint8_t sp,uint8_t LorR,uint16_t angle);    //�Զ�����ת�亯��
void Full_Terrain_Track(uint8_t sp,uint8_t Terrain_Type);     //�����ѭ������
void Full_Terrain_XG(uint8_t sp,uint16_t mp);//ָ��ǰ��������ѭ��
void Full_XJ_Track(void);   //Ѱ������     
void Full_TXXJ_Track(void); //�������ѭ����
void Full_TXTrack(uint8_t sp);   //�������ѭ��
void Full_TXDX_XG(uint8_t sp,uint16_t mp);

/*******************************************************************************************
ȫ�Զ�С�����־�ｻ��������������
*******************************************************************************************/
void Full_Write_Card(void);
void Full_Read_Card(void);

void TFT_turn_over(u8 num);                                   //TFT��ҳ����A
void Control_TFT_show_data(u8 *Data);                         //TFT��ʾ����A
void Control_TFT_show_distance(u8 *Data);                     //TFT��ʾ����A
void Control_TFT_show_Time(u8 Mode_num);                      //TFT��ʱ����A
void Control_TFT_HEX_distance(u8 *Data);
void TFTB_turn_over(u8 num);                                   //TFT��ҳ����B
void Control_TFTB_show_data(u8 *Data);                         //TFT��ʾ����B
void Control_TFTB_show_distance(u8 *Data);                     //TFT��ʾ����B
void Control_TFTB_show_Time(u8 Mode_num);                      //TFT��ʱ����B

void full_Speech(u8 *s_data,u16 lenght);                      //ȫ�Զ���������

void Full_Light_Goal(u8 End_Goal);                            //�Զ�����·�Ƶ�λ

int Full_current_brightness(void);                      //��ó�ʼ�ƹ⵵λ
void fht_frared(u8 *Data);                                   //���̨������


void Revolve_Show(u8 *data);                                   //������ʾ �ֶ�����
void Stereo_Display(u8 *data);                                //������ʾ����

extern u8 Infrared_Tab1[6];
          

void Track_gate_Open(int mode,u8 *data);                     //������բ
void Control_DZ_show_data(u8 *Data);                          //��բ��ʾ�������ݿ���

void Wireless_charging(void);                                 //�������޳��

void Full_astern_garage_Control(void);                        //���峵�⵹�������ƺ���A
void Send_Get_Stereo_Return(u8 Query_mode);                   //�������峵�ⷵ��״ָ̬��A
void SetStereo_garage(uint8_t mode);                          //���峵�����A
void Full_Bstern_garage_Control(void);                        //���峵�⵹�������ƺ���B
void Send_Get_Btereo_Return(u8 Query_mode);                   //�������峵�ⷵ��״ָ̬��B
void SetStereo_Barage(uint8_t mode);                          //���峵�����B
void Full_Fuche_garage_Control(void);        //�������峵�⵹�������ƺ��� ==1:A��   2��B��

void LED_Distance(void);                                      //��������� LED��ʾ����
void LT_Distance(void);                                       //��������� ������ʾ����
void LED_graphics(u8 *Data);                                  //LED��ʾ����TFTͼ�β���
void Control_LED_show_data(u8 X,u8 *Data);                    //LED��ʾ����
void Control_LED_Nixie_light(u8 Mode_num);                    //LED��ʱ����

/*******************************************************************************************
ȫ�Զ�С�����ܺ���
*******************************************************************************************/
void Full(void);                                              //ȫ�Զ���Ӧ����
void Full_Diplay(uint32_t vluae);                             //��ӡ������ʾ
void Full_Car_CKS(uint8_t *srcdata);                          //����У���
void RFID_Data_Handler(uint8_t *RFIDBuf);                     //RFID���ݴ�����

void Full_Traffic_Light_Recognize_patterns(void);                 //��ͨ�ƽ���ʶ��ģʽA
void Full_Identification_result_comparison(u8 mode_data);     //��ͨ��ʶ�����ȶ�A
void Full_Braffic_Light_Recognize_patterns(void);                 //��ͨ�ƽ���ʶ��ģʽB
void Full_Bdentification_result_comparison(u8 Bode_data);     //��ͨ��ʶ�����ȶ�B
/*******************************************************************************************
ȫ�Զ�С���ӳ����ƺ�������
*******************************************************************************************/
void AGV_Start1(uint8_t LightValue_X,uint8_t N_AGV_Start,uint8_t RFID_AGVData_Y);
void AGV_Start2(void);
void Send_AGV_Interaction_Function(uint8_t nun_data);            //�����븱����������
void AGV_Control_Infrared(u8 *Infrared_Data);                    //�������������ͺ�������	
/*******************************************************************************************
ȫ�Զ�С��ͨ�ź���
*******************************************************************************************/
void A72_Send(u8 *A72_Data);                             //������A72
void wifi_Send(u8 *wifi_Data);


/*******************************************************************************************
�Լ�С��Ѱ������
*******************************************************************************************/

/*******************************************************************/
void Control_TFT_show_distance1(void);
void TFT_GD(int mode,int num);
void Full_Find_card(void);
void Send_AVG_data(uint8_t *data);
int Full_Light(uint8_t gear,int mode);
void More_Read_card(char *Sector);
int Full_Read1_Card(u8 Addr,u8 *data);
/*******************************************************************/

void Modify_Beacon_tower_Infrared(u8 *Beacon_tower_Data);

#endif
