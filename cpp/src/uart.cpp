#include "uart.h"


uint32_t uart::sp[4] = {9600 , 38400 , 57600 , 115200};

uart::uart(baud b)
:pin (Gpio::D)
{
  //настройка пинов
  pin.setOutPin (TX);
  pin.setOutPin (RX, Gpio::Input , Gpio::Low , Gpio::OpenDrain);
  
  //настройка uart
  
  uint16_t brr = tact::get_frq()*1000000/sp[b];
  UART1->BRR2 = brr & 0x000F;
  UART1->BRR2 |= brr >> 12;
  UART1->BRR1 = (brr >> 4) & 0x00FF;
  
    //Четность отключена
  UART1->CR1 &= ~UART1_CR1_PIEN;
  //Контроль четности отключен
  UART1->CR1 &= ~UART1_CR1_PCEN;
  //8-битный режим
  UART1->CR1 &= ~UART1_CR1_M;
  //Включить UART
  UART1->CR1 &= ~UART1_CR1_UARTD;
  
  //Запретить прерывание по опустошению передающ. регистра
  UART1->CR2 &= ~UART1_CR2_TIEN;
  //Запретить прерывание по завершению передачи
  UART1->CR2 &= ~UART1_CR2_TCIEN;
  
  //Запретить прерывание по освобождению линии
   UART1->CR2 &= ~UART1_CR2_ILIEN;
  //Передатчик включить
  UART1->CR2 |= UART1_CR2_TEN;
  //Приемник включить
  UART1->CR2 |= UART1_CR2_REN ;
  //Не посылать break-символ
  UART1->CR2 &= ~UART1_CR2_SBK;
  
  //Один стоп-бит
  UART1->CR3 &= ~UART1_CR3_STOP;
  
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

    