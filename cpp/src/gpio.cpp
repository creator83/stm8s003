#include "gpio.h"

unsigned int Gpio::portAdr [4] = {GPIOA_BaseAddress , 0x5004 , 0x5009, 0x500E};

Gpio::Gpio(Port p )
{
  prt = p;
}

Gpio::Gpio(uint8_t p )
{
  prt = p;
}

void Gpio::setOutPin (unsigned char pin , mode m , speed s  , out o )
{
  //��������� ����/�����
  *(reg)(portAdr[prt]+DDR) &= ~(1 << pin);
  *(reg)(portAdr[prt]+DDR) |= m << pin;
  
  //��������� ��������
  *(reg)(portAdr[prt]+CR2) &= ~(1 << pin);
  *(reg)(portAdr[prt]+CR2) |= s << pin;
  
  //��������� ��������� Pushpull/Opendrain
  *(reg)(portAdr[prt]+CR1) &= ~(1 << pin);
  *(reg)(portAdr[prt]+CR1) |= o << pin;  
}

void Gpio::setIntrpt (unsigned char pin , in i)
{
  //������� ��������� CR1 � DDR
  *(reg)(portAdr[prt]+DDR) &= ~(1 << pin);
  *(reg)(portAdr[prt]+CR1) &= ~(1 << pin);
  //��������� Pullup , floating
  *(reg)(portAdr[prt]+CR1) |= (i << pin);
  *(reg)(portAdr[prt]+CR2) |= (1 << pin);
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
  
}