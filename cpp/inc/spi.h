#include "stm8s.h"
#include "gpio.h"

#ifndef SPI_H
#define SPI_H


class Spi
{
//variables
public:
  enum Division {div2 , div4 , div8 , div16 , div32 , div64 , div128 , div256};
  enum Role {slave , master};
  enum Cpol {neg, pos};
  enum Cpha {falling, rising};
  enum pinDef {SCK=5, MOSI , MISO };
private:
  Gpio pin;
  
//function
public:
  Spi(Division div, Cpol cpl = neg, Cpha cph = falling, Role r = master);
  void putData (uint8_t data);
  uint8_t getData ();
  bool flagBsy ();
  bool flagTxe ();
  bool flagRxne ();
private:
};

inline void Spi::putData (uint8_t data){SPI->DR = data;}
inline uint8_t Spi::getData () {return SPI->DR;}
inline bool Spi::flagBsy (){return SPI->SR&SPI_SR_BSY;}
inline bool Spi::flagTxe (){return SPI->SR&SPI_SR_TXE;}
inline bool Spi::flagRxne (){return SPI->SR&SPI_SR_RXNE;}

#endif
