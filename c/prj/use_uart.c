#include "stm8s.h"
#include "delay.h"
#include "uart.h"

#define led 1

int main( void )
{
  CLK->CKDIVR &= ~CLK_CKDIVR_HSIDIV;
  CLK->CKDIVR &= ~CLK_CKDIVR_CPUDIV;
  uart_init ();
  
  
  while (1)
  {
    uart_tx_string ("Hello from STM8S003F3");
    delay_ms (1000);
  }
}