#include "Light.h"
#include <Wire.h>
Light::Light()
{
  m_data[0] = 0;
  m_data[1] = 0;
}
void Light::lightInit()
{
  Wire.beginTransmission(address);    //初始化IIC总线

  Wire.write(0x10);                   //设定模式

  Wire.endTransmission();
}

int Light::lightRead()
{
  int i=0;

  Wire.beginTransmission(address);

  Wire.requestFrom(address, 2);

  while(Wire.available()) 

  {

    m_data[i] = Wire.read();  

    i++;

  }

  Wire.endTransmission();  

  return i;

}

int Light::lightShow()
{
  
 uint16_t val=0;


 if(lightRead()==2)

  {

   val=((m_data[0]<<8)|m_data[1])/1.2;  //光照强度=（读取值*分辨率）/（1.2*灵敏度）
   
   
   //Serial.println("光照强度为："); 
   //Serial.print(val,DEC);     

   //Serial.println("[lux]");

   return  val;

  }

 delay(150);

  
}
