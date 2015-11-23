#include "uart.h"

uint16_t uart::speed [2]={9600,57600};

uart::uart(baud b)
:pin (Gpio::D)
{
  //===��������� �����===//
  pin.setOutPin (TX);
  pin.setInPin (RX);
  
  //===��������� UART===//
  brr_val = (tact::get_frq()*1000000/speed[b]);
  UART1->BRR2 = brr_val & 0x000F;
  UART1->BRR2 |= brr_val >> 12;
  UART1->BRR1 = (brr_val >> 4) & 0x00FF;
  
  //�������� ���������
  UART1->CR1 &= ~UART1_CR1_PIEN;
  
  //�������� �������� ��������
  UART1->CR1 &= ~UART1_CR1_PCEN;
  
  //8-������ �����
  UART1->CR1 &= ~UART1_CR1_M;
  
  //�������� UART
  UART1->CR1 &= ~UART1_CR1_UARTD;
  
  //��������� ���������� �� ����������� ��������. ��������
  UART1->CR2 &= ~UART1_CR2_TIEN;
  
  //��������� ���������� �� ���������� ��������
  UART1->CR2 &= ~UART1_CR2_TCIEN;
  
  //��������� ���������� �� ������������ �����
   UART1->CR2 &= ~UART1_CR2_ILIEN;

  //�� �������� break-������
  UART1->CR2 &= ~UART1_CR2_SBK;
  
  //���� ����-���
  UART1->CR3 &= ~UART1_CR3_STOP;
 
  //���������� ��������
  UART1->CR2 |= UART1_CR2_TEN;
  //�������� ��������
  UART1->CR2 |= UART1_CR2_REN ;
}

void uart::transmit (uint8_t data)
{
  while(!(UART1->SR & UART1_SR_TXE));
  UART1->DR = data;
}

void uart::transmit (char * str)
{
  while (*str)
  {
    while(!(UART1->SR & UART1_SR_TXE));
    UART1->DR = *str;
    str++;
  }  
}

uint8_t uart::receive ()
{
  return 0;
}

    