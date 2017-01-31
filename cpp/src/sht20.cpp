#include "sht20.h"



Sht20::Sht20 (SoftI2c *d)
:address(0x80)
{
  driver = d;
}

void Sht20::readTemperature ()
{
  driver->start ();
  driver->write (address&0xFE);
  driver->waiteAck ();
  driver->write (sht20commands::tMesurementHold);
  driver->waiteAck ();
  driver->restart ();
  driver->write (address|0x01);
  driver->waiteAck ();
  driver->waiteStretching ();
  temperatureCode = driver->readwStretching ();
  driver->generateAck ();
  temperatureCode <<= 8;
  temperatureCode |= driver->read ();
  driver->generateAck ();
  checksum = driver->read ();
  driver->generateNack ();
  driver->stop ();
  temperatureCode &= ~0x0003;
}

void Sht20::readHummidity  ()
{
  driver->start ();
  driver->write (address&0xFE);
  driver->waiteAck ();
  driver->write (sht20commands::rMesurementHold);
  driver->waiteAck ();
  driver->restart ();
  driver->write (address|0x01);
  driver->waiteAck ();
  driver->waiteStretching ();
  hummdityCode = driver->readwStretching ();
  driver->generateAck ();
  hummdityCode <<= 8;
  hummdityCode |= driver->read ();
  driver->generateAck ();
  checksum = driver->read ();
  driver->generateNack ();
  driver->stop ();
  hummdityCode &= ~0x0003;
}

uint8_t & Sht20::getUserRegister ()
{
  driver->start ();
  driver->write (address&0xFE);
  driver->waiteAck ();
  driver->write (sht20commands::readUserRegister);
  driver->waiteAck ();
  driver->restart ();
  driver->write (address|0x01);
  driver->waiteAck ();
  uRegister = driver->read ();
  driver->generateNack ();
  driver->stop ();
  return uRegister;
}

uint16_t & Sht20::getTemperature ()
{
  readTemperature ();
  convertTemperature ();
  return temperature;
}
  
uint16_t & Sht20::getHummidity ()
{
  readHummidity  ();
  convertHummdity ();
  return hummdity;
}
  
void Sht20::convertHummdity ()
{
  hummdityCode *=1250;
  hummdityCode>>=16;
  hummdity = hummdityCode - 60;
}

void Sht20::convertTemperature ()
{
  temperatureCode *= 17572;
  temperatureCode >>= 16;
  temperature = (temperatureCode - 4685)/10;
}

