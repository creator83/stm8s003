#include "adc.h"

Gpio::Port Adc::adcPort [7] = {Gpio::D, Gpio::D, Gpio::C, Gpio::D, Gpio::D, Gpio::D, Gpio::D};
uint8_t Adc::adcP [7] = {0, 0, 3, 2, 3, 5, 6};

Adc::Adc (channel ch_)
:adcPin (adcPort [ch_], adcP [ch_], Gpio::Floating)
{
  ch = ch_;
  ADC1->CR1 = ADC1_CR1_ADON|(1 << 6);
  ADC1->CR2 = ADC1_CR2_ALIGN;
  ADC1->CR3 = 0;
  ADC1->CSR = ch;
}

void Adc::clearFlags ()
{
  ADC1->CSR = ch;
}

uint16_t Adc::getValue ()
{
  ADC1->CR1 = ADC1_CR1_ADON;
  while (!ADC1->CSR&ADC1_CSR_EOC);
  uint16_t result = ADC1->DRH << 8;
  result |= ADC1->DRL;
  clearFlags ();
  return result&0x03FF;
}

void Adc::enableInterrupt ()
{
  ADC1->CSR |= ADC1_CSR_EOCIE;
  enableInterrupts();
}

void Adc::disableInterrupt ()
{
  ADC1->CSR &= ~ ADC1_CSR_EOCIE;
}

void Adc::setContiniusMode ()
{
   ADC1->CR1 |= ADC1_CR1_CONT;
}

void Adc::setBuffer ()
{
  ADC1->CR3 |= ADC1_CR3_DBUF;
}

void Adc::setTrigger (trigger t)
{
  //TIM1->CR2 = (4<<4) & TIM1_CR2_MMS;
  //TIM1->CCMR1= (6<<4) & TIM1_CCMR_OCM;
  //TIM1->IER|= TIM1_IER_CC1IE;	
  //TIM1->CCER1|= TIM1_CCER1_CC1P;			// OC1 negative polarity
  //TIM1->CCER1|= TIM1_CCER1_CC1E;
  //TIM1->BKR|= TIM1_BKR_MOE;
  TIM1->CR2 =  2 << 4;
  TIM1->SMCR |= TIM1_SMCR_MSM;
  ADC1->CR2 &= ~ ADC1_CR2_EXTSEL;
  ADC1->CR2 |= t;
  ADC1->CR2 |= ADC1_CR2_EXTTRIG;
}

void Adc::start ()
{
  ADC1->CR1 |= ADC1_CR1_ADON;
}

void Adc::clearEoc ()
{
  ADC1->CSR &= ~ ADC1_CSR_EOC;
}

void Adc::getBuffer (uint16_t *dta)
{
  dta [0] = ADC1->DB0RH<<8;
  dta [0] |= ADC1->DB0RL;
  dta [1] = ADC1->DB1RH<<8;
  dta [1] |= ADC1->DB1RL;
  dta [2] = ADC1->DB2RH<<8;
  dta [2] |= ADC1->DB2RL;
  dta [3] = ADC1->DB3RH<<8;
  dta [3] |= ADC1->DB3RL;
  dta [4] = ADC1->DB4RH<<8;
  dta [4] |= ADC1->DB4RL;
  dta [5] = ADC1->DB5RH<<8;
  dta [5] |= ADC1->DB5RL;
  dta [6] = ADC1->DB6RH<<8;
  dta [6] |= ADC1->DB6RL;
  dta [7] = ADC1->DB7RH<<8;
  dta [7] |= ADC1->DB7RL;
  dta [8] = ADC1->DB8RH<<8;
  dta [8] |= ADC1->DB8RL;
  dta [9] = ADC1->DB9RH<<8;
  dta [9] |= ADC1->DB9RL;
}

