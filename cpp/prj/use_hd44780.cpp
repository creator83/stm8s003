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
#include "senc.h"
#include "adc.h"



const char cursorChar[8] =
{ 
0x18,
0x1C,
0x1E,
0x1F,
0x1F,
0x1E,
0x1C,
0x18,
};

const char celsiusChar[8] =
{ 
0x18,
0x18,
0x00,
0x06,
0x09,
0x08,
0x09,
0x06,
};

Tact frq;
Hd44780 lcd;
Adc sensor(Adc::channel5);
Buffer value;


INTERRUPT_HANDLER(adc, ADC1_EOC_vector)
{
  sensor.clearEoc ();
  uint16_t data [10];
  uint16_t result=0;
  sensor.getBuffer (data);
  for (uint8_t i=0;i<10;++i) result += data [i];
  result<<=1;
  value.parsDec16 (result, 5);
  lcd.setPosition (0,2);
  lcd.sendString (value.getElement (0));   
}

int main()
{ 
  value.setFont (Buffer::Array_char);
  lcd.setPosition (1,0);
  lcd.sendString ("Hello");
  lcd.newChar (celsiusChar,1);
  lcd.setPosition (0,0); 
  lcd.data (1);
  
  lcd.setPosition (0,8);
  lcd.sendString ("Bye");
  
  lcd.setPosition (0,16);
  lcd.sendString ("Hey");

  lcd.setPosition (0,24);
  lcd.sendString ("really");
  
  lcd.setPosition (0,32);
  lcd.sendString ("HeHe");
  sensor.setContiniusMode ();
  sensor.setBuffer ();
  sensor.enableInterrupt ();
  sensor.start ();
  while (1)
  {
  }
}

