#include "stm8s.h"
#include "uart.h"
#include "delay.h"
#include "tact.h"
#include "nrf24l01.h"
#include "pin.h"


Tact frq;
Nrf24l01 radio;

uint8_t inData;

INTERRUPT_HANDLER(inerrupt, EXTI0_vector)
{
    uint8_t b = radio.readStatus();
    radio.writeRegister (STATUS, b);
    if (b & 1<<6)
    {
      inData = radio.readRegister (R_RX_PAYLOAD);
    }

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
  //Pin radioIrq (Gpio::A, 1, Gpio::Pullup);

  while (1)
  {
   /* if (!radioIrq.state())
    {
      uint8_t a = radio.readStatus ();
      radio.writeRegister (STATUS, a);
      if (a & (1 << 6))
      {
        
      }
    }
    delay_ms (1);*/
  }
}




