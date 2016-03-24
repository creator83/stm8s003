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
  uart1.transmit (radio.r_reg (CONFIG));
  radio.change_bit (CONFIG, PRIM_RX, 0);
  uart1.transmit ("===");
  delay_ms (2000);
  uart1.transmit (radio.r_reg (CONFIG));
  uart1.transmit ("///");
  
  /*
  radio.change_bit (CONFIG, PRIM_RX, 1);
  delay_ms (2000);
  uart1.transmit ("====");
  uart1.transmit (radio.r_reg (CONFIG));
  uart1.transmit ("====");
  uart1.transmit (radio.r_status());*/
  
  /*bin (radio.r_reg (CONFIG));
  delay_ms (2000);
  radio.change_bit (CONFIG, PRIM_RX, 1);
  delay_ms (2000);
  uart1.transmit ("====");
  bin (radio.r_reg (CONFIG));
  uart1.transmit ("====");
  bin (radio.r_status()); */ 
  while (1)
  {
    
    uart1.transmit ("   CONFIG=");
    bin (radio.r_reg (CONFIG));
    delay_ms (500);
    uart1.transmit ("   STATUS=");
    bin (radio.r_status());
    delay_ms (500);    
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


