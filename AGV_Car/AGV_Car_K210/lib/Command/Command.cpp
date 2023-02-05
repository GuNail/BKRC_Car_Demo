
#include "Command.h"

_Command Command;

_Command::_Command()
{
}

_Command::~_Command()
{
}

/************************************************************************************************************
���� �� ������  Judgment		����У�麯��
������˵������	command	��		Ҫ���͵�����
���� �� ֵ����	��
����    ������	Judgment(command01);	У��command01����
************************************************************************************************************/
void _Command::Judgment(uint8_t *command)
{
	uint16_t tp = command[ 2 ] + command[ 3 ] + command[ 4 ] + command[ 5 ];
	command[ 6 ] = uint8_t(tp % 256);
}


