#include "SEG.h"
#include "stm32f4xx.h"
#include "delay.h"

void Seg_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);

	// SEG -- PF11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		// ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		// ������� 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		// ����
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
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_OUT;		// ���ģʽ
	GPIO_TypeDefStructure.GPIO_OType = GPIO_OType_PP;		// �������
	GPIO_TypeDefStructure.GPIO_PuPd = GPIO_PuPd_UP;   		// ����
	GPIO_TypeDefStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOH,&GPIO_TypeDefStructure);
	
	// SER -- PB15
	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_OUT;		// ���ģʽ
	GPIO_TypeDefStructure.GPIO_OType = GPIO_OType_PP;		// �������
	GPIO_TypeDefStructure.GPIO_PuPd = GPIO_PuPd_UP;   		// ����
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
		if((SegSet<<i)& 0x80)//�ж����ݸߵ�λ
       {
          SER = 1;                   //�����߸ߵ�ƽ
       }
       else
			 {
          SER = 0;          //�����ߵ͵�ƽ
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

