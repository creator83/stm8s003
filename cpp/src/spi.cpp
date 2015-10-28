#include "spi.h"


spi::spi(Division div, Cpol cpl , Cpha cph , Role r )
:pin(Gpio::C)
{
  CLK->PCKENR1 |= CLK_PCKENR1_SPI;
  pin.setOutPin (CS , Gpio::Output , Gpio::High);
  pin.setOutPin (SCK , Gpio::Output , Gpio::High);
  pin.setOutPin (MOSI , Gpio::Output , Gpio::High);
  pin.setPin (CS);
  
  //настройка SPI
  SPI->CR1 |= div << 3;
  SPI->CR1 |= r << 2;
  SPI->CR2 |= SPI_CR2_SSM;
  SPI->CR2 |= SPI_CR2_SSI;
  SPI->CR1 |= SPI_CR1_SPE;
}

uint8_t spi::transfer (uint8_t data)
{
  uint8_t d;
  SPI->DR = data;
  if (SPI->SR&SPI_SR_BSY);
  d = SPI->DR;
  return d;
}

void spi::Set_CS ()
{
  pin.setPin (CS);  
}

void spi::Clear_CS ()
{
  pin.clearPin (CS);
} 

/*
uint16_t spi::transfer_16 (uint8_t data1 , uint8_t data2)
{
  uint16_t data;
  pin.clearPin (CS);
  SPI->DR = data1;
  if (!(SPI->SR&SPI_SR_TXE));
  data = SPI->DR;
  SPI->DR = data2;
  if (SPI->SR&SPI_SR_BSY);  
  data <<= SPI->DR;
  pin.setPin (CS);
  return data;  
}*/

void spi::transfer_16 (uint16_t data)
{
  pin.clearPin (CS);
  SPI->DR = static_cast <uint8_t>(data);
  if (!(SPI->SR&SPI_SR_TXE));
  SPI->DR = static_cast <uint8_t>((data >> 8));
  if (SPI->SR&SPI_SR_BSY);  
  pin.setPin (CS);
}

