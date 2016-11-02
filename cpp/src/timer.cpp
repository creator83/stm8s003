#include "timer.h"

Atimer::Atimer (uint16_t divider)
{
  setPsc (divider);
}


Atimer::Atimer ()
{
}

void Atimer::setCnt (uint16_t val)
{
  
}

void Atimer::setPsc (uint16_t p)
{
  TIM1->PSCRH = p >> 8;
  TIM1->PSCRL = static_cast <uint8_t> (p) + 1;
}

void Atimer::setArr (uint16_t a)
{
  TIM1->ARRH = a >> 8;
  TIM1->ARRL = static_cast <uint8_t> (a) + 1;
}

void Atimer::setChannelValue (uint16_t val)
{
  
}

uint16_t Atimer::getCnt ()
{
  uint16_t valueCounter;
  valueCounter = TIM1->CNTRH << 8;
  valueCounter |= TIM1->CNTRL;
  return valueCounter;
}

void Atimer::start ()
{
  TIM1->CR1 |= TIM1_CR1_CEN;
}

void Atimer::stop ()
{
  TIM1->CR1 &= ~ TIM1_CR1_CEN;
}

void Atimer::clearFlag()
{
	
}
