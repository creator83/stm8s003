#include "stm8s.h"
#include "pin.h"
#include "tact.h"
#include "delay.h"

#ifndef SOFTI2C_H
#define SOFTI2C_H


class I2c;
typedef void (I2c::*init)(uint8_t);

class SoftI2c
{
//variables
public:

  enum mode {master,slave};
  enum operation {wOperation = 0xFE, rOperation = 0x01};
  
private:
  Pin sda, scl;
  static init init_mode [2];
  enum speed {standart, fast};
  uint8_t speed_mode;
 //function
public:

  SoftI2c(Gpio::Port, uint8_t sda_, Gpio::Port, uint8_t scl_);

  void start ();
  void restart ();
  void stop ();
  void write (uint8_t );
  uint8_t read ();
  bool waiteAck ();
  void generateAck ();
  void generateNack ();
  void waiteStretching ();
  
  
  
  
private:
  void initMaster ();
  void initSlave ();
};

#endif


