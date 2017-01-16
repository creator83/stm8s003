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

namespace valueMask
{
  const uint8_t seconds = 0x0F;
  const uint8_t Dseconds = 0x70;
  const uint8_t minutes = 0x0F;
  const uint8_t Dminutes = 0xF0;
  const uint8_t hours   = 0x0F;
  const uint8_t Dhours   = 0x30;
  const uint8_t day     = 0x07;
  const uint8_t date    = 0x0F;
  const uint8_t Ddate    = 0x30;
  const uint8_t year    = 0x0F;
  const uint8_t Dyear    = 0xF0;
}



const uint8_t address = 0xD0;





class Ds1307
{
public:
  enum convertMode {bcdToDec, decToBcd};
private:
  I2c * driver;
  uint8_t bcdData [7];
  uint8_t decData [7];
  uint8_t stopCommand;
  uint8_t startCommand;

//variables
public:
  Ds1307 (I2c *);
  void read (uint8_t l);
  void read (uint8_t ptr, uint8_t l);
  void write (uint8_t reg,  uint8_t l);
  void update ();
  void setData ();
  void start ();
  void stop ();  
  void setSeconds (uint8_t);
  void setMinutes (uint8_t);
  void setHours (uint8_t);
  void setDay (uint8_t);
  void setDate (uint8_t);
  void setMonth (uint8_t);
  void setYear (uint8_t);

  uint8_t & getSeconds ();
  uint8_t & getMinutes ();
  uint8_t & getHours ();
  uint8_t & getDay ();
  uint8_t & getDate ();
  uint8_t & getMonth ();
  uint8_t & getYear ();
  void convert (convertMode);

  
  
private:
  void convertBcd ();
  void convertDec ();
};

#endif
