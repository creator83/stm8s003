#include "stm8s.h"
#include "uart.h"
#include "delay.h"
#include "tact.h"
#include "nrf24l01.h"



tact frq;
uart uart1 (uart::b9600);
nrf24l01 radio;

INTERRUPT_HANDLER(inerrupt, EXTI0_vector)
{
  uart1.transmit (radio.check_radio());
}


/*Gpio D (Gpio::D);

  
extern "C" {
    __interrupt void EXTI_PORTB_IRQHandler();
}

#pragma vector = 6
__interrupt void EXTI_PORTB_IRQHandler()
{
  static char i=0;
  D.ChangePinState (3);
  radio.transmit (i);
  ++i;
}
*/



int main()
{
  radio.rx_state ();

  while (1)
  {

  }
}




