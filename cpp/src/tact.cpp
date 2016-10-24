#include "tact.h"

uint8_t tact::f_cpu;

tact::tact ()
{
  init_hsi ();
  f_cpu = 16;
}

tact::tact (src_tact s)
{
  if (s) init_hse ();
  else init_hsi ();
  f_cpu = 16;
  CLK->CSSR |= CLK_CSSR_CSSEN + CLK_CSSR_CSSDIE;
}

void tact::init_hsi ()
{
  CLK->ICKR |= CLK_ICKR_HSIEN;  
  CLK->SWCR |= CLK_SWCR_SWEN;
  while (!(CLK->ICKR&CLK_ICKR_HSIRDY));
  CLK->CKDIVR = 0;
  CLK->SWR = 0xE1;
  while (!(CLK->SWCR&CLK_SWCR_SWIF));
}

void tact::init_hse ()
{
  CLK->ECKR |= CLK_ECKR_HSEEN;
  CLK->SWCR |= CLK_SWCR_SWEN;
  while (!(CLK->ECKR&CLK_ECKR_HSERDY));
  CLK->CKDIVR = 0;
  CLK->SWR = 0xB4;
  while (!(CLK->SWCR&CLK_SWCR_SWIF));
}