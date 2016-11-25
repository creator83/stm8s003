#include "stm8s.h"
#include "pin.h"
#include "delay.h"



int main()
{
  CLK->CKDIVR = 0;
  Pin triac1 (Gpio::A, 3, Gpio::Low);
  Pin triac2 (Gpio::D, 3, Gpio::Low);
  Pin triac3 (Gpio::D, 4, Gpio::Low);
  Pin triac4 (Gpio::D, 5, Gpio::Low);
  Pin triac5 (Gpio::D, 6, Gpio::Low);
  triac1.set();
  triac2.set();
  triac3.set();
  triac4.set();
  triac5.set();
  while (1)
  {
  }
}
