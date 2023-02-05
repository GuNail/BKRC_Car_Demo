#ifndef _LED_DIS_HPP_
#define _LED_DIS_HPP_

#include <ExtSRAMInterface.h> 
#include <Command.h>
#include <stdio.h>
#include <Ultrasonic.h>

class LED_DIS
{
private:
    void Send_TO_LED(uint8_t first,uint8_t commed1,uint8_t commed2,uint8_t commed3);
public:
    LED_DIS(/* args */){}
    ~LED_DIS(){}
    void Display_data_1(uint8_t data_1_2,uint8_t data_3_4,uint8_t data_5_6); 
    void Display_data_1(String str);
    void Display_data_1(uint32_t num);
    void Display_data_2(uint8_t data_1_2,uint8_t data_3_4,uint8_t data_5_6);
    void Display_data_2(String str);
    void Display_data_2(uint32_t num);
    void Start_Time(void);
    void Stop_Time(void);
    void Clean_Time(void);
    void Display_distance(uint16_t distance);
    void Ultrasonic_Ranging();
};

//*****LED*******/
inline void LED_DIS::Send_TO_LED(uint8_t first,uint8_t commed1,uint8_t commed2,uint8_t commed3)
{
    uint8_t data[]={0x55,0x04,first,commed1,commed2,commed3,0x00,0xBB};
    Command.Judgment(data);
    ExtSRAMInterface.ExMem_Write_Bytes(0x6008,data,8);
    delay(300);
    ExtSRAMInterface.ExMem_Write_Bytes(0x6008,data,8);
    delay(300);
}

void LED_DIS::Display_data_1(uint8_t data_1_2,uint8_t data_3_4,uint8_t data_5_6)
{
    Send_TO_LED(0x01,data_1_2,data_3_4,data_5_6);
}

void LED_DIS::Display_data_1(String str)
{
    String f= str.substring(0,2);
    String s= str.substring(2,4);
    String t= str.substring(4,6);
    int num[3];
    const char *f1=f.c_str();
    const char *s1=s.c_str();
    const char *t1=t.c_str();
    sscanf(f1,"%x",&num[0]);
    sscanf(s1,"%x",&num[1]);
    sscanf(t1,"%x",&num[2]);

    Send_TO_LED(0x01,num[0],num[1],num[2]);
}

void LED_DIS::Display_data_1(uint32_t num)
{
    Send_TO_LED(0x01,num>>16,num>>8,num&0xff);
}

void LED_DIS::Display_data_2(String str)
{
    String f= str.substring(0,2);
    String s= str.substring(2,4);
    String t= str.substring(4,6);
    int num[3];
    const char *f1=f.c_str();
    const char *s1=s.c_str();
    const char *t1=t.c_str();
    sscanf(f1,"%x",&num[0]);
    sscanf(s1,"%x",&num[1]);
    sscanf(t1,"%x",&num[2]);

    Send_TO_LED(0x02,num[0],num[1],num[2]);
}

void LED_DIS::Display_data_2(uint8_t data_1_2,uint8_t data_3_4,uint8_t data_5_6)
{
    Send_TO_LED(0x02,data_1_2,data_3_4,data_5_6);
}

void LED_DIS::Display_data_2(uint32_t num)
{
    Send_TO_LED(0x02,num>>16,num>>8,num&0xff);
}

void LED_DIS::Start_Time(void)
{
    Send_TO_LED(0x03,0x01,0x00,0x00);
}

void LED_DIS::Stop_Time(void)
{
    Send_TO_LED(0x03,0x00,0x00,0x00);
}

void LED_DIS::Clean_Time(void)
{
    Send_TO_LED(0x03,0x02,0x00,0x00);  
}

void LED_DIS::Display_distance(uint16_t distance)
{
    char s[10];
    sprintf(s,"0x%d",distance);
    int num;
    sscanf(s,"%x",&num);
    Send_TO_LED(0x04,0x00,num>>8,num&0xFF);
}

void LED_DIS::Ultrasonic_Ranging()
{
    uint16_t Distance=0;
    Distance=(uint16_t)Ultrasonic.Ranging(1);
    Display_distance(Distance);
}



#endif
