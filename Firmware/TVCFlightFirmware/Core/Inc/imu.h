/*
 * imu.h
 *
 *  Created on: Jun 24, 2025
 *      Author: if-himanshu
 */

#ifndef INC_IMU_H_
#define INC_IMU_H_


#include "stm32g4xx_hal.h"
#include <stdint.h>  // this is special lib convert all variables according
					// to microcontroller bit design
#include "stdio.h"


// MPU6050 I2C address
#define IMU_MPU6050_ADDR     (0x68 << 1)  // 0xD0 (8-bit address for HAL)

// Register addresses
#define MPU6050_REG_PWR_MGMT_1    0x6B
#define MPU6050_REG_ACCEL_XOUT_H  0x3B
#define MPU6050_REG_GYRO_XOUT_H   0x43
#define MPU6050_REG_WHO_AM_I      0x75

// IMU data structure
typedef struct {
    int16_t Accel_X, Accel_Y, Accel_Z;
    int16_t Gyro_X, Gyro_Y, Gyro_Z;
    int16_t Temp;
} IMU_Data;

extern IMU_Data imu_data;

uint8_t IMU_init(I2C_HandleTypeDef *hi2c);   // we can initialize any i2c peripheral with this fnc
uint8_t IMU_read(I2C_HandleTypeDef *hi2c, IMU_Data *data);  // to read data from i2c



#endif /* INC_IMU_H_ */
