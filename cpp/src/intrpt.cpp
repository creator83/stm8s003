#include "intrpt.h"


Intrpt::Intrpt (Gpio::Port p , uint8_t gp , sense s)
:pin_ (p, gp, Gpio::Pullup, Gpio::On)
{
  EXTI->CR1 &=~ (0x03<<(p<<1));
  EXTI->CR1 |= 1<<(p<<1); 
  __enable_interrupt ();
}


bool Intrpt::check_int ()
{
  return pin_.state ();
}
