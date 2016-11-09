#include "gpio.h"

unsigned int Gpio::portAdr [4] = {GPIOA_BaseAddress , GPIOB_BaseAddress , GPIOC_BaseAddress, GPIOD_BaseAddress};

Gpio::Gpio(Port p )
{
  prt = p;
}

Gpio::Gpio()
{
}

void Gpio::setPort (Port p)
{
  prt = p;
}

void Gpio::setOutPin (unsigned char pin , speed s  , out o )
{
  //настройка выход
  *(reg)(portAdr[prt]+DDR) |= 1 << pin;
  
  //настройка скорости
  *(reg)(portAdr[prt]+CR2) &= ~(1 << pin);
  *(reg)(portAdr[prt]+CR2) |= s << pin;
  
  //настройка состояний Pushpull/Opendrain
  *(reg)(portAdr[prt]+CR1) &= ~(1 << pin);
  *(reg)(portAdr[prt]+CR1) |= o << pin;  
}

void Gpio::setInPin (unsigned char pin ,  PP p, Interrupt i)
{
  //настройка вход
  *(reg)(portAdr[prt]+DDR) &= ~(1 << pin);
  
  //настройка подтяжки Floating/Pull-up
  *(reg)(portAdr[prt]+CR1) &= ~(1 << pin);
  *(reg)(portAdr[prt]+CR1) |= p << pin; 
  
  //настройка прерывания
  *(reg)(portAdr[prt]+CR2) &= ~(1 << pin);
  *(reg)(portAdr[prt]+CR2) |= i << pin;
}

void Gpio::setPin (unsigned int pin )
{
  *(reg)(portAdr[prt]+ODR) |= 1 << pin;
}

void Gpio::clearPin (unsigned char pin)
{
  *(reg)(portAdr[prt]+ODR) &= ~(1 << pin);
}

void Gpio::setValPort (unsigned int value)
{
  
}

void Gpio::setOutPort (unsigned int value, speed s )
{
  
}

void Gpio::ChangePinState (unsigned char pin)
{
  *(reg)(portAdr[prt]+ODR) ^= (1 << pin);
}

void Gpio::SetPinState (unsigned char pin , unsigned char state)
{
  if (state)setPin (pin);
  else clearPin (pin);
}

bool  Gpio::pinState(uint8_t pin)
{
  return (*(reg)(portAdr[prt]+IDR)& (1 << pin));
}


