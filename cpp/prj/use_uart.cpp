#include "stm8s.h"
#include "uart.h"
#include "delay.h"
#include "tact.h"

int main()
{
  tact frq;
  
  uart uart1 (uart::baud9600);
  while (1)
  {
    uart1.transmit ("Hello from STM8");
    delay_ms (1000);
  }
}