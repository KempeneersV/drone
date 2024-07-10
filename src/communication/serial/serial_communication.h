#ifndef SERIAL_COMMUNICATION_H_
#define SERIAL_COMMUNICATION_H_

// Function prototypes
void USART_Init(uint16_t ubrr);
void USART_Transmit(uint8_t data);
uint8_t USART_Receive(void);

#endif // SERIAL_COMMUNICATION_H_