#ifndef MPU6050_H_
#define MPU6050_H_

#include <stdint.h>

// Define MPU6050 I2C address
#define MPU6050_ADDR        0x68

// Define Register Map
#define PWR_MGMT_1_ADDR     0x6B
#define ACCEL_XOUT_H_ADDR   0x3B
#define TEMP_OUT_H_ADDR     0x41
#define GYRO_XOUT_H_ADDR    0x43
#define GYRO_CONFIG_ADDR    0x1B
#define ACCEL_CONFIG_ADDR   0x1C
#define CONFIG              0x1A

// Function prototypes
void MPU6050_Init(void);
void MPU6050_CalibrateGyro(void);
void Calculate_GyroAngles(void);
void Calculate_GyroAngles_Print(void);
void Calculate_AccAngles(void);
void Calculate_Angles(float *measures);
void Calculate_Angles_Print(float *measures);

#endif /* MPU6050_H_ */
