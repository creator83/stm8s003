#include "stm8s.h"
#include "gpio.h"
#include "delay.h"

int main()
{
  CLK->CKDIVR = 0;
  Gpio A (Gpio::A);
  A.setOutPin(3);

  
  while (1)
  {
    A.ChangePinState (3);
    delay_ms (1000);
  }
}