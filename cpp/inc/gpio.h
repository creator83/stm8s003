#include "stm8s.h"

#ifndef GPIO_H
#define GPIO_H

typedef uint8_t* reg;

class Gpio
{
  //variables
public:
  enum Port {A , B , C , D};
  enum PP {Floating , Pullup};
  enum Interrupt {Off , On};
  enum speed {lowSpeed,highSpeed};
  enum out {OpenDrain , PushPull};
  enum rgstr {
  ODR ,
  IDR ,
  DDR ,
  CR1 ,
  CR2 };
private:


protected:
    static unsigned int portAdr [4];
  unsigned char prt;
  //functions
public:
  Gpio(Port p );
  Gpio();
  void setPort (Port);
  void setOutPin (unsigned char pin , speed s = lowSpeed , out o = PushPull);	
  void setInPin (unsigned char pin,  PP p=Floating, Interrupt i=Off);
  void setPin (unsigned int pin );
  void clearPin (unsigned char pin);	
  void setValPort (unsigned int value);	
  void setOutPort (unsigned int value, speed s = lowSpeed );		
  void ChangePinState (unsigned char pin);
  void SetPinState (unsigned char pin , unsigned char state);
  bool pinState (uint8_t);
};




#endif
