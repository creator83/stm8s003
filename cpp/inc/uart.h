#include "stm8s.h"
#include "gpio.h"
#include "tact.h"

#ifndef UART_H
#define UART_H

class Uart
{
//variables
public:
  enum baud {b9600 , b57600 , div8 , div16 , div32 , div64 , div128 , div256};
  enum pin {TX = 5 , RX};
private:
  Gpio pin;
  static uint16_t speed [2];
  uint16_t brr_val;
 //function
public:
  Uart(baud b);
  Uart ();
  void transmit (uint8_t data);
  void transmit (char * str);
  uint8_t receive ();
private:
};

#endif

