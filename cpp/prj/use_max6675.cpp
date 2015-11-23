#include "stm8s.h"
#include "uart.h"
#include "delay.h"
#include "tact.h"
#include "max6675.h"


tact frq;

int main()
{  
  uart uart1 (uart::b9600);
  max6675 temp;  
   
  while (1)
  {
    temp.get_buffer( temp.readCelsius());
    uart1.transmit (temp.buffer);
    //uart1.transmit ("Hello from STM8");
    delay_ms (500);
  }
}