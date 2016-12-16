#include "i2c.h"

init I2c::init_mode [2]= {};

I2c::I2c(mode m, speed s)
:sda (Gpio::B, SDA,Gpio::Pullup), scl (Gpio::B, SCL,Gpio::Pullup)
{
  speed_mode = s;
  (this->*(I2c::init_mode[m]))(speed_mode);
}

void I2c::init_slave ()
{
  
}

void I2c::init_master (uint8_t speed)
{
  unsigned long int ccr;
  //Отключаем I2C
  I2C->CR1 &= ~I2C_CR1_PE;
  //В стандартном режиме скорость I2C max = 100 кбит/с
  //Выбираем стандартный режим 
   I2C->CCRH &= ~I2C_CCRH_FS;
  //CCR = Fmaster/2*Fiic= MHz/2*100kHz
  ccr = Tact::get_frq()/(2*speed_ [speed]);
  
  
  //Set Maximum Rise Time: 1000ns max in Standard Mode
  //= [1000ns/(1/InputClockFrequencyMHz.10e6)]+1
  //= InputClockFrequencyMHz+1
  I2C->TRISER = Tact::get_frq()+1;
  I2C->CCRL = ccr & 0xFF;

  I2C->CCRH &= ~I2C_CCRH_CCR;
  I2C->CCRH = (ccr >> 8) & 0x0F;
  //Включаем I2C
  I2C->CR1 |= I2C_CR1_PE;
  //Разрешаем подтверждение в конце посылки
  
}

void I2c::start ()
{
  //Генерация СТАРТ-посылки
  I2C->CR2 |= I2C_CR2_START;
 }

void I2c::stop ()
{
  I2C->CR2 |= I2C_CR2_STOP;
}

void I2c::putData (uint8_t val)
{
  I2C->DR = val;
}

uint8_t I2c::getData ()
{
  return I2C->DR;
}

bool I2c::wReg (uint8_t adress, uint8_t reg, uint8_t *data, uint8_t l)
{
  start ();
  //Записываем в регистр данных адрес ведомого устройства
  putData (adress & 0xFE);
  
  while(!flagAddr());
  //Очистка бита ADDR чтением регистра SR3
  uint8_t temp = I2C->SR3;
  
  
  //Ждем освобождения регистра данных
  while(!flagTxe());
  //Отправляем адрес регистра
  putData (reg);
  
  //Отправка данных
  while(l--){
    //Ждем освобождения регистра данных
    while(!flagTxe());
    //Отправляем адрес регистра
    putData (*data++);
  }
  
  //Ловим момент, когда DR освободился и данные попали в сдвиговый регистр
  while(!(flagTxe() && flagBtf ()));
  
  //Посылаем СТОП-посылку
  stop ();
  //Ждем выполнения условия СТОП
  while(flagStop ());
  return true;
}

bool I2c::flagBusy ()
{
  return I2C->SR3 & I2C_SR3_BUSY;
}

bool I2c::flagTxe ()
{
  return I2C->SR1 & I2C_SR1_TXE;
}

bool I2c::flagRxne ()
{
  return I2C->SR1&I2C_SR1_RXNE;
}

bool I2c::flagBtf ()
{
  return I2C->SR1 & I2C_SR1_BTF;
}

bool I2c::flagSb ()
{
  return I2C->SR1 & I2C_SR1_SB;
}

bool I2c::flagAddr ()
{
  return I2C->SR1 &I2C_SR1_ADDR;
}

bool I2c::flagStop ()
{
  return I2C->CR2&I2C_CR2_STOP;
}


