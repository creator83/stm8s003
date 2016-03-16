#include "stm8s.h"
#include "uart.h"
#include "delay.h"
#include "tact.h"

tact frq;

int main()
{  
  uart uart1 (uart::b9600);
  
  while (1)
  {
    uart1.transmit ("Hello from STM8");
    delay_ms (1000);
  }
}