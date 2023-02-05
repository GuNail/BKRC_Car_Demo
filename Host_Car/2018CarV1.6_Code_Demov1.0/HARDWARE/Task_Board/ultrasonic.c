#include "stm32f4xx.h"
#include "ultrasonic.h"
#include "delay.h"
float Ultrasonic_Value = 0;
uint32_t Ultrasonic_Num=0;						// 计数值
uint16_t dis =0 ;
void Ultrasonic_Port()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB,ENABLE);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource14,GPIO_AF_SWJ);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource13,GPIO_AF_SWJ);
	
	//INC--TX
	GPIO_InitStructure.GPIO_Pin = INC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  //通用输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   //上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_Init(INC_PROT,&GPIO_InitStructure);
	
	//INT0--RX
	GPIO_InitStructure.GPIO_Pin = INT0_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   //浮空 
	GPIO_Init(INT0_PROT,&GPIO_InitStructure);
}


void Ultrasonic_TIM(uint16_t arr,uint16_t psc)
{
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	
	TIM_InitStructure.TIM_Period = arr;
	TIM_InitStructure.TIM_Prescaler = psc;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM6,&TIM_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 8;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM6, DISABLE);
}

void Ultrasonic_EXTI()
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	SYSCFG_EXTILineConfig(Ultrasonic_EXTI_PROT,Ultrasonic_EXTI_PIN);
	EXTI_InitStructure.EXTI_Line = Ultrasonic_EXTI_Line;
	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = Ultrasonic_EXTI_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void Ultrasonic_Init()
{
	Ultrasonic_Port();
	Ultrasonic_TIM(9,83);
	Ultrasonic_EXTI();
}

//超声波测距
void Ultrasonic_Ranging()
{
    INC = 1;            
	  delay_us(3);
    INC = 0;

	TIM_Cmd(TIM6,ENABLE);
	//EXTI_ClearITPendingBit(EXTI_Line4);	
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
   Ultrasonic_Num  = 0;			 // 定时器清零
	delay_ms(30);			 //等待一段时间，等待发送超声波控制信号
	INC = 1;
	delay_ms(5);
}

void TIM6_DAC_IRQHandler()
{
	if(TIM_GetITStatus(TIM6,TIM_IT_Update) == SET)
	{
		Ultrasonic_Num++;		
	}
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
}

void Ultrasonic_EXTI_IRQHandler(void)
{
	if(EXTI_GetITStatus(Ultrasonic_EXTI_Line) == SET)
	{
		if(GPIO_ReadInputDataBit(INT0_PROT,INT0_PIN) == RESET)
		{	
			TIM_Cmd(TIM6,DISABLE);
			Ultrasonic_Value = Ultrasonic_Num;
			Ultrasonic_Value =(float)Ultrasonic_Value*1.72-15;       // 计算距离定时10us，S=Vt/2（减2是误差补尝）
			dis = (uint16_t) Ultrasonic_Value;
		}		
		EXTI_ClearITPendingBit(Ultrasonic_EXTI_Line);
	}  
}

