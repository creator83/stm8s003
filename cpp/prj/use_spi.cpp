#include "stm8s.h"
#include "spi.h"
#include "delay.h"
#include "tact.h"

tact frq;

int main()
{
  
  spi spi1 (spi::div4);
  spi1.transmit (0xF0);
  
  //  spi1.transfer_16 (0xF000);  
  while (1)
  {
 /*    
    delay_ms (200);
    spi1.transfer (0x0F);
    delay_ms (200);    

    for (uint8_t i = 8;i<16;++i)
    {
      spi1.transfer_16 (1<<i);
      delay_ms (20);
    }
    for (int8_t i = 0;i<8;++i)
    {
      spi1.transfer_16 (1<<i);
      delay_ms (20);
    }   
    for (int8_t i = 6;i>=0;--i)
    {
      spi1.transfer_16 (1<<i);
      delay_ms (20);
    }    
    for (int8_t i = 15;i>8;--i)
    {
      spi1.transfer_16 (1<<i);
      delay_ms (20);
    }       */
  }
}