#include "stm8s.h"
#include "uart.h"
#include "delay.h"
#include "tact.h"
#include "nrf24l01.h"


Nrf24l01 radio;

uint8_t value ;
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
  uint8_t status = radio.readStatus ();
  if (status>15)
  {
    radio.writeRegister (STATUS, status);
    value = radio.receiveByte ();
  }
  
}

int main()
{
  CLK->CKDIVR = 0;
  
  radio.readRegister (CONFIG);
  uint8_t status = radio.readStatus ();
  radio.writeRegister (STATUS, status);
  value = radio.receiveByte ();
  while (1)
  {
    
  
  }
}




