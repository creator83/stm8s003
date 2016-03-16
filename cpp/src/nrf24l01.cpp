#include "nrf24l01.h"



nrf24l01::nrf24l01 ()
:spi1 (spi::div16) , pin (Gpio::D) , irq (3 , irq_ , intrpt::falling)
{
  pin.setOutPin (ce_);
  init ();
}

uint8_t nrf24l01::wr_reg (uint8_t r_n_reg , uint8_t mask1, uint8_t mask2)
{
  __disable_interrupt ();
  uint8_t reg = 0;
  spi1.Clear_CS();
  spi1.transfer (mask1 + r_n_reg);
  reg = spi1.transfer (mask2);
  spi1.Set_CS ();
  __enable_interrupt ();
  return reg;
}

uint8_t nrf24l01::command (uint8_t mask)
{
  __disable_interrupt ();
  uint8_t status = 0;
  spi1.Clear_CS();
  status = spi1.transfer (mask);
  spi1.Set_CS ();
  __enable_interrupt ();  
  return status;
}

void nrf24l01::set_bit (uint8_t reg_ister, uint8_t register_bit, uint8_t W)
{
  uint8_t buf;
  buf = wr_reg (reg_ister , R_REGISTER , NOP);
  if (W) buf=buf|(1<<register_bit);
  else buf=buf&(~(1<<register_bit));
  wr_reg (reg_ister , W_REGISTER , buf);
}

void nrf24l01::mode (state st)
{
  //переход в режим приемника
  pin.clearPin (ce_);
  //переключаемся между режимами меняя PRIM_RX бит
  set_bit (CONFIG , PRIM_RX , st);
  //переходим в один из режимов
  pin.setPin (ce_);
  delay_us (15);
  if (!st) pin.clearPin (ce_);
  delay_us (150);  
}

void nrf24l01::write_data (uint8_t data)
{
  spi1.Clear_CS ();
  spi1.transfer (W_TX_PAYLOAD);
  spi1.transfer (data);
  spi1.Set_CS ();
}

void nrf24l01::transmit (uint8_t data)
{
  write_data (data);
  mode (transmitter);
  mode (receiver);
}

uint8_t nrf24l01::read_data ()
{
  uint8_t data;
  spi1.Clear_CS ();
  spi1.transfer (R_RX_PAYLOAD);
  data = spi1.transfer (NOP);
  spi1.Set_CS ();
  return data;
}
    

void nrf24l01::init (uint8_t m )
{
  spi1.Set_CS ();
  pin.clearPin (ce_);
  delay_ms (15);
  set_bit (RX_PW_P0,0,1);
  
  //определяем прерывания (вкл/выключаем) их 3-и по умолчанию включены все!
  set_bit (CONFIG , MASK_MAX_RT , m&(1 << 0));
  set_bit (CONFIG , MASK_TX_DS , m&(1 << 1));
  set_bit (CONFIG , MASK_RX_DR , m&(1 << 2));
  
  //устанавливаем бит вкл в единицу (включаем модуль)
  set_bit (CONFIG , PWR_UP , 1);
  delay_ms (2);
  
}
