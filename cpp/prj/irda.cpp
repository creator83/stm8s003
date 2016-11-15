#include "stm8s.h"
#include "gpio.h"
#include "delay.h"
#include "buffer.h"
#include "atimer.h"
#include "btimer.h"
#include "uart.h"

Atimer irda (16);
Btimer timer4;
Uart uart0;

#define ARRAY
#define CODE


const uint16_t startH = 15000;
const uint16_t startL = 10000;
const uint16_t bit1 = 2000;



struct flags_
{
  unsigned start : 1;
  unsigned ready : 1;
}flag;

union c
{
  uint32_t full;
  uint16_t half[2];
  uint8_t quater [4];
}c_;


uint16_t falling [32];



INTERRUPT_HANDLER(irdaChannel, TIM1_CAPCOM_CC1IF_vector)
{
  static uint8_t i=0;
  if (flag.start&&flag.ready==0)
  {
    falling [i] = TIM1->CCR1H << 8;
    falling [i]|= TIM1->CCR1L;
    ++i;
  }  
  if (!flag.start)
  {
    uint16_t value;
    value = TIM1->CCR1H << 8;
    value |= TIM1->CCR1L;
    if (value>startL&&value<startH) flag.start =1;
  }

  if (i==33) 
  {
    i=0;
    flag.ready = 1;
    disableInterrupts() ;
  }
  irda.clearFlag();
  if (flag.ready)
  {
    c_.full = 0;
    uint32_t v;
    
    for (uint8_t j=0;j<16;++j)
    {
      if (falling [31-j]>bit1)
      {
        v = 1 << j;
        c_.half[1] |= v;
      }
    }
    for (uint8_t j=0;j<16;++j)
    {
      if (falling [16-j]>bit1)
      {
        v = 1 << j;
        c_.half[0] |= v;
      }
    }    
#ifdef CODE
    for (uint8_t i=0;i<4;++i)
    {
      uart0.transmit (c_.quater[i]);
    }
#endif
#ifdef ARRAY
    for (uint8_t i=0;i<32;++i)
    {
      
      uart0.transmit (falling[i]);
    }    
    
#endif
    
    flag.ready = 0;
    flag.start = 0;
  }
}

INTERRUPT_HANDLER(mainLoop, TIM4_OVR_UIF_vector)
{
  timer4.clearFlag ();
  if (flag.ready)
  {
    flag.ready = 0;
  }
}

void timer4_init ();


int main()
{ 
  
  CLK->CKDIVR = 0;
  
  irda.pwmInputMode();
 
  enableInterrupts();
  
  //timer4_init ();
  
 
  while (1)
  {

  }
}

void timer4_init ()
{
  timer4.setPsc (Btimer::div128);
  timer4.setArr (125);
  enableInterrupts();
  timer4.interrupt (true);
  enableInterrupts();
  timer4.start ();
}

