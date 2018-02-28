#include "stm8s.h"
#include "uart.h"
#include "delay.h"
#include "tact.h"
#include "nrf24l01.h"



Tact frq;
Uart uart1 (Uart::b9600);
Nrf24l01 radio;
//const uint8_t led = 3;
Gpio D (Gpio::D);


INTERRUPT_HANDLER(EXTA_i, EXTI0_vector)
{
}




int main()
{
  while (1)
  {  
    /*for (uint8_t i=0;i<100;++i)
    {
      radio.send_byte (i);
      delay_ms (500);
    }*/
  }
}




