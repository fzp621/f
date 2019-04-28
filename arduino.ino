/*********************

一号超声波传感器 trigPIN:3
              echoPIn:4

二号超声波传感器 trigPIN:5
              echoPIn:7

三号超声波传感器 trigPIN:6
              echoPIn:8             

四号超声波传感器 trigPIN:10
              echoPIn:12      

人体红外传感器   out: 9  单次检测模式


光强传感器 :   SDL:A4  SCL:A5  ps:300-700之间最适合阅读
 
*********************/
#include "DistanceMessage.h"
#include "Light.h"
#include <Wire.h>
#include <math.h>



//配置三个超声波传感器
DistanceMessage dic1(0,3,4);  //超声波传感器id 控制口 接收口
DistanceMessage dic2(1,5,7);
DistanceMessage dic3(2,6,8);
DistanceMessage dic4(3,10,12);

//实例光强传感器
Light light;



//人体红外传感器初始化为无人，输出低电平
int takedSingal = 0;
//坐姿是否正确标志
int isSitshCorrect = 0;   

//led
int led = A2;
//接收到的光感度
int lightVal;
//初始化pwm
int ledPwm=50;

static int isTurnOn = 0;


//报警灯
int warningLed = A3;


//报文
String Mes;

//头+ ID + 消息 + 尾
String head = "0x";   
String id_1 = "0";     //两位id
String id_2 = "1";
String mes_1 = "0";    //总报警位，一号报警位，二号报警位，三号报警位
String mes_2 = "0";
String mes_3 = "0";
String mes_4 = "0";
String tail = "x0";


void setup() {
  
  //串口初始化
  Serial.begin (9600);
  
  //红外线传感器配置
  pinMode(9,INPUT);
  
  //IIC初始化
  Wire.begin();

  //初始化led
  pinMode(led,OUTPUT);

  //初始化报警led
  pinMode(warningLed,OUTPUT); 

 
}

void loop() {

  //判断是否有人使用
  takedSingal = isTaked();
  takedSingal = 1; //测试
  if(takedSingal == 1)
  {
    /***************
     * 超声波数据显示
     *************/
    //设定超声波控制引脚
    dic1.trigPinSet();
    dic1.distanceGet();
    //dic1.serialShow();
   
    
    dic2.trigPinSet();
    dic2.distanceGet();
    //dic2.serialShow();
 
    
    dic3.trigPinSet();
    dic3.distanceGet();
    //dic3.serialShow();

    


    dic4.trigPinSet();
    dic4.distanceGet();
    //dic4.serialShow();


    handleData(dic1,dic2,dic3,dic4);   //坐姿判断
    //报文封装
    Mes = head + id_1 + id_2 + mes_1 + mes_2 + mes_3 + mes_4 + tail;
    //发送报文
    Serial.println(Mes);

    if(mes_1 == 1)
    {

      warning();
    }
    
    //复位
    mes_1 = "0"; mes_2 = "0"; mes_3 = "0"; mes_4 = "0";
    
    delay(1000);

   /***************
   * led控制与调节
   **************/
   if(Serial.available()>0)
  {
      isTurnOn = Serial.read();
      isTurnOn = isTurnOn - 48;
      Serial.println(isTurnOn);
      if(isTurnOn == 1)
      {

        
        Serial.println("开灯");

        //开灯并调节灯光
        lightSet();
        
        
      }
      if(isTurnOn == 0)
      {
        //关灯
        
        analogWrite(led,0);
       
        Serial.println("关灯");
        
        
      }
    
  }
   
   

   
   
  }
  else
  {
    //Serial.println("当前没有用户使用");
    delay(1000);
  }

 
  

}
//红外数据
int isTaked()
{
  takedSingal = digitalRead(9);
  return takedSingal;
  
}

//坐姿数据处理函数
void handleData(DistanceMessage& dic1,DistanceMessage& dic2,DistanceMessage& dic3,DistanceMessage& dic4)
{
  double distance_1,distance_2,distance_3,distance_4,cos_angle,arc_angle;
  int angle;
  
  
  
  
  distance_1 = dic1.returnDis();      //身体到桌子的距离
  
  distance_2 = dic2.returnDis();      //头部到桌面的距离
  distance_4 = dic4.returnDis();
  
  distance_3 = dic3.returnDis();      //3号定标点到身体的距离


  //判断坐姿是否正确
  if(distance_1 != 0)
  {
    if(distance_1 < 10.0)
      { 
        isSitshCorrect = 1;   //身体到桌子的距离太近
        //Serial.println("身体到桌子的距离太近!");
        mes_1 = "1";
        mes_2 = "1";
        
        delay(1000);
      }
  }

  if(distance_2 != 0.0 && distance_4 != 0.0)
  {    
    if(distance_2<30.0 || distance_4<30.0)
    {
       isSitshCorrect = 2;   //头部到桌面的距离太近
       //Serial.println("头部到桌面的距离太近!");

       mes_1 = "1";
       mes_3 = "1";
       
       delay(1000);
    }
  }

  if(distance_1 != 0.0 && distance_3 != 0.0)
  {
    float b = 8.0;
    float a,c;
    
    a = distance_1 - (distance_3 - 14.0);
    
   
    if(a > 0.0)
    {
      c = sqrt(a*a + b*b);
      //求得cos值
      cos_angle = (a*a+c*c - b*b)/(2*a*c);
      //求其对应的角度
      arc_angle = acos(cos_angle);
      //转化为度数值
      angle = (int)(arc_angle/1.57 * 90);
      
      
      Serial.println(angle,DEC);
      if(angle < 80)
      {
        //Serial.println("身体过于倾斜");
        mes_1 = "1";
        mes_4 = "1";
        
      }
    }
    
   
    
  }
  
  
}


void lightSet()
{
   
     while(1)
    {

      light.lightInit();
      light.lightRead();
      lightVal = light.lightShow();
      Serial.println(lightVal);
      delay(500);

      if(lightVal < 400)
      {
        ledPwm += 5;
        analogWrite(led,ledPwm);

        if(ledPwm > 240)
          break;
        
      }
      
      if(lightVal > 500)
      {
        ledPwm -= 5;
        analogWrite(led,ledPwm);

        if(ledPwm < 10)
          break;
        
  
        
      }

      if(lightVal>400 && lightVal<500)
        break;
      
    
    }
    delay(500);
   
}


void warning()
{

  for(int i=0;i<3;i++)
  {
    analogWrite(warningLed,255);
    delay(500);
    analogWrite(warningLed,0);
  }
}
