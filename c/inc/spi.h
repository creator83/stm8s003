#include "stm8s.h"

#define CS   4
#define SCK  5
#define MOSI 6
#define MISO 7

#define CS_SET GPIOC->ODR |= 1 << CS
#define CS_CLEAR GPIOC->ODR &= ~(1 << CS)


#define div2   0
#define div4   1
#define div8   2
#define div16  3
#define div32  4
#define div128 5
#define div256 6


#ifndef SPI_H
#define SPI_H

void spi_init (void);

void spi_tr_byte (uint8_t data);

uint8_t spi_transfer (uint8_t data);

#endif