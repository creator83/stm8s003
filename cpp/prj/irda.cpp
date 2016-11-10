#include "stm8s.h"
#include "gpio.h"
#include "delay.h"
#include "buffer.h"
#include "atimer.h"
#include "hd44780.h"
#include "btimer.h"

Buffer val;
Atimer irda (16);
Btimer timer4;
Hd44780 lcd;
/*

const uint16_t start1 = 9000;
const uint16_t start2 = 4000;

const uint16_t edge = 500;
const uint16_t edge1 = 700;


struct flags_
{
  unsigned start : 1;
  unsigned ready : 1;
}flag;

uint32_t code;*/
uint16_t falling;
uint16_t rising;

INTERRUPT_HANDLER(irdaChannel, TIM1_CAPCOM_CC1IF_vector)
{
  falling = TIM1->CCR1H << 8;
  falling |= TIM1->CCR1L;
  rising = TIM1->CCR2H << 8;
  rising |= TIM1->CCR2L;
  irda.clearFlag();
  val.parsDec16 (falling);
  lcd.setPosition (0, 5);
  lcd.sendString (val.getContent ());
  val.parsDec16 (rising);
  lcd.setPosition (1, 5);
  lcd.sendString (val.getContent ());
}
  /*
INTERRUPT_HANDLER(irdaChannel, TIM1_CAPCOM_CC1IF_vector)
{
  irda.clearFlag();
  uint16_t fallingEdge, risingEdge;
  int8_t count;
  fallingEdge = TIM1->CCR1H << 8;
  fallingEdge |= TIM1->CCR1L;
  risingEdge = TIM1->CCR2H << 8;
  risingEdge |= TIM1->CCR2L;
  if (fallingEdge>start1&&risingEdge>start2) 
  {
    flag.start = 1;
    flag.ready = 0;
    count = 31;
    code = 0;
  }
  if (flag.start)
  {
    fallingEdge = TIM1->CCR1H << 8;
    fallingEdge |= TIM1->CCR1L;
    risingEdge = TIM1->CCR2H << 8;
    risingEdge |= TIM1->CCR2L;
    if (fallingEdge>edge&&risingEdge>edge1) 
    {
      code |= 1 << count;
      --count;
    }
    else
    {
      code |= 0 << count;
      --count;
    }
  }
  if (count<0)
  {
    flag.start = 0;
    flag.ready = 1;
  }
}

INTERRUPT_HANDLER(mainLoop, TIM4_OVR_UIF_vector)
{
  timer4.clearFlag ();
  if (flag.ready)
  {
    val.parsHex32 (code);
    lcd.setPosition (1, 2);
    lcd.sendString (val.getArray());
    flag.ready = 0;
  }
}*/

void timer4_init ();


int main()
{ 
  CLK->CKDIVR = 0;
  val.setFont (Buffer::Array_char);
  irda.pwmInputMode();
  

  lcd.setPosition (0, 0);
  lcd.sendString ("CCR1");
  lcd.setPosition (1, 0);
  lcd.sendString ("CCR2");
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
  timer4.start ();
}

