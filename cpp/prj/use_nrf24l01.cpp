#include "stm8s.h"
#include "uart.h"
#include "delay.h"
#include "tact.h"
#include "nrf24l01.h"


Nrf24l01 radio;
//

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

void bin (uint8_t data);

INTERRUPT_HANDLER(irqB, EXTI1_vector)
{
  radio.readRegister (STATUS);
}

int main()
{
  CLK->CKDIVR = 0;
  
  radio.readRegister (CONFIG);

  
  while (1)
  {
    
  
  }
}




