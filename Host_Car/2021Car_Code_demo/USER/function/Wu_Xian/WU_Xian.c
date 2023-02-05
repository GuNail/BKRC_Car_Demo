#include "Wu_Xian.h"

void Wu_Xian_Open(void)
{
    u8 data[8] = {0x55, 0x0A, 0x01, 0x01, 0x00, 0x00, 0x02, 0xBB};
    Send_ZigbeeData_To_Fifo(data, 8);
    delay_ms(100);
    Send_ZigbeeData_To_Fifo(data, 8);
    delay_ms(100);
    Send_ZigbeeData_To_Fifo(data, 8);
}


