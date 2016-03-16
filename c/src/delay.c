#include "delay.h"

void delay_us(uint16_t delay)
{

  CLK->PCKENR1 |=CLK_PCKENR1_TIM2;
  TIM2->PSCR = 4;
  TIM2->CR1 |= TIM2_CR1_OPM;
  TIM2->ARRL = delay;
  TIM2->ARRH = delay>>8;
  TIM2->CR1 |= TIM2_CR1_CEN;
  while (TIM2->CR1 & TIM2_CR1_CEN);
}

void delay_ms(uint16_t delay)
{
  CLK->PCKENR1 |=CLK_PCKENR1_TIM2;
  TIM2->PSCR = 14;
  TIM2->CR1 |= TIM2_CR1_OPM;
  TIM2->ARRL = delay;
  TIM2->ARRH = delay>>8;
  TIM2->CR1 |= TIM2_CR1_CEN;
  while (TIM2->CR1 & TIM2_CR1_CEN);
}
