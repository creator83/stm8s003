#include "stm8s.h"

#ifndef GPIO_H
#define GPIO_H

typedef uint16_t* reg;

class Gpio
{
  //variables
public:
  enum Port {A , B , C , D};
  enum mode {Input , Output};
  enum speed {Low,High};
  enum out {OpenDrain , PushPull};
  enum in {Floating , Pullup};
  enum rgstr {
  ODR ,
  IDR ,
  DDR ,
  CR1 ,
  CR2 };
private:
  private:
  static unsigned int portAdr [4];
  unsigned char prt;
  //functions
public:
  Gpio(Port p );
  Gpio(uint8_t p );
  void setOutPin (unsigned char pin , mode m = Output , speed s = Low , out o = PushPull);
  void setIntrpt (unsigned char pin , in i);
  void setPin (unsigned int pin );
  void clearPin (unsigned char pin);	
  void setValPort (unsigned int value);	
  void setOutPort (unsigned int value, speed s = Low );		
  void ChangePinState (unsigned char pin);
  void SetPinState (unsigned char pin , unsigned char state);
};




#endif