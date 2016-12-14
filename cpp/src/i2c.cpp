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
  //��������� I2C
  I2C->CR1 &= ~I2C_CR1_PE;
  //� ����������� ������ �������� I2C max = 100 ����/�
  //�������� ����������� ����� 
   I2C->CCRH &= ~I2C_CCRH_FS;
  //CCR = Fmaster/2*Fiic= 12MHz/2*100kHz
  ccr = Tact::get_frq()/(2*speed_ [speed]);
  
  
  //Set Maximum Rise Time: 1000ns max in Standard Mode
  //= [1000ns/(1/InputClockFrequencyMHz.10e6)]+1
  //= InputClockFrequencyMHz+1
  I2C->TRISER = 12+1;
  I2C->CCRL = ccr & 0xFF;

  I2C->CCRH &= ~I2C_CCRH_CCR;
  I2C->CCRH = (ccr >> 8) & 0x0F;
  //�������� I2C
  I2C->CR1 |= I2C_CR1_PE;
  //��������� ������������� � ����� �������
  I2C->CR2 |= I2C_CR2_ACK;
}

void I2c::putData (uint8_t &val)
{
  I2C->DR = val;
}


bool I2c::wReg (uint8_t adress, uint8_t reg, uint8_t *data, uint8_t l)
{
  //���� ������������ ���� I2C
  delay_ms (10);
  while (I2C->SR3 & I2C_SR3_BUSY);
    
  //��������� �����-�������
  I2C->CR2 |= I2C_CR2_START;
  //���� ��������� ���� SB
  delay_ms (1);
  while(!(I2C->SR1 & I2C_SR1_SB));
  
  
  //���������� � ������� ������ ����� �������� ����������
  I2C->DR = adress & 0xFE;
  //���� ������������� �������� ������
  delay_ms (1);
  while(!(I2C->SR1 &I2C_SR1_ADDR));
  //������� ���� ADDR ������� �������� SR3
  uint8_t temp = I2C->SR3;
  
  
  //���� ������������ �������� ������
  while(!I2C->SR1& I2C_SR1_TXE);
  //���������� ����� ��������
  I2C->DR = reg;
  
  //�������� ������
  while(l--){
    //���� ������������ �������� ������
    while(!I2C->SR1& I2C_SR1_TXE);
    //���������� ����� ��������
    I2C->DR = *data++;
  }
  
  //����� ������, ����� DR ����������� � ������ ������ � ��������� �������
  while(!(I2C->SR1 & I2C_SR1_TXE && I2C->SR1 & I2C_SR1_BTF));
  
  //�������� ����-�������
  I2C->CR2 |= I2C_CR2_STOP;
  //���� ���������� ������� ����
  while(I2C->CR2&I2C_CR2_STOP);
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


