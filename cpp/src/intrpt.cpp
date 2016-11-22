#include "intrpt.h"


Intrpt::Intrpt (Gpio::Port p , uint8_t gp , sense s)
:pin_ (p, gp, Gpio::Pullup, Gpio::On)
{
  EXTI->CR1 &=~ (0x03<<(p*2));
  EXTI->CR1 |= s <<(p*2); 
}


bool Intrpt::check_int ()
{
  return pin_.state ();
}
