// 
// 
// 

#include "ExtSRAMInterface.h"
#include <Command.h>

_ExtSRAMInterface ExtSRAMInterface;

_ExtSRAMInterface::_ExtSRAMInterface()
{
}

_ExtSRAMInterface::~_ExtSRAMInterface()
{
}

/************************************************************************************************************
���� �� ������  Initialization		�ⲿ���߽ӿڳ�ʼ������
������˵������	��
���� �� ֵ����	��
����    ������	Initialization();	��ʼ���ⲿ���߽ӿ�
************************************************************************************************************/
void _ExtSRAMInterface::Initialization(void)
{
	XMCRA = 0x00;
	XMCRB = 0x00;
	XMCRA |= _BV(SRW10);
	XMCRA |= _BV(SRE);	/*��һλʹ���ⲿSRAM*/
}

/************************************************************************************************************
���� �� ������  ExMem_Write					���ⲿ����дһ���ֽ����ݺ���
������˵������	add	��						�ⲿ���ߵ�ַ
				dat	��						���ⲿ����д������
���� �� ֵ����	��
����    ������	ExMem_Write(0x6000,0x55);	���ⲿ���߽ӿڵ�ַΪ0x6000д0x55����
************************************************************************************************************/
void _ExtSRAMInterface::ExMem_Write(uint16_t add, uint8_t dat)
{
	rw_temp = add & 0x0ff;
	exm_p[ 0x3000 + rw_temp ] = rw_temp;
	exm_p[ add ] = dat;
}

void _ExtSRAMInterface::ExMem_JudgeWrite(uint16_t add, uint8_t dat)
{
	unsigned long t = millis();
	do
	{
		ExtSRAMInterface.ExMem_Write(add, dat);
	} while ((ExtSRAMInterface.ExMem_Read(add) != dat) && ((millis() - t) < 1000));
}
/************************************************************************************************************
���� �� ������  ExMem_Write_Btyes				�������ⲿ����д���ݺ���
������˵������	BaseAdd	��						���ⲿ����д���ݵĵ�һ����ַ
				Tbuf	��						���ⲿ����д���ݵ�����
				len		��						����ĳ���
���� �� ֵ����	��
����    ������	uint8_t Tbuf[8] = {0x00};
				ExMem_Write_Btyes(0x6000,Tbuf,8);	���ⲿ����д����Ϊ8����������
************************************************************************************************************/
void _ExtSRAMInterface::ExMem_Write_Bytes(uint8_t *Tbuf, uint8_t len)
{
	unsigned long t = millis();
	while ((ExMem_Read(WRITEADDRESS) != 0x00) && ((millis() - t) < 1000));
	for (int8_t i = len - 1; i >= 0; i--)
	{
		ExMem_JudgeWrite(WRITEADDRESS + i, Tbuf[ i ]);
	}
}

void _ExtSRAMInterface::ExMem_Write_Bytes(uint16_t BaseAdd, uint8_t *Tbuf, uint8_t len)
{
	unsigned long t = millis();
	while ((ExMem_Read(BaseAdd) != 0x00) && ((millis() - t) < 1000));
	for (int8_t i = len - 1; i >= 0; i--)
	{
		ExMem_JudgeWrite(BaseAdd + i, Tbuf[i]);
	}
}

/************************************************************************************************************
���� �� ������  ExMem_Read					���ⲿ���߶�һ���ֽ����ݺ���
������˵������	add	��						�ⲿ���߶���ַ
���� �� ֵ����	exm_p[ add ]				�����ⲿ���ߵ�ַΪadd������
����    ������	tp = ExMem_Read(0x6000);	��ȡ�ⲿ���߽ӿڵ�ַΪ0x6000������
************************************************************************************************************/
uint8_t _ExtSRAMInterface::ExMem_Read(uint16_t add)
{
	rw_temp = add & 0x0ff;
	//rw_temp = exm_p[ 0x3000 + rw_temp ];
	exm_p[ 0x3000 + rw_temp ] = rw_temp;
	return exm_p[add];
}

/************************************************************************************************************
���� �� ������  ExMem_Read_Bytes				�������ⲿ���߶�ȡ���ݺ���
������˵������	BaseAdd	��						���ⲿ���߶����ݵĵ�һ����ַ
				Tbuf	��						��ȡ�ⲿ����д���ݴ�ŵ�����
				len		��						����ĳ���
���� �� ֵ����	��
����    ������	uint8_t Rbuf[8] = {0x00};
				ExMem_Read_Bytes(0x6000,Rbuf,8);	���ⲿ������ʼ��ַΪ0x6000��ʼ��ȡ8�����ݣ������Rbuf��
************************************************************************************************************/
void _ExtSRAMInterface::ExMem_Read_Bytes(uint8_t *Rbuf, uint8_t len)
{
	unsigned long t = millis();
	while ((ExMem_Read(READADDRESS) == 0x00) && ((millis() - t) < 5000));
	//delay(10);
	for (size_t i = 0; i < len; i++)
	{
		Rbuf[i] = ExMem_Read(READADDRESS + i);
	}
	ExMem_JudgeWrite(READADDRESS, 0x00);
}

void _ExtSRAMInterface::ExMem_Read_Bytes(uint16_t BaseAdd, uint8_t *Rbuf, uint8_t len)
{
	unsigned long t = millis();
	while ((ExMem_Read(BaseAdd) == 0x00) && ((millis() - t) < 5000));
	for (size_t i = 0; i < len; i++)
	{
		Rbuf[ i ] = ExMem_Read(BaseAdd + i);
	}
	ExMem_JudgeWrite(BaseAdd, 0x00);
}


