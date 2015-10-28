#include "stm8s.h"
#include "gpio.h"
#include "tact.h"

#ifndef UART_H
#define UART_H

class uart
{
//variables
public:
  enum baud {baud9600 , baud38400 , baud57600 , baud115200};
  enum role {Transmitter , Receiver};
  enum pin_def {TX = 5 , RX};
private:
  Gpio pin;
  static uint32_t sp[4];
 //function
public:
  uart(baud b);
  void transmit (uint8_t data);
  void transmit (char * str);
  uint8_t receive ();
private:
};

#endif