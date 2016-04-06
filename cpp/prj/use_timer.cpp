#include "stm8s.h"
#include "gpio.h"
#include "delay.h"

const char led = 3;
void init_timer ();

Gpio D (Gpio::D);
/*
extern "C"
{
#pragma vector=TIM4_OVR_UIF_vector
  __interrupt void TIM4_Handler();
}
#pragma vector=TIM4_OVR_UIF_vector
__interrupt void TIM4_Handler()
{
  static uint16_t i = 0;
  TIM4->SR1 &=  ~ TIM4_SR1_UIF;
  ++i;
  if (i>499)
  {
    D.ChangePinState (led);
    i=0;
  }
}*/
/**/
INTERRUPT_HANDLER(TIM4_OVR_UIF, TIM4_OVR_UIF_vector)
{
  static uint16_t i = 0;
  TIM4->SR1 &=  ~ TIM4_SR1_UIF;
  ++i;
  if (i>499)
  {
    D.ChangePinState (led);
    i=0;
  }
}
int main()
{
  
  CLK->CKDIVR = 0;
  init_timer ();
  D.setOutPin(led);

  
  while (1)
  {
   
  }
}

void init_timer ()
{
  CLK->PCKENR1 |= CLK_PCKENR1_TIM4;
  TIM4->PSCR = 7;
  TIM4->IER = TIM4_IER_UIE;
  TIM4->CR1 = TIM4_CR1_ARPE|TIM4_CR1_CEN;
  __enable_interrupt();
}
