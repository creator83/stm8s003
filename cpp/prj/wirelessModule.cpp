#include "stm8s.h"
#include "pin.h"
#include "delay.h"
#include "tact.h"
#include "nrf24l01.h"
#include "uart.h"
#include "intrpt.h"


Pin triac1 (Gpio::A, 3, Gpio::Low);
Pin triac2 (Gpio::D, 3, Gpio::Low);
Pin triac3 (Gpio::D, 4, Gpio::Low);
Pin triac4 (Gpio::D, 5, Gpio::Low);
Pin triac5 (Gpio::D, 6, Gpio::Low);

Intrpt input (Gpio::B, 4, Intrpt::falling);

Pin * triacs [5] = {&triac1, &triac2, &triac3, &triac4, &triac5};
struct val
{
  uint8_t previus;
  uint8_t current;
}value;

Tact frq;
Nrf24l01 radio;
Uart uart1;



INTERRUPT_HANDLER(radioIrq, EXTI1_vector)
{
  uint8_t status = radio.readStatus ();
  radio.writeRegister (STATUS, status);
  //status &=0xF0;
  if (status&(1 << 6))
  {
     value.previus = value.current;
      radio.writeRegister (STATUS, status);
      value.current = radio.receiveByte ();
      uart1.transmit ("Data received: ");
      uart1.transmit (value.current);
      for (uint8_t i=0;i<5;++i)
      {
        if (value.current& (1 << i))triacs [i]->set();
        else triacs [i]->clear ();
      }
  }
  /*switch (status>>4)
  {
  case 1:
    {
      radio.comm (FLUSH_TX);
      uart1.transmit ("Max number retransmit");
      break;
    }
  case 2:
    {
      uart1.transmit ("Data sent");
      break;
    }
  case 4:
    {
      value.previus = value.current;
      radio.writeRegister (STATUS, status);
      value.current = radio.receiveByte ();
      uart1.transmit ("Data received: ");
      uart1.transmit (value.current);
      for (uint8_t i=0;i<5;++i)
      {
        if (value.current& (1 << i))triacs [i]->set();
        else triacs [i]->clear ();
      }
      break;
    }
  }   */
  
}


int main()
{
  CLK->CKDIVR = 0;
  for (uint8_t i=0;i<5;++i) triacs [i]->clear ();
  
  while (1)
  {
  }
}
