#include "stm8s.h"
#include "gpio.h"
#include "delay.h"
#include "segled.h"
#include "buffer.h"
#include "button.h"
#include "qenc.h"

Buffer value (3);
Button button1 (Gpio::A, 1);
Button buttonEncoder (Gpio::B, 1);
Qenc encoder (999);

int main()
{
  CLK->CKDIVR = 0;
  button1.setShortLimit (10);
  Segled indicator (3);
  value.pars (356);
  
  while (1)
  {
    for (uint8_t i=0;i<3;++i)
    {
      indicator.frame (value.getArray(), i);
      delay_ms (1);
    }
  }
}
