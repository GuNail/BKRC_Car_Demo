#ifndef _MAIN_H_
#define _MAIN_H_
#include "stm32f4xx.h"
#include "delay.h"
#include "canp_hostcom.h"
#include "ultrasonic.h"
#include "Data_proce.h"
#include "DEBUG.h"
#include <stdlib.h>
#include "plan.h"
void TIMx_Configuration(void);

//��ע�ͼ����������
//#define SEG_open


//*******��������Ŷ���*******//
//������
#define INC_Prot											GPIOA
#define INC_Pin												GPIO_Pin_15
#define INT0_Prot											GPIOB
#define INT0_Pin											GPIO_Pin_4

#define INT0_EXTI_Line								EXTI_Line4
#define INT0_EXTI_Port								EXTI_PortSourceGPIOB
#define INT0_EXTI_Pin									EXTI_PinSource4
#define INT0_EXTI_IRQn								EXTI4_IRQn
#define INT0_EXTI_IRQHandler					EXTI4_IRQHandler

//BH1750
#define BH1750_SDA_Prot								GPIOB
#define BH1750_SDA_Pin								GPIO_Pin_7
#define BH1750_SCL_Prot								GPIOB
#define BH1750_SCL_Pin								GPIO_Pin_6
#define BH1750_ADDR_Prot							GPIOG
#define BH1750_ADDR_Pin								GPIO_Pin_15

#define IIC_SCL    										PBout(6) //SCL 
#define IIC_SDA    										PBout(7) //SDA���	 
#define READ_SDA   										PBin(7)  //SDA���� 
#define ADDR 	   											PGout(15)

//����
#define RI_TXD_RCC_Periph							RCC_AHB1Periph_GPIOF
#define RI_TXD_Prot										GPIOF
#define RI_TXD_Pin										GPIO_Pin_11
#define RI_TXD 												PFout(11)

//��������
#define	R_OUT_RCC_Periph							RCC_AHB1Periph_GPIOG
#define R_OUT_Prot										GPIOG
#define	R_OUT_Pin											GPIO_Pin_8

//ת���
#define L_LED_RCC_Periph							RCC_AHB1Periph_GPIOH
#define L_LED_Prot										GPIOH
#define L_LED_Pin											GPIO_Pin_10

#define R_LED_RCC_Periph							RCC_AHB1Periph_GPIOH
#define R_LED_Prot										GPIOH
#define R_LED_Pin											GPIO_Pin_11




#endif

