// 
// 
// 

#include "infrare.h"

_Infrare Infrare;

_Infrare::_Infrare(uint8_t _pin)
{
	pinMode(_pin, OUTPUT);
	pin = _pin;
	digitalWrite(_pin, TXD_OFF);
}
_Infrare::_Infrare()
{

}

_Infrare::~_Infrare()
{
}

/***************************************************************
** ���ܣ�	���ⷢ��˿ڳ�ʼ��
** ������	�޲���
** ����ֵ��	��
****************************************************************/
void _Infrare::Initialization(void)
{
	pinMode(RITX_PIN, OUTPUT);
	digitalWrite(RITX_PIN, TXD_OFF);
	//pin = _pin;

}

/***************************************************************
** ���ܣ�	���ⷢ���ӳ���
** ������	*s��ָ��Ҫ���͵�����
**          *n�����ݳ���
** ����ֵ��	��
****************************************************************/
void _Infrare::Transmition(uint8_t *s, int n)
{
	u8 i, j, temp;

	digitalWrite(RITX_PIN, TXD_ON);
	delayMicroseconds(9000);
	digitalWrite(RITX_PIN, TXD_OFF);
	delayMicroseconds(4560);
	
	for (i = 0; i < n; i++)
	{
		for (j = 0; j<8; j++)
		{
			temp = (s[ i ] >> j) & 0x01;
			if (temp == 0)						//����0
			{
				digitalWrite(RITX_PIN, TXD_ON);
				delayMicroseconds(500);			//��ʱ0.5ms
				digitalWrite(RITX_PIN,TXD_OFF);
				delayMicroseconds(500);			//��ʱ0.5ms
			}
			if (temp == 1)						//����1
			{
				digitalWrite(RITX_PIN, TXD_ON);
				delayMicroseconds(500);			//��ʱ0.5ms
				digitalWrite(RITX_PIN, TXD_OFF);
				delayMicroseconds(1000);
				delayMicroseconds(800);			//��ʱ1.69ms  690

			}
		}
	}
	digitalWrite(RITX_PIN, TXD_ON);				//����
	delayMicroseconds(560);						//��ʱ0.56ms
	digitalWrite(RITX_PIN, TXD_OFF);			//�رպ��ⷢ��
}



