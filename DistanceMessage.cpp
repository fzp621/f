#include "DistanceMessage.h"



DistanceMessage::~DistanceMessage()
{
  //析构函数
}

void DistanceMessage::trigPinSet()
{
	//持续10毫秒的高电平
	digitalWrite(m_trigPin, LOW);
	delayMicroseconds(2);
	digitalWrite(m_trigPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(m_trigPin, LOW);

}

double DistanceMessage::returnDis()
{
	return m_disMes;
}

void DistanceMessage::distanceGet()
{
	double duration, distance,sum;
	duration = pulseIn(m_echoPin, HIGH);
	distance = (duration / 2) * 0.0343;
	if (distance >= 400 || distance <= 2) {
		//Serial.println("超出范围");
		m_disMes = 0.0;
	}
	else
	{
		m_disMes = distance;

	}
	
	
	



}

void DistanceMessage::serialShow()
{
	int id;
	float distance;
	distance = m_disMes;
	id = m_id;
	if (distance >= 400 || distance <= 2) {
		//Serial.println("超出范围");
		m_disMes = 0.0;
	}
	else {
		switch (id) {
		case 0:
			Serial.println("一号超声波传感器测得距离：");
			Serial.print(distance);
			Serial.println(" cm");
			delay(500);
			break;
		case 1:
			Serial.println("二号超声波传感器测得距离：");
			Serial.print(distance);
			Serial.println(" cm");
			delay(500);
			break;
		case 2:
			Serial.println("三号超声波传感器测得距离：");
			Serial.print(distance);
			Serial.println(" cm");
			delay(500);
			break;
		case 3:
			Serial.println("四号超声波传感器测得距离：");
			Serial.print(distance);
			Serial.println(" cm");
			delay(500);
			break;
		}
	}
	delay(500);


}