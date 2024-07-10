#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>

// Function prototypes
void I2C_Init(void);
void I2C_Start(void);
void I2C_RepeatedStart(void);
void I2C_Stop(void);
void I2C_Write(uint8_t data);
uint8_t I2C_ReadAck(void);
uint8_t I2C_ReadNak(void);

#endif /* I2C_H_ */
