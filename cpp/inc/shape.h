#include "stm8s.h"                   // Device header
#include "hd44780.h"


#ifndef SHAPE_H
#define SHAPE_H

class Shape
{
public:

  static Hd44780 * driver;
public:
  virtual void draw () const = 0;
  virtual ~Shape(){}
};




#endif
