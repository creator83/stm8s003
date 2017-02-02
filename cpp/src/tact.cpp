#include "tact.h"

uint8_t Tact::f_cpu;

Tact::Tact ()
{
  CLK->ECKR = 0;
  CLK->HSITRIMR = 0;
  CLK->SWIMCCR = 0;
  CLK->CKDIVR = 0;
  f_cpu = 16;
}

Tact::Tact (src_tact s)
{
  if (s) init_hse ();
  else init_hsi ();
  f_cpu = 16;
  CLK->CSSR = 0;
}

void Tact::init_hsi (uint8_t mdiv, uint8_t cdiv)
{
  CLK->ICKR = 0;
  CLK->ECKR = 0;
  CLK->ICKR |= CLK_ICKR_HSIEN;  
  while (!(CLK->ICKR&CLK_ICKR_HSIRDY));
  CLK->SWCR &=~ CLK_SWCR_SWIF;
  CLK->SWR = 0xE1;
  while (CLK->SWCR&CLK_SWCR_SWBSY);
  while (!(CLK->SWCR&CLK_SWCR_SWIF));
  CLK->SWCR &=~ CLK_SWCR_SWIF;
  CLK->SWCR |= CLK_SWCR_SWEN;
  while (CLK->SWCR&CLK_SWCR_SWBSY);
  CLK->CKDIVR = 0;
  CLK->CKDIVR |= (mdiv << 3)|cdiv;

  //CCO 
  CLK->CCOR = 0;
  CLK->CCOR |= 0x07 << 1;
  CLK->CCOR |= CLK_CCOR_CCOEN;  
  while (!CLK->CCOR&CLK_CCOR_CCORDY);
}

void Tact::init_hse ()
{
  CLK->ICKR |= CLK_ECKR_HSEEN;
  CLK->SWCR |= CLK_SWCR_SWEN;
  while (!(CLK->ECKR&CLK_ECKR_HSERDY));
  CLK->CKDIVR = 0;
  CLK->SWR = 0xB4;
  while (!(CLK->SWCR&CLK_SWCR_SWIF));
}

void Tact::init_lsi ()
{
  
  CLK->ICKR |= CLK_ICKR_LSIEN;
  while (!(CLK->ICKR&CLK_ICKR_LSIRDY));
  
 CLK->SWCR |= CLK_SWCR_SWEN;
  CLK->SWR = 0xD2;
  while((CLK->SWCR & CLK_SWCR_SWBSY) != 0 );
  while (!(CLK->SWCR&CLK_SWCR_SWIF));
  CLK->SWCR &=~ CLK_SWCR_SWIF;
   
  
/*   CLK->SWCR &=~ CLK_SWCR_SWIF;
  CLK->SWR = 0xD2;
  while (CLK->SWCR&CLK_SWCR_SWBSY);
  while (!(CLK->SWCR&CLK_SWCR_SWIF));
  CLK->SWCR &=~ CLK_SWCR_SWIF;
  CLK->SWCR |= CLK_SWCR_SWEN;
  while (CLK->SWCR&CLK_SWCR_SWBSY);*/
  CLK->CKDIVR = 0;

  //CCO
  /*
  CLK->CCOR = 0;
  CLK->CCOR |= 0x0C << 1;
  CLK->CCOR |= CLK_CCOR_CCOEN;
  while (!CLK->CCOR&CLK_CCOR_CCORDY);*/
  
  CLK->ICKR &= ~ CLK_ICKR_HSIEN;
  //CLK->ICKR |= CLK_ICKR_SWUAH;
  //FLASH->CR1 |=   FLASH_CR1_AHALT;
}

void Tact::setHsiFrq (uint8_t mdiv, uint8_t cdiv)
{
  CLK->CKDIVR = 0;
  CLK->CKDIVR |= (mdiv << 3)|cdiv;
  //CCO 
  CLK->CCOR = 0;
  CLK->CCOR |= 0x07 << 1;
  CLK->CCOR |= CLK_CCOR_CCOEN;  
  while (!CLK->CCOR&CLK_CCOR_CCORDY);
}