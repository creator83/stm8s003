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
Buffer value;
uint8_t *eepromPtr;
SoftI2c driver (Gpio::B , 5, Gpio::B, 4);
Sht20 sensor (&driver);


//const uint8_t address = 0xD0;

void write (uint8_t add, uint8_t reg, uint8_t data);
int main()
{ 

  value.setFont (Buffer::Array_char);

  FLASH->DUKR = 0xAE;
  FLASH->DUKR = 0x56;
  eepromPtr = (uint8_t*)0x004000;
  
  
  /*//write
  driver.start ();
  driver.write (0xD0&0xFE);
  driver.waiteAck ();
  driver.write (0x02);
  driver.waiteAck ();
  driver.stop ();
  
  //read
  driver.start ();
  driver.write (0xD0&0xFE);
  driver.waiteAck ();
  driver.write (0x02);
  driver.waiteAck ();
  driver.restart ();
  driver.write (0xD0|0x01);
  driver.waiteAck ();
  driver.read ();  
  driver.generateNack ();
  driver.stop ();
  
  //SHT20
  driver.start ();
  driver.write (0x80&0xFE);
  driver.waiteAck ();
  driver.write (0xE3);
  driver.waiteAck ();
  driver.restart ();
  driver.write (0x80|0x01);
  driver.waiteAck ();
  driver.waiteStretching ();
  driver.readwStretching ();
  driver.generateAck ();
  driver.read ();
  driver.generateAck ();
  driver.read ();
  driver.generateNack ();
  driver.stop ();*/
  
   
  
  while (1)
  {
    sensor.getHummidity ();
    sensor.getTemperature ();
    delay_ms (100);
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
}*/

void sht30Write ()
{
  /*driverI2c.start ();
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
  driverI2c.setAddress (0x44|0x01);*/
  
  //temp MSB
}

void sht30Read ()
{
}



