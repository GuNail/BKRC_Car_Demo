#include "stm32f4xx.h"
#include "tba.h"
/**
函数功能：任务板光敏电阻配置
参    数：无
返 回 值：无
*/
void Tba_Photoresistance_Config(void)
{
	GPIO_InitTypeDef  GPIO_TypeDefStructure;
	RCC_AHB1PeriphClockCmd(R_OUT_RCC_Periph	,ENABLE);
	
	GPIO_TypeDefStructure.GPIO_Pin = R_OUT_Pin;
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_IN;		//输入
	GPIO_TypeDefStructure.GPIO_PuPd = GPIO_PuPd_UP;   	//上拉
	GPIO_Init(R_OUT_Prot,&GPIO_TypeDefStructure);
}

uint8_t Get_tba_phsis_value(void)
{
	return GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_8);
}

/**
函数功能：任务板蜂鸣器配置
参    数：无
返 回 值：无
*/
void Tba_Beep_Config(void)
{
	GPIO_InitTypeDef  GPIO_TypeDefStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_OUT;		//输入
	GPIO_TypeDefStructure.GPIO_PuPd = GPIO_PuPd_UP;   		//上拉
	GPIO_Init(GPIOC,&GPIO_TypeDefStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
}

/**
函数功能：设置任务板蜂鸣器状态
参    数：swch SET 开启 RESET 关闭
返 回 值：无
*/

void Set_tba_Beep(uint8_t swch)
{
	if(swch == SET)
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	} 
	else if(swch == RESET)
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
	}
}


/*****
数码管初始化
******/

void Tab_seg_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOH | RCC_AHB1Periph_GPIOB,ENABLE);
	// SEG -- PF11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		// 输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		// 推挽输出 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		// 上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		// 输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		// 推挽输出 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		// 上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_Init(GPIOH,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		// 输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		// 推挽输出 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		// 上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	SER = 0;
	RCK = 0;
	SCK = 0;
	SEG = 0;
}

void SEG_Show(uint8_t SegSet)
{
	uint8_t i = 0;

	for(i = 0; i < 8; i++)
	{
		if((SegSet<<i)& 0x80)//判断数据高低位
       {
          SER = 1;                   //数据线高电平
       }
       else
			 {
          SER = 0;          //数据线低电平
       }
		SCK = 0;
		delay_us(1);
		delay_us(1);
		SCK = 1;	
	}
	RCK = 0;
	delay_us(1);
	delay_us(1);
	RCK = 1;
}



/**
函数功能：任务板转向灯配置
参    数: 无
返 回 值：无
*/
void Tba_WheelLED_Config(void)
{
	GPIO_InitTypeDef  GPIO_TypeDefStructure;
	RCC_AHB1PeriphClockCmd(R_LED_RCC_Periph|L_LED_RCC_Periph,ENABLE);
	// LED_L -- PH10  LED_R --PH11
	GPIO_TypeDefStructure.GPIO_Pin = R_LED_Pin|L_LED_Pin;
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_OUT;		//输入
	GPIO_TypeDefStructure.GPIO_PuPd = GPIO_PuPd_UP;   		//上拉
	GPIO_Init(GPIOH,&GPIO_TypeDefStructure);
	GPIO_SetBits(GPIOH,GPIO_Pin_10|GPIO_Pin_11);
}


/**
函数功能: 任务板转向灯控制
参    数：LorR  L_LED--左侧   R_LED--右侧
		  swch  SET -- 开     RESET--关
返 回 值: 无
*/
void Set_tba_WheelLED(uint8_t LorR,uint8_t swch)
{
	switch(LorR)
	{
		case L_LED:
			if(swch)
				GPIO_ResetBits(GPIOH,L_LED_Pin);
			else	
				GPIO_SetBits(GPIOH,L_LED_Pin);
			break;
		case R_LED:
			if(swch)
				GPIO_ResetBits(GPIOH,R_LED_Pin);				
			else
				GPIO_SetBits(GPIOH,R_LED_Pin);
			break;
	}
}

void open_tba_leftLED(void)
{
	Set_tba_WheelLED(L_LED,SET);
	Set_tba_WheelLED(R_LED,RESET);
}

void open_tba_rightLED(void)
{
	Set_tba_WheelLED(R_LED,SET);
	Set_tba_WheelLED(L_LED,RESET);
}

void open_tba_allLED(void)
{
	Set_tba_WheelLED(L_LED,SET);
	Set_tba_WheelLED(R_LED,SET);
}

void close_tba_allLED(void)
{
	Set_tba_WheelLED(L_LED,RESET);
	Set_tba_WheelLED(R_LED,RESET);
}

void A72_Control_LED(uint8_t LorR)
{
	switch(LorR)
	{
				case 0x00:
			close_tba_allLED();
		break;
		
				case 0x01:
			open_tba_rightLED();
		break;
		
				case 0x10:
			open_tba_leftLED();
		break;
				
				case 0x11:
			open_tba_allLED();
		break;
		
	}
}

/**
函数功能: 任务板初始化
参    数：无
返 回 值: 无
*/
void Tba_Init(void)
{
	Tba_Photoresistance_Config();
	Tba_Beep_Config();
	Tba_WheelLED_Config();
#ifdef SEG_open
	Tab_seg_Config();
#endif
}

