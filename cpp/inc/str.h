#include "stm8s.h"                // Device header
#include "shape.h"


#ifndef STR_H
#define STR_H

class Str : public Shape
{
private:
  uint16_t x, y;
  char str [8];
public:

  Str (uint16_t x_, uint16_t y_, char *);
  void draw () const;
};

#endif
