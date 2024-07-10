#ifndef MPU6050_H_
#define MPU6050_H_

#include <stdint.h>

// Define MPU6050 I2C address
#define MPU6050_ADDR 0x68

// Function prototypes
void MPU6050_Init(void);
void MPU6050_ReadAcceleration(int16_t *accX, int16_t *accY, int16_t *accZ);
void MPU6050_ReadGyro(int16_t *gyroX, int16_t *gyroY, int16_t *gyroZ);
void MPU6050_ReadTemperature(int16_t *temperature);

#endif /* MPU6050_H_ */
