#include "mpu6050.h"
#include "i2c.h"

void MPU6050_Init(void)
{
    I2C_Init();

    // Start I2C communication
    I2C_Start();
    I2C_Write((MPU6050_ADDR << 1) | 0); // Write mode

    // Write to PWR_MGMT_1 register to wake up MPU6050
    I2C_Write(PWR_MGMT_1_ADDR);
    I2C_Write(0x00);
/*
    // Write to GYRO_CONFIG_ADDR register to configure gyro sensitivity
    I2C_Write(GYRO_CONFIG_ADDR);
    I2C_Write(0x08); // Set full scale range to ±500°/sec

    // Write to ACCEL_CONFIG register to configure accel sensitivity
    I2C_Write(ACCEL_CONFIG_ADDR);
    I2C_Write(0x10); // Set full scale range to ±8g

    // Write to CONFIG register to configure low pass filter
    I2C_Write(CONFIG);
    I2C_Write(0x03); // Set DLPF about 43Hz
*/
    // Stop I2C communication
    I2C_Stop();
}

void MPU6050_ReadAcceleration(int16_t *accX, int16_t *accY, int16_t *accZ)
{
    uint8_t buffer[6];
    
    // Read acceleration data
    I2C_Start();
    I2C_Write((MPU6050_ADDR << 1) | 0); // Write mode
    I2C_Write(ACCEL_XOUT_H_ADDR); // Start with register 0x3B (ACCEL_XOUT_H)
    I2C_RepeatedStart();
    I2C_Write((MPU6050_ADDR << 1) | 1); // Read mode
    
    for (int i = 0; i < 5; ++i)
    {
        buffer[i] = I2C_ReadAck(); // Read data byte with ACK
    }
    buffer[5] = I2C_ReadNak(); // Read last data byte with NAK
    I2C_Stop();
    
    // Combine data into 16-bit values
    *accX = (buffer[0] << 8) | buffer[1];
    *accY = (buffer[2] << 8) | buffer[3];
    *accZ = (buffer[4] << 8) | buffer[5];
}

void MPU6050_ReadGyro(int16_t *gyroX, int16_t *gyroY, int16_t *gyroZ)
{
    uint8_t buffer[6];
    
    // Read gyro data
    I2C_Start();
    I2C_Write((MPU6050_ADDR << 1) | 0); // Write mode
    I2C_Write(GYRO_XOUT_H_ADDR); // Start with register 0x43
    I2C_RepeatedStart();
    I2C_Write((MPU6050_ADDR << 1) | 1); // Read mode
    
    for (int i = 0; i < 5; ++i)
    {
        buffer[i] = I2C_ReadAck(); // Read data byte with ACK
    }
    buffer[5] = I2C_ReadNak(); // Read last data byte with NAK
    I2C_Stop();
    
    // Combine data into 16-bit values
    *gyroX = (buffer[0] << 8) | buffer[1];
    *gyroY = (buffer[2] << 8) | buffer[3];
    *gyroZ = (buffer[4] << 8) | buffer[5];
}

void MPU6050_ReadTemperature(int16_t *temperature)
{
    uint8_t buffer[2];
    
    // Read temperature data
    I2C_Start();
    I2C_Write((MPU6050_ADDR << 1) | 0); // Write mode
    I2C_Write(TEMP_OUT_H_ADDR); // Start with register 0x41 (TEMP_OUT_H)
    I2C_RepeatedStart();
    I2C_Write((MPU6050_ADDR << 1) | 1); // Read mode
    
    buffer[0] = I2C_ReadAck(); // Read high byte with ACK
    buffer[1] = I2C_ReadNak(); // Read low byte with NAK
    I2C_Stop();
    
    // Combine data into 16-bit signed value
    *temperature = (buffer[0] << 8) | buffer[1];
}

void MPU6050_ReadSensor(int16_t *temperature, int16_t *gyro, int16_t *acc) {

    // Read data sensor
    I2C_Start();
    I2C_Write((MPU6050_ADDR << 1) | 0); // Write mode
    I2C_Write(ACCEL_XOUT_H_ADDR); // Start with register 0x3B
    I2C_RepeatedStart();
    I2C_Write((MPU6050_ADDR << 1) | 1); // Read mode
    
    acc[0] = (I2C_ReadAck() << 8) | I2C_ReadAck();
    acc[1] = (I2C_ReadAck() << 8) | I2C_ReadAck();
    acc[2] = (I2C_ReadAck() << 8) | I2C_ReadAck();
    *temperature = (I2C_ReadAck() << 8) | I2C_ReadAck();
    gyro[0] = (I2C_ReadAck() << 8) | I2C_ReadAck();
    gyro[1] = (I2C_ReadAck() << 8) | I2C_ReadAck();
    gyro[2] = (I2C_ReadAck() << 8) | I2C_ReadNak();

    I2C_Stop();
}
