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

//Tact frq;


void initAwu ();

int main()
{ 
 // Nrf24l01 ();
  /*CLK->PCKENR1 = 1 << 1;
  CLK->PCKENR2 = 1 << 2;*/
  CLK->PCKENR1 = 0;
  CLK->PCKENR2 = 0;
  GPIOA->DDR = 0Xff;
  GPIOA->CR1 = 0XFF;
  GPIOA->CR2 = 0X00;
  GPIOB->DDR = 0Xff;
  GPIOB->CR1 = 0XFF;
  GPIOB->CR2 = 0X00;
  GPIOC->DDR = 0Xff;
  GPIOC->CR1 = 0XFF;
  GPIOC->CR2 = 0X00;
  GPIOD->DDR = 0Xff;
  GPIOD->CR1 = 0XFF;
  GPIOD->CR2 = 0X00;
  CLK->ICKR |= CLK_ICKR_LSIEN;
  while (!(CLK->ICKR&CLK_ICKR_LSIRDY));
  
  CLK->SWCR |= CLK_SWCR_SWEN;
  CLK->SWR = 0xD2;
  while((CLK->SWCR & CLK_SWCR_SWBSY) != 0 );
  while (!(CLK->SWCR&CLK_SWCR_SWIF));
  CLK->SWCR &=~ CLK_SWCR_SWIF;
  CLK->CKDIVR = 0;
  CLK->ICKR &= ~ CLK_ICKR_HSIEN;
  //SoftI2c driver (Gpio::B , 5, Gpio::B, 4);
//SoftI2c driver;
  //Sht20 sensor (&driver);
  //initAwu ();
  //halt ();
 //frq.setHsiFrq(3);
  while (1)
  {
   // CLK->PCKENR1 &= ~ 1 << 5; 
    //sensor.getHummidity ();
    //sensor.getTemperature ();
  //  CLK->PCKENR1 = 1 << 5;
//    
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



