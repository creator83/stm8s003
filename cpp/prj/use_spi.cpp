#include "stm8s.h"
#include "spi.h"
#include "delay.h"
#include "tact.h"

Tact frq;
Spi spi1 (Spi::div128);
Pin cs (Gpio::C, 4, Gpio::lowSpeed);

void spi_transmitte_byte (uint8_t d)
{
	cs.clear();
	spi1.transfer (d);
	cs.set();
}


int main()
{
	cs.set();
	while (1){
		for (uint8_t i=0;i<8;++i){
		spi_transmitte_byte (1<<i);
		delay_ms (100);
	}
	for (uint8_t i=6;i>0;--i){
		spi_transmitte_byte (1<<i);
		delay_ms (100);
	}    

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