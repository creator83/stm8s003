#include "stm8s.h"                  // Device header
#include "spi.h"
#include "delay.h"


#ifndef MAX6675_H
#define MAX6675_H

class max6675
{
//variables
public:
  char buffer [4];
  

private:
  spi spi1;
  static char number [11];

//functions
public:
  max6675 ();
  uint16_t readCelsius();
  void get_buffer (uint16_t val);
};

#endif
