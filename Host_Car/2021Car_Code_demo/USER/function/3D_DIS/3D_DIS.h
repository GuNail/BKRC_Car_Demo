#ifndef _3D_DIS_H_
#define _3D_DIS_H_

#include "infrared.h"
#include "delay.h"
#include "main.h"

typedef enum
{
    _3D_DIS_Graph_MODE_JuXin		= 0x01,
    _3D_DIS_Graph_MODE_Yuan,
    _3D_DIS_Graph_MODE_SanJiao,
    _3D_DIS_Graph_MODE_LenXin,
    _3D_DIS_Graph_MODE_TiXin,
    _3D_DIS_Graph_MODE_BinXin,
    _3D_DIS_Graph_MODE_BaXin,
    _3D_DIS_Graph_MODE_TiaoXin
} _3D_DIS_Graph_MODE;

typedef enum
{
    Red				= 0x01,
    Green,
    Blue,
    Yellow,
    purple,
    cyan,
    Black,
    Write
} _3D_DIS_Color_MODE;



void _3D_DIS_Send(u8 *Data);
void _3D_DIS_CarID(u8 *CarID);
void _3D_DIS_Distan(u8 *distan);
void _3D_DIS_Distan_num(uint16_t mm);

void _3D_DIS_Graph(_3D_DIS_Graph_MODE Graph);
void _3D_DIS_Color(uint8_t Color);
void _3D_DIS_Trffic_Warning(uint8_t mode);
void _3D_DIS_default(void);

void _3D_DIS_Trffic_Warning(uint8_t mode);
void _3D_DIS_Trffic_Tips(uint8_t mode);
void _3d_DIS_Text_Color(uint8_t hex_R, uint8_t hex_G, uint8_t hex_B);

#endif
