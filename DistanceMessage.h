//超声波距离类
#ifndef _DISTANCE_H
#define _DISTANCE_H
#include "Arduino.h"

class DistanceMessage
{
private:
  int m_id;                 //设备id
  int m_trigPin;            //控制端口
  int m_echoPin;            //接收端口
  double m_disMes;          //超声波传感器的距离数据
  
public:
  DistanceMessage(const int& id, const int& trigPin,const int& echoPin)
  {
    
    m_id = id;
    m_trigPin = trigPin;
    m_echoPin = echoPin;
    m_disMes = 0.0;
    pinMode(m_trigPin, OUTPUT);
    pinMode(m_echoPin, INPUT);
	
    
  }                         //构造函数
  ~DistanceMessage();       //析构函数
  
  void trigPinSet();        //设置控制端口触发脉冲波

  void distanceGet();       //获取距离数据

  void serialShow();        //串口显示三个距离数据

  double returnDis();       //返回距离数据

  
  
 


};

#endif
