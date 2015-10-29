#include "stm8s.h"

#define F_CPU 16000000

#define BAUD 9600
#define BRR_VAL F_CPU/BAUD

#define RX 6
#define TX 5


#ifndef UART_H
#define UART_H

void uart_init (void);

void uart_tx_byte (uint8_t data);

void uart_tx_string (char * str);

#endif