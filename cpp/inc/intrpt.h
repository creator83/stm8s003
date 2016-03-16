#include "stm8s.h"
#include "gpio.h"
#include "intrinsics.h"


#ifndef INTRPT_H
#define INTRPT_H

class intrpt
{
//variables
public:

  enum Port {A , B , C , D};
  enum sense {fallLow , rising , falling , riseFall};
  
private:
  
  Gpio pin;
    
  //functions
public:

  intrpt (Port p , uint8_t gp , sense s);
  intrpt (uint8_t p , uint8_t gp , sense s);
  

private:
  
};


#endif