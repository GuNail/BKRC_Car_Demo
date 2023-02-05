#ifndef _LI_TI_HPP_
#define _LI_TI_HPP_
#include <Infrare.h>
#include <Ultrasonic.h>

class LI_Ti
{
private:
    void Send_TO_LiTi(uint8_t *data)
    {
        Infrare.Transmition(data,6);
        delay(100);
        Infrare.Transmition(data,6);
        delay(100);
    }
public:
    LI_Ti(/* args */){}
    ~LI_Ti(){}
    void Dis_CarID(String str,uint8_t x,uint8_t y);
    void Dis_CarID(uint8_t *data,uint8_t x,uint8_t y);
    void Dis_CarID(uint8_t f1,uint8_t f2,uint8_t f3,uint8_t f4,uint8_t f5,uint8_t f6,uint8_t x,uint8_t y);
    void Dis_distan(uint16_t distan,uint8_t x,uint8_t y);
    void Dis_Tx(uint8_t Tx);
    void Dis_Color(uint8_t Color);
    void Dis_Trffic_Warning(uint8_t Trffic_Warning);
    void Dis_Trffic_Flag(uint8_t Trffic_Fiag);
    void Dis_default();
    void Set_Text_Color(uint8_t R,uint8_t G,uint8_t B);
    void Ultrasonic_Ranging();
};

//显示车牌，str：车牌字符串，x,y,坐标
void LI_Ti::Dis_CarID(String str,uint8_t x=0,uint8_t y=0)
{
    uint8_t data[]={0xFF,0x20,str.charAt(0),str.charAt(1),str.charAt(2),str.charAt(3)};
    uint8_t data1[]={0xFF,0x10,str.charAt(4),str.charAt(5),x,y};
    Send_TO_LiTi(data);
    Send_TO_LiTi(data1);
}

//显示车牌，data：车牌字符串，x,y,坐标
void LI_Ti::Dis_CarID(uint8_t *data,uint8_t x=0,uint8_t y=0)
{
    uint8_t f1[]={0xFF,0x20,data[0],data[1],data[2],data[3]};
    uint8_t h1[]={0xFF,0x10,data[4],data[5],x,y};
    Send_TO_LiTi(f1);
    Send_TO_LiTi(h1);
}

//显示车牌，f1-f6：车牌字符，x,y,坐标
void LI_Ti::Dis_CarID(uint8_t f1,uint8_t f2,uint8_t f3,uint8_t f4,uint8_t f5,uint8_t f6,uint8_t x=0,uint8_t y=0)
{
    uint8_t Q1[]={0xFF,0x20,f1,f2,f3,f4};
    uint8_t h1[]={0xFF,0x10,f5,f6,x,y};
    Send_TO_LiTi(Q1);
    Send_TO_LiTi(h1);
}

//显示距离 CM
void LI_Ti::Dis_distan(uint16_t distan,uint8_t x=0,uint8_t y=0)
{
    uint8_t data[]={0xFF,0x11,0x00,0x00,0x00,0x00};
    data[2]=(distan/10)+0x30;
    data[3]=(distan%10)+0x30;
    Send_TO_LiTi(data);
}

/*
显示图形
0x01 矩形
0x02 圆形
0x03 三角形
0x04 菱形
0x05 五角星
*/
void LI_Ti::Dis_Tx(uint8_t Tx)
{
    uint8_t data[]={0xFF,0x12,Tx,0x00,0x00,0x00};
    Send_TO_LiTi(data);
}

/*
显示颜色
0x01 红色(255,0,0)
0x02 绿色(0,255,0)
0x03 蓝色(0,0,255)
0x04 黄色(255,255,0)
0x05 品色(255,0,255)
0x06 青色(0,255,255)
0x07 黑色(0,0,0)
0x08 白色(255,255,255)
*/
void LI_Ti::Dis_Color(uint8_t Color)
{
    uint8_t data[]={0xFF,0x13,Color,0x00,0x00,0x00};

}

/*
交通警示牌信息显示模式
0x01 前方学校 减速慢行
0x02 前方施工 禁止通行
0x03 塌方路段 注意安全
0x04 追尾危险 保持车距
0x05 严禁 酒后驾车！
0x06 严禁 乱扔垃圾！
*/
void LI_Ti::Dis_Trffic_Warning(uint8_t Trffic_Warning)
{
    uint8_t data[]={0xFF,0x14,Trffic_Warning,0,0,0};
    Send_TO_LiTi(data);
}

/*
交通标志信息显示模式
0x01 直行
0x02 左转
0x03 右转
0x04 掉头
0x05 禁止直行
0x06 禁止通行
*/
void LI_Ti::Dis_Trffic_Flag(uint8_t Trffic_Fiag)
{
    uint8_t data[]={0xFF,0x15,Trffic_Fiag,0,0,0};
    Send_TO_LiTi(data);
}

/*
显示默认信息
*/
void LI_Ti::Dis_default()
{
    uint8_t data[]={0xFF,0x16,0x01,0,0,0};
    Send_TO_LiTi(data);
}

//设置文字颜色（RGB 格式）
void LI_Ti::Set_Text_Color(uint8_t R,uint8_t G,uint8_t B)
{
    uint8_t data[]={0xFF,0x17,0x01,R,G,B};
    Send_TO_LiTi(data);
}

//超声波并显示
void LI_Ti::Ultrasonic_Ranging()
{
    uint16_t Distance=Ultrasonic.Ranging(1);
    Dis_distan(Distance);
}













#endif