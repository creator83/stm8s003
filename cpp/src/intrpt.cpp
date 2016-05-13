#include "intrpt.h"


intrpt::intrpt (Port p , uint8_t gp , sense s)
:pin (p)
{
  pin.setInPin (gp , Gpio::Pullup, Gpio::On);
  EXTI->CR1 &=~ (0x03<<(p<<1));
  EXTI->CR1 |= 1<<(p<<1); 
  __enable_interrupt ();
}

intrpt::intrpt (uint8_t p , uint8_t gp , sense s)
:pin (p)
{
  pin.setInPin (gp , Gpio::Floating, Gpio::On);
  EXTI->CR1 = 0;
  EXTI->CR1 |= 2; 
  __enable_interrupt ();  
}

bool intrpt::check_int (uint8_t p)
{
  return pin.pin_state (p);
}
