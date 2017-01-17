#include "ds3231.h"

Ds3231::Ds3231 (I2c *d)
:stopCommand(0x80), startCommand (0x00)
{
  driver = d;
}
  
void Ds3231::update ()
{
   for (uint8_t i=0;i<7;++i) bcdData[i] = driver->rByte (address, i);
}

void Ds3231::setData ()
{
  convertDec ();
  for (uint8_t i=0;i<7;++i) driver->wByte (address, i, bcdData[i]); 
}

void Ds3231::setSeconds (uint8_t val)
{
  decData [ds3231Registers::seconds] = val;
}

void Ds3231::setMinutes (uint8_t val)
{
  decData [ds3231Registers::minutes] = val;
}

void Ds3231::setHours (uint8_t val)
{
  decData [ds3231Registers::hours] = val;
}

void Ds3231::setDay (uint8_t val)
{
  decData [ds3231Registers::day] = val;
}

void Ds3231::setDate (uint8_t val)
{
  decData [ds3231Registers::date] = val;
}

void Ds3231::setMonth (uint8_t val)
{
  decData [ds3231Registers::month] = val;
}

void Ds3231::setYear (uint8_t val)
{
  decData [ds3231Registers::year] = val;
}

uint8_t & Ds3231::getSeconds ()
{
  return bcdData [ds3231Registers::seconds];
}

uint8_t & Ds3231::getMinutes ()
{
  return bcdData [ds3231Registers::minutes];
}

uint8_t & Ds3231::getHours ()
{
  return bcdData [ds3231Registers::hours];
}

uint8_t & Ds3231::getDay ()
{
  return bcdData [ds3231Registers::day];
}

uint8_t & Ds3231::getDate ()
{
  return bcdData [ds3231Registers::date];
}

uint8_t & Ds3231::getMonth ()
{
  return bcdData [ds3231Registers::month];
}

uint8_t & Ds3231::getYear ()
{
  return bcdData [ds3231Registers::year];
}

void Ds3231::convert (convertMode)
{
  
}

void Ds3231::convertBcd ()
{
  //seconds
  uint8_t temp = (bcdData[ds3231Registers::seconds]&valueMask::Dseconds)>>4;
  decData[ds3231Registers::seconds] = temp*10;
  temp = bcdData[ds3231Registers::seconds]&valueMask::seconds;
  decData[ds3231Registers::seconds] += temp;
  
  //minutes
  temp = (bcdData[ds3231Registers::minutes]&valueMask::Dminutes)>>4;
  decData[ds3231Registers::minutes] = temp*10;
  temp = bcdData[ds3231Registers::minutes]&valueMask::minutes;
  decData[ds3231Registers::minutes] += temp;
  
  //hours
  temp = (bcdData[ds3231Registers::hours]&valueMask::Dhours)>>4;
  decData[ds3231Registers::hours] = temp*10;
  temp = bcdData[ds3231Registers::hours]&valueMask::hours;
  decData[ds3231Registers::hours] += temp;
  
  //day
  decData[ds3231Registers::day] = bcdData[ds3231Registers::day];
  
  //date
  temp = (bcdData[ds3231Registers::date]&valueMask::Ddate)>>4;
  decData[ds3231Registers::date] = temp*10;
  temp = bcdData[ds3231Registers::date]&valueMask::date;
  decData[ds3231Registers::date] += temp;
  
  //year
  temp = (bcdData[ds3231Registers::year]&valueMask::Dyear)>>4;
  decData[ds3231Registers::year] = temp*10;
  temp = bcdData[ds3231Registers::year]&valueMask::year;
  decData[ds3231Registers::year] += temp;
}

void Ds3231::convertDec ()
{
  //seconds
  uint8_t temp = decData[ds3231Registers::seconds];
  uint8_t result;
  for (result =0;temp>=10;++result, temp-=10) ;
  bcdData[ds3231Registers::seconds] = result << 4;
  bcdData[ds3231Registers::seconds] |= temp%10;
  
  //minutes
  temp = decData[ds3231Registers::minutes];
  for (result =0;temp>=10;++result, temp-=10) ;
  bcdData[ds3231Registers::minutes] = result << 4;
  bcdData[ds3231Registers::minutes] |= temp%10;
  
  //hours
  temp = decData[ds3231Registers::hours];
  for (result =0;temp>=10;++result, temp-=10) ;
  bcdData[ds3231Registers::hours] =  (result << 4)|0x40;
  bcdData[ds3231Registers::hours] |= temp%10;
  
  //day
  bcdData[ds3231Registers::day] = decData[ds3231Registers::day];
  
  //date
  temp = decData[ds3231Registers::date];
  for (result =0;temp>=10;++result, temp-=10) ;
  bcdData[ds3231Registers::date] = result << 4;
  bcdData[ds3231Registers::date] |= temp%10;
  
  //year
  temp = decData[ds3231Registers::year];
  for (result =0;temp>=10;++result, temp-=10) ;
  bcdData[ds3231Registers::year] = result << 4;
  bcdData[ds3231Registers::year] |= temp%10;
}

