#include "stm8s.h"
#include "gpio.h"

#ifndef SPI_H
#define SPI_H

typedef uint16_t* reg;


class spi
{
//variables
public:
  enum Division {div2 , div4 , div8 , div16 , div32 , div64 , div128 , div256};
  enum Role {slave , master};
  enum Cpol {neg, pos};
  enum Cpha {falling, rising};
  enum pin_def {CS = 4 , SCK = 5, MOSI , MISO };
private:
  Gpio pin;
  
//function
public:
  spi(Division div, Cpol cpl = neg, Cpha cph = falling, Role r = master);
  void Set_CS ();
  void Clear_CS ();
  uint8_t transfer (uint8_t data);
  //uint16_t transfer_16 (uint8_t data1 , uint8_t data2);
  void transfer_16 (uint16_t data);
private:
};

#endif