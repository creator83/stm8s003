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
  //���������� � ������� ������ ����� �������� ����������
  I2C->DR = address & I2c::wOperation;
  while (!driver->flagAddr());
  driver->readSR3();
  while (!driver->flagTxe());
  I2C->DR = sht20commands::tMesurementHold;
  while (!(driver->flagTxe()&&driver->flagBtf()));
  I2C->CR2 |= I2C_CR2_START;
  while (!driver->flagSb());
  I2C->DR = address | I2c::rOperation;

  //���� ������������� �������� ������
  while(!driver->flagAddr());
  //�������� �� Errata
  __disable_interrupt();
  //������� ���� ADDR ������� �������� SR3
  driver->readSR3 ();
  //�������� �� Errata
  __enable_interrupt();
   //���� �������, ����� ������ ���� �������� � DR,
  //� ������ � ��������� ��������
  while(!driver->flagBtf());
  //��������� ������������� � ����� �������
   I2C->CR2 &= ~I2C_CR2_ACK;
   //�������� �� Errata
   __disable_interrupt();
  temperatureCode = I2C->DR;
  temperatureCode <<= 8;
  //����������� ��� STOP
  I2C->CR2 |= I2C_CR2_STOP;
  //������ �������� �����
  temperatureCode |= I2C->DR;
 
  //�������� �� Errata
  __enable_interrupt();
  //����, ����� N-� ���� ������� � DR �� ���������� ��������
    while(!driver->flagRxne());
    uint8_t crc = I2C->DR;
//���� �������� ���� �������
  while(I2C->CR2&I2C_CR2_STOP);
  
  //���������� ��� POS, ���� ����� �� ��� ����������
  I2C->CR2 &= ~I2C_CR2_POS; 
}

void Sht20::readHummidity  ()
{
  driver->start ();
  //���������� � ������� ������ ����� �������� ����������
  driver->setAddress (address & I2c::wOperation);
  
  driver->write (sht20commands::rMesurement);
  while (!(driver->flagTxe()&&driver->flagBtf()));
  delay_us (30);
  //�������� ����-�������
  driver->stop ();
  delay_ms (15);
  
  driver->start ();
  driver->putData (address|I2c::rOperation);
  driver->stop ();
  delay_ms (15);
  
  driver->start ();
  driver->putData (address|I2c::rOperation);
  //��� ������� ��������� NACK �� ��������� �������� �����
  I2C->CR2 |= I2C_CR2_POS;
  //���� ������������� �������� ������
  while(!driver->flagAddr());
  //�������� �� Errata
  __disable_interrupt();
  //������� ���� ADDR ������� �������� SR3
  driver->readSR3 ();
  //��������� ������������� � ����� �������
  I2C->CR2 &= ~I2C_CR2_ACK;
  //�������� �� Errata
  __enable_interrupt();
  //���� �������, ����� ������ ���� �������� � DR,
  //� ������ � ��������� ��������
  while(!driver->flagBtf());
     
  //�������� �� Errata
  __disable_interrupt();
  //����������� ��� STOP
  I2C->CR2 |= I2C_CR2_STOP;
  //������ �������� �����
  hummdityCode = I2C->DR << 8;
  //�������� �� Errata
  __enable_interrupt();
  hummdityCode |= I2C->DR;
  //���� �������� ���� �������
  while(I2C->CR2&I2C_CR2_STOP);
  
  //���������� ��� POS, ���� ����� �� ��� ����������
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

  //���� �������� ���� �������
  while(I2C->CR2&I2C_CR2_STOP);
  
  //���������� ��� POS, ���� ����� �� ��� ����������
  I2C->CR2 &= ~I2C_CR2_POS;
}

void Sht20::writeUserR  (uint8_t data)
{
  driver->start ();
  //���������� � ������� ������ ����� �������� ����������
  driver->setAddress (address & I2c::wOperation);
  driver->write (sht20commands::writeUserRegister);
  //�������� ������
  driver->write (data);
  while (!(driver->flagTxe()&&driver->flagBtf()));
  //�������� ����-�������
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

