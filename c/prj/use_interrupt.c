#include "stm8s.h"
#include "delay.h"


#define led            1
#define interrupt      2
#define falling_ed_lev 0
#define rising_edge    1
#define falling_edge   2
#define rising_falling 3

#define A              0
#define B              2
#define C              4
#define D              6




int main( void )
{
  CLK->CKDIVR &= ~CLK_CKDIVR_HSIDIV;
  CLK->CKDIVR &= ~CLK_CKDIVR_CPUDIV;
  
  //init led
  
  GPIOA->DDR |= 1 << led;
  GPIOA->CR1 |= 1 << led;
  
  //init interrupt
  GPIOD->DDR &= ~(1 << interrupt);
  GPIOD->CR1 &= ~(1 << interrupt);
  GPIOD->CR2 |= 1 << interrupt;
  EXTI->CR1 &= ~EXTI_CR1_PDIS;
  EXTI->CR1 |= rising_falling << D;
  __enable_interrupt();
  
  while (1)
  {
   
  }
}