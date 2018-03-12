#include "stm8s.h"
#include "pin.h"
#include "delay.h"
#include "buffer.h"
#include "atimer.h"
#include "btimer.h"
#include "gtimer.h"
#include "sht20.h"
#include "softi2c.h"
#include "nrf24l01.h"
#include "uart.h"

Btimer timer4;
Gtimer timer2;
SoftI2c i2cDriver (Gpio::A, 3, Gpio::A, 2);
Sht20 sensor (&i2cDriver);
Uart uart1 (Uart::b9600);
const uint8_t csPin = 4;
const uint8_t cePin = 3;
const uint8_t irqPin = 1; 
Pin cs (Gpio::C, csPin, Gpio::lowSpeed);
Pin ce (Gpio::C, cePin, Gpio::lowSpeed);
Intrpt irq (Gpio::A, irqPin, Intrpt::falling);
Nrf24l01 radio(cs, ce, irq);

const uint8_t hummidity = 0;
const uint8_t temperature = 1;

union 
{
  uint16_t data16 [2];
  uint8_t data8 [4];
}data;

INTERRUPT_HANDLER(radioInterrupt, EXTI0_vector)
{
  uint8_t status = radio.getStatus();
  if (status & 1 << TX_DS)
  {
    //ledOk.set ();
  }
  else if (status & 1 << MAX_RT)
  {
    //ledErr.set();
  }
  radio.writeRegister (STATUS, status);
}

INTERRUPT_HANDLER(period, TIM2_OVR_UIF_vector)
{
  timer2.clearFlag();
  data.data16[hummidity] = sensor.getHummidity ();
  data.data16[temperature] = sensor.getTemperature ();
  radio.sendData (data.data8, 4);
}


void buttonShortAction ();
void shortActionEnc ();
void longActionEnc ();

void timer2_init ();
void timer4_init ();

int main()
{ 
  CLK->CKDIVR = 0;
  timer2_init ();

  while (1)
  {
  }
}

void timer2_init ()
{
  timer2.setPsc (Gtimer::div256);
  timer2.setArr (6250);
  timer2.interrupt (true);
}

