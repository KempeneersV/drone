#include "i2c.h"
#include <avr/io.h>
#include <util/twi.h>
#include "config.h"

void I2C_Init(void)
{
    // Set prescaler for desired SCL frequency
    TWSR &= ~((1 << TWPS0) | (1 << TWPS1)); // Prescaler = 1
    TWBR = ((F_CPU / SCL_CLOCK) - 16) / 2; // Calculate TWBR value
}

void I2C_Start(void)
{
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); // Send START condition
    while (!(TWCR & (1 << TWINT))); // Wait for TWINT flag set
}

void I2C_RepeatedStart(void)
{
    I2C_Start();
}

void I2C_Stop(void)
{
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN); // Send STOP condition
}

void I2C_Write(uint8_t data)
{
    TWDR = data; // Load data to be transmitted into data register
    TWCR = (1 << TWINT) | (1 << TWEN); // Start transmission
    while (!(TWCR & (1 << TWINT))); // Wait for operation to complete
}

uint8_t I2C_ReadAck(void)
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA); // Receive data with ACK
    while (!(TWCR & (1 << TWINT))); // Wait for operation to complete
    return TWDR; // Return received data
}

uint8_t I2C_ReadNak(void)
{
    TWCR = (1 << TWINT) | (1 << TWEN); // Receive data with NACK
    while (!(TWCR & (1 << TWINT))); // Wait for operation to complete
    return TWDR; // Return received data
}
