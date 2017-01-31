#include "pin.h"

Pin::Pin(Port prt, uint8_t p)
:Gpio(prt)
{
  pin_=p;
}

Pin::Pin(Port p_, uint8_t p, speed s, out o)
:Gpio(p_)
{
  pin_=p;
    //��������� �����
  *(reg)(portAdr[prt]+DDR) |= 1 << pin_;
  
  //��������� ��������
  *(reg)(portAdr[prt]+CR2) &= ~(1 << pin_);
  *(reg)(portAdr[prt]+CR2) |= s << pin_;
  
  //��������� ��������� Pushpull/Opendrain
  *(reg)(portAdr[prt]+CR1) &= ~(1 << pin_);
  *(reg)(portAdr[prt]+CR1) |= o << pin_; 
}

Pin::Pin(Port port_, uint8_t p, PP p_, Interrupt i)
:Gpio(port_)
{
  pin_=p;
  //��������� ����
  *(reg)(portAdr[prt]+DDR) &= ~(1 << pin_);
  
  //��������� �������� Floating/Pull-up
  *(reg)(portAdr[prt]+CR1) &= ~(1 << pin_);
  *(reg)(portAdr[prt]+CR1) |= p << pin_; 
  
  //��������� ����������
  *(reg)(portAdr[prt]+CR2) &= ~(1 << pin_);
  *(reg)(portAdr[prt]+CR2) |= i << pin_;
}

void Pin::setIn (PP p)
{
  //��������� ����
  *(reg)(portAdr[prt]+DDR) &= ~(1 << pin_);
  //��������� �������� Floating/Pull-up
  *(reg)(portAdr[prt]+CR1) &= ~(1 << pin_);
  *(reg)(portAdr[prt]+CR1) = p << pin_;
}

void Pin::setOut (out o)
{
  //��������� �����
  *(reg)(portAdr[prt]+DDR) |= 1 << pin_;
  //��������� ��������� Pushpull/Opendrain
  *(reg)(portAdr[prt]+CR1) &= ~(1 << pin_);
  *(reg)(portAdr[prt]+CR1) |= o << pin_; 
}

void Pin::set ()
{
  *(reg)(portAdr[prt]+ODR) |= 1 << pin_;
}

void Pin::clear ()
{
  *(reg)(portAdr[prt]+ODR) &= ~(1 << pin_);
}

void Pin::togle ()
{
  *(reg)(portAdr[prt]+ODR) ^= (1 << pin_);
}

bool Pin::state ()
{
  return (*(reg)(portAdr[prt]+IDR)& (1 << pin_));
}


