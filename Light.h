#ifndef _LIGHT_H
#define _LIGHT_H

#define address 0x23
#include "Arduino.h"

class Light
{
public:
  Light();
  void lightInit();
  int lightRead();
  int lightShow();
private:
 byte m_data[2];

  
};

#endif
