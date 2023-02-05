#ifndef __SYN7318_H
#define __SYN7318_H
#include "stm32f4xx.h"
#include "sys.h"

#define SYN7318RESET PBout(5)

typedef enum
{
	Voice_1	=	0x01,
	Voice_2	=	0x02,
	Voice_3	=	0x03,
	Voice_4	=	0x04,
	Voice_5	=	0x05,
	Voice_6	=	0x06
}_SYN_VoiceType;


void SYN7318_Init(void);
void SYN_TTS(uint8_t *Pst);
void SYN7318_Test( void);					// 开启语音测试
void SYN_7318_One_test(uint8_t mode,uint8_t num);		// mode 模式测试 1 随机指令 0 指定指令
void Yu_Yin_Asr(void);						// 语音识别处理函数
void SYN_Voice(u8 Mode,_SYN_VoiceType Voice);
	
	
#endif

