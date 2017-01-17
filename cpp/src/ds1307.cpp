#include "ds1307.h"

Ds1307::Ds1307 (I2c *d)
:stopCommand(0x80), startCommand (0x00)
{
  driver = d;
}
  
void Ds1307::update ()
{
   for (uint8_t i=0;i<7;++i) bcdData[i] = driver->rByte (address, i);
}

void Ds1307::setData ()
{
  convertDec ();
  for (uint8_t i=0;i<7;++i) driver->wByte (address, i, bcdData[i]); 
}

void Ds1307::start ()
{
  driver->wReg (address, ds1307Registers::seconds, &startCommand, 1); 
}

void Ds1307::stop ()
{
  driver->wReg (address, ds1307Registers::seconds, &stopCommand, 1); 
}

void Ds1307::setSeconds (uint8_t val)
{
  decData [ds1307Registers::seconds] = val;
}

void Ds1307::setMinutes (uint8_t val)
{
  decData [ds1307Registers::minutes] = val;
}

void Ds1307::setHours (uint8_t val)
{
  decData [ds1307Registers::hours] = val;
}

void Ds1307::setDay (uint8_t val)
{
  decData [ds1307Registers::day] = val;
}

void Ds1307::setDate (uint8_t val)
{
  decData [ds1307Registers::date] = val;
}

void Ds1307::setMonth (uint8_t val)
{
  decData [ds1307Registers::month] = val;
}

void Ds1307::setYear (uint8_t val)
{
  decData [ds1307Registers::year] = val;
}

uint8_t & Ds1307::getSeconds ()
{
  return bcdData [ds1307Registers::seconds];
}

uint8_t & Ds1307::getMinutes ()
{
  return bcdData [ds1307Registers::minutes];
}

uint8_t & Ds1307::getHours ()
{
  return bcdData [ds1307Registers::hours];
}

uint8_t & Ds1307::getDay ()
{
  return bcdData [ds1307Registers::day];
}

uint8_t & Ds1307::getDate ()
{
  return bcdData [ds1307Registers::date];
}

uint8_t & Ds1307::getMonth ()
{
  return bcdData [ds1307Registers::month];
}

uint8_t & Ds1307::getYear ()
{
  return bcdData [ds1307Registers::year];
}

void Ds1307::convert (convertMode)
{
  
}

void Ds1307::convertBcd ()
{
  //seconds
  uint8_t temp = (bcdData[ds1307Registers::seconds]&valueMask::Dseconds)>>4;
  decData[ds1307Registers::seconds] = temp*10;
  temp = bcdData[ds1307Registers::seconds]&valueMask::seconds;
  decData[ds1307Registers::seconds] += temp;
  
  //minutes
  temp = (bcdData[ds1307Registers::minutes]&valueMask::Dminutes)>>4;
  decData[ds1307Registers::minutes] = temp*10;
  temp = bcdData[ds1307Registers::minutes]&valueMask::minutes;
  decData[ds1307Registers::minutes] += temp;
  
  //hours
  temp = (bcdData[ds1307Registers::hours]&valueMask::Dhours)>>4;
  decData[ds1307Registers::hours] = temp*10;
  temp = bcdData[ds1307Registers::hours]&valueMask::hours;
  decData[ds1307Registers::hours] += temp;
  
  //day
  decData[ds1307Registers::day] = bcdData[ds1307Registers::day];
  
  //date
  temp = (bcdData[ds1307Registers::date]&valueMask::Ddate)>>4;
  decData[ds1307Registers::date] = temp*10;
  temp = bcdData[ds1307Registers::date]&valueMask::date;
  decData[ds1307Registers::date] += temp;
  
  //year
  temp = (bcdData[ds1307Registers::year]&valueMask::Dyear)>>4;
  decData[ds1307Registers::year] = temp*10;
  temp = bcdData[ds1307Registers::year]&valueMask::year;
  decData[ds1307Registers::year] += temp;
}

void Ds1307::convertDec ()
{
  //seconds
  uint8_t temp = decData[ds1307Registers::seconds];
  uint8_t result;
  for (result =0;temp>=10;++result, temp-=10) ;
  bcdData[ds1307Registers::seconds] = result << 4;
  bcdData[ds1307Registers::seconds] |= temp%10;
  
  //minutes
  temp = decData[ds1307Registers::minutes];
  for (result =0;temp>=10;++result, temp-=10) ;
  bcdData[ds1307Registers::minutes] = result << 4;
  bcdData[ds1307Registers::minutes] |= temp%10;
  
  //hours
  temp = decData[ds1307Registers::hours];
  for (result =0;temp>=10;++result, temp-=10) ;
  bcdData[ds1307Registers::hours] =  (result << 4)|0x40;
  bcdData[ds1307Registers::hours] |= temp%10;
  
  //day
  bcdData[ds1307Registers::day] = decData[ds1307Registers::day];
  
  //date
  temp = decData[ds1307Registers::date];
  for (result =0;temp>=10;++result, temp-=10) ;
  bcdData[ds1307Registers::date] = result << 4;
  bcdData[ds1307Registers::date] |= temp%10;
  
  //year
  temp = decData[ds1307Registers::year];
  for (result =0;temp>=10;++result, temp-=10) ;
  bcdData[ds1307Registers::year] = result << 4;
  bcdData[ds1307Registers::year] |= temp%10;
}

