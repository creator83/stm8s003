#include "stm8s.h"
#include "pin.h"
#include "tact.h"
#include "delay.h"

#ifndef I2C_H
#define I2C_H

//Скорости в kHz при standart mode и fast mode
const uint16_t speed_[2] = {100,400};

class I2c;
typedef void (I2c::*init)(uint8_t);

class I2c
{
//variables
public:

  enum mode {master,slave};
  enum operation {write, read};
  
private:
  Pin sda, scl;
  static init init_mode [2];
  enum pin_dif {SCL = 4,SDA};
  enum speed {standart, fast};
  uint8_t speed_mode;
 //function
public:
  I2c(mode m, speed d = standart);
  void start ();
  void restart ();
  void stop ();
  void putData (uint8_t );
  uint8_t getData ();
  
  bool wReg (uint8_t adress, uint8_t reg, uint8_t *data, uint8_t l);
  
  bool flagBusy ();
  bool flagTxe ();
  bool flagRxne ();
  bool flagBtf ();
  bool flagSb ();
  bool flagAddr ();
  bool flagStop ();
 
  uint8_t readSR3 ();
  
  
private:
  void init_master (uint8_t speed);
  void init_slave ();
};

#endif


