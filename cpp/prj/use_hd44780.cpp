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
#include "i2c.h"
#include "ds1307.h"

Tact frq;
Hd44780 lcd;
Adc sensor(Adc::channel5);
Buffer value;
uint8_t *eepromPtr;
I2c driverI2c ;
Ds1307 clock (&driverI2c);

//const uint8_t address = 0xD0;



INTERRUPT_HANDLER(adc, ADC1_EOC_vector)
{
  sensor.clearEoc ();
  uint16_t data [10];
  uint16_t result=0;
  sensor.getBuffer (data);
  for (uint8_t i=0;i<10;++i) result += data [i];
  result<<=1;
  value.parsDec16 (result, 5);
  lcd.setPosition (1,0);
  lcd.sendString (value.getElement (0)); 
  value.parsDec16 (*eepromPtr, 2);
  lcd.setPosition (1,6);
  lcd.sendString (value.getElement (3)); 
}

void setClock ();

uint8_t data;
int main()
{ 
  
  value.setFont (Buffer::Array_char);
  lcd.setPosition (0,0);
  lcd.sendString ("CLOCK");
  FLASH->DUKR = 0xAE;
  FLASH->DUKR = 0x56;
  eepromPtr = (uint8_t*)0x004000;
  //*eepromPtr = 50;
  sensor.setContiniusMode ();
  sensor.setBuffer ();
  sensor.enableInterrupt ();
  //setClock ();
  //sensor.start ();
  for (uint8_t i=0;i<7;++i) driverI2c.rReg (0xD0, i, &data,1);
  uint8_t temp,result;
  while (1)
  {
    clock.update ();
    temp = clock.getHours ();
    result = (temp&valueMask::Dhours)>> 4;
    lcd.setPosition (1,0);
    lcd.data (Buffer::Array_char [result]);
    result = temp&valueMask::hours;
    lcd.data (Buffer::Array_char [result]);
    
    lcd.data (':');
    
    temp = clock.getMinutes ();
    result = (temp&valueMask::Dminutes)>> 4;
    lcd.data (Buffer::Array_char [result]);
    result = temp&valueMask::minutes;
    lcd.data (Buffer::Array_char [result]);
    
    lcd.data (':');
    
    temp = clock.getSeconds ();
    result = (temp&valueMask::Dseconds)>> 4;
    lcd.data (Buffer::Array_char [result]);
    result = temp&valueMask::seconds;
    lcd.data (Buffer::Array_char [result]);
    
    delay_ms (100);
  }
}

void setClock ()
{
  clock.stop ();
  clock.setSeconds (0);
  clock.setMinutes (22);
  clock.setHours (0);
  clock.setDay (1);
  clock.setDate (17);
  clock.setMonth (0);
  clock.setYear (17);
  clock.setData ();
  clock.start ();
}
