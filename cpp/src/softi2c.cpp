#include "softi2c.h"


#ifdef HSI
SoftI2c::SoftI2c(Gpio::Port pSda, uint8_t sda_, Gpio::Port pScl, uint8_t scl_)
:sda (pSda, sda_, Gpio::highSpeed, Gpio::OpenDrain), scl (pScl, scl_, Gpio::highSpeed, Gpio::OpenDrain)
{
  delay_ms (16);
}
#else

SoftI2c::SoftI2c ()
{
  GPIOB->DDR |= 1 << SDA| 1 << SCL;
  GPIOB->CR2 |= 1 << SDA| 1 << SCL;
  GPIOB->CR1 &= ~(1 << SDA| 1 << SCL);
}
#endif

void SoftI2c::start ()
{
#ifdef HSI
  sda.setOut (Gpio::OpenDrain);
  sda.set ();
  scl.set ();
  delay_us (1);
  sda.clear ();
  delay_us (1);
  scl.clear ();
  delay_us (1);
#else
  GPIOB->DDR |= 1 << SDA;
  GPIOB->ODR |= 1 << SDA;  
  GPIOB->ODR |= 1 << SCL;
  nop ();
  GPIOB->ODR &=~ 1 << SDA;
  nop ();
  GPIOB->ODR &=~ 1 << SCL;
  nop ();
#endif
}

void SoftI2c::restart ()
{
#ifdef HSI
  sda.setOut (Gpio::OpenDrain);
  scl.set ();
  sda.set ();
  delay_us (1);
  sda.clear ();
  delay_us (1);
  scl.clear ();
  delay_us (1);
#else 
  GPIOB->DDR |= 1 << SDA;
  GPIOB->ODR |= 1 << SCL;
  GPIOB->ODR |= 1 << SDA;
  nop ();
  GPIOB->ODR &=~ 1 << SDA;
  GPIOB->ODR &=~ 1 << SCL;
  nop ();
#endif
}

void SoftI2c::stop ()
{
#ifdef HSI
  sda.setOut (Gpio::OpenDrain);
  sda.clear ();
  scl.clear ();
  scl.set (); 
  sda.set ();
#else
  GPIOB->DDR |= 1 << SDA;
  GPIOB->ODR &=~ 1 << SDA;
  GPIOB->ODR &=~ 1 << SCL;
  nop ();
  GPIOB->ODR |= 1 << SCL;
  GPIOB->ODR |= 1 << SDA;
#endif
}

void SoftI2c::write (uint8_t addr)
{
#ifdef HSI
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
  }
#else
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
#endif
}

uint8_t SoftI2c::read ()
{
#ifdef HSI
  sda.setIn (Gpio::Floating);
  uint8_t data = 0;
  for (uint8_t i=0;i<8;++i)
  {
    scl.set ();
    //delay_us (1);
    if (sda.state ()) data |= 1 << (7-i);
    scl.clear (); 
  }
  return data;
#else 
  GPIOB->DDR &= ~ 1 << SDA;
  uint8_t data = 0;
  for (uint8_t i=0;i<8;++i)
  {
    GPIOB->ODR |= 1 << SCL;
    if (GPIOB->IDR&(1 << SDA)) data |= 1 << (7-i);
    GPIOB->ODR &= ~ 1 << SCL;
  }
  return data;
#endif  
}

uint8_t SoftI2c::readwStretching ()
{
#ifdef HSI
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
  return data;
#else    
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
#endif  
}

bool SoftI2c::waiteAck ()
{
#ifdef HSI  
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
  }
#else  
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
#endif  
}

void SoftI2c::generateAck ()
{
#ifdef HSI  
  sda.setOut (Gpio::OpenDrain);
  sda.clear ();
  scl.set (); 
  scl.clear ();
#else 
  GPIOB->DDR |= 1 << SDA;
  GPIOB->ODR &= ~ 1 << SDA;
  GPIOB->ODR |= 1 << SCL;
  GPIOB->ODR &=~ 1 << SCL;
#endif 
}

void SoftI2c::generateNack ()
{
#ifdef HSI
  sda.setOut (Gpio::OpenDrain);
  sda.set ();
  scl.set ();  
  //delay_us (1); 
  scl.clear ();
#else  
  GPIOB->DDR |= 1 << SDA;
  GPIOB->ODR |= 1 << SDA;
  GPIOB->ODR |= 1 << SCL;
  GPIOB->ODR &=~ 1 << SCL;
#endif  
}

void SoftI2c::waiteStretching ()
{
#ifdef HSI
  scl.setIn (Gpio::Floating);
  while (scl.state () == 0);
#else  
  GPIOB->DDR &=~ 1 << SCL;
  while ((GPIOB->IDR&(1 << SCL))==0);
#endif 
}


