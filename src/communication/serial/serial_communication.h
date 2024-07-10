#ifndef SERIAL_COMMUNICATION_H_
#define SERIAL_COMMUNICATION_H_

// Function prototypes
void USART_Init(uint16_t baud);
void USART_Transmit(uint8_t data);
uint8_t USART_Receive(void);
void USART_Print_String(const char *str);
void USART_PrintLn_String(const char *str);
void USART_Print_Int(int32_t integer);
void USART_PrintLn_Int(int32_t integer);
void USART_Print_Float(float float_number, int32_t decimal_places);
void USART_Print_FloatLn(float float_number, int32_t decimal_places);

#endif // SERIAL_COMMUNICATION_H_