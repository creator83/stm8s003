#include "ds1307.h"

Ds1307::Ds1307 (I2c *d)
:adress(0xD0)
{
  driver = d;
}