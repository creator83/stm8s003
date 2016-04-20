#include "stm8s.h"
#include "gpio.h"

#ifndef SPI_H
#define SPI_H

typedef uint8_t* reg;


class spi
{
//variables
public:
  enum Division {div2 , div4 , div8 , div16 , div32 , div64 , div128 , div256};
  enum Role {slave , master};
  enum Cpol {neg, pos};
  enum Cpha {falling, rising};
  enum pin_def {CS = 4 , SCK, MOSI , MISO };
private:
  Gpio pin;
  
//function
public:
  spi(Division div, Cpol cpl = neg, Cpha cph = falling, Role r = master);
  void Set_CS ();
  void Clear_CS ();
  void put_data (uint8_t data);
  uint8_t get_data ();
  bool flag_bsy ();
  bool flag_txe ();
  bool flag_rxne ();
  void transmit (uint8_t data);
  uint8_t receive ();
  uint8_t exchange (uint8_t data);
private:
};

inline void spi::put_data (uint8_t data){SPI->DR = data;}
inline uint8_t spi::get_data () {return SPI->DR;}
inline bool spi::flag_bsy (){return SPI->SR&SPI_SR_BSY;}
inline bool spi::flag_txe (){return SPI->SR&SPI_SR_TXE;}
inline bool spi::flag_rxne (){return SPI->SR&SPI_SR_RXNE;}

#endif
