#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "communication/i2c/i2c.h"
#include "communication/serial/serial_communication.h"
#include "config.h"
#include "mpu6050.h"
#include "utils.h"

int main(void) {

    int16_t temp_raw;
    char intBuffer[7];
    char floatBuffer[50];
    float temperature;

    // Initialize USART
    USART_Init(BAUD_COM_SERIAL);
    //USART_Init(115200);

    // Initialize MPU6050
    MPU6050_Init();

    // Main loop
    while (1)
    {
        // MPU6050_ReadAcceleration(&accX, &accY, &accZ)
        MPU6050_ReadTemperature(&temp_raw);
        temperature = (float)temp_raw / 340.0 + 36.53; 

        // Display temperature on serial port
        USART_Transmit('T');
        USART_Transmit(':');
        USART_Transmit(' ');

    
        float_to_char(temperature, floatBuffer);
        for(int i = 0; floatBuffer[i] != '\0'; i++) {
            USART_Transmit(floatBuffer[i]);
        }
        
        USART_Transmit('\r');
        USART_Transmit('\n');

        // Delay or additional processing can be added here if needed
        _delay_ms(1000); // Example delay of 500 milliseconds
    }
}