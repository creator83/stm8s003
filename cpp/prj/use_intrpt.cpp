#include "stm8s.h"
#include "uart.h"
#include "delay.h"
#include "tact.h"
#include "intrpt.h"
#include "gpio.h"
#include "intrinsics.h"

/*
extern "C" {
    __interrupt void EXTI_PORTB_IRQHandler();
}
*/
tact frq;
const uint8_t led = 3;
const uint8_t button = 1;
Gpio D (Gpio::D);

INTERRUPT_HANDLER(EXTA_i, EXTI0_vector)
{
  D.setPin (led);
  delay_ms (1500);
  D.clearPin (led);
}

int main()
{
  
  D.setOutPin (led);
  intrpt intrA (intrpt::A , button , intrpt::falling);
  
  while (1)
  {
  }
}