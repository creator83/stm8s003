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
/*
Buffer value ;

Button button1 (Gpio::A, 1);
Button buttonEncoder (Gpio::B, 1);
Qenc encoder (999);

Btimer timer4;
Gtimer timer2;*/
Atimer timer1;
uint16_t encValue = 65;
Gpio D (Gpio::D);
//Gpio C (Gpio::C);
uint16_t encMemVal [3] = {65, 135, 20};
const char triac = 1;
Segled indicator (3);
/*

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
  //value.pars (encoder.getValue());
  if (i<3)
  {
    indicator.frame (value.getArray(), i); 
    ++i;
  }
  else
  {
    i=0;
    indicator.frame (value.getArray(), i);
  }
  
  if (!butt.button)
  {
    encoder.start ();
    button1.scan ();
  }
  else
  {
    encoder.stop();
    timer2.start ();
  }
}
*/

void buttonShortAction ();
void timer2_init ();
void timer1_init ();
void timer4_init ();

int main()
{ 
  CLK->CKDIVR = 0;
  D.setOutPin (triac);
  GPIOC->ODR = 1 << 6;
  timer1.pwmMode (Atimer::channel1);
  timer1.setPsc (32);
  timer1.setArr (100);
  timer1.setChannelValue (90);
  
 /* CFG->GCR |= CFG_GCR_SWD;
  encoder.setValue (encValue);
  value.pars (356);
  
  timer4.setPsc (Btimer::div128);
  timer4.setArr (125);
  //timer4.interrupt (true);
  
  timer2.setPsc (Gtimer::div256);
  timer2.setArr (6250);*/
  //timer2.interrupt (true);
  /*
   
  //
  
  button1.setShortLimit (10);
  button1.setshortPressAction (buttonShortAction);*/
  char val = 0xFF;
  indicator.frame (&val, 1);

  while (1)
  {
    for (uint8_t i=0;i<100;++i)
    {
      timer1.setChannelValue (i);
      delay_ms (10);
    }
  /*   D.ChangePinState (triac);
    delay_ms (3000);
   for (uint8_t i=0;i<3;++i)
    {
      indicator.frame (value.getArray(), i);
      delay_ms (1);
    }*/
  }
}
/*
void buttonShortAction ()
{
  butt.button = 1;
}

void timer2_init ()
{
  
}

void timer1_init ()
{
  
}

void timer4_init ()
{
  
}*/
