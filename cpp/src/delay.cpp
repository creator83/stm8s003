#include "stm8s.h"
/*
void delay_ms()
{
  CLK_PCKENR1 |=0x10;
  TIM4_SR_bit.UIF = 0;
  TIM4_PSCR = 0x06;
  TIM4_IER_bit.UIE = 1;
  TIM4_ARR = 250;
  TIM4_CR1_bit.CEN = 1;
}



void delay_ms(unsigned int delay)
{
  CLK_PCKENR1 |=0x20;
  //TIM2_SR1_bit.UIF = 0;
  TIM2_PSCR = 14;
  //TIM2_IER_bit.UIE = 1;
  TIM2_CR1_bit.OPM = 1;
  TIM2_ARRL = delay;
  TIM2_ARRH = delay>>8;
  TIM2_CR1_bit.CEN = 1;
  while (TIM2_CR1_bit.CEN){}
}
*/

void delay_us(unsigned int delay)
{
  //CLK_PCKENR1 |=0x20;
  CLK->PCKENR1 |=CLK_PCKENR1_TIM2;
  //TIM2_SR1_bit.UIF = 0;
  //TIM2->SR1 &= ~TIM2_SR1_UIF;
  
  //TIM2_PSCR = 4;
  TIM2->PSCR = 4;
  //TIM2_IER_bit.UIE = 1;
 // TIM2_CR1_bit.OPM = 1;
  TIM2->CR1 |= TIM2_CR1_OPM;
  TIM2->ARRL = delay;
  TIM2->ARRH = delay>>8;
  //TIM2_CR1_bit.CEN = 1;
  TIM2->CR1 |= TIM2_CR1_CEN;
  while (TIM2->CR1 & TIM2_CR1_CEN);
}

void delay_ms(unsigned int delay)
{
  CLK->PCKENR1 |=CLK_PCKENR1_TIM2;
  TIM2->PSCR = 14;
  TIM2->CR1 |= TIM2_CR1_OPM;
  TIM2->ARRL = delay;
  TIM2->ARRH = delay>>8;
  TIM2->CR1 |= TIM2_CR1_CEN;
  while (TIM2->CR1 & TIM2_CR1_CEN);
}
