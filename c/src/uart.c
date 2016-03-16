#include "uart.h"

void uart_init (void)
{
  //===Settings pins===//
  //TX
  GPIOD->DDR |= 1 << TX;
  GPIOD->CR1 |= 1 << TX;
  GPIOD->CR2 &= ~(1 << TX);
  
  //RX
  GPIOD->DDR &= ~(1 << RX);
  GPIOD->CR1 &= ~(1 << RX);
  GPIOD->CR2 &= ~(1 << RX);
  
  //===Settings UART===//
  UART1->BRR2 = BRR_VAL & 0x000F;
  UART1->BRR2 |= BRR_VAL >> 12;
  UART1->BRR1 = (BRR_VAL >> 4) & 0x00FF;
  
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

  //Не посылать break-символ
  UART1->CR2 &= ~UART1_CR2_SBK;
  
  //Один стоп-бит
  UART1->CR3 &= ~UART1_CR3_STOP;
 
  //Передатчик включить
  UART1->CR2 |= UART1_CR2_TEN;
  //Приемник включить
  UART1->CR2 |= UART1_CR2_REN ;
}

void uart_tx_byte (uint8_t data)
{
  while(!(UART1->SR & UART1_SR_TXE));
  UART1->DR = data;
}

void uart_tx_string (char * str)
{
  while (*str)
  {
    while(!(UART1->SR & UART1_SR_TXE));
    UART1->DR = *str;
    str++;
  }
}