#include "stm8s.h"
#include "uart.h"
#include "delay.h"
#include "tact.h"
#include "nrf24l01.h"



tact frq;
uart uart1 (uart::b9600);
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

  nrf24l01 radio;
  for (uint8_t i=0;i<5;++i)
  {
    uart1.transmit (radio.get_status());
    delay_ms (500);
  }
   uart1.transmit ("===");
    for (uint8_t i=0;i<5;++i)
  {
    uart1.transmit (radio.r_reg (CONFIG));
    delay_ms (500);
  }
  uart1.transmit ("===");
  uart1.transmit(radio.r_reg (CONFIG));
  uart1.transmit ("===");
  radio.change_bit (CONFIG, PRIM_RX, 1);
  uart1.transmit(radio.r_reg (CONFIG));
  uart1.transmit ("===");
  delay_ms (1000);
  radio.change_bit (CONFIG, PWR_UP, 1);
  uart1.transmit(radio.r_reg (CONFIG));
  uart1.transmit ("===");  
  uart1.transmit(radio.get_status());
  delay_ms (1000);
  uart1.transmit ("==="); 
  radio.change_bit (CONFIG, PWR_UP, 0);
  uart1.transmit(radio.r_reg (CONFIG));
  while (1)
  {
   
    
  }
}




