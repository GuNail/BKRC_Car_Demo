#include "Data_proce.h"
#include <string.h>

int p16(int n)//����16���Ƶ�n�η�
{
    int sum = 1, i;

    for (i = 0; i < n; i++)
        sum = sum * 16;

    return sum;
}
/****************************************************************
�������ܣ����ַ����16���ƣ�����a1���0xa1��
��	  ����*arr,Ҫ������ַ�,�������ABC123�������0xabc123
����  ֵ��16����
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
 * ��������itoa
 * ����  ������������ת�����ַ���
 * ����  ��-radix =10 ��ʾ10���ƣ��������Ϊ0
 *         -value Ҫת����������
 *         -buf ת������ַ���
 *         -radix = 10
 * ���  ����
 * ����  ����
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
        bit = num & 1;            //ȡ�����һλ
        new_num <<= 1;            //��������
        new_num = new_num | bit;  //�Ѹ�ȡ����һλ�ӵ�����
        num >>= 1;                //ԭ�����ƣ�׼��ȡ�ڶ�λ
    }

    return new_num;
}
/**
�����������ݵĶ�����0�ĸ���
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
����Ч���
*/
void _Checksum_(uint8_t *srcdata)
{
    srcdata[6] = (srcdata[2] + srcdata[3] + srcdata[4] + srcdata[5]) % 256;
}


