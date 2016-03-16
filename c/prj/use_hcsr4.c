#include "stm8s.h"
#include "delay.h"
#include "hcsr4.h"

#define led 1

int main( void )
{
  CLK->CKDIVR &= ~CLK_CKDIVR_HSIDIV;
  CLK->CKDIVR &= ~CLK_CKDIVR_CPUDIV;
  
  while (1)
  {
   
  }
}