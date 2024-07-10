#include <avr/io.h>
#include <stdint.h>
#include "utils.h"

void USART_Init(uint16_t baud)
{
    uint16_t ubrr =  F_CPU/16/baud-1;
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

void USART_Print_String(const char *str) {
    while (*str) {
        USART_Transmit(*str);
        str++;
    }
}

void USART_PrintLn_String(const char *str) {
    while (*str) {
        USART_Transmit(*str);
        str++;
    }
    USART_Transmit('\r');
    USART_Transmit('\n');
}

void USART_Print_Int(int32_t integer) {
    int8_t i = 0;
    char buffer[7];
    int_to_ascii(integer, buffer);
    while(buffer[i] != '\0') {
        USART_Transmit(buffer[i]);
        i++;
    }
}

void USART_PrintLn_Int(int32_t integer) {
    int8_t i = 0;
    char buffer[7];
    int_to_ascii(integer, buffer);
    while(buffer[i] != '\0') {
        USART_Transmit(buffer[i]);
        i++;
    }
    USART_Transmit('\r');
    USART_Transmit('\n');
}

void USART_Print_Float(float float_number, int32_t decimal_places) {
    int8_t i = 0;
    char buffer[41];
    float_to_ascii(float_number, buffer, decimal_places);
    while(buffer[i] != '\0') {
        USART_Transmit(buffer[i]);
        i++;
    }
}

void USART_PrintLn_Float(float float_number, int32_t decimal_places) {
    int8_t i = 0;
    char buffer[41];
    float_to_ascii(float_number, buffer, decimal_places);
    while(buffer[i] != '\0') {
        USART_Transmit(buffer[i]);
        i++;
    }
    USART_Transmit('\r');
    USART_Transmit('\n');
}