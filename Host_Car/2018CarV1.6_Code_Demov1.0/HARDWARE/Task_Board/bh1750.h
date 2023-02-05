#ifndef __BH1750_H
#define __BH1750_H	 
#include "sys.h"

//IO操作函数	 
#define IIC_SCL    PBout(7) //SCL 
#define IIC_SDA    PBout(6)  //SDA输出	 
#define READ_SDA   PBin(6)  //SDA输入 
#define ADDR 	   PGout(15)


void BH1750_Configure(void);
uint16_t Get_Bh_Value(void);

#endif
