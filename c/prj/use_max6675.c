#include "stm8s.h"
#include "delay.h"
#include "uart.h"
#include "max6675.h"

extern char buff [4];

#pragma vector = 7
__interrupt void EXTI_PORTC_IRQHandler(void)
{
  GPIOD->ODR ^= 1;     //  Toggle Port D, pin 3.
}


int main( void )
{
  uint8_t i;
  CLK->CKDIVR &= ~CLK_CKDIVR_HSIDIV;
  CLK->CKDIVR &= ~CLK_CKDIVR_CPUDIV;

  uart_init ();
  max6675_init ();
  while (1)
  {
    buffer (readCelsius());
    for (i=0;i<4;++i)
    {
      uart_tx_byte (buff [i]);
    }
    delay_ms (500);
  }
}