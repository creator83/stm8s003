#include "spi.h"


Spi::Spi(Division div, Cpol cpl , Cpha cph , Role r )
:pin(Gpio::C)
{
  CLK->PCKENR1 |= CLK_PCKENR1_SPI;
  pin.setOutPin (SCK , Gpio::highSpeed);
  pin.setOutPin (MOSI , Gpio::highSpeed);
  pin.setInPin (MISO,Gpio::Pullup);
  
  //настройка SPI
  SPI->CR1 |= div << 3;
  SPI->CR1 |= r << 2;
  SPI->CR2 |= SPI_CR2_SSM;
  SPI->CR2 |= SPI_CR2_SSI;
  SPI->CR1 |= SPI_CR1_SPE;
}




