#include "softi2c.h"



SoftI2c::SoftI2c(Gpio::Port pSda, uint8_t sda_, Gpio::Port pScl, uint8_t scl_)
:sda (pSda, sda_, Gpio::highSpeed, Gpio::OpenDrain), scl (pScl, scl_, Gpio::highSpeed, Gpio::OpenDrain)
{
  
}

void SoftI2c::start ()
{
  sda.setOut (Gpio::OpenDrain);
  sda.set ();
  scl.set ();
  delay_us (1);
  sda.clear ();
  delay_us (1);
  scl.clear ();
  delay_us (1);
}

void SoftI2c::restart ()
{
  sda.setOut (Gpio::OpenDrain);
  scl.set ();
  sda.set ();
  delay_us (1);
  sda.clear ();
  delay_us (1);
  scl.clear ();
  delay_us (1);
}

void SoftI2c::stop ()
{
  sda.setOut (Gpio::OpenDrain);
  sda.clear ();
  scl.clear ();
  //delay_us (2);
  scl.set ();
  //delay_us (2); 
  sda.set ();
}

void SoftI2c::write (uint8_t addr)
{
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
}

uint8_t SoftI2c::read ()
{
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
}

uint8_t SoftI2c::readwStretching ()
{
  sda.setIn (Gpio::Floating);
  uint8_t data = 0;
  if (sda.state ()) data |= 1 << 7;
  scl.setOut (Gpio::OpenDrain);
  for (uint8_t i=1;i<8;++i)
  {
    scl.set ();
    //delay_us (1);
    if (sda.state ()) data |= 1 << (7-i);
    scl.clear (); 
  }
  return data;
}

bool SoftI2c::waiteAck ()
{
  sda.setIn (Gpio::Floating);
  scl.clear ();
  //delay_us (1);
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
}

void SoftI2c::generateAck ()
{
  sda.setOut (Gpio::OpenDrain);
  sda.clear ();
  scl.set ();
  //delay_us (1); 
  scl.clear ();
}

void SoftI2c::generateNack ()
{
  sda.setOut (Gpio::OpenDrain);
  sda.set ();
  scl.set ();  
  //delay_us (1); 
  scl.clear ();
}

void SoftI2c::waiteStretching ()
{
  scl.setIn (Gpio::Floating);
  while (scl.state () == 0);
}


