#include "str.h"


Str::Str (uint16_t x_, uint16_t y_, char * str_)
:x (x_), y (y_)
{
  while (*str_++=*str_++);
  str[7] = 0;
}


void Str::draw () const
{
  driver->sendString (str);
}
