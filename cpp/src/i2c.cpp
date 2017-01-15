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
  unsigned long int ccr;
  //Отключаем I2C
  I2C->FREQR = Tact::get_frq();
  I2C->CR1 &= ~I2C_CR1_PE;
  //В стандартном режиме скорость I2C max = 100 кбит/с
  //Выбираем стандартный режим 
   I2C->CCRH &= ~I2C_CCRH_FS;
  //CCR = Fmaster/2*Fiic= 12MHz/2*100kHz
  ccr = Tact::get_frq()*1000/(2*100);
  
  
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
  I2C->CR2 |= I2C_CR2_ACK;
}

void I2c::putData (uint8_t &val)
{
  I2C->DR = val;
}

void I2c::start ()
{
  //Ждем освобождения шины I2C
//  delay_ms (10);
  while (I2C->SR3 & I2C_SR3_BUSY);
    
  //Генерация СТАРТ-посылки
  I2C->CR2 |= I2C_CR2_START;
}

void I2c::restart ()
{
  
}

void I2c::stop ()
{
  
}

void I2c::write (uint8_t val)
{
  
}

uint8_t I2c::write (bool ack)
{
  
}

bool I2c::wReg (uint8_t adress, uint8_t reg, uint8_t *data, uint8_t l)
{
  //Ждем освобождения шины I2C
  delay_ms (10);
  while (I2C->SR3 & I2C_SR3_BUSY);
    
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
  uint8_t temp = I2C->SR3;
  
  
  //Ждем освобождения регистра данных
  while(!I2C->SR1& I2C_SR1_TXE);
  //Отправляем адрес регистра
  I2C->DR = reg;
  
  //Отправка данных
  while(l--){
    //Ждем освобождения регистра данных
    while(!I2C->SR1& I2C_SR1_TXE);
    //Отправляем адрес регистра
    I2C->DR = *data++;
  }
  
  //Ловим момент, когда DR освободился и данные попали в сдвиговый регистр
  while(!(I2C->SR1 & I2C_SR1_TXE && I2C->SR1 & I2C_SR1_BTF));
  
  //Посылаем СТОП-посылку
  I2C->CR2 |= I2C_CR2_STOP;
  //Ждем выполнения условия СТОП
  while(I2C->CR2&I2C_CR2_STOP);
  return true;
}

bool I2c::rReg (uint8_t address, uint8_t reg, uint8_t *data, uint8_t length)
{
   //Ждем освобождения шины I2C
 while (I2C->SR3 & I2C_SR3_BUSY);
     
  //Разрешаем подтверждение в конце посылки
  I2C->CR2 |= I2C_CR2_ACK;
   
  //Генерация СТАРТ-посылки
  I2C->CR2 |= I2C_CR2_START;
  //Ждем установки бита SB
  while(!I2C->SR1 & I2C_SR1_SB);
   
  //Записываем в регистр данных адрес ведомого устройства
  I2C->DR = address & 0xFE;
  //Ждем подтверждения передачи адреса
  while(!I2C->SR1 &I2C_SR1_ADDR);
  //Очистка бита ADDR чтением регистра SR3
  uint8_t temp = I2C->SR3;
   
  //Ждем освобождения регистра данных RD
  while(!I2C->SR1& I2C_SR1_TXE);
   
  //Передаем адрес регистра slave-устройства, который хотим прочитать
  I2C->DR = reg;
  //Ловим момент, когда DR освободился и данные попали в сдвиговый регистр
  while(!(I2C->SR1 & I2C_SR1_TXE && I2C->SR1 & I2C_SR1_BTF));
   
  //Генерация СТАРТ-посылки (рестарт)
  I2C->CR2 |= I2C_CR2_START;
  //Ждем установки бита SB
  while(!(I2C->SR1 & I2C_SR1_SB));
   
  //Записываем в регистр данных адрес ведомого устройства и переходим
  //в режим чтения (установкой младшего бита в 1)
  I2C->DR = address | 0x01;
   
  //Дальше алгоритм зависит от количества принимаемых байт
  //N=1
  if(length == 1){
    //Запрещаем подтверждение в конце посылки
    I2C->CR2 &= ~I2C_CR2_ACK;
    //Ждем подтверждения передачи адреса
    while(!(I2C->SR1 &I2C_SR1_ADDR));
     
    //Заплатка из Errata
    __disable_interrupt();
    //Очистка бита ADDR чтением регистра SR3
    uint8_t temp = I2C->SR3;
     
    //Устанавлием бит STOP
    I2C->CR2 |= I2C_CR2_STOP;
    //Заплатка из Errata
    __enable_interrupt();
     
    //Ждем прихода данных в RD
    while(!(I2C->SR1 &I2C_SR1_RXNE));
     
    //Читаем принятый байт
    *data = I2C->DR;
  } 
  //N=2
  else if(length == 2){
    //Бит который разрешает NACK на следующем принятом байте
    I2C->CR2 |= I2C_CR2_POS;
    //Ждем подтверждения передачи адреса
    while(!(I2C->SR1 &I2C_SR1_ADDR));
    //Заплатка из Errata
    __disable_interrupt();
    //Очистка бита ADDR чтением регистра SR3
    uint8_t temp = I2C->SR3;
    //Запрещаем подтверждение в конце посылки
    I2C->CR2 &= ~I2C_CR2_ACK;
    //Заплатка из Errata
    __enable_interrupt();
    //Ждем момента, когда первый байт окажется в DR,
    //а второй в сдвиговом регистре
    while(!I2C->SR1 & I2C_SR1_BTF);
     
    //Заплатка из Errata
    __disable_interrupt();
    //Устанавлием бит STOP
    I2C->CR2 |= I2C_CR2_STOP;
    //Читаем принятые байты
    *data++ = I2C->DR;
    //Заплатка из Errata
    __enable_interrupt();
    *data = I2C->DR;
  } /*
  //N>2
  else if(length > 2){
    //Ждем подтверждения передачи адреса
    while(!(I2C->SR1 &I2C_SR1_ADDR));
     
    //Заплатка из Errata
    __disable_interrupt();
     
    //Очистка бита ADDR чтением регистра SR3
    uint8_t temp = I2C->SR3;
     
    //Заплатка из Errata
    __enable_interrupt();
     
    while(length-- > 3){
      //Ожидаем появления данных в DR и сдвиговом регистре
      while(!I2C->SR1 & I2C_SR1_BTF);
      //Читаем принятый байт из DR
      *data++ = I2C->DR;
    }
    //Время таймаута вышло
    if(!tmo) return I2C_TIMEOUT;
     
    //Осталось принять 3 последних байта
    //Ждем, когда в DR окажется N-2 байт, а в сдвиговом регистре
    //окажется N-1 байт
    wait_event(!I2C_SR1_BTF, 1);
    //Запрещаем подтверждение в конце посылки
    I2C_CR2_ACK = 0;
    //Заплатка из Errata
    __disable_interrupt();
    //Читаем N-2 байт из RD, тем самым позволяя принять в сдвиговый
    //регистр байт N, но теперь в конце приема отправится посылка NACK
    *data++ = I2C_DR;
    //Посылка STOP
    I2C_CR2_STOP = 1;
    //Читаем N-1 байт
    *data++ = I2C_DR;
    //Заплатка из Errata
    __enable_interrupt();
    //Ждем, когда N-й байт попадет в DR из сдвигового регистра
    wait_event(!I2C_SR1_RXNE, 1);
    //Читаем N байт
    *data++ = I2C_DR;
  }
   */
  //Ждем отправки СТОП посылки
  while(I2C->CR2&I2C_CR2_STOP);
  
  //Сбрасывает бит POS, если вдруг он был установлен
  I2C->CR2 &= ~I2C_CR2_POS;
  return true;
  //return I2C_SUCCESS;
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


