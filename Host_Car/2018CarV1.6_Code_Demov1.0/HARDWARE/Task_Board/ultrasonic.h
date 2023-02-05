#include "stm32f4xx.h"
#include "sys.h"

#ifndef __ULTRASONIC_H
#define __ULTRASONIC_H

#define INC PBout(4)


#define INC_PIN 		GPIO_Pin_4
#define INC_PROT 		GPIOB

#define INT0_PIN 		GPIO_Pin_15
#define INT0_PROT 	GPIOA

#define Ultrasonic_EXTI_PROT				EXTI_PortSourceGPIOB
#define Ultrasonic_EXTI_PIN					EXTI_PinSource4
#define Ultrasonic_EXTI_Line				EXTI_Line4
#define Ultrasonic_EXTI_IRQn				EXTI4_IRQn
#define Ultrasonic_EXTI_IRQHandler	EXTI4_IRQHandler


void Ultrasonic_Init(void);
void Ultrasonic_Ranging(void);

extern uint16_t dis;

#endif




