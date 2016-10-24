#include "spi.h"


spi::spi(Division div, Cpol cpl , Cpha cph , Role r )
:pin(Gpio::C), cs (Gpio::A)
{
  CLK->PCKENR1 |= CLK_PCKENR1_SPI;
  cs.setOutPin (CS , Gpio::Low);
  pin.setOutPin (SCK , Gpio::High);
  pin.setOutPin (MOSI , Gpio::High);
  pin.setInPin (MISO,Gpio::Pullup);
  cs.setPin (CS);
  
  //настройка SPI
  SPI->CR1 |= div << 3;
  SPI->CR1 |= r << 2;
  SPI->CR2 |= SPI_CR2_SSM;
  SPI->CR2 |= SPI_CR2_SSI;
  SPI->CR1 |= SPI_CR1_SPE;
}

void spi::transmit (uint8_t data)
{
  SPI->DR = data;
  while (SPI->SR&SPI_SR_BSY);
}

void spi::Set_CS ()
{
  cs.setPin (CS);
}
void spi::Clear_CS ()
{
  cs.clearPin (CS);
}

uint8_t spi::receive ()
{
  while (SPI->SR&SPI_SR_BSY);
  SPI->DR = 0;
  while (!(SPI->SR&SPI_SR_RXNE));
  return SPI->DR;
}

uint8_t spi::exchange (uint8_t data)
{
  while (SPI->SR&SPI_SR_BSY);
  SPI->DR = data;
  while (!(SPI->SR&SPI_SR_RXNE));
  return SPI->DR;
}


