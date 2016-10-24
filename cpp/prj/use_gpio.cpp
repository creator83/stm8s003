#include "stm8s.h"
#include "gpio.h"
#include "delay.h"
const char b = 5;
const char l = 4;

int main()
{
  CLK->CKDIVR = 0;
  Gpio B (Gpio::B);
  B.setInPin (b);
  B.setOutPin (l);
  B.clearPin (l);
  
  while (1)
  {
    if (!B.pin_state(b))
    {
      delay_ms (200);
      B.ChangePinState (l);
    }
  }
}
