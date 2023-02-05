#ifndef _TFT_HPP_
#define _TFT_HPP_
#include <ExtSRAMInterface.h>
#include <Command.h>
class TFT
{
private:
    uint8_t Type;
    void Send_TO_TFT(uint8_t first,uint8_t commd1,uint8_t commd2,uint8_t commd3)
    {
        uint8_t data[]={0x55,Type,first,commd1,commd2,commd3,0x00,0xBB};
        Command.Judgment(data);
        ExtSRAMInterface.ExMem_Write_Bytes(0x6008,data,8);
        delay(100);
        ExtSRAMInterface.ExMem_Write_Bytes(0x6008,data,8);
        delay(100);
        ExtSRAMInterface.ExMem_Write_Bytes(0x6008,data,8);
        delay(100);
    }
public:
    TFT(uint8_t AorB){
        Type=AorB;
    }
    ~TFT(){}
    void photo(uint8_t num);
    void photo();
    void photo_last();
    void photo_next();
    void CarID();
    void CarID(uint8_t *data);
    void CarID(String CarID);
    void CarID(uint8_t f1,uint8_t f2,uint8_t f3,uint8_t h1,uint8_t h2,uint8_t h3);
    void Start_Time();
    void Stop_Time();
    void Clean_Time();
    void Hex_Display(String str);
    void Hex_Display(uint8_t *data);
    void Hex_Display(uint8_t f1,uint8_t f2,uint8_t f3);
    void distan_dis(uint16_t distance);
    void Trffic_dis(uint8_t flag);

};

//指定图片0x01-0x20
void TFT::photo(uint8_t num)
{
    Send_TO_TFT(0x10,0x00,num,0x00);
}

//自动切换
void TFT::photo()
{
    Send_TO_TFT(0x10,0x03,0x00,0x00);
}

//上翻页
void TFT::photo_last()
{
    Send_TO_TFT(0x10,0x01,0x00,0x00);
}

//下翻页
void TFT::photo_next()
{
    Send_TO_TFT(0x10,0x02,0x00,0x00);
}

//测试车牌
void TFT::CarID()
{
    Send_TO_TFT(0x20,'A','1','B');
    Send_TO_TFT(0x21,'2','C','3');
}

//车牌显示 参数 数组
void TFT::CarID(uint8_t *data)
{
    Send_TO_TFT(0x20,data[0],data[1],data[2]);
    Send_TO_TFT(0x21,data[3],data[4],data[5]);
}

//车牌显示 参数 字符
void TFT::CarID(uint8_t f1,uint8_t f2,uint8_t f3,uint8_t h1,uint8_t h2,uint8_t h3)
{
    Send_TO_TFT(0x20,f1,f2,f3);
    Send_TO_TFT(0x21,h1,h2,h3);
}

//车牌显示 参数 字符串
void TFT::CarID(String CarID)
{
    const char *data=CarID.c_str();
    Send_TO_TFT(0x20,data[0],data[1],data[2]);
    Send_TO_TFT(0x21,data[3],data[4],data[5]);
}

//TFT 开始计时
void TFT::Start_Time()
{
    Send_TO_TFT(0x30,0x01,0x00,0x00);
}

//TFT 停止计时
void TFT::Stop_Time()
{
    Send_TO_TFT(0x30,0x00,0x00,0x00);
}

//TFT 计时清零
void TFT::Clean_Time()
{
    Send_TO_TFT(0x30,0x02,0x00,0x00);
}

//HEX 显示模式
void TFT::Hex_Display(String str)
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

    Send_TO_TFT(0x40,num[0],num[1],num[2]);
}

//HEX 显示模式
void TFT::Hex_Display(uint8_t *data)
{
    Send_TO_TFT(0x40,data[0],data[1],data[2]);
}

//HEX 显示模式
void TFT::Hex_Display(uint8_t f1,uint8_t f2,uint8_t f3)
{
    Send_TO_TFT(0x40,f1,f2,f3);
}

//距离显示模式
void TFT::distan_dis(uint16_t distance)
{
    char s[10];
    sprintf(s,"0x%d",distance);
    int num;
    sscanf(s,"%x",&num);
    Send_TO_TFT(0x50,0x00,num>>8,num&0xFF);
}

/*
交通标志显示模式
0x01 显示交通标志“直行”
0x02 显示交通标志“左转”
0x03 显示交通标志“右转”
0x04 显示交通标志“掉头”
0x05 显示交通标志“禁止直行”
0x06 显示交通标志“禁止通行”
*/
void TFT::Trffic_dis(uint8_t flag)
{
    Send_TO_TFT(0x60,flag,0x00,0x00);
}







#endif