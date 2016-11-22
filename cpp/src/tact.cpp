#include "tact.h"

uint8_t Tact::f_cpu;

Tact::Tact ()
{
  init_hsi ();
  f_cpu = 16;
}

Tact::Tact (src_tact s)
{
  if (s) init_hse ();
  else init_hsi ();
  f_cpu = 16;
  CLK->CSSR |= CLK_CSSR_CSSEN + CLK_CSSR_CSSDIE;
}

void Tact::init_hsi ()
{
  CLK->ICKR = 0;
  CLK->ICKR |= CLK_ICKR_HSIEN;  
  CLK->ECKR = 0;
  while (!(CLK->ICKR&CLK_ICKR_HSIRDY));
  CLK->CKDIVR = 0;
  CLK->CCOR = 0;
  CLK->HSITRIMR = 0;
  CLK->SWIMCCR = 0;
  CLK->SWR = 0xE1;
  CLK->SWCR = 0;
  CLK->SWCR |= CLK_SWCR_SWEN;
  while (CLK->SWCR&CLK_SWCR_SWBSY);
}

void Tact::init_hse ()
{
  CLK->ECKR |= CLK_ECKR_HSEEN;
  CLK->SWCR |= CLK_SWCR_SWEN;
  while (!(CLK->ECKR&CLK_ECKR_HSERDY));
  CLK->CKDIVR = 0;
  CLK->SWR = 0xB4;
  while (!(CLK->SWCR&CLK_SWCR_SWIF));
}