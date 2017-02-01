#include "stm8s.h"
#include "tact.h"
#include "delay.h"
#include "hd44780.h"
#include "qenc.h"
#include "gtimer.h"
#include "atimer.h"
#include "btimer.h"
#include "buffer.h"
#include "button.h"
#include "pwm.h"
#include "senc.h"
#include "adc.h"
#include "i2c.h"
#include "nrf24l01.h"
#include "sht20.h"
#include "softi2c.h"

Tact frq;
Buffer value;
uint8_t *eepromPtr;
//SoftI2c driver (Gpio::B , 5, Gpio::B, 4);
SoftI2c driver;
Sht20 sensor (&driver);

void initAwu ();

int main()
{ 
  Sht20 sensor (&driver);
 // Nrf24l01 ();
//  CLK->PCKENR1 = 1 << 1;
 // CLK->PCKENR2 = 1 << 2;
  /*initAwu ();
  halt ();*/
  while (1)
  {
   // CLK->PCKENR1 &= ~ 1 << 5;
    frq.init_lsi();
    sensor.getHummidity ();
    sensor.getTemperature ();
  //  CLK->PCKENR1 = 1 << 5;
//    frq.init_hsi(2);
    //send data nrf24l01
    
  }
}

void initAwu ()
{
  AWU->CSR &= ~ AWU_CSR_AWUEN;
  //AWU->TBR = 8;
  AWU->TBR = 0x0F;
  AWU->APR = 60;
  AWU->CSR |= AWU_CSR_AWUEN;
}



