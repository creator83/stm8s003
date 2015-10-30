#include "stm8s.h"
#include "delay.h"
#include "spi.h"

int main( void )
{
  CLK->CKDIVR &= ~CLK_CKDIVR_HSIDIV;
  CLK->CKDIVR &= ~CLK_CKDIVR_CPUDIV;
  spi_init ();
  
  
  while (1)
  {
    spi_tr_byte (0x0F);
    delay_ms (1000);
    spi_tr_byte (0xF0);
    delay_ms (1000);    
  }
}