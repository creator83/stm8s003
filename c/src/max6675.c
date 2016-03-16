#include "max6675.h"


char number [10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
char buff [4];

#ifdef SOFTSPI

void max6675_init (void)
{
  //===Settings pins===//
  //cs and sclk
  PORT->DDR |= (1 << sclk)|(1 << cs);
  PORT->CR1 |= (1 << sclk)|(1 << cs);
  PORT->CR2 &= ~((1 << sclk)|(1 << cs));
  PORT->ODR |= 1 << sclk;
  
  //miso
  PORT->DDR &= ~(1 << miso);
  PORT->CR1 &= ~(1 << miso);
  PORT->CR2 &= ~(1 << miso);
  
  CS_SET;
}

uint16_t spiread(void)
{
  int8_t i;
  uint16_t d = 0;

  for (i=15; i>=0; i--)
  {
    PORT->ODR &=~(1 << sclk);
    delay_ms(1);
    if ((PORT->IDR & (1 << miso)))
    {
      //set the bit to 0 no matter what
      d |= (1 << i);
    }

    PORT->ODR |=(1 << sclk);
    delay_ms(1);
  }
  return d;
}

double readCelsius(void)
{
  uint16_t v;

  CS_CLEAR;
  delay_ms(1);

  v = spiread();
  
  CS_SET;

  if (v & 0x4) {
    // uh oh, no thermocouple attached!
    return 0; 
    //return -100;
  }

  v >>= 3;

  return v*0.25;
}
#else

void max6675_init (void)
{
	init_spi ();
}

double readCelsius(void)
{
  uint16_t v;

  clear_cs ();
  delay_ms(1);

  v = transfer(0);
  v <<= 8;
  v |= transfer(0);

  set_cs ();

  if (v & 0x4) {
    // uh oh, no thermocouple attached!
    return 0; 
    //return -100;
  }

  v >>= 3;

  return v*0.25;
}
#endif

void buffer (double val)
{
	char dec, ones, decimal;
	dec = (uint16_t)val/10;
	buff[0] = number [dec];
	ones = (int)val%10;
	buff[1] = number [ones];
	decimal = (int)(val*10)%10;
	buff[3] = number [decimal];
}




