#include "adc.h"

Gpio::Port Adc::adcPort [7] = {Gpio::D, Gpio::D, Gpio::C, Gpio::D, Gpio::D, Gpio::D, Gpio::D};
uint8_t Adc::adcP [7] = {0, 0, 3, 2, 3, 5, 6};

Adc::Adc (channel ch_)
:adcPin (adcPort [ch_], adcP [ch_], Gpio::Floating)
{
  ch = ch_;
  ADC1->CR1 = ADC1_CR1_ADON;
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
