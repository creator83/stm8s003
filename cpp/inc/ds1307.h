#include "stm8s.h"                  // Device header
#include "delay.h"
#include "i2c.h"


#ifndef DS1307_H
#define DS1307_H

namespace ds1307Registers
{
  const uint8_t seconds = 0x00;
  const uint8_t minutes = 0x01;
  const uint8_t hours   = 0x02;
  const uint8_t day     = 0x03;
  const uint8_t date    = 0x04;
  const uint8_t month   = 0x05;
  const uint8_t year    = 0x06;
  const uint8_t control = 0x07;
}



class Ds1307
{
  I2c * driver;
  uint8_t adress;
  uint8_t data [8];
//variables
public:
  Ds1307 (I2c *);
  void read (uint8_t l);
  void read (uint8_t ptr, uint8_t l);
  void write (uint8_t reg, uint8_t val);
  void write (uint8_t reg, uint8_t * arr, uint8_t l);
  
};

#endif
