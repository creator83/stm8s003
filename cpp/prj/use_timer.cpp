#include "stm8s.h"
#include "gpio.h"
#include "delay.h"
#include "segled.h"
#include "buffer.h"
#include "button.h"
#include "qenc.h"
#include "atimer.h"
#include "btimer.h"
#include "gtimer.h"

Atimer timer1;

int main()
{ 
  CLK->CKDIVR = 0;

 /* GPIOC->DDR |= 1 << 6;
  GPIOC->CR1 |= 1 << 6;
  GPIOC->ODR |= 1 << 6;*/
  timer1.pwmMode (Atimer::channel1);
  timer1.setPsc (32);
  timer1.setArr (100);
  timer1.setChannelValue (90);
 
  while (1)
  {
    for (uint8_t i=100;i>0;--i)
    {
      timer1.setChannelValue (i);
      delay_ms (10);
    }
    for (uint8_t i=0;i<100;++i)
    {
      timer1.setChannelValue (i);
      delay_ms (10);
    }    
        
  }
}
