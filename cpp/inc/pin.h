#include "stm8s.h"
#include "gpio.h"

#ifndef PIN_H
#define PIN_H


class Pin : protected Gpio
{
  //variables
public:

private:
  private:
  uint8_t pin_;
  //functions
public:
  Pin(Port prt, uint8_t p_);
  Pin(Port prt, uint8_t p_, speed s, out o = PushPull);
  Pin(Port prt, uint8_t p_, PP p, Interrupt i=Off);
  
  void setIn (PP p);
  void setOut ();
  
  void set ();
  void clear ();
  void togle ();
  bool state ();
};




#endif
