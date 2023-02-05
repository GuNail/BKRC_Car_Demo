#include "SEG.h"
#include "stm32f4xx.h"
#include "delay.h"

void Seg_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);

	// SEG -- PF11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		// 输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		// 推挽输出 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		// 上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	SEG = 0;
}

void HC595_Config(void)
{
	GPIO_InitTypeDef  GPIO_TypeDefStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH | RCC_AHB1Periph_GPIOB,ENABLE);
	
	// RCK -- PH10  SCK -- PH11
	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_10;
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_OUT;		// 输出模式
	GPIO_TypeDefStructure.GPIO_OType = GPIO_OType_PP;		// 推挽输出
	GPIO_TypeDefStructure.GPIO_PuPd = GPIO_PuPd_UP;   		// 上拉
	GPIO_TypeDefStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOH,&GPIO_TypeDefStructure);
	
	// SER -- PB15
	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_OUT;		// 输出模式
	GPIO_TypeDefStructure.GPIO_OType = GPIO_OType_PP;		// 推挽输出
	GPIO_TypeDefStructure.GPIO_PuPd = GPIO_PuPd_UP;   		// 上拉
	GPIO_TypeDefStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB,&GPIO_TypeDefStructure);
	
	SER = 0;
	RCK = 0;
	SCK = 0;
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

