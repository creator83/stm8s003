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
  //��������� I2C
  I2C->FREQR = Tact::get_frq();
  I2C->CR1 &= ~I2C_CR1_PE;
  //� ����������� ������ �������� I2C max = 100 ����/�
  //�������� ����������� ����� 
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
  //�������� I2C
  I2C->CR1 |= I2C_CR1_PE;
  //��������� ������������� � ����� �������
  I2C->CR2 |= I2C_CR2_ACK;
}

void I2c::putData (uint8_t &val)
{
  I2C->DR = val;
}

void I2c::start ()
{
  //���� ������������ ���� I2C
//  delay_ms (10);
  while (I2C->SR3 & I2C_SR3_BUSY);
    
  //��������� �����-�������
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

bool I2c::rReg (uint8_t address, uint8_t reg, uint8_t *data, uint8_t length)
{
   //���� ������������ ���� I2C
 while (I2C->SR3 & I2C_SR3_BUSY);
     
  //��������� ������������� � ����� �������
  I2C->CR2 |= I2C_CR2_ACK;
   
  //��������� �����-�������
  I2C->CR2 |= I2C_CR2_START;
  //���� ��������� ���� SB
  while(!I2C->SR1 & I2C_SR1_SB);
   
  //���������� � ������� ������ ����� �������� ����������
  I2C->DR = address & 0xFE;
  //���� ������������� �������� ������
  while(!I2C->SR1 &I2C_SR1_ADDR);
  //������� ���� ADDR ������� �������� SR3
  uint8_t temp = I2C->SR3;
   
  //���� ������������ �������� ������ RD
  while(!I2C->SR1& I2C_SR1_TXE);
   
  //�������� ����� �������� slave-����������, ������� ����� ���������
  I2C->DR = reg;
  //����� ������, ����� DR ����������� � ������ ������ � ��������� �������
  while(!(I2C->SR1 & I2C_SR1_TXE && I2C->SR1 & I2C_SR1_BTF));
   
  //��������� �����-������� (�������)
  I2C->CR2 |= I2C_CR2_START;
  //���� ��������� ���� SB
  while(!(I2C->SR1 & I2C_SR1_SB));
   
  //���������� � ������� ������ ����� �������� ���������� � ���������
  //� ����� ������ (���������� �������� ���� � 1)
  I2C->DR = address | 0x01;
   
  //������ �������� ������� �� ���������� ����������� ����
  //N=1
  if(length == 1){
    //��������� ������������� � ����� �������
    I2C->CR2 &= ~I2C_CR2_ACK;
    //���� ������������� �������� ������
    while(!(I2C->SR1 &I2C_SR1_ADDR));
     
    //�������� �� Errata
    __disable_interrupt();
    //������� ���� ADDR ������� �������� SR3
    uint8_t temp = I2C->SR3;
     
    //����������� ��� STOP
    I2C->CR2 |= I2C_CR2_STOP;
    //�������� �� Errata
    __enable_interrupt();
     
    //���� ������� ������ � RD
    while(!(I2C->SR1 &I2C_SR1_RXNE));
     
    //������ �������� ����
    *data = I2C->DR;
  } 
  //N=2
  else if(length == 2){
    //��� ������� ��������� NACK �� ��������� �������� �����
    I2C->CR2 |= I2C_CR2_POS;
    //���� ������������� �������� ������
    while(!(I2C->SR1 &I2C_SR1_ADDR));
    //�������� �� Errata
    __disable_interrupt();
    //������� ���� ADDR ������� �������� SR3
    uint8_t temp = I2C->SR3;
    //��������� ������������� � ����� �������
    I2C->CR2 &= ~I2C_CR2_ACK;
    //�������� �� Errata
    __enable_interrupt();
    //���� �������, ����� ������ ���� �������� � DR,
    //� ������ � ��������� ��������
    while(!I2C->SR1 & I2C_SR1_BTF);
     
    //�������� �� Errata
    __disable_interrupt();
    //����������� ��� STOP
    I2C->CR2 |= I2C_CR2_STOP;
    //������ �������� �����
    *data++ = I2C->DR;
    //�������� �� Errata
    __enable_interrupt();
    *data = I2C->DR;
  } /*
  //N>2
  else if(length > 2){
    //���� ������������� �������� ������
    while(!(I2C->SR1 &I2C_SR1_ADDR));
     
    //�������� �� Errata
    __disable_interrupt();
     
    //������� ���� ADDR ������� �������� SR3
    uint8_t temp = I2C->SR3;
     
    //�������� �� Errata
    __enable_interrupt();
     
    while(length-- > 3){
      //������� ��������� ������ � DR � ��������� ��������
      while(!I2C->SR1 & I2C_SR1_BTF);
      //������ �������� ���� �� DR
      *data++ = I2C->DR;
    }
    //����� �������� �����
    if(!tmo) return I2C_TIMEOUT;
     
    //�������� ������� 3 ��������� �����
    //����, ����� � DR �������� N-2 ����, � � ��������� ��������
    //�������� N-1 ����
    wait_event(!I2C_SR1_BTF, 1);
    //��������� ������������� � ����� �������
    I2C_CR2_ACK = 0;
    //�������� �� Errata
    __disable_interrupt();
    //������ N-2 ���� �� RD, ��� ����� �������� ������� � ���������
    //������� ���� N, �� ������ � ����� ������ ���������� ������� NACK
    *data++ = I2C_DR;
    //������� STOP
    I2C_CR2_STOP = 1;
    //������ N-1 ����
    *data++ = I2C_DR;
    //�������� �� Errata
    __enable_interrupt();
    //����, ����� N-� ���� ������� � DR �� ���������� ��������
    wait_event(!I2C_SR1_RXNE, 1);
    //������ N ����
    *data++ = I2C_DR;
  }
   */
  //���� �������� ���� �������
  while(I2C->CR2&I2C_CR2_STOP);
  
  //���������� ��� POS, ���� ����� �� ��� ����������
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


