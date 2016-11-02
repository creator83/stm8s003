#include "btimer.h"

Btimer::Btimer (divider d)
{
  setPsc (d);
}


Btimer::Btimer ()
{
}

void Btimer::setCnt (uint8_t val)
{
  TIM4->CNTR = val;
}

void Btimer::setPsc (divider d)
{
  TIM4->CNTR = d;
}

void Btimer::setArr (uint8_t a)
{
  TIM4->ARR = a;
}

uint8_t Btimer::getCnt ()
{
  return TIM4->CNTR;
}

void Btimer::start ()
{
  TIM4->CR1 |= TIM1_CR1_CEN;
}

void Btimer::stop ()
{
  TIM4->CR1 &= ~ TIM1_CR1_CEN;
}

void Btimer::clearFlag()
{
  TIM4->SR1 &=  ~ TIM4_SR1_UIF;
}

void Btimer::interrupt (bool state)
{
  TIM4->IER &= ~ TIM4_IER_UIE;
  TIM4->IER |= state << 0;
}

