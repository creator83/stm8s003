#include "softi2c.h"



SoftI2c::SoftI2c(Gpio::Port pSda, uint8_t sda_, Gpio::Port pScl, uint8_t scl_)
:sda (pSda, sda_, Gpio::lowSpeed, Gpio::OpenDrain), scl (pScl, scl_, Gpio::lowSpeed, Gpio::OpenDrain)
{
  sda.clear ();
  scl.clear ();
  delay_us (5);
}

void SoftI2c::start ()
{
  sda.clear ();
  scl.clear ();
  delay_us (5);
  sda.set ();
  delay_us (5);
  scl.set ();
  delay_us (5);
}

void SoftI2c::restart ()
{
  scl.clear ();
  sda.clear ();
  delay_us (5);
  sda.set ();
  delay_us (5);
  scl.set ();
  delay_us (5);
}

void SoftI2c::stop ()
{
  sda.set ();
  scl.set ();
  delay_us (5);
  sda.clear ();
  scl.clear ();
  delay_us (5);
}

void SoftI2c::write (uint8_t addr)
{
  sda.set ();
  scl.set ();
  for (uint8_t i=0;i<8;++i)
  {
    if (addr&(7-i)) sda.clear();
    else sda.set ();
    scl.clear ();
    delay_us (5);
    scl.set ();
  }
}

uint8_t SoftI2c::read ()
{
  sda.clear ();
  scl.set ();
  uint8_t data = 0;
  for (uint8_t i=0;i<8;++i)
  {
    scl.clear ();
    if (sda.state ()) data |= 1 << (7-i);
    delay_us (5);
    scl.set ();
  }
  return data;
}

bool SoftI2c::waiteAck ()
{
  sda.clear ();
  scl.clear ();
  delay_us (5);
  if (!sda.state()) 
  {
    scl.set ();
    return true;  
  }
  else 
  {
    scl.set ();
    return false;
  }
}

void SoftI2c::generateAck ()
{
  sda.set ();
  scl.clear ();
  delay_us (5); 
  scl.set ();
}

void SoftI2c::generateNack ()
{
  sda.clear ();
  scl.clear ();
  delay_us (5); 
  scl.set ();  
}

void SoftI2c::waiteStretching ()
{
  scl.clear ();
  while (scl.state () == 0);
}


