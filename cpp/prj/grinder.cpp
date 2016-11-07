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

Button button1 (Gpio::B, 5);
Button buttonEncoder (Gpio::B, 4);
Qenc encoder (999);

Btimer timer4;
Gtimer timer2;

uint16_t encTemp;

/*
uint8_t *baseAddress = (uint8_t *) 0x4000;
uint8_t *baseCurrent;
uint8_t Val [3];*/

Gpio D (Gpio::D);
const char triac = 1;

uint16_t encMemVal [3] = {65, 185, 5};
//uint8_t * memaddres [3] = {(uint8_t *) 0x4001,(uint8_t *)  0x4002, (uint8_t *) 0x4003};
volatile uint8_t encPosition = 1;

Segled indicator (3);


struct flags_
{
  unsigned button : 1;
  unsigned longEnc : 1;
}flag;

INTERRUPT_HANDLER(backCounter, TIM2_OVR_UIF_vector)
{
  timer2.clearFlag();
  if (encTemp)
  {
    encTemp--;
    D.setPin (triac);
  }
  else
  { 
    flag.button = 0;
    encTemp = encMemVal [encPosition];
    timer2.stop ();
    timer2.setCnt (0);
    D.clearPin (triac);
  }
}

INTERRUPT_HANDLER(TIM4_OVR_UIF, TIM4_OVR_UIF_vector)
{
  timer4.clearFlag();
  static uint8_t i=0;
  value.pars (encTemp);
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
   
  if (!flag.button)
  {
    encoder.start ();
    encMemVal [encPosition] = encoder.getValue ();
    encTemp = encMemVal [encPosition];
    button1.scan ();
    buttonEncoder.scan ();
  }
  else
  {
    encoder.stop();
    timer2.start ();
  }
}


void buttonShortAction ();
void shortActionEnc ();
void longActionEnc ();

void timer2_init ();
void timer4_init ();

int main()
{ 
  CLK->CKDIVR = 0;
/*  FLASH->DUKR = 0x56;
  FLASH->DUKR = 0xAE;
  
  baseCurrent = baseAddress;
  *baseCurrent = 250;
  *baseCurrent++ = 15;
  *baseCurrent++ = 148;
   baseCurrent = baseAddress;
   Val [0] = *baseCurrent;
   Val [1] = *baseCurrent++;
   Val [2] = *baseCurrent++;
 baseCurrent = baseAddress+1;
  *baseCurrent++ = 243;
    encTemp =  *baseCurrent;*/
  
 
  CFG->GCR |= CFG_GCR_SWD;
  D.setOutPin (triac);
  
  button1.setShortLimit (20);
  button1.setshortPressAction (buttonShortAction);

  buttonEncoder.setShortLimit (20);
  buttonEncoder.setshortPressAction (shortActionEnc);
  
  timer2_init ();
  timer4_init ();
  encTemp = encMemVal [encPosition];
  encoder.setValue (encMemVal [encPosition]);
 
  while (1)
  {
  }
}

void buttonShortAction ()
{
  flag.button = 1;
}

void shortActionEnc ()
{
 
  encPosition++;
  if (encPosition>2)
  {
    encPosition = 0;
  }

  encoder.setValue (encMemVal [encPosition]);

}
void longActionEnc ()
{
  if (flag.longEnc) flag.longEnc = 0;
  else flag.longEnc = 1;
}

void timer2_init ()
{
  timer2.setPsc (Gtimer::div256);
  timer2.setArr (6250);
  timer2.interrupt (true);
}

void timer4_init ()
{
  timer4.setPsc (Btimer::div128);
  timer4.setArr (125);
  enableInterrupts();
  timer4.interrupt (true);
  timer4.start ();
}

