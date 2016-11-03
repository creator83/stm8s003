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

Buffer value ;

Button button1 (Gpio::A, 1);
Button buttonEncoder (Gpio::B, 1);
Qenc encoder (999);

Btimer timer4;
Gtimer timer2;

uint16_t encValue = 65;
/*Gpio D (Gpio::D);
const char triac = 1;*/

uint16_t encMemVal [3] = {65, 135, 20};

Segled indicator (3);


struct flags_
{
  unsigned button : 1;
}butt;

INTERRUPT_HANDLER(backCounter, TIM2_OVR_UIF_vector)
{
  timer2.clearFlag();
  if (encValue)
  {
    encValue--;
    //switch on motor
  }
  else
  {
    timer2.stop ();
    timer2.setCnt (0);
    butt.button = 0;
    //switch off motor
  }
}

INTERRUPT_HANDLER(TIM4_OVR_UIF, TIM4_OVR_UIF_vector)
{
  timer4.clearFlag();
  static uint8_t i=0;
  value.pars (encValue);
  if (i<value.getCount())
  {
    indicator.frame (value.getElement (4-i), i); 
    ++i;
  }
  else
  {
    i=0;
    indicator.frame (value.getElement (4-i), i);
  }
  
  if (!butt.button)
  {
    encoder.start ();
    button1.scan ();
    encValue = encoder.getValue ();
  }
  else
  {
    encoder.stop();
    timer2.start ();
  }
}


void buttonShortAction ();
void timer2_init ();
void timer4_init ();

int main()
{ 
  CLK->CKDIVR = 0;
  /*D.setOutPin (triac);
  
  CFG->GCR |= CFG_GCR_SWD;*/
  timer2_init ();
  timer4_init ();
  encoder.setValue (encValue);
  value.pars (encValue);
  button1.setShortLimit (10);
  button1.setshortPressAction (buttonShortAction);

  while (1)
  {
  }
}

void buttonShortAction ()
{
  butt.button = 1;
}

void timer2_init ()
{
  timer2.setPsc (Gtimer::div256);
  timer2.setArr (6250);
}

void timer4_init ()
{
  timer4.setPsc (Btimer::div128);
  timer4.setArr (125);
  timer4.interrupt (true);
}

