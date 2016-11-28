#include "stm8s.h"
#include "pin.h"

#ifndef ADC_H
#define ADC_H


class Adc
{
  //variables
public:
  enum channel {channel2=2, channel3, channel4, channel5, channel6};
  
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
 
};




#endif