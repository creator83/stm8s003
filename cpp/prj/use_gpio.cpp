#include "stm8s.h"
#include "gpio.h"
#include "delay.h"

int main()
{
  CLK->CKDIVR = 0;
  Gpio B (Gpio::B);
  B.setOutPin(4);

  
  while (1)
  {
    B.ChangePinState (4);
    delay_ms (500);
  }
}
