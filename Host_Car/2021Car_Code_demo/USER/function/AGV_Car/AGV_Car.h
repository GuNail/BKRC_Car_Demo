#ifndef _AGV_CAR_H_
#define _AGV_CAR_H_
#include "canp_hostcom.h"
#include "can_user.h"
#include "delay.h"

void Send_To_AGV_Car(uint8_t first, uint8_t commd1, uint8_t commd2, uint8_t commd3);

void AGV_Car_Zigbee(void);

void AGV_Car_Start(void);
void AGV_Car_Start_Rode(void);
void AGV_Car_Rode1(void);
void AGV_Car_Rode2(void);
void AGV_Car_Rode3(void);

#endif
