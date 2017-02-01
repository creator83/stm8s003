#include "softi2c.h"


/*
SoftI2c::SoftI2c(Gpio::Port pSda, uint8_t sda_, Gpio::Port pScl, uint8_t scl_)
:sda (pSda, sda_, Gpio::highSpeed, Gpio::OpenDrain), scl (pScl, scl_, Gpio::highSpeed, Gpio::OpenDrain)
{
  
}*/

SoftI2c::SoftI2c ()
{
  GPIOB->DDR |= 1 << SDA| 1 << SCL;
  GPIOB->CR2 |= 1 << SDA| 1 << SCL;
  GPIOB->CR1 &= ~(1 << SDA| 1 << SCL);
}

void SoftI2c::start ()
{/*
  sda.setOut (Gpio::OpenDrain);
  sda.set ();
  scl.set ();
  delay_us (1);
  sda.clear ();
  delay_us (1);
  scl.clear ();
  delay_us (1);*/
  GPIOB->DDR |= 1 << SDA;
  GPIOB->ODR |= 1 << SDA;
  GPIOB->ODR |= 1 << SCL;
  nop ();
  GPIOB->ODR &=~ 1 << SDA;
  GPIOB->ODR &=~ 1 << SCL;
  nop ();
  
}

void SoftI2c::restart ()
{/*
  sda.setOut (Gpio::OpenDrain);
  scl.set ();
  sda.set ();
  delay_us (1);
  sda.clear ();
  delay_us (1);
  scl.clear ();
  delay_us (1);*/
  GPIOB->DDR |= 1 << SDA;
  GPIOB->ODR |= 1 << SCL;
  GPIOB->ODR |= 1 << SDA;
  nop ();
  GPIOB->ODR &=~ 1 << SDA;
  GPIOB->ODR &=~ 1 << SCL;
  nop ();
}

void SoftI2c::stop ()
{/*
  sda.setOut (Gpio::OpenDrain);
  sda.clear ();
  scl.clear ();
  scl.set (); 
  sda.set ();*/
  GPIOB->DDR |= 1 << SDA;
  GPIOB->ODR &=~ 1 << SDA;
  GPIOB->ODR &=~ 1 << SCL;
  nop ();
  GPIOB->ODR |= 1 << SCL;
  GPIOB->ODR |= 1 << SDA;
}

void SoftI2c::write (uint8_t addr)
{
  /*
  sda.setOut (Gpio::OpenDrain);
  sda.clear ();
  scl.clear ();
  for (uint8_t i=0;i<8;++i)
  {
    if (addr&(1 <<(7-i))) sda.set();
    else sda.clear ();
    scl.set ();
    //delay_us (1);
    scl.clear ();
  }*/
  GPIOB->DDR |= 1 << SDA;
  GPIOB->ODR &=~ 1 << SDA;
  GPIOB->ODR &=~ 1 << SCL;
  for (uint8_t i=0;i<8;++i)
  {
    if (addr&(1 <<(7-i))) GPIOB->ODR |= 1 << SDA;
    else GPIOB->ODR &=~ 1 << SDA;;
    GPIOB->ODR |= 1 << SCL;
    GPIOB->ODR &=~ 1 << SCL;
  }
}

uint8_t SoftI2c::read ()
{
  /*
  sda.setIn (Gpio::Floating);
  uint8_t data = 0;
  for (uint8_t i=0;i<8;++i)
  {
    scl.set ();
    //delay_us (1);
    if (sda.state ()) data |= 1 << (7-i);
    scl.clear (); 
  }
  return data;*/
  GPIOB->DDR &= ~ 1 << SDA;
  uint8_t data = 0;
  for (uint8_t i=0;i<8;++i)
  {
    GPIOB->ODR |= 1 << SCL;
    if (GPIOB->IDR&(1 << SDA)) data |= 1 << (7-i);
    GPIOB->ODR &= ~ 1 << SCL;
  }
  return data;
}

uint8_t SoftI2c::readwStretching ()
{
  /*
  sda.setIn (Gpio::Floating);
  uint8_t data = 0;
  if (sda.state ()) data |= 1 << 7;
  scl.setOut (Gpio::OpenDrain);
  for (uint8_t i=1;i<8;++i)
  {
    scl.set ();
    if (sda.state ()) data |= 1 << (7-i);
    scl.clear (); 
  }
  return data;*/
  GPIOB->DDR &= ~ 1 << SDA;
  uint8_t data = 0;
  if (GPIOB->IDR&(1 << SDA))data |= 1 << 7;
  GPIOB->DDR |= 1 << SCL;
  for (uint8_t i=1;i<8;++i)
  {
    GPIOB->ODR |= 1 << SCL;
    if (GPIOB->IDR&(1 << SDA)) data |= 1 << (7-i);
    GPIOB->ODR &=~ 1 << SCL;
  }
  return data;
}

bool SoftI2c::waiteAck ()
{/*
  sda.setIn (Gpio::Floating);
  scl.set ();
  if (!sda.state()) 
  {
    scl.clear ();
    return true;  
  }
  else 
  {
    scl.clear ();
    return false;
  }*/
  GPIOB->DDR &= ~ 1 << SDA;
  GPIOB->ODR |= 1 << SCL;
  nop ();
  if (GPIOB->IDR&(1 << SDA)) 
  {
    GPIOB->ODR &= ~ 1 << SCL;
    return true;  
  }
  else
  {
    GPIOB->ODR &= ~ 1 << SCL;
    return false;
  }
}

void SoftI2c::generateAck ()
{/*
  sda.setOut (Gpio::OpenDrain);
  sda.clear ();
  scl.set (); 
  scl.clear ();*/
  GPIOB->DDR |= 1 << SDA;
  GPIOB->ODR &= ~ 1 << SDA;
  GPIOB->ODR |= 1 << SCL;
  GPIOB->ODR &=~ 1 << SCL;
}

void SoftI2c::generateNack ()
{
  /*
  sda.setOut (Gpio::OpenDrain);
  sda.set ();
  scl.set ();  
  //delay_us (1); 
  scl.clear ();*/
  GPIOB->DDR |= 1 << SDA;
  GPIOB->ODR |= 1 << SDA;
  GPIOB->ODR |= 1 << SCL;
  GPIOB->ODR &=~ 1 << SCL;
}

void SoftI2c::waiteStretching ()
{
  /*
  scl.setIn (Gpio::Floating);
  while (scl.state () == 0);*/
  GPIOB->DDR &=~ 1 << SCL;
  while ((GPIOB->IDR&(1 << SCL))==0);
}


