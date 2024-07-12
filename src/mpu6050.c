#include "mpu6050.h"
#include "i2c.h"
#include "timer.h"
#include <math.h>
#include <stdlib.h>
#include "communication/serial/serial_communication.h"

#define X           0
#define Y           1
#define Z           2

#define YAW         0
#define PITCH       1
#define ROLL        2     

#define NB_SAMPLES  2000
#define PI          3.1415927f
#define FREQ        250
#define SSF_GYRO    65.5f


int16_t gyro_raw[3]         = {0, 0, 0};
int16_t gyro_offset[3]      = {0, 0, 0};
float   gyro_angle[3]       = {0, 0, 0};
int16_t acc_raw[3]          = {0, 0, 0};
int16_t acc_offset[3]       = {0, 0, 0};
float   acc_angle[3]        = {0, 0, 0};
int32_t acc_total_vector    = 0;
int16_t temp_raw            = 0;
uint8_t initialized         = 0;
void MPU6050_Init(void)
{
    I2C_Init();

    // Start I2C communication
    I2C_Start();
    I2C_Write((MPU6050_ADDR << 1) | 0); // Write mode
    // Write to PWR_MGMT_1 register to wake up MPU6050
    I2C_Write(PWR_MGMT_1_ADDR);
    I2C_Write(0x00);
    I2C_Stop();

    I2C_Start();
    I2C_Write((MPU6050_ADDR << 1) | 0); // Write mode
    // Write to GYRO_CONFIG_ADDR register to configure gyro sensitivity
    I2C_Write(GYRO_CONFIG_ADDR);
    I2C_Write(0x08); // Set full scale range to ±500°/sec
    I2C_Stop();

    I2C_Start();
    I2C_Write((MPU6050_ADDR << 1) | 0); // Write mode
    // Write to ACCEL_CONFIG register to configure accel sensitivity
    I2C_Write(ACCEL_CONFIG_ADDR);
    I2C_Write(0x10); // Set full scale range to ±8g
    I2C_Stop();

    I2C_Start();
    I2C_Write((MPU6050_ADDR << 1) | 0); // Write mode
    // Write to CONFIG register to configure low pass filter
    I2C_Write(CONFIG);
    I2C_Write(0x03); // Set DLPF about 43Hz
    I2C_Stop();
}

void MPU6050_ReadAcc(void)
{    
    // Read acceleration data
    I2C_Start();
    I2C_Write((MPU6050_ADDR << 1) | 0); // Write mode
    I2C_Write(ACCEL_XOUT_H_ADDR); // Start with register 0x3B (ACCEL_XOUT_H)
    I2C_RepeatedStart();
    I2C_Write((MPU6050_ADDR << 1) | 1); // Read mode

    // Combine data into 16-bit values
    acc_raw[X] = (I2C_ReadAck() << 8) | I2C_ReadAck();
    acc_raw[Y] = (I2C_ReadAck() << 8) | I2C_ReadAck();
    acc_raw[Z] = (I2C_ReadAck() << 8) | I2C_ReadNak();

    I2C_Stop();
}

void MPU6050_ReadGyro(void)
{    
    // Read gyro data
    I2C_Start();
    I2C_Write((MPU6050_ADDR << 1) | 0); // Write mode
    I2C_Write(GYRO_XOUT_H_ADDR); // Start with register 0x43
    I2C_RepeatedStart();
    I2C_Write((MPU6050_ADDR << 1) | 1); // Read mode
    
    // Combine data into 16-bit values
    gyro_raw[X] = (I2C_ReadAck() << 8) | I2C_ReadAck();
    gyro_raw[Y] = (I2C_ReadAck() << 8) | I2C_ReadAck();
    gyro_raw[Z] = (I2C_ReadAck() << 8) | I2C_ReadNak();

    I2C_Stop();
}

void MPU6050_ReadSensor(void) {

    // Read data sensor
    I2C_Start();
    I2C_Write((MPU6050_ADDR << 1) | 0); // Write mode
    I2C_Write(ACCEL_XOUT_H_ADDR); // Start with register 0x3B
    I2C_RepeatedStart();
    I2C_Write((MPU6050_ADDR << 1) | 1); // Read mode
    
    acc_raw[X]  = (I2C_ReadAck() << 8) | I2C_ReadAck();
    acc_raw[Y]  = (I2C_ReadAck() << 8) | I2C_ReadAck();
    acc_raw[Z]  = (I2C_ReadAck() << 8) | I2C_ReadAck();
    temp_raw    = (I2C_ReadAck() << 8) | I2C_ReadAck();
    gyro_raw[X] = (I2C_ReadAck() << 8) | I2C_ReadAck();
    gyro_raw[Y] = (I2C_ReadAck() << 8) | I2C_ReadAck();
    gyro_raw[Z] = (I2C_ReadAck() << 8) | I2C_ReadNak();

    I2C_Stop();
}

void MPU6050_CalibrateGyro(void) {

    int32_t temp[3] = {0, 0, 0};

    for(int i = 0; i < NB_SAMPLES; i++) {
        MPU6050_ReadGyro();

        temp[X] += (int32_t)gyro_raw[X];
        temp[Y] += (int32_t)gyro_raw[Y];
        temp[Z] += (int32_t)gyro_raw[Z];

        delay(3);
    }

    gyro_offset[X] = temp[X] / NB_SAMPLES;
    gyro_offset[Y] = temp[Y] / NB_SAMPLES;
    gyro_offset[Z] = temp[Z] / NB_SAMPLES;

}

/**
 * Calculate pitch & roll angles using only the gyro.
 */
void Calculate_GyroAngles()
{
    MPU6050_ReadGyro(); // DEBUG

    // Subtract offsets
    gyro_raw[X] -= gyro_offset[X];
    gyro_raw[Y] -= gyro_offset[Y];
    gyro_raw[Z] -= gyro_offset[Z];

    // USART_PrintLn_String("Raw :");
    // USART_Print_String("X : ");
    // USART_PrintLn_Int(gyro_raw[X]);
    // USART_Print_String("Y : ");
    // USART_PrintLn_Int(gyro_raw[Y]);
    // USART_Print_String("Z : ");
    // USART_PrintLn_Int(gyro_raw[Z]);
    // USART_PrintLn_String(" ");

    // Angle calculation using integration
    gyro_angle[X] += ((float)gyro_raw[X]    / (FREQ * SSF_GYRO));
    gyro_angle[Y] += ((float)(-gyro_raw[Y]) / (FREQ * SSF_GYRO)); // Change sign to match the accelerometer's one

    // Transfer roll to pitch if IMU has yawed
    gyro_angle[Y] += gyro_angle[X] * sin(gyro_raw[Z] * (PI / (FREQ * SSF_GYRO * 180)));
    gyro_angle[X] -= gyro_angle[Y] * sin(gyro_raw[Z] * (PI / (FREQ * SSF_GYRO * 180)));

    // USART_PrintLn_String("Raw :");
    // USART_Print_String("X : ");
    // USART_PrintLn_Float(gyro_angle[X], 2);
    // USART_Print_String("Y : ");
    // USART_PrintLn_Float(gyro_angle[Y], 2);
    // USART_Print_String("Z : ");
    // USART_PrintLn_Int(gyro_raw[Z]);
    // USART_PrintLn_String(" ");
}

// void Calculate_GyroAngles_Print()
// {
//     // MPU6050_ReadGyro(); // DEBUG

//     // Subtract offsets
//     gyro_raw[X] -= gyro_offset[X];
//     gyro_raw[Y] -= gyro_offset[Y];
//     gyro_raw[Z] -= gyro_offset[Z];

//     // USART_PrintLn_String("Raw :");
//     // USART_Print_String("X : ");
//     // USART_PrintLn_Int(gyro_raw[X]);
//     // USART_Print_String("Y : ");
//     // USART_PrintLn_Int(gyro_raw[Y]);
//     // USART_Print_String("Z : ");
//     // USART_PrintLn_Int(gyro_raw[Z]);
//     // USART_PrintLn_String(" ");

//     // Angle calculation using integration
//     gyro_angle[X] += ((float)gyro_raw[X]    / (FREQ * SSF_GYRO));
//     gyro_angle[Y] += ((float)(-gyro_raw[Y]) / (FREQ * SSF_GYRO)); // Change sign to match the accelerometer's one

//     // Transfer roll to pitch if IMU has yawed
//     gyro_angle[Y] += gyro_angle[X] * sin(gyro_raw[Z] * (PI / (FREQ * SSF_GYRO * 180)));
//     gyro_angle[X] -= gyro_angle[Y] * sin(gyro_raw[Z] * (PI / (FREQ * SSF_GYRO * 180)));

//     // USART_PrintLn_String("Raw :");
//     // USART_Print_String("X : ");
//     // USART_PrintLn_Float(gyro_angle[X], 2);
//     // USART_Print_String("Y : ");
//     // USART_PrintLn_Float(gyro_angle[Y], 2);
//     // USART_Print_String("Z : ");
//     // USART_PrintLn_Int(gyro_raw[Z]);
//     // USART_PrintLn_String(" ");
// }

/**
 * Calculate pitch & roll angles using only the accelerometer.
 */
void Calculate_AccAngles()
{   
    // MPU6050_ReadAcc(); // DEBUG

    // Calculate total 3D acceleration vector : √(X² + Y² + Z²)
    acc_total_vector = sqrt(pow(acc_raw[X], 2) + pow(acc_raw[Y], 2) + pow(acc_raw[Z], 2));

    // To prevent asin to produce a NaN, make sure the input value is within [-1;+1]
    if (abs(acc_raw[X]) < acc_total_vector) {
    acc_angle[X] = asin((float)acc_raw[Y] / acc_total_vector) * (180 / PI); // asin gives angle in radian. Convert to degree multiplying by 180/pi
    }

    if (abs(acc_raw[Y]) < acc_total_vector) {
    acc_angle[Y] = asin((float)acc_raw[X] / acc_total_vector) * (180 / PI);
    }

    // USART_PrintLn_String("Raw :");
    // USART_Print_String("X : ");
    // USART_PrintLn_Float(acc_angle[X], 2);
    // USART_Print_String("Y : ");
    // USART_PrintLn_Float(acc_angle[Y], 2);
}

/**
 * Calculate real angles from gyro and accelerometer's values
 */
void Calculate_Angles(float *measures)
{
    MPU6050_ReadSensor();
    Calculate_GyroAngles();
    Calculate_AccAngles();

    if (initialized) {
    // Correct the drift of the gyro with the accelerometer
    gyro_angle[X] = gyro_angle[X] * 0.9996 + acc_angle[X] * 0.0004;
    gyro_angle[Y] = gyro_angle[Y] * 0.9996 + acc_angle[Y] * 0.0004;
    } else {
    // At very first start, init gyro angles with accelerometer angles
    gyro_angle[X] = acc_angle[X];
    gyro_angle[Y] = acc_angle[Y];

    initialized = 1;
    }

    // To dampen the pitch and roll angles a complementary filter is used
    measures[ROLL]  = measures[ROLL]  * 0.9 + gyro_angle[X] * 0.1;
    measures[PITCH] = measures[PITCH] * 0.9 + gyro_angle[Y] * 0.1;
    measures[YAW]   = -gyro_raw[Z] / SSF_GYRO; // Store the angular motion for this axis
}

/**
 * Calculate real angles from gyro and accelerometer's values
 */
void Calculate_Angles_Print(float *measures)
{
    MPU6050_ReadSensor();
    Calculate_GyroAngles();
    Calculate_AccAngles();

    if (initialized) {
    // Correct the drift of the gyro with the accelerometer
    gyro_angle[X] = gyro_angle[X] * 0.9996 + acc_angle[X] * 0.0004;
    gyro_angle[Y] = gyro_angle[Y] * 0.9996 + acc_angle[Y] * 0.0004;
    } else {
    // At very first start, init gyro angles with accelerometer angles
    gyro_angle[X] = acc_angle[X];
    gyro_angle[Y] = acc_angle[Y];

    initialized = 1;
    }

    // To dampen the pitch and roll angles a complementary filter is used
    measures[ROLL]  = measures[ROLL]  * 0.9 + gyro_angle[X] * 0.1;
    measures[PITCH] = measures[PITCH] * 0.9 + gyro_angle[Y] * 0.1;
    measures[YAW]   = -gyro_raw[Z] / SSF_GYRO; // Store the angular motion for this axis

    USART_PrintLn_String("Position :");
    USART_Print_String("X : ");
    USART_PrintLn_Float(measures[ROLL], 2);
    USART_Print_String("Y : ");
    USART_PrintLn_Float(measures[PITCH], 2);
    USART_Print_String("Z : ");
    USART_PrintLn_Float(measures[YAW], 2);
}