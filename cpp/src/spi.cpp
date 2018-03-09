#include "spi.h"


Spi::Spi(Division div, Cpol cpl , Cpha cph , Role r )
:sck(Gpio::C, SCK , Gpio::highSpeed), mosi(Gpio::C, MOSI , Gpio::highSpeed), 
miso(Gpio::C, MISO, Gpio::Pullup)
{
  //turn on Spi takt
  CLK->PCKENR1 |= CLK_PCKENR1_SPI;
  
  //настройка SPI
  SPI->CR1 |= div << 3;
  SPI->CR1 |= r << 2;
  SPI->CR2 |= SPI_CR2_SSM;
  SPI->CR2 |= SPI_CR2_SSI;
  SPI->CR1 |= SPI_CR1_SPE;
}

uint8_t Spi::transfer (uint8_t data)
{
  putData (data);
  while (!flagRxne());
  return getData();
}




