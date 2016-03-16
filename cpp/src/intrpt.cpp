#include "intrpt.h"


intrpt::intrpt (Port p , uint8_t gp , sense s)
:pin (p)
{
  pin.setIntrpt (gp , Gpio::Floating);
  EXTI->CR1 = 0;
  EXTI->CR1 |= s << 6;  
  __enable_interrupt ();
}

intrpt::intrpt (uint8_t p , uint8_t gp , sense s)
:pin (p)
{
  pin.setIntrpt (gp , Gpio::Floating);
  EXTI->CR1 = 0;
  EXTI->CR1 |= s << 6;  
  __enable_interrupt ();  
}