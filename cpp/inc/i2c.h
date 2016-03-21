#include "stm8s.h"
#include "gpio.h"
#include "tact.h"

#ifndef I2C_H
#define I2C_H

//Скорости в kHz при standart mode и fast mode
const uint16_t speed_[2] = {100,400};

class i2c;
typedef void (i2c::*init)(uint8_t);

class i2c
{
//variables
public:

  enum mode {master,slave};
private:
  Gpio pin;
  static const init init_mode [2];
  enum pin_dif {SCL = 4,SDA};
  enum speed {standart, fast};
  uint8_t speed_mode;
 //function
public:
  i2c(mode m, speed d = standart);

private:
  void init_master (uint8_t speed);
  void init_slave ();
};

#endif


