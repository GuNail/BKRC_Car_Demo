#include "Feng_huo.h"

void Feng_Huo_send(u8 *data)
{
    u8 B_data[8] = {0x55, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBB};
    B_data[2] = data[0];
    B_data[3] = data[1];
    B_data[4] = data[2];
    B_data[5] = data[3];
    _Checksum_(B_data);
    Send_ZigbeeData_To_Fifo(B_data, 8);
    delay_ms(300);
}

void Fen_Huo_Rewrite(void)
{
    u8 Rewrite[6] = {0x03, 0x05, 0x14, 0x45, 0xDE, 0x92};

    u8 send_data[4] = {0x00, 0x00, 0x00, 0x00};
    send_data[0] = 0x10;
    send_data[1] = Rewrite[0];
    send_data[2] = Rewrite[1];
    send_data[3] = Rewrite[2];
    Feng_Huo_send(send_data);
    delay_ms(300);
    send_data[0] = 0x11;
    send_data[1] = Rewrite[3];
    send_data[2] = Rewrite[4];
    send_data[3] = Rewrite[5];
    Feng_Huo_send(send_data);
}


void Feng_huo_Open(u8 *data)
{
    Infrared_Send(data, 6);
}
