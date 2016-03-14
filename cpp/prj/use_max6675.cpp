#include "stm8s.h"
#include "uart.h"
#include "delay.h"
#include "tact.h"
#include "max6675.h"


tact frq;
class pot;

  typedef uint8_t(pot::*PotMemFn)() ;
class pot
{

public:
  static PotMemFn foo[2];
  uint8_t f1 ();
  uint8_t f2 ();
  uint8_t result (uint8_t n);
  pot();
};
PotMemFn pot::foo[2] = {&pot::f1, &pot::f2};
pot::pot ()
{
}

uint8_t pot::result (uint8_t n)
{
  return (this->*(pot::foo[n]))();
}

uint8_t pot::f1 ()
{
  return 1;
}

uint8_t pot::f2 ()
{
  return 2;
}
int main()
{  /*
  uart uart1 (uart::b9600);
  max6675 temp;  */
   pot probe;
   uint8_t m;
   m = (probe.*(pot::foo[1]))();
   m++;
   m += (probe.*(pot::foo[0]))();
   m++;
   m += (probe.*(pot::foo[1]))();
   m += probe.result(0);
  while (1)
  {
    /*temp.get_buffer( temp.readCelsius());
    uart1.transmit (temp.buffer);
    //uart1.transmit ("Hello from STM8");
    delay_ms (500);*/
  }
}