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
  enum operation {wOperation = 0xFE, rOperation = 0x01};
  
private:
  //Pin sda, scl;
  static init init_mode [2];
  enum pin_dif {SCL = 4,SDA = 5};
  enum speed {standart, fast};
  uint8_t speed_mode;
 //function
public:

  I2c();
  void putData (uint8_t );
  uint8_t getData ();

  void start ();
  void restart ();
  void stop ();
  void setAddress (uint8_t );
  
  void write (uint8_t val);

  
  bool wReg (uint8_t adress, uint8_t reg, uint8_t *data, uint8_t l);
  bool rReg (uint8_t adress, uint8_t reg, uint8_t *data, uint8_t l);
  
  void wReg1 (uint8_t adress, uint8_t reg, uint8_t data);
  uint8_t rReg1 (uint8_t adress, uint8_t reg);
  
  void wByte (uint8_t adress, uint8_t reg, uint8_t data);
  uint8_t rByte (uint8_t adress, uint8_t reg);
  
  bool flagBusy ();
  bool flagTxe ();
  bool flagRxne ();
  bool flagBtf ();
  bool flagSb ();
  bool flagAddr ();
  bool flagStop ();
 
  uint8_t readSR3 ();
  
  
private:
  void init_master ();
  void init_slave ();
};

#endif


