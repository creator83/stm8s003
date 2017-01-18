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
  //��������� I2C
  I2C->FREQR = Tact::get_frq();
  I2C->CR1 &= ~I2C_CR1_PE;
  //� ����������� ������ �������� I2C max = 100 ����/�
  //�������� ����������� ����� 
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
  //�������� I2C
  I2C->CR1 |= I2C_CR1_PE;
  //��������� ������������� � ����� �������
  
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
  //���� ������������ ���� I2C
//  delay_ms (10);
  while (flagBusy());   
  //��������� �����-�������
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
  //���������� � ������� ������ ����� �������� ����������
  setAddress (address & wOperation);
  
  write (reg);
  
  //�������� ������
  while(l--){
    write (*data++);
  }
  
  while (!(flagTxe()&&flagBtf()));
  
  //�������� ����-�������
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
    //��� ������� ��������� NACK �� ��������� �������� �����
    I2C->CR2 |= I2C_CR2_POS;
    //���� ������������� �������� ������
    while(!flagAddr());
    //�������� �� Errata
    __disable_interrupt();
    //������� ���� ADDR ������� �������� SR3
    readSR3 ();
    //��������� ������������� � ����� �������
    I2C->CR2 &= ~I2C_CR2_ACK;
    //�������� �� Errata
    __enable_interrupt();
    //���� �������, ����� ������ ���� �������� � DR,
    //� ������ � ��������� ��������
    while(!flagBtf());
     
    //�������� �� Errata
    __disable_interrupt();
    //����������� ��� STOP
    I2C->CR2 |= I2C_CR2_STOP;
    //������ �������� �����
    *data++ = I2C->DR;
    //�������� �� Errata
    __enable_interrupt();
    *data = I2C->DR;
  } 

  else if(length > 2){
    //���� ������������� �������� ������
    while(!flagAddr());
     
    //�������� �� Errata
    __disable_interrupt();
     
    //������� ���� ADDR ������� �������� SR3
    readSR3();
     
    //�������� �� Errata
    __enable_interrupt();
     
    while(length-- > 3){
      //������� ��������� ������ � DR � ��������� ��������
      while(!flagBtf());
      //������ �������� ���� �� DR
      *data++ = I2C->DR;
    }
     
    //�������� ������� 3 ��������� �����
    //����, ����� � DR �������� N-2 ����, � � ��������� ��������
    //�������� N-1 ����
    while(!flagBtf());
    //��������� ������������� � ����� �������
    I2C->CR2 &= ~I2C_CR2_ACK;
    //�������� �� Errata
    __disable_interrupt();
    //������ N-2 ���� �� RD, ��� ����� �������� ������� � ���������
    //������� ���� N, �� ������ � ����� ������ ���������� ������� NACK
    *data++ = I2C->DR;
    //������� STOP
    I2C->CR2 |= I2C_CR2_STOP;
    //������ N-1 ����
    *data++ = I2C->DR;
    //�������� �� Errata
    __enable_interrupt();
    //����, ����� N-� ���� ������� � DR �� ���������� ��������
    while(!flagRxne());
    //������ N ����
    *data++ = I2C->DR;
  }

  //���� �������� ���� �������
  while(I2C->CR2&I2C_CR2_STOP);
  
  //���������� ��� POS, ���� ����� �� ��� ����������
  I2C->CR2 &= ~I2C_CR2_POS;
  return true;

}

void I2c::wByte (uint8_t address, uint8_t reg, uint8_t data)
{
   start ();
  //���������� � ������� ������ ����� �������� ����������
  setAddress (address & wOperation);
  
  write (reg);
  
  //�������� ������
  write (data);
  while (!(flagTxe()&&flagBtf()));
  //�������� ����-�������
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

  //���� �������� ���� �������
  while(I2C->CR2&I2C_CR2_STOP);
  
  //���������� ��� POS, ���� ����� �� ��� ����������
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
