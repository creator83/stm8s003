#include "gtimer.h"

uint8_t Gtimer::channelShift [3][2] = 
{
  {0x0F, 0x10},
  {0x11, 0x12},
  {0x13, 0x14},
};

uint8_t Gtimer::ccmrShift [3] = {0x05, 0x06, 0x07};


Gtimer::Gtimer (divider d)
{
  setPsc (d);
}


Gtimer::Gtimer ()
{
}

void Gtimer::setCnt (uint16_t val)
{
  
}

void Gtimer::setPsc (divider d)
{
  TIM2->PSCR = d;
}

void Gtimer::setArr (uint16_t a)
{
  TIM2->ARRH = a >> 8;
  TIM2->ARRL = static_cast <uint8_t> (a);
}

void Gtimer::setChannelValue (uint16_t val)
{
  *(uint8_t *)(TIM2_BaseAddress + channelShift [ch_][0]) = val >> 8;
  *(uint8_t *)(TIM2_BaseAddress + channelShift [ch_][1]) = static_cast <uint8_t> (val);
}

uint16_t Gtimer::getCnt ()
{
  uint16_t valueCounter;
  valueCounter = TIM2->CNTRH << 8;
  valueCounter |= TIM2->CNTRL;
  return valueCounter;
}

void Gtimer::start ()
{
  TIM2->CR1 |= TIM2_CR1_CEN;
}

void Gtimer::stop ()
{
  TIM2->CR1 &= ~ TIM2_CR1_CEN;
}

void Gtimer::clearFlag()
{
	
}

void Gtimer::setMode (mode m)
{
  
}

void Gtimer::pwmMode (nChannel ch)
{
  ch_ = ch;
  *(uint8_t *)(TIM2_BaseAddress + ccmrShift [ch]) = 0x06 << 4;
  TIM2->CCER1 = 1 << (4*ch_)| TIM1_CCMR_OCxPE;
  start();
}

void Gtimer::interrupt (bool state)
{
  TIM2->IER &= ~ TIM2_IER_UIE;
  TIM2->IER |= state << 0;
}