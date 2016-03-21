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
void bin (uint8_t data);

int main()
{

  nrf24l01 radio;
  bin (radio.r_reg (STATUS));
  delay_ms (2000);
  radio.change_bit (STATUS, PRIM_RX, 1);
  delay_ms (2000);
  bin (radio.r_reg (STATUS));
  
  while (1)
  {
    
  }
}


void bin (uint8_t data)
{
  for (int8_t i=7;i>=0;--i)
  {
    if (data&(1 << i)) uart1.transmit ('1');
    else uart1.transmit ('0');
  }
}


