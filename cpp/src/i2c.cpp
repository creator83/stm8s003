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

uint8_t i2c::wr_reg (uint8_t adress, uint8_t reg, uint8_t *data, uint8_t l)
{
  //Ждем освобождения шины I2C
  delay_ms (10);
  while (I2C->SR3 & (1<<I2C_SR3_BUSY));
    
  //Генерация СТАРТ-посылки
  I2C->CR2 |= I2C_CR2_START;
  //Ждем установки бита SB
  delay_ms (1);
  while(!(I2C->SR1 & I2C_SR1_SB));
  
  
  //Записываем в регистр данных адрес ведомого устройства
  I2C->DR = adress & 0xFE;
  //Ждем подтверждения передачи адреса
  delay_ms (1);
  while(!(I2C->SR1 &I2C_SR1_ADDR));
  //Очистка бита ADDR чтением регистра SR3
  I2C_SR3;
  
  
  //Ждем освобождения регистра данных
  wait_event(!I2C_SR1_TXE, 1);
  //Отправляем адрес регистра
  I2C_DR = reg_addr;
  
  //Отправка данных
  while(length--){
    //Ждем освобождения регистра данных
    wait_event(!I2C_SR1_TXE, 1);
    //Отправляем адрес регистра
    I2C_DR = *data++;
  }
  
  //Ловим момент, когда DR освободился и данные попали в сдвиговый регистр
  wait_event(!(I2C_SR1_TXE && I2C_SR1_BTF), 1);
  
  //Посылаем СТОП-посылку
  I2C_CR2_STOP = 1;
  //Ждем выполнения условия СТОП
  wait_event(I2C_CR2_STOP, 1);
  
  return I2C_SUCCESS;
  return 0;
}


