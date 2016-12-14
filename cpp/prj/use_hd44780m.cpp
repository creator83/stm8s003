#include "stm8s.h"
#include "tact.h"
#include "delay.h"
#include "hd44780.h"
#include "qenc.h"
#include "gtimer.h"
#include "atimer.h"
#include "btimer.h"
#include "buffer.h"
#include "button.h"
#include "pwm.h"
#include "pid.h"
#include "senc.h"
#include "adc.h"
#include "str.h"
#include "shape.h"

Hd44780 lcd;


int main()
{
  Shape * mscreen[3];
  Str str1 (0,1, "Hello");
  str1.driver = &lcd;
  mscreen [0] = &str1;
  mscreen[0]->draw();
  
  while (1)
  {
    
  }
}
