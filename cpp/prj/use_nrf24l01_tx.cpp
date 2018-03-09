#include "stm8s.h"
#include "uart.h"
#include "delay.h"
#include "tact.h"
#include "nrf24l01.h"



Tact frq;
Uart uart1 (Uart::b9600);
const uint8_t csPin = 4;
const uint8_t cePin = 3;
const uint8_t irqPin = 1; 




Pin cs (Gpio::C, csPin, Gpio::lowSpeed);
Pin ce (Gpio::C, cePin, Gpio::lowSpeed);
Intrpt irq (Gpio::A, irqPin, Intrpt::falling);
Nrf24l01 radio(cs, ce, irq);


INTERRUPT_HANDLER(EXTA_i, EXTI0_vector)
{
  delay_ms (2000);
  uart1.transmit ("   Status");
  uint8_t status = radio.getStatus();
  radio.writeRegister (STATUS, status);
  if (status&(1 << 4)){
	radio.command (FLUSH_TX);
	uart1.transmit (" flush_tx   ");
  }
  else if (status&(1 << 5)){
	uart1.transmit (" tx_normal   ");
  }
}

int main()
{
  
  uart1.transmit ("Hello from stm8!!!");
  for (uint8_t i=0;i<=23;++i){
	uart1.transmit (i);
	uart1.transmit (radio.readRegister(i));
  }
  for (uint8_t i=28;i<=29;++i){
	uart1.transmit (radio.readRegister(i));
  }
  while (1)
  {  
    for (uint8_t i=0;i<100;++i)
    {
	  uart1.transmit ("   Sending");
	  uart1.transmit (i);
      radio.sendByte (i);
      delay_ms (500);
    }
  }
}




