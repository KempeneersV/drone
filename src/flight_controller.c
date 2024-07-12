#include <avr/io.h>
#include <stdio.h>
#include "communication/i2c/i2c.h"
#include "communication/serial/serial_communication.h"
#include "config.h"
#include "timer.h"
#include "mpu6050.h"

#define X               1
#define Y               2
#define Z               3

float measures[3] = {0, 0, 0};
int16_t angular_motions[3];

int main(void) {

    // Define PortB as output
    DDRB |= (1 << DDB5);
    // Set led on while initialisation
    PORTB |= (1 << PORTB5);

    // Initialize USART
    USART_Init(BAUD_COM_SERIAL);

    // Initialize Timer for delay
    initTimer();

    // Initialize MPU6050
    MPU6050_Init();

    // Calibrate MPU6050 Gryo and Acc
    MPU6050_CalibrateGyro();

    PORTB &= ~(1 << PORTB5);
    uint32_t start = millis();
    // Main loop
    while (1)
    {
        Calculate_Angles(measures);
        
        if((millis() - start) > 1000) {
            Calculate_Angles_Print(measures);
            start = millis();
        }
        delay(4);
    }
}