#ifndef _TFT_H_
#define _TFT_H
#include "main.h"

typedef enum
{
    Photo_1			= 1,
    Photo_2, Photo_3, Photo_4, Photo_5,
    Photo_6, Photo_7, Photo_8, Photo_9,
    Photo_10, Photo_11, Photo_12, Photo_13,
    Photo_14, Photo_15, Photo_16, Photo_17,
    Photo_18, Photo_19, Photo_20
} _TFT_Photo_typedef;

typedef enum
{
    Straight_Mode = 0x01,
    Left_Mode,
    Right_Mode,
    Turn_around_Mode,
    No_Straight,
    Do_Not_Enter_Mode

} _TFT_Trffic_Typedef;


void TFT_Send_A(u8 *Data);
void TFT_Choose_Photo_A(_TFT_Photo_typedef Photo);
void TFT_Next_Photo_A(void);
void TFT_Last_Photo_A(void);
void TFT_Next_Photo_aout_A(void);
void TFT_CarID_A(u8 *CarID);
void TFT_TimeStop_A(void);
void TFT_TimeStart_A(void);
void TFT_TimeClean_A(void);
void TFT_Display_HEX_A(u8 *Hex);
void TFT_Distan_A(u8 *Distan);
void TFT_Distan_num_A(uint16_t data);
void TFT_Trffic_Mode_A(_TFT_Trffic_Typedef mode);
void TFT_ultrasonic_A(void);

void TFT_Send_B(u8 *Data);
void TFT_Choose_Photo_B(_TFT_Photo_typedef Photo);
void TFT_Next_Photo_B(void);
void TFT_Last_Photo_B(void);
void TFT_Next_Photo_Bout_B(void);
void TFT_CarID_B(u8 *CarID);
void TFT_TimeStop_B(void);
void TFT_TimeStart_B(void);
void TFT_TimeClean_B(void);
void TFT_Display_HEX_B(u8 *Hex);
void TFT_Distan_B(u8 *Distan);
void TFT_Distan_num_B(uint16_t data);
void TFT_Trffic_Mode_B(_TFT_Trffic_Typedef mode);
void TFT_ultrasonic_B(void);


#endif


