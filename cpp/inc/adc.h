#include "stm8s.h"
#include "pin.h"

#ifndef ADC_H
#define ADC_H


class Adc
{
  //variables
public:
  enum channel {channel2=2, channel3, channel4, channel5, channel6};
  enum trigger {timer=0, external=0x10};
  
private:
  Pin adcPin;
  channel ch;
  static Gpio::Port adcPort [7];
  static uint8_t adcP [7];
  //functions
public:
  Adc(channel);
  void clearFlags ();
  uint16_t getValue ();
  void enableInterrupt ();
  void disableInterrupt ();
  void setContiniusMode ();
  void setBuffer ();
  void setTrigger (trigger);
  void start ();
  void clearEoc ();
  void getBuffer (uint16_t *dta);
};




#endif
