#include "stm8s.h"

#ifndef UART_H
#define UART_H

void uart_init (void);

void uart_tx_byte (uint8_t data);

void uart_tx_string (char * str);

#endif