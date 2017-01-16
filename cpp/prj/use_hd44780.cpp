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


Tact frq;
Hd44780 lcd;
Adc sensor(Adc::channel5);
Buffer value;
uint8_t *eepromPtr;

const uint8_t address = 0xD0;



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

uint8_t data;

int main()
{ 
  I2c driverI2c ;
  value.setFont (Buffer::Array_char);
  lcd.setPosition (0,0);
  lcd.sendString ("ADC res");
  FLASH->DUKR = 0xAE;
  FLASH->DUKR = 0x56;
  eepromPtr = (uint8_t*)0x004000;
  //*eepromPtr = 50;
  sensor.setContiniusMode ();
  sensor.setBuffer ();
  sensor.enableInterrupt ();
  //sensor.start ();
  while (1)
  {
    driverI2c.rReg (address, 0x00, &data,1);
    
  }
}

