#include "DEBUG.h"
#include <stdlib.h>
#include <stdarg.h>
#include "Data_proce.h"
#include <stdio.h>

int fputc(int ch, FILE *F)
{
    Send_InfoData_To_Fifo((uint8_t*)&ch, 1);
    return ch;
}

/**
 ��������DEBUG_LOG
 * ����  ����ʽ�������������C���е�printf��������û���õ�C��
 * ����  ��
 *		     -Data   Ҫ���͵����ڵ����ݵ�ָ��
 *			   -...    ��������
 * ���  ����
 * ����  ����
				Ŀǰֻ֧���ַ�����10������� ���Ҳû��Ҫ
 *       ��printfһ���÷�
					DEBUG_LOG("Card_Data:\n\r");
					DEBUG_LOG("%s",RXRFID\n);
					DEBUG_LOG("%d",RXRFID\r);
*/
//������Ϊ�ɱ���� ���� https://www.runoob.com/cprogramming/c-variable-arguments.html
void DEBUG_LOG(const char *Data, ...)
{
    const char *s;
    int d;
    char buf[16];

    va_list ap;
    va_start(ap, Data);

    while ( * Data != 0 )     // �ж��Ƿ񵽴��ַ���������
    {
        if ( * Data == 0x5c )  //'\'
        {
            switch ( *++Data )
            {
                case 'r':							          //�س���
                    Send_InfoData_To_Fifo((uint8_t*)&"r", 1);
                    Data ++;
                    break;

                case 'n':							          //���з�
                    Send_InfoData_To_Fifo((uint8_t*)&"n", 1);
                    Data ++;
                    break;

                default:
                    Data ++;
                    break;
            }
        }
        else if ( * Data == '%')
        {
            //
            switch ( *++Data )
            {
                case 's':										  //�ַ���
                    s = va_arg(ap, const char *);

                    for ( ; *s; s++)
                    {
                        Send_InfoData_To_Fifo((uint8_t*)s++, 1);
                    }

                    Data++;
                    break;

                case 'd':
                    //ʮ����
                    d = va_arg(ap, int);
                    itoa(d, buf, 10);

                    for (s = buf; *s; s++)
                    {
                        Send_InfoData_To_Fifo((uint8_t*)s++, 1);
                    }

                    Data++;
                    break;

                default:
                    Data++;
                    break;
            }
        }
        else Send_InfoData_To_Fifo((uint8_t*)Data++, 1);
    }
}





