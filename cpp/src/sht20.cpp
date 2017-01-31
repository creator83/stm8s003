#include "sht20.h"



Sht20::Sht20 (I2c *d)
:address(0x80)
{
  driver = d;
  GPIOB->ODR |= (1 <<4|1 << 5);
}

void Sht20::readTemperature ()
{
  while (driver->flagBusy());
  I2C->CR2 |= I2C_CR2_ACK;
  I2C->CR2 |= I2C_CR2_START;
  while (!driver->flagSb());
  //Записываем в регистр данных адрес ведомого устройства
  I2C->DR = address & I2c::wOperation;
  while (!driver->flagAddr());
  driver->readSR3();
  while (!driver->flagTxe());
  I2C->DR = sht20commands::tMesurementHold;
  while (!(driver->flagTxe()&&driver->flagBtf()));
  I2C->CR2 |= I2C_CR2_START;
  while (!driver->flagSb());
  I2C->DR = address | I2c::rOperation;

  //Ждем подтверждения передачи адреса
  while(!driver->flagAddr());
  //Заплатка из Errata
  __disable_interrupt();
  //Очистка бита ADDR чтением регистра SR3
  driver->readSR3 ();
  //Заплатка из Errata
  __enable_interrupt();
   //Ждем момента, когда первый байт окажется в DR,
  //а второй в сдвиговом регистре
  while(!driver->flagBtf());
  //Запрещаем подтверждение в конце посылки
   I2C->CR2 &= ~I2C_CR2_ACK;
   //Заплатка из Errata
   __disable_interrupt();
  temperatureCode = I2C->DR;
  temperatureCode <<= 8;
  //Устанавлием бит STOP
  I2C->CR2 |= I2C_CR2_STOP;
  //Читаем принятые байты
  temperatureCode |= I2C->DR;
 
  //Заплатка из Errata
  __enable_interrupt();
  //Ждем, когда N-й байт попадет в DR из сдвигового регистра
    while(!driver->flagRxne());
    uint8_t crc = I2C->DR;
//Ждем отправки СТОП посылки
  while(I2C->CR2&I2C_CR2_STOP);
  
  //Сбрасывает бит POS, если вдруг он был установлен
  I2C->CR2 &= ~I2C_CR2_POS; 
}

void Sht20::readHummidity  ()
{
  driver->start ();
  //Записываем в регистр данных адрес ведомого устройства
  driver->setAddress (address & I2c::wOperation);
  
  driver->write (sht20commands::rMesurement);
  while (!(driver->flagTxe()&&driver->flagBtf()));
  delay_us (30);
  //Посылаем СТОП-посылку
  driver->stop ();
  delay_ms (15);
  
  driver->start ();
  driver->putData (address|I2c::rOperation);
  driver->stop ();
  delay_ms (15);
  
  driver->start ();
  driver->putData (address|I2c::rOperation);
  //Бит который разрешает NACK на следующем принятом байте
  I2C->CR2 |= I2C_CR2_POS;
  //Ждем подтверждения передачи адреса
  while(!driver->flagAddr());
  //Заплатка из Errata
  __disable_interrupt();
  //Очистка бита ADDR чтением регистра SR3
  driver->readSR3 ();
  //Запрещаем подтверждение в конце посылки
  I2C->CR2 &= ~I2C_CR2_ACK;
  //Заплатка из Errata
  __enable_interrupt();
  //Ждем момента, когда первый байт окажется в DR,
  //а второй в сдвиговом регистре
  while(!driver->flagBtf());
     
  //Заплатка из Errata
  __disable_interrupt();
  //Устанавлием бит STOP
  I2C->CR2 |= I2C_CR2_STOP;
  //Читаем принятые байты
  hummdityCode = I2C->DR << 8;
  //Заплатка из Errata
  __enable_interrupt();
  hummdityCode |= I2C->DR;
  //Ждем отправки СТОП посылки
  while(I2C->CR2&I2C_CR2_STOP);
  
  //Сбрасывает бит POS, если вдруг он был установлен
  I2C->CR2 &= ~I2C_CR2_POS;
  hummdityCode &= ~0x03;
  temperatureCode &= ~0x03;
}

void Sht20::setResolution (resolution r)
{
  res = r;
  
}

void Sht20::readUserR ()
{
  driver->start ();
  driver->setAddress (address&I2c::wOperation);
  driver->write (sht20commands::readUserRegister);
  while (!(driver->flagTxe()&&driver->flagBtf()));
  driver->restart ();
  driver->putData (address|I2c::rOperation);

  I2C->CR2 &= ~I2C_CR2_ACK;
  while (!driver->flagAddr());
  disableInterrupts();
  driver->readSR3();
  I2C->CR2 |= I2C_CR2_STOP;
  enableInterrupts();
  while (!driver->flagRxne());
  uint8_t data = driver->getData();

  //Ждем отправки СТОП посылки
  while(I2C->CR2&I2C_CR2_STOP);
  
  //Сбрасывает бит POS, если вдруг он был установлен
  I2C->CR2 &= ~I2C_CR2_POS;
}

void Sht20::writeUserR  (uint8_t data)
{
  driver->start ();
  //Записываем в регистр данных адрес ведомого устройства
  driver->setAddress (address & I2c::wOperation);
  driver->write (sht20commands::writeUserRegister);
  //Отправка данных
  driver->write (data);
  while (!(driver->flagTxe()&&driver->flagBtf()));
  //Посылаем СТОП-посылку
  driver->stop ();  
}

uint8_t & Sht20::getUserRegister ()
{
  return uRegister;
}
  
void Sht20::convertHummdity ()
{
  hummdity = (((125*hummdityCode)>>16) - 60)/10;
}

void Sht20::convertTemperature ()
{
  temperature = (((17572*temperatureCode)>>16) - 4685)/10;
}

