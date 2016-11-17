#include "stm8s.h"
#include "pin.h"
#include "intrinsics.h"


#ifndef INTRPT_H
#define INTRPT_H

class Intrpt
{
//variables
public:

  enum sense {fallLow , rising , falling , riseFall};
  
private:
  
  Pin pin_;
    
  //functions
public:

  Intrpt (Gpio::Port p , uint8_t gp , sense s);
  bool check_int ();

private:
  
};


#endif
