#include "i2c.h"

const init i2c::init_mode [2]= {};

i2c::i2c(mode m, speed s)
:pin (Gpio::B)
{
  speed_mode = s;
  pin.setInPin (SCL, Gpio::Pullup);
  pin.setInPin (SDA, Gpio::Pullup);
  (this->*(i2c::init_mode[m]))(speed_mode);
}

void i2c::init_slave ()
{
  
}

void i2c::init_master (uint8_t speed)
{
  unsigned long int ccr;
  //Отключаем I2C
  I2C->CR1 &= ~I2C_CR1_PE;
  //В стандартном режиме скорость I2C max = 100 кбит/с
  //Выбираем стандартный режим 
   I2C->CCRH &= ~I2C_CCRH_FS;
  //CCR = Fmaster/2*Fiic= 12MHz/2*100kHz
  ccr = tact::get_frq()/(2*speed_ [speed]);
  
  
  //Set Maximum Rise Time: 1000ns max in Standard Mode
  //= [1000ns/(1/InputClockFrequencyMHz.10e6)]+1
  //= InputClockFrequencyMHz+1
  I2C->TRISER = 12+1;
  I2C->CCRL = ccr & 0xFF;

  I2C->CCRH &= ~I2C_CCRH_CCR;
  I2C->CCRH = (ccr >> 8) & 0x0F;
  //Включаем I2C
  I2C->CR1 |= I2C_CR1_PE;
  //Разрешаем подтверждение в конце посылки
  I2C->CR2 |= I2C_CR2_ACK;
}