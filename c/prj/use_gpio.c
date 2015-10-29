#include "stm8s.h"
#include "delay.h"

#define led 4
#define PORT GPIOB

int main( void )
{
  CLK->CKDIVR &= ~CLK_CKDIVR_HSIDIV;
  CLK->CKDIVR &= ~CLK_CKDIVR_CPUDIV;
  PORT->DDR |= 1 << led;
  PORT->CR1 |= 1 << led;
  
  
  while (1)
  {
    PORT->ODR ^= 1 << led;
    delay_ms (1000);
  }
}
