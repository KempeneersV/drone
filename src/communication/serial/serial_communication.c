#include <avr/io.h>
#include <stdint.h>

void USART_Init(uint16_t ubrr)
{
    // Set baud rate registers
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)ubrr;

    // Enable transmitter and receiver
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);

    // Set frame format: 8 data bits, 1 stop bit
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void USART_Transmit(uint8_t data)
{
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0)))
        ;

    // Put data into buffer, sends the data
    UDR0 = data;
}

uint8_t USART_Receive(void)
{
    // Wait for data to be received
    while (!(UCSR0A & (1 << RXC0)))
        ;

    // Get and return received data from buffer
    return UDR0;
}
