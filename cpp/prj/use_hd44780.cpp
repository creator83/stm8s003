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
#include "sht20.h"
#include "softi2c.h"

Tact frq;
Hd44780 lcd;
Adc sensor(Adc::channel2);
Atimer adcTrigger (16000);
Buffer value;

/*I2c driverI2c ;
Sht20 temp (&driverI2c);*/


//const uint8_t address = 0xD0;

uint16_t adcData [10];

INTERRUPT_HANDLER(adc, ADC1_EOC_vector)
{
  uint16_t result=0;

  //adcTrigger.clearFlag ();
  //TIM1->SR1 &= ~TIM1_SR1_TIF ;
  sensor.getBuffer (adcData);
  for (uint8_t i=0;i<10;++i)
  {
    result += adcData[i];
  }
  sensor.clearEoc ();
  result /= 10;
  value.parsDec16(result, 4);
  lcd.setPosition (1,1);
  lcd.sendString (4, value.getElement(1));
}

void setClock ();

uint8_t data;

void sht30Write ();
void sht30Read ();
int main()
{ 
  /*softi2c driver (Gpio::B, 5, Gpio::B, 4);
  value.setFont (Buffer::Array_char);
  lcd.setPosition (0,0);
  lcd.sendString ("CLOCK");
  FLASH->DUKR = 0xAE;
  FLASH->DUKR = 0x56;
  eepromPtr = (uint8_t*)0x004000;*/
  value.setFont (Buffer::Array_char);
  lcd.setPosition (0, 1);
  lcd.sendString ("ADC");
  adcTrigger.setArr (100);
  sensor.setBuffer ();
  //sensor.setTrigger (Adc::timer);
  sensor.setContiniusMode ();
  sensor.enableInterrupt ();
  sensor.start ();
  
  //adcTrigger.start ();
  //*eepromPtr = 50;
  /*sensor.setContiniusMode ();
  sensor.setBuffer ();
  sensor.enableInterrupt ();*/
  //setClock ();
  //sensor.start ();
  //for (uint8_t i=0;i<7;++i) driverI2c.rReg (0xD0, i, &data,1);
 // driverI2c.wByte (0xD0, 0x02, 0x18);
  //uint8_t temp,result;
  while (1)
  {
    /*temp.readTemperature ();
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
    
    delay_ms (100);*/
  }
}
/*
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

void sht30Write ()
{
  driverI2c.start ();
  driverI2c.setAddress (0x44);
  driverI2c.write (0x24);
  driverI2c.write (0x16);
  driverI2c.stop ();
  //проверка уровня сигнала
  driverI2c.start ();
  
  //set address NACK
  driverI2c.putData (0x44|0x01);
  driverI2c.readSR3();
  driverI2c.stop ();
  delay_ms (1);
  driverI2c.start ();
  driverI2c.setAddress (0x44|0x01);
  
  //temp MSB
}

void sht30Read ()
{
}*/



