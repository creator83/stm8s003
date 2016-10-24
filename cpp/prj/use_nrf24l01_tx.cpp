#include "stm8s.h"
#include "uart.h"
#include "delay.h"
#include "tact.h"
#include "nrf24l01.h"



tact frq;
uart uart1 (uart::b9600);
nrf24l01 radio;
const uint8_t led = 3;
Gpio D (Gpio::D);


INTERRUPT_HANDLER(EXTA_i, EXTI0_vector)
{
   uint8_t status = radio.get_status ();
   uart1.transmit (status);
   radio.w_reg (STATUS, status);
   status = radio.get_status ();
   uart1.transmit (status);
  D.setPin (led);
  delay_ms (1500);
  D.clearPin (led);
}




int main()
{
  D.setOutPin (led);
  radio.send_byte (125);
  while (1)
  {  
    /*for (uint8_t i=0;i<100;++i)
    {
      radio.send_byte (i);
      delay_ms (500);
    }*/
  }
}




