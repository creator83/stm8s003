#include "stm8s.h"
#include "uart.h"
#include "delay.h"
#include "tact.h"
#include "intrpt.h"
#include "gpio.h"
#include "intrinsics.h"

/*
extern "C" {
    __interrupt void EXTI_PORTB_IRQHandler();
}
*/


Gpio D (Gpio::D);

#pragma vector = 6
__interrupt void EXTI_PORTB_IRQHandler()
{
  D.setPin (3);
  delay_ms (1500);
  D.clearPin (3);
}

int main()
{
  tact frq;
  D.setOutPin (3);
/*  GPIOB->DDR &= ~ (1 << 5);
  GPIOB->CR1 &= ~ (1 << 5);
  GPIOB->CR2 |= (1 << 5);
  EXTI->CR1 |= 1 << 2;
  __enable_interrupt ();*/
  
  intrpt intrB (intrpt::B , 5 , intrpt::rising);
  
  while (1)
  {
  }
}