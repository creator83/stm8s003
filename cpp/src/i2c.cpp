#include "i2c.h"

init I2c::init_mode [2]= {};

I2c::I2c()
//:sda (Gpio::B, SDA,Gpio::Pullup), scl (Gpio::B, SCL,Gpio::Pullup)
{
  //speed_mode = s;
  GPIOB->ODR |= 1 << 4;
  GPIOB->ODR |= 1 << 5;
  //(this->*(I2c::init_mode[m]))(speed_mode);
  init_master ();
}

void I2c::init_slave ()
{
  
}

void I2c::init_master ()
{
  //Отключаем I2C
  I2C->FREQR = Tact::get_frq();
  I2C->CR1 &= ~I2C_CR1_PE;
  //В стандартном режиме скорость I2C max = 100 кбит/с
  //Выбираем стандартный режим 
   I2C->CCRH &= ~I2C_CCRH_FS;

  //CCR = Fmaster/2*Fiic= 12MHz/2*100kHz
  uint16_t ccr = Tact::get_frq()*1000/(2*100);

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

void I2c::putData (uint8_t val)
{
  I2C->DR = val;
}

uint8_t I2c::getData ()
{
  return I2C->DR;
}

void I2c::start ()
{
  //Ждем освобождения шины I2C
//  delay_ms (10);
  while (flagBusy());   
  //Генерация СТАРТ-посылки
  I2C->CR2 |= I2C_CR2_START;
  while (!flagSb());
  
}

void I2c::restart ()
{
  I2C->CR2 |= I2C_CR2_START;
  while (!flagSb());
}

void I2c::stop ()
{
  I2C->CR2 |= I2C_CR2_STOP;
  while (!flagStop());
}

void I2c::setAddress (uint8_t addr)
{
  putData (addr);
  while(!flagAddr());
  readSR3();
}


void I2c::write (uint8_t val)
{
    while (!flagTxe());
    putData (val);
}

bool I2c::wReg (uint8_t address, uint8_t reg, uint8_t *data, uint8_t l)
{
  
  start ();
  //Записываем в регистр данных адрес ведомого устройства
  setAddress (address & wOperation);
  
  write (reg);
  
  //Отправка данных
  while(l--){
    write (*data++);
  }
  
  while (!(flagTxe()&&flagBtf()));
  
  //Посылаем СТОП-посылку
  stop ();
  return true;
}

bool I2c::rReg (uint8_t address, uint8_t reg, uint8_t *data, uint8_t length)
{
  start ();
  setAddress (address&I2c::wOperation);
  write (reg);
  while (!(flagTxe()&&flagBtf()));
  restart ();
  putData (address|I2c::rOperation);
  if (length==1)
  {
    I2C->CR2 &= ~I2C_CR2_ACK;
    while (!flagAddr());
    disableInterrupts();
    readSR3();
    I2C->CR2 |= I2C_CR2_STOP;
    enableInterrupts();
    while (!flagRxne());
    *data = I2C->DR;
  }
  else if(length == 2){
    //Бит который разрешает NACK на следующем принятом байте
    I2C->CR2 |= I2C_CR2_POS;
    //Ждем подтверждения передачи адреса
    while(!flagAddr());
    //Заплатка из Errata
    __disable_interrupt();
    //Очистка бита ADDR чтением регистра SR3
    readSR3 ();
    //Запрещаем подтверждение в конце посылки
    I2C->CR2 &= ~I2C_CR2_ACK;
    //Заплатка из Errata
    __enable_interrupt();
    //Ждем момента, когда первый байт окажется в DR,
    //а второй в сдвиговом регистре
    while(!flagBtf());
     
    //Заплатка из Errata
    __disable_interrupt();
    //Устанавлием бит STOP
    I2C->CR2 |= I2C_CR2_STOP;
    //Читаем принятые байты
    *data++ = I2C->DR;
    //Заплатка из Errata
    __enable_interrupt();
    *data = I2C->DR;
  } 

  else if(length > 2){
    //Ждем подтверждения передачи адреса
    while(!flagAddr());
     
    //Заплатка из Errata
    __disable_interrupt();
     
    //Очистка бита ADDR чтением регистра SR3
    readSR3();
     
    //Заплатка из Errata
    __enable_interrupt();
     
    while(length-- > 3){
      //Ожидаем появления данных в DR и сдвиговом регистре
      while(!flagBtf());
      //Читаем принятый байт из DR
      *data++ = I2C->DR;
    }
     
    //Осталось принять 3 последних байта
    //Ждем, когда в DR окажется N-2 байт, а в сдвиговом регистре
    //окажется N-1 байт
    while(!flagBtf());
    //Запрещаем подтверждение в конце посылки
    I2C->CR2 &= ~I2C_CR2_ACK;
    //Заплатка из Errata
    __disable_interrupt();
    //Читаем N-2 байт из RD, тем самым позволяя принять в сдвиговый
    //регистр байт N, но теперь в конце приема отправится посылка NACK
    *data++ = I2C->DR;
    //Посылка STOP
    I2C->CR2 |= I2C_CR2_STOP;
    //Читаем N-1 байт
    *data++ = I2C->DR;
    //Заплатка из Errata
    __enable_interrupt();
    //Ждем, когда N-й байт попадет в DR из сдвигового регистра
    while(!flagRxne());
    //Читаем N байт
    *data++ = I2C->DR;
  }

  //Ждем отправки СТОП посылки
  while(I2C->CR2&I2C_CR2_STOP);
  
  //Сбрасывает бит POS, если вдруг он был установлен
  I2C->CR2 &= ~I2C_CR2_POS;
  return true;

}

void I2c::wByte (uint8_t address, uint8_t reg, uint8_t data)
{
   start ();
  //Записываем в регистр данных адрес ведомого устройства
  setAddress (address & wOperation);
  
  write (reg);
  
  //Отправка данных
  write (data);
  while (!(flagTxe()&&flagBtf()));
  //Посылаем СТОП-посылку
  stop ();
}

uint8_t I2c::rByte (uint8_t address, uint8_t reg)
{
  start ();
  setAddress (address&I2c::wOperation);
  write (reg);
  while (!(flagTxe()&&flagBtf()));
  restart ();
  putData (address|I2c::rOperation);

  I2C->CR2 &= ~I2C_CR2_ACK;
  while (!flagAddr());
  disableInterrupts();
  readSR3();
  I2C->CR2 |= I2C_CR2_STOP;
  enableInterrupts();
  while (!flagRxne());
  uint8_t data = I2C->DR;

  //Ждем отправки СТОП посылки
  while(I2C->CR2&I2C_CR2_STOP);
  
  //Сбрасывает бит POS, если вдруг он был установлен
  I2C->CR2 &= ~I2C_CR2_POS;
  return data;
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

uint8_t I2c::readSR3 ()
{
  return I2C->SR3;
}
