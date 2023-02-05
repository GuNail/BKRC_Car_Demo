#ifndef _VOICE_PLAYER_HPP_
#define _VOICE_PLAYER_HPP_

#include <ExtSRAMInterface.h>
#include <Command.h>
#include <SYN7318.h>

class voice_Player
{
    private:
         uint8_t Read_Data[8];
        void Send_To_voice_Player(uint8_t first,uint8_t commd1,uint8_t commd2,uint8_t commd3);
        void Read_To_voice_player();
        void TTS(String str);
        void TTS(uint8_t *data);
    public:
        voice_Player(){}
        ~voice_Player(){}
        void report();
        void report(uint8_t i);
        //void report(uint8_t *data);
        void report(String str);
        void Set_RTC_Date(uint8_t year,uint8_t month,uint8_t day);
        void Set_RTC_Time(uint8_t hour,uint8_t min,uint8_t sec);
        void Speech_Disc();
        String Get_RTC_Date(void);
        String Get_RTC_Time(void);
};

void voice_Player::Send_To_voice_Player(uint8_t first,uint8_t commd1,uint8_t commd2,uint8_t commd3)
{
    uint8_t data[8]={0x55,0x06,first,commd1,commd2,commd3,0x00,0xBB};
    Command.Judgment(data);
    ExtSRAMInterface.ExMem_Write_Bytes(0x6008,data,8);
    delay(50);
    ExtSRAMInterface.ExMem_Write_Bytes(0x6008,data,8);
    delay(50);
    ExtSRAMInterface.ExMem_Write_Bytes(0x6008,data,8);
}

void voice_Player::Read_To_voice_player()
{
    ExtSRAMInterface.ExMem_Read_Bytes(0x6100,Read_Data,8);
}

void voice_Player::TTS(String str)
{
    uint8_t len=str.length()+2;
    uint8_t data[5]={0xFD,0x00,0x00,0x01,0x00};
    data[1]=len>>8;
    data[2]=len&0xFF;
    const char *readly_str=str.c_str();
    ExtSRAMInterface.ExMem_Write_Bytes(0x6180,data,5);
    ExtSRAMInterface.ExMem_Write_Bytes(0x6180+5,(uint8_t*)readly_str,len);
}

void voice_Player::TTS(uint8_t *str)
{
    uint8_t len=strlen((char*)str)+2;
    uint8_t data[5]={0xFD,(uint8_t)len>>8,(uint8_t)len&0xFF,0x01,0x00};
    ExtSRAMInterface.ExMem_Write_Bytes(0x6180,str,5);
    ExtSRAMInterface.ExMem_Write_Bytes(0x6180+5,str,len);
}

void voice_Player::report()
{
    Send_To_voice_Player(0x20,0x01,0x00,0x00);
}

void voice_Player::report(uint8_t i)
{
    Send_To_voice_Player(0x10,i,0x00,0x00);
}

void voice_Player::report(String str)
{
    TTS(str);
}

// void voice_Player::report(uint8_t *data)
// {
//      TTS(data);
// }

void voice_Player::Set_RTC_Date(uint8_t year,uint8_t month,uint8_t day)
{
    Send_To_voice_Player(0x30,year,month,day);
}

void voice_Player::Set_RTC_Time(uint8_t hour,uint8_t min,uint8_t sec)
{
    Send_To_voice_Player(0x40,hour,min,sec);
}

String voice_Player::Get_RTC_Date()
{
    for(int i=0;i<3;i++)
    {
        Send_To_voice_Player(0x31,0x01,0x00,0x00);
        delay(500);
        Read_To_voice_player();
        delay(500);
    }
    if(Read_Data[0]==0x55 && Read_Data[1]==0x06 && Read_Data[2]==0x02)
    {
        String Date=""+(Read_Data[3]<<16|Read_Data[4]<<8|Read_Data[5]);
        return Date;
    }else{
        return "";
    }  
}

String voice_Player::Get_RTC_Time()
{
     for(int i=0;i<3;i++)
    {
        Send_To_voice_Player(0x41,0x01,0x00,0x00);
        delay(500);
        Read_To_voice_player();
        delay(500);
    }
    if(Read_Data[0]==0x55 && Read_Data[1]==0x06 && Read_Data[2]==0x03)
    {
        String Date=""+(Read_Data[3]<<16|Read_Data[4]<<8|Read_Data[5]);
        return Date;
    }else{
        return "";
    }
}

void voice_Player::Speech_Disc()
{
    uint8_t trm_buf[] = {0xAF,0x06,0x00,0x02,0x00,0x00,0x01,0xBB};
    uint8_t sph_id = 0;
    SYN7318.Start_ASR_send(4);     //语音词条04
    delay(200);
    ExtSRAMInterface.ExMem_Write_Bytes(0x6008, Command.command33, 8);
    sph_id = SYN7318.Start_ASR_rec(true);          //获取识别结果 
    switch(sph_id)
    {
        case 0x02:
            SYN7318.VSPTest(str[0],1); 
            break;
        case 0x03:
            SYN7318.VSPTest(str[1],1); 
            break;
        case 0x04:
            SYN7318.VSPTest(str[2],1); 
            break;
        case 0x05:
            SYN7318.VSPTest(str[3],1); 
            break;
        case 0x06:
            SYN7318.VSPTest(str[4],1); 
            break;
        default:
    //     SYN7318.VSPTest(str[10],1);
            break;
    }
    trm_buf[2] = sph_id;
    ExtSRAMInterface.ExMem_Write_Bytes(0x6008, trm_buf, 8);
}







#endif
