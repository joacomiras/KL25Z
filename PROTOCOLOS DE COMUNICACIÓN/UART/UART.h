#ifndef UART_H
#define UART_H


void UART0_Init(uint32_t baud_rate);
char UART0_Receive(void);
void UART_ReceiveString(char* palabra, uint16_t tMax);

#endif