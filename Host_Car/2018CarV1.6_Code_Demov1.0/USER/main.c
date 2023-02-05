/**
工程名称：2018主车综合程序
修改时间：2018.05.08
*/
#include "main.h"

RCC_ClocksTypeDef RCC_Clocks;
uint16_t main_cont;
extern uint8_t DZCP_data[6];
uint8_t RFID1[16]="123456789";
/**
函数功能：按键检测
参    数：无
返 回 值：无
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
static uint32_t Power_check_times;		//电量检测周期
static uint32_t LED_twinkle_times;		//LED闪烁周期
static uint32_t WIFI_Upload_data_times; //通过Wifi上传数据周期
static uint32_t Strero_Get_return_times;

static uint32_t Full_Run_Error = 0;

extern uint32_t Rs522_cmd_cnt;

uint32_t time_m_1s;

int main(void)
{
    uint16_t Light_Value = 0;				//光强度值
    uint16_t CodedDisk_Value = 0;			//码盘
    uint16_t Nav_Value = 0;					//角度

    Rs522_cmd_cnt = 0;
    Hardware_Init();						//硬件初始化

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
        KEY_Check();									//按键检测
        Can_WifiRx_Check();						//对接收到的wifi数据处理
        Can_ZigBeeRx_Check();                    //对接收到的ZigBee数据处理
        DX_Panduan();                           //特殊地形自动判断函数（可无视特殊地形与RFID卡）
        TXDX_Panduan();                         //特殊地形自动判断函数（可无视特殊地形与RFID卡）

        while(Full_Run_Flag)
        {
            if(gt_get_sub(LED_twinkle_times) == 0) //比较当前时间值是否大于目标时间值
            {
                LED_twinkle_times =  gt_get() + 500;			//LED4状态取反，获取目标时间值
                LED3 = !LED3;
								planning();
            }
            Full_Run_Error = 0;
        }

        if(gt_get_sub(Power_check_times) == 0)
        {
            Power_check_times =  gt_get() + 200;			//LED4状态取反
            Power_Check();									//电量检测

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
        if(Full_Run_Error == 150)//全局容错，待测试时间10S？200*100=20000ms
        {
            Cba_Beep(50, 10, 10);
            Full_Run_Flag = 1;
        }
			}
}


/**
函数功能：硬件初始化
参    数：无
返 回 值：无
*/
void Hardware_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);					//中断分组

    delay_init(168);
    Tba_Init();														//任务板初始化
    Infrared_Init();												//红外初始化
    Cba_Init();														//核心板初始化
    Ultrasonic_Init();												//超声波初始化
    Hard_Can_Init();												//CAN总线初始化
    BH1750_Configure();												//BH1750初始化配置
    SYN7318_Init();													//语音识别初始化
    Electricity_Init();												//电量检测初始化
    UartA72_Init();
    Can_check_Init(83, 7);											//CAN总线定时器初始化
    roadway_check_TimInit(167, 1999);								//路况检测
    Timer_Init(167, 999);										   //串行数据通讯时间帧
    Readcard_daivce_Init();
}

