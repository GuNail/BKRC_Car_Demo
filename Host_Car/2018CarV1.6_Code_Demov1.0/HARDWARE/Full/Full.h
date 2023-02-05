#ifndef __FULL_H
#define __FULL_H

#define L 1
#define R 2

#define Get_Go_sp		60
#define Get_Go_mp		410
#define Get_RL_sp		60
#define Get_Tra_sp	60


extern int Full_Run_Flag;				//全自动运行状态标志位
extern int Full_ETC_Open_Flag;			//允许接收ETC开启标志位
extern int Full_FC_Open_Flag;	  //等待副车入库标志位
extern int Full_DZ_Open_Flag;				//ETC允许返回标志位
extern int Full_Stereo_Return_Flag;		//立体车库允许返回标志位A
extern int Full_astern_garage_Flag;		//立体车库允许倒车标志位A
extern int Full_Btereo_Return_Flag;		//立体车库允许返回标志位B
extern int Full_Bstern_garage_Flag;		//立体车库允许倒车标志位B
extern int Full_AGV_Return_Flag1;	    //允许从车返回标志位1
extern int Full_AGV_Return_Flag2;	    //允许从车返回标志位2
extern int Full_Fucheruku_Return_Flag;		//副车立体车库标志位A 
extern int mark;
extern int RFID_DKWC_Flag; //RFID读卡完成标志位

extern uint8_t Start_Light_dear;			//当前档位值
extern uint8_t  AGV_StartXY;					//从车起始坐标
extern uint16_t RFID_Y;						    //RFID内有效信息

extern uint8_t Shape_Number[];
extern uint8_t Color_Number[];
extern uint8_t Plate_Number[];
extern uint8_t Qr_Code[];
extern uint8_t AGV_Return;
extern uint8_t Section_Number;		//扇区编号
extern uint8_t Block_Number;		 //数据块编号
extern uint8_t Speech_recognition_data[];
extern uint8_t LTXS_YY[];
extern uint8_t WIFI_Data[8];


/*******************************************************************************************
全自动小车运行动作函数
*******************************************************************************************/
void Full_Stop(void);                                         //全自动停止函数
void Track(uint8_t sp);                                       //循迹函数
void Full_Left(uint8_t sp);                                   //全自动左转函数
void Full_Right(uint8_t sp);                                  //全自动右转函数
void Back_Track(uint8_t gd);                                  //后退循迹函数
void Full_Track(uint8_t sp);                                  //全自动循迹函数
void Go(uint8_t sp,uint16_t mp);                              //前进函数
void Full_Back_Track(uint8_t sp);                             //自动后退循迹函数
void Full_Go(uint8_t sp,uint16_t mp);                         //自动前进函数
void Full_Back(uint8_t sp,uint16_t mp);                       //自动后退函数
void Full_Mp_Track(uint8_t sp,uint16_t mp);                   //自动指定码盘循迹
void Full_LefRigX(uint8_t sp,uint8_t LorR,uint16_t angle);    //自动任意转弯函数
void Full_Terrain_Track(uint8_t sp,uint8_t Terrain_Type);     //多地形循迹函数
void Full_Terrain_XG(uint8_t sp,uint16_t mp);//指定前进距多地形循迹
void Full_XJ_Track(void);   //寻迹函数     
void Full_TXXJ_Track(void); //特殊地形循迹加
void Full_TXTrack(uint8_t sp);   //特殊地形循迹
void Full_TXDX_XG(uint8_t sp,uint16_t mp);

/*******************************************************************************************
全自动小车与标志物交互函数参数函数
*******************************************************************************************/
void Full_Write_Card(void);
void Full_Read_Card(void);

void TFT_turn_over(u8 num);                                   //TFT翻页控制A
void Control_TFT_show_data(u8 *Data);                         //TFT显示车牌A
void Control_TFT_show_distance(u8 *Data);                     //TFT显示距离A
void Control_TFT_show_Time(u8 Mode_num);                      //TFT定时控制A
void Control_TFT_HEX_distance(u8 *Data);
void TFTB_turn_over(u8 num);                                   //TFT翻页控制B
void Control_TFTB_show_data(u8 *Data);                         //TFT显示车牌B
void Control_TFTB_show_distance(u8 *Data);                     //TFT显示距离B
void Control_TFTB_show_Time(u8 Mode_num);                      //TFT定时控制B

void full_Speech(u8 *s_data,u16 lenght);                      //全自动语音播报

void Full_Light_Goal(u8 End_Goal);                            //自动调节路灯档位

int Full_current_brightness(void);                      //获得初始灯光档位
void fht_frared(u8 *Data);                                   //烽火台开起函数


void Revolve_Show(u8 *data);                                   //立体显示 手动控制
void Stereo_Display(u8 *data);                                //立体显示车牌

extern u8 Infrared_Tab1[6];
          

void Track_gate_Open(int mode,u8 *data);                     //开启道闸
void Control_DZ_show_data(u8 *Data);                          //道闸显示车牌数据开启

void Wireless_charging(void);                                 //开启无限充电

void Full_astern_garage_Control(void);                        //立体车库倒车入库控制函数A
void Send_Get_Stereo_Return(u8 Query_mode);                   //发送立体车库返回状态指令A
void SetStereo_garage(uint8_t mode);                          //立体车库控制A
void Full_Bstern_garage_Control(void);                        //立体车库倒车入库控制函数B
void Send_Get_Btereo_Return(u8 Query_mode);                   //发送立体车库返回状态指令B
void SetStereo_Barage(uint8_t mode);                          //立体车库控制B
void Full_Fuche_garage_Control(void);        //副车立体车库倒车入库控制函数 ==1:A库   2：B库

void LED_Distance(void);                                      //超声波测距 LED显示距离
void LT_Distance(void);                                       //超声波测距 立体显示距离
void LED_graphics(u8 *Data);                                  //LED显示距离TFT图形参数
void Control_LED_show_data(u8 X,u8 *Data);                    //LED显示控制
void Control_LED_Nixie_light(u8 Mode_num);                    //LED计时控制

/*******************************************************************************************
全自动小车功能函数
*******************************************************************************************/
void Full(void);                                              //全自动响应函数
void Full_Diplay(uint32_t vluae);                             //打印数据显示
void Full_Car_CKS(uint8_t *srcdata);                          //计算校验和
void RFID_Data_Handler(uint8_t *RFIDBuf);                     //RFID数据处理函数

void Full_Traffic_Light_Recognize_patterns(void);                 //交通灯进入识别模式A
void Full_Identification_result_comparison(u8 mode_data);     //交通灯识别结果比对A
void Full_Braffic_Light_Recognize_patterns(void);                 //交通灯进入识别模式B
void Full_Bdentification_result_comparison(u8 Bode_data);     //交通灯识别结果比对B
/*******************************************************************************************
全自动小车从车控制函数函数
*******************************************************************************************/
void AGV_Start1(uint8_t LightValue_X,uint8_t N_AGV_Start,uint8_t RFID_AGVData_Y);
void AGV_Start2(void);
void Send_AGV_Interaction_Function(uint8_t nun_data);            //主车与副车交互函数
void AGV_Control_Infrared(u8 *Infrared_Data);                    //主车给副车发送红外数据	
/*******************************************************************************************
全自动小车通信函数
*******************************************************************************************/
void A72_Send(u8 *A72_Data);                             //主车给A72
void wifi_Send(u8 *wifi_Data);


/*******************************************************************************************
自加小车寻卡函数
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
