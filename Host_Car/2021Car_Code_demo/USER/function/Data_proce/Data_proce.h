#ifndef _DATA_PROCE_H_
#define _DATA_PROCE_H_
#include "stm32f4xx.h"

char * itoa( int value, char *string, int radix );

uint8_t reverse_bit(uint8_t num);

int BitCount(unsigned int n);
int p16(int n);
int data_Handler(const char* arr);
int Char_To_int(const char* arr);

void _Checksum_(uint8_t *srcdata);

#endif
