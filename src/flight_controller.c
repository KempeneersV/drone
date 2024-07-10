#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "communication/i2c/i2c.h"
#include "communication/serial/serial_communication.h"
#include "config.h"
#include "mpu6050.h"

int main(void) {

    int16_t acc_raw[3];
    int16_t gyro_raw[3];
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
        MPU6050_ReadSensor(&temp_raw, gyro_raw, acc_raw);
        temperature = (float)temp_raw / 340.0 + 36.53;

        USART_Print_String("Temperature : ");
        USART_Print_Float(temperature, 2);
        USART_PrintLn_String(" °C");

        USART_Print_String("AccX : ");
        USART_Print_Int(acc_raw[0]);
        USART_PrintLn_String(" ?");

        USART_Print_String("AccY : ");
        USART_Print_Int(acc_raw[1]);
        USART_PrintLn_String(" ?");

        USART_Print_String("AccZ : ");
        USART_Print_Int(acc_raw[2]);
        USART_PrintLn_String(" ?");

        USART_Print_String("GyroX : ");
        USART_Print_Int(gyro_raw[0]);
        USART_PrintLn_String(" ?");

        USART_Print_String("GyroY : ");
        USART_Print_Int(gyro_raw[1]);
        USART_PrintLn_String(" ?");

        USART_Print_String("GyroZ : ");
        USART_Print_Int(gyro_raw[2]);
        USART_PrintLn_String(" ?");

        // Delay or additional processing can be added here if needed
        _delay_ms(2000); // Example delay of 500 milliseconds
    }
}