#include "tact.h"

uint8_t tact::f_cpu;

tact::tact ()
{
  init_hsi ();
  CLK->CKDIVR = 0;
  f_cpu = 8;
}

void tact::init_hsi ()
{
  CLK->ICKR |= CLK_ICKR_HSIEN;  
  while (!(CLK->ICKR&CLK_ICKR_HSIRDY));
}