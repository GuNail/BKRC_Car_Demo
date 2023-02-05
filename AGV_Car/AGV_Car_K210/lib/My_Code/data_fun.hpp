#ifndef _DATA_FUN_HPP_
#define _DATA_FUN_HPP_

#include <Arduino.h>

//******算法处理

int Char_To_int(const char* arr);











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

#endif
