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

Btimer timer4;
Gtimer timer2;
Nrf24l01 radio;
SoftI2c i2cDriver (Gpio::A, 1, Gpio::A, 2);
Sht20 sensor (&i2cDriver);
Pin ledOk (Gpio::A, 2, Gpio::lowSpeed);
Pin ledErr (Gpio::A, 3, Gpio::lowSpeed);
union 
{
  uint16_t data16 [2];
  uint8_t data8 [4];
}data;

INTERRUPT_HANDLER(radioInterrupt, EXTI0_vector)
{
  uint8_t status = radio.command (NOP);
  if (status & 1 << TX_DS)
  {
    ledOk.set ();
  }
  else if (status & 1 << MAX_RT)
  {
    ledErr.set();
  }
  radio.writeRegister (STATUS, status);
}

INTERRUPT_HANDLER(backCounter, TIM2_OVR_UIF_vector)
{
  timer2.clearFlag();
}

INTERRUPT_HANDLER(TIM4_OVR_UIF, TIM4_OVR_UIF_vector)
{
  timer4.clearFlag();
  data.data16[0] = sensor.getHummidity ();
  data.data16[1] = sensor.getTemperature ();
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
  timer4_init ();

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

void timer4_init ()
{
  timer4.setPsc (Btimer::div128);
  timer4.setArr (125);
  enableInterrupts();
  timer4.interrupt (true);
  timer4.start ();
}

