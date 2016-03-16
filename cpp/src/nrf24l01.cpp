#include "nrf24l01.h"



nrf24l01::nrf24l01 ()
:spi1 (spi::div64) , pin (Gpio::D) //, irq (3 , irq_ , intrpt::falling)
{
  pin.setOutPin (ce_);
  init ();
}
uint8_t nrf24l01::w_reg (uint8_t reg , uint8_t val)
{
  spi1.Clear_CS();
  uint8_t status = spi1.exchange (W_REGISTER|reg);
  spi1.transmit (val);
  spi1.Set_CS ();
  return status;
}

uint8_t nrf24l01::r_reg (uint8_t reg)
{
  spi1.Clear_CS();
  spi1.transmit (R_REGISTER|reg);
  uint8_t status = spi1.exchange (NOP);
  spi1.Set_CS ();
  return status;  
}

void nrf24l01::change_bit (uint8_t reg, uint8_t bit, bool state)
{
  uint8_t val = r_reg (reg);
  
  if (state) val = val|(1 << bit);
  else val = val&(~(1 << bit));
  w_reg (reg, val);
}

void nrf24l01::init ()
{
  delay_ms (15);
  change_bit (CONFIG, PWR_UP, 1);
  delay_ms (5);
}
