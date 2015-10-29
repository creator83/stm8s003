#include "stm8s.h"
#include "delay.h"

#define led 1

int main( void )
{
  CLK->CKDIVR &= ~CLK_CKDIVR_HSIDIV;
  CLK->CKDIVR &= ~CLK_CKDIVR_CPUDIV;
  GPIOA->DDR |= 1 << led;
  GPIOA->CR1 |= 1 << led;
  
  
  while (1)
  {
    GPIOA->ODR ^= 1 << led;
    delay_ms (1000);
  }
}
