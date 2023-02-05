#include "Data_proce.h"
#include <string.h>

int p16(int n)//计算16进制的n次方
{
    int sum = 1, i;

    for (i = 0; i < n; i++)
        sum = sum * 16;

    return sum;
}
/****************************************************************
函数功能：把字符变成16进制，例如a1变成0xa1；
参	  数：*arr,要传入的字符,如果输入ABC123，则输出0xabc123
返回  值：16进制
*****************************************************************/
int Char_To_int(const char* arr)
{
    int a = 0, x = 0, str = 0, len = strlen(arr);

    for (int i = len - 1; i >= 0; i--)
    {
        if(arr[i] >= '0' && arr[i] <= '9')
        {
            a = arr[i] - 0x30;
        }
        else if (arr[i] >= 'a' && arr[i] <= 'f')
        {
            a = arr[i] - 'a' + 10;
        }
        else if (arr[i] >= 'A' && arr[i] <= 'F')
        {
            a = arr[i] - 'A' + 10;
        }

        str = str + a * p16(x);
        x++;
    }

    return str;
}


/*
 * 函数名：itoa
 * 描述  ：将整形数据转换成字符串
 * 输入  ：-radix =10 表示10进制，其他结果为0
 *         -value 要转换的整形数
 *         -buf 转换后的字符串
 *         -radix = 10
 * 输出  ：无
 * 返回  ：无
 */
char * itoa( int value, char *string, int radix )
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;

    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */


uint8_t reverse_bit(uint8_t num)
{
    uint8_t i;
    uint8_t bit;
    uint8_t new_num = 0;

    for (i = 0; i < 8; i++)
    {
        bit = num & 1;            //取出最后一位
        new_num <<= 1;            //新数左移
        new_num = new_num | bit;  //把刚取出的一位加到新数
        num >>= 1;                //原数右移，准备取第二位
    }

    return new_num;
}
/**
计算输入数据的二进制0的个数
*/
int BitCount(unsigned int n)
{
    unsigned int c = 0 ;

    while (n)
    {
        if (n & 1)
        {
            c++;
        }

        n = n >> 1;
    }

    return 16 - c ;
}


/*
计算效验和
*/
void _Checksum_(uint8_t *srcdata)
{
    srcdata[6] = (srcdata[2] + srcdata[3] + srcdata[4] + srcdata[5]) % 256;
}


