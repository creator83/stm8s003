#include "ds1307.h"

Ds1307::Ds1307 (I2c *d)
:adress(0xD0)
{
  driver = d;
}

void Ds1307::read (uint8_t l)
{
  uint8_t * ptr = data;
  I2C->CR2 |= I2C_CR2_ACK;
  driver->start();
  driver->putData (adress|I2c::read);
  while(!driver->flagAddr());
  while (l--)
  {
    while (driver->flagRxne());
    *ptr = driver->getData();
  }
  driver->stop ();
  uint8_t temp = I2C->SR1;
  while(driver->flagStop ());
}

void Ds1307::read (uint8_t ptr, uint8_t l)
{
  I2C->CR2 |= I2C_CR2_ACK;
  while (driver->flagBusy());
  driver->start();
  while(!driver->flagSb());
  driver->putData (adress|I2c::write);
  while(!driver->flagAddr());
  uint8_t temp = I2C->SR3;
  while(!driver->flagTxe());
  driver->putData (ptr);
  while(!(driver->flagTxe() && driver->flagBtf ()));
  driver->start();
  uint8_t * dataPtr = &data [ptr];
  while(!driver->flagSb());
  driver->putData (adress|I2c::read);
  --l;
  while (l--)
  {
    while (driver->flagRxne());
    *dataPtr = driver->getData();
    dataPtr++;
  }
  
  driver->stop ();
  temp = I2C->SR1;
  while(driver->flagStop ());
}

void Ds1307::write (uint8_t reg, uint8_t val)
{
  driver->start();
  driver->putData (adress|I2c::write);
  while(!driver->flagAddr());
  uint8_t temp = I2C->SR3;
  while(!driver->flagTxe());
  driver->putData (reg);
  while(!driver->flagTxe());
  driver->putData (val);
  while(!(driver->flagTxe() && driver->flagBtf ()));
  driver->stop ();
  while(driver->flagStop ());
}

void Ds1307::write (uint8_t reg, uint8_t * arr, uint8_t l)
{
  driver->start();
  driver->putData (adress|I2c::write);
  while(!driver->flagAddr());
  uint8_t temp = I2C->SR3;
  while(!driver->flagTxe());
  driver->putData (reg);
  while (l--)
  {
    while(!driver->flagTxe());
    driver->putData (*arr++);
  }
  while(!(driver->flagTxe() && driver->flagBtf ()));
  driver->stop ();
  while(driver->flagStop ());
}
  

