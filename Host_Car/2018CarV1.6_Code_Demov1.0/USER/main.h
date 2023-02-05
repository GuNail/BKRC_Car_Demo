#ifndef _MAIN_H_
#define _MAIN_H_

//数码管启用 0不启用，1启用
#define SEG_ENABLE 1 


#include <stdio.h>
#include "stm32f4xx.h"
#include "delay.h"
#include "infrared.h"
#include "cba.h"
#include "ultrasonic.h"
#include "canp_hostcom.h"
#include "hard_can.h"
#include "bh1750.h"
#include "syn7318.h"
#include "power_check.h"
#include "can_user.h"
#include "data_base.h"
#include "roadway_check.h"
#include "tba.h"
#include "data_base.h"
#include "swopt_drv.h"
#include "uart_a72.h"
#include "Can_check.h"
#include "delay.h"
#include "can_user.h"
#include "Timer.h"
#include "Full.h"
#include "math.h"
#include "stdio.h"
#include "Rc522.h"
#include "my_lib.h"
#include "string.h"
#include "SEG.h"
#include "planning.h"

void Hardware_Init(void);


#endif


