#include "max6675.h"

char max6675::number [11] = {'0', '1', '2', '3','4','5','6','7','8','9', 0};



max6675::max6675 ()
:spi1(spi::div8)
{
}

uint16_t max6675::readCelsius()
{
  uint16_t v;

  spi1.Clear_CS();
  delay_ms(1);
  v = spi1.receive();
  v <<= 8;
  v |= spi1.receive();
  spi1.Set_CS();

  if (v & 0x4) {
    // uh oh, no thermocouple attached!
    return 0; 
    //return -100;
  }

  v >>= 5;
	
  return v;
}

void max6675::get_buffer (uint16_t val)
{
  char tous, hundr, dec, ones;
  uint16_t temp = val;
     	
  for (tous=0;temp>=1000;++tous)temp -=1000;

  for (hundr=0;temp>=100;++hundr)temp -=100;
	
  for (dec=0;temp>=10;++dec)temp -=10;

  for (ones=0;temp>=1;++ones)temp--;
  if (tous)
  {	
    buffer[3] = tous;
    buffer[2] = hundr;
    buffer[1] = dec;
    buffer[0] = ones;
  }
  else if (!(tous || hundr))
  {
    buffer[3] = 0;
    buffer[2] = 0;  
    buffer[1] = dec;
    buffer[0] = ones; 
  }
  else if (!(tous || hundr||dec))
  {
    buffer[3] = 0;
    buffer[2] = 0;  
    buffer[1] = 0;          
    buffer[0] = ones;
  }	
  else
  {
    buffer[3] = 0;
    buffer[2] = hundr;
    buffer[1] = dec;
    buffer[0] = ones;
  }
}