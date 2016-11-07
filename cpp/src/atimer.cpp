#include "atimer.h"

uint8_t Atimer::channelShift [4][2] = 
{
  {0x15, 0x16},
  {0x17, 0x18},
  {0x19, 0x1A},
  {0x1B, 0x1C}
};

uint8_t Atimer::ccmrShift [4] = {0x08, 0x09, 0x0A, 0x0B};


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
  TIM1->PSCRL = static_cast <uint8_t> (p) - 1;
}

void Atimer::setArr (uint16_t a)
{
  TIM1->ARRH = a >> 8;
  TIM1->ARRL = static_cast <uint8_t> (a);
}

void Atimer::setChannelValue (uint16_t val)
{
  *(uint8_t *)(TIM1_BaseAddress + channelShift [ch_][0]) = val >> 8;
  *(uint8_t *)(TIM1_BaseAddress + channelShift [ch_][1]) = static_cast <uint8_t> (val);
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

void Atimer::setMode (mode m)
{
  
}

void Atimer::pwmMode (nChannel ch)
{
  ch_ = ch;
  *(uint8_t *)(TIM1_BaseAddress + ccmrShift [ch]) = ((0x06 << 4)|TIM1_CCMR_OCxPE);
  TIM1->CCER1 = 1 << (4*ch_) ;
  TIM1->BKR |= TIM1_BKR_MOE;
  start();
}

void Atimer::pwmInputMode ()
{
  TIM1->CCMR1 |= 1 << 0;
  TIM1->CCMR2 |= 1 << 1;
  TIM1->SMCR |= (1 << 4|1 << 6|1 << 2);
  TIM1->CCER1 |= 1 << 0;
  TIM1->CCER2 |= (1 << 0|1 << 5);
  TIM1->IER |= TIM1_IER_CC1IE;
  start();
}
