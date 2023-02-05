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
 函数名：DEBUG_LOG
 * 描述  ：格式化输出，类似于C库中的printf，但这里没有用到C库
 * 输入  ：
 *		     -Data   要发送到串口的内容的指针
 *			   -...    其他参数
 * 输出  ：无
 * 返回  ：无
				目前只支持字符串和10进制输出 多的也没必要
 *       和printf一个用法
					DEBUG_LOG("Card_Data:\n\r");
					DEBUG_LOG("%s",RXRFID\n);
					DEBUG_LOG("%d",RXRFID\r);
*/
//三个点为可变参数 链接 https://www.runoob.com/cprogramming/c-variable-arguments.html
void DEBUG_LOG(const char *Data, ...)
{
    const char *s;
    int d;
    char buf[16];

    va_list ap;
    va_start(ap, Data);

    while ( * Data != 0 )     // 判断是否到达字符串结束符
    {
        if ( * Data == 0x5c )  //'\'
        {
            switch ( *++Data )
            {
                case 'r':							          //回车符
                    Send_InfoData_To_Fifo((uint8_t*)&"r", 1);
                    Data ++;
                    break;

                case 'n':							          //换行符
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
                case 's':										  //字符串
                    s = va_arg(ap, const char *);

                    for ( ; *s; s++)
                    {
                        Send_InfoData_To_Fifo((uint8_t*)s++, 1);
                    }

                    Data++;
                    break;

                case 'd':
                    //十进制
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





