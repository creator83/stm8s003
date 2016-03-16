#include "stm8s.h"                  // Device header
#include "delay.h"

//Soft Spi
#define SOFTSPI

//Pins for Soft Spi
#define sclk 5
#define miso 7
#define cs 4

#define PORT GPIOC

#define CS_SET PORT->ODR |= 1 << cs
#define CS_CLEAR PORT->ODR &= ~(1 << cs)

#ifndef MAX6675_H
#define MAX6675_H


void max6675_init (void);

double readCelsius(void);

uint16_t spiread(void);

void buffer (double val);


#endif
