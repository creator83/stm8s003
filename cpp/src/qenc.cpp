#include "qenc.h"




Qenc::Qenc (uint16_t range)
{
	high = range << 2;
	setMode ();
}


void Qenc::setMode ()
{
	//===Setings pin===//
	//pha
	
	//phb
	

	//===Settings timer===//
	
	TIM1->CCMR1 |= TIM1_CCMR_CCxS|TIM1_CCMR_ICxF;
	TIM1->CCER1 &= ~(TIM1_CCER1_CC1P|TIM1_CCER1_CC2P);
	TIM1->SMCR |= 0x03;
	TIM1->CR1 |= TIM1_CR1_CEN;
}

uint16_t Qenc::getValue ()
{
	value = TIM1->CNTRH << 8;
        value |= TIM1->CNTRL;
	return value >> 2;
}

void Qenc::setValue  (uint16_t val)
{
	value = val << 2;
	TIM1->CNTRH = value>>8;
        TIM1->CNTRL = static_cast <uint8_t> (value);
}

