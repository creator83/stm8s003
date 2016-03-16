#include "stm8s.h"
#include "uart.h"
#include "delay.h"
#include "tact.h"
#include "nrf24l01.h"
#include "intrpt.h"

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
  tact frq;
//  D.setOutPin (3);
 // uart uart1 (uart::b9600);
//  intrpt intrB (intrpt::B , 5 , intrpt::rising);
  nrf24l01 radio;
  radio.mode (nrf24l01::receiver);
  char i=0;
  while (1)
  {
    radio.transmit (i);
    ++i;
    delay_ms (1000);
  }
}