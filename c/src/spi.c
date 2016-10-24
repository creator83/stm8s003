#include "spi.h"


void spi_init (void)
{
  //===Settings PINS===//
  //CS
  GPIOC->DDR |= 1 << CS;
  GPIOC->CR1 |= 1 << CS;
  GPIOC->CR2 &= ~(1 << CS);
  GPIOC->ODR |= 1 << CS;
  
  //SCK
  GPIOC->DDR |= 1 << SCK;
  GPIOC->CR1 |= 1 << SCK;
  GPIOC->CR2 |= 1 << SCK;
  
  //MOSI
  GPIOC->DDR |= 1 << MOSI;
  GPIOC->CR1 |= 1 << MOSI;
  GPIOC->CR2 |= 1 << MOSI;
  
  //MISO
  GPIOC->DDR &= ~(1 << MISO);
  GPIOC->CR1 &= ~(1 << MISO);
  GPIOC->CR2 &= ~(1 << MISO);
  
  //===Settings SPI===//
  //Settings frq
  SPI->CR1 &= ~SPI_CR1_BR;
  SPI->CR1 |= div8 << 3;
  
  //SPI clock mode
  //SPI->CR1 |= SPI_CR1_CPOL;
  //SPI->CR1 |= SPI_CR1_CPHA;
    
  //Master selection
  SPI->CR1 |= SPI_CR1_MSTR;
  
  //Soft managment CS
  SPI->CR2 |= SPI_CR2_SSM|SPI_CR2_SSI;
         
  //Turn on SPI
  SPI->CR1 |= SPI_CR1_SPE;
}

void spi_tr_byte (uint8_t data)
{
  CS_CLEAR;
  spi_transfer (data);
  CS_SET;
}

uint8_t spi_transfer (uint8_t data)
{
  SPI->DR = data;
  if (SPI->SR&SPI_SR_BSY);
  return SPI->DR;
}