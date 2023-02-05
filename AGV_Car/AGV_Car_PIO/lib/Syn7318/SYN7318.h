// SYN7318.h

#ifndef _SYN7318_h
#define _SYN7318_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif


#define SYN7318_RST_ON			LOW			/*�͵�ƽ��ʹ��ģ�鸴λ*/
#define SYN7318_RST_OFF			HIGH		/*�ߵ�ƽ����ֹģ�鸴λ*/

#define SYN7318_FRAME_RH		0xFC
#define SYN7318_FRAME_SH		0xFD

#define RESET_PIN				PIN_A13

#define STOP_FLAG				0
#define SUSPEND_FLAG			1
#define RESTORE_FLAG			2
#define VOLUME_FLAG				3

class _SYN7318
{
public:
	_SYN7318();
	~_SYN7318();

	void Initialization(void);							/*��ʼ����ؽӿڼ�����*/
								
	void VSPTest(const char* text, uint8_t textcode);	/*�����ϳɲ�������*/
	void Start_ASR(uint8_t Dict);				/*����ʶ����Ժ���*/
	uint8_t Start_ASR_return(uint8_t Dict,bool whatID);
	void Start_ASR_send(uint8_t Dict);		/*��������ʶ��*/
	uint8_t Start_ASR_rec(bool whatID);		/*����ʶ����*/
	uint8_t Start_ASR_WEN(uint8_t Dict);
	
private:
	uint8_t REc_Flag;
	uint8_t Back_Flag;
	void ResetCom(void);
	bool ResetCheck(void);
	void ResetTest(void);								/*SYN7318��λ����*/
	void QueryStatus(void);					/*״̬��ѯ��ģ��æµ��λ*/
	bool CommandSendCheck(void);
	bool CommandSendCheckBusy(void);
	bool CommandSendCheckASR(void);
	bool CommandSendCheckACK(void);
	void VoiceSynthesisPlayback(const char* text,uint8_t textcode);
	void VSPCom(const char* text, uint8_t textcode);



	uint8_t respose_com[4] = { 0x00, 0x00, 0x00, 0x00 };
	uint8_t SSRId[5] = { 0x00, 0x00, 0x00, 0x00 ,0x00 };

};

extern _SYN7318 SYN7318;



#endif

