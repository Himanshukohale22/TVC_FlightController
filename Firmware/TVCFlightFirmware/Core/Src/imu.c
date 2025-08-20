/*
 * imu.c
 *
 *  Created on: Jun 26, 2025
 *      Author: if-himanshu
 */


#include "imu.h"
#include "math.h"
#include "main.h"

//uint8_t IMU_init(I2C_HandleTypeDef *hi2c){
//	uint8_t check; 	// check buffer
//	uint8_t data;	// data buffer
//	// I2C read for data
//
//
//}



float Q_angle = 0.001;   // Process noise variance for angle
float Q_bias  = 0.003;   // Process noise variance for gyro bias
float R_measure = 0.03;  // Measurement noise variance

float angle = 0.0;       // Angle estimate
float bias = 0.0;        // Gyro bias estimate
float rate;              // Unbiased rate

float P[2][2] = { {0, 0}, {0, 0} };  // Error covariance matrix


IMU_Data imu_data;

uint8_t IMU_init(I2C_HandleTypeDef *hi2c) {
    uint8_t check;
    uint8_t data;

    // Check WHO_AM_I register / read register
    if (HAL_I2C_Mem_Read(hi2c, IMU_MPU6050_ADDR, MPU6050_REG_PWR_MGMT_1, 1, &check, 1, HAL_MAX_DELAY) != HAL_OK)
        return 1;

    if (check != 0x68)
        return printf("address is not 0x68, not connected");  // Not detected

    // Wake up the MPU6050
    data = 0;
    if (HAL_I2C_Mem_Write(hi2c, IMU_MPU6050_ADDR, MPU6050_REG_PWR_MGMT_1, 1, &data, 1, HAL_MAX_DELAY) != HAL_OK)
        return 1;

    return 0; // Success
}


uint8_t IMU_read(I2C_HandleTypeDef *hi2c, IMU_Data *data){
	//creating buffer of 8 bit for i2c
	uint8_t buff[14];
	// read memory again from buffer now
	if(HAL_I2C_Mem_Read(hi2c, IMU_MPU6050_ADDR, MPU6050_REG_ACCEL_XOUT_H, 1, buff, 1, HAL_MAX_DELAY)!=HAL_OK)
		return 1;

	//buffer data

	data->Accel_X=(int16_t)(buff[0] << 8 | buff[1]);	// 8-but for accel_x data in frame
	data->Accel_Y=(int16_t)(buff[2] << 8 | buff[3]);	// 8-bit accel_y data in frame
	data->Accel_Z=(int16_t)(buff[4] << 8 | buff[5]);	// 8-bit accel_z data in frame
	data->Temp	 =(int16_t)(buff[6] << 8 | buff[7]);
	data->Gyro_X=(int16_t)(buff[8] << 8 | buff[9]);
	data->Gyro_Y=(int16_t)(buff[10] << 8 | buff[11]);
	data->Gyro_Z=(int16_t)(buff[12] << 8 | buff[13]);
	return 0;
}

int kalman_filter(IMU_Data *data){
	// Placeholder for Kalman filter implementation
	// This function should implement the Kalman filter algorithm
	// to process the IMU data and return filtered values.
	// Currently, it returns 0 as a placeholder.

	// orientation and position estimation logic would go here
	// values of ax, ay, az are we get from IMU_read function
	// and we can use them to estimate the orientation and position
	float roll_accel = atan(data->Accel_x/data->Accel_z) * 180 / M_PI; // Convert to degrees
	float pitch_accel = atan(data->Accel_y/data->Accel_z) * 180 / M_PI; // Convert to degrees

	// values of gx, gy, gz are we get from IMU_read function
	float roll_gyro ;
	float pitch_gyro ;

	// Assuming gx, gy, gz are the gyro readings in degrees per second

	uint32_t last_time = 0;
	float dt;

	// In your loop:
	uint32_t now = HAL_GetTick(); // in milliseconds
	dt = (now - last_time) / 1000.0f; // convert to seconds
	last_time = now;

	pitch_gyro += gx * dt;  // dt = time step in seconds
	roll_gyro  += gy * dt;

	return 0;
}

void kalman_update(float newAngle, float newRate, float dt) {
    // Predict
    rate = newRate - bias;
    angle += dt * rate;

    // Update error covariance
    P[0][0] += dt * (dt*P[1][1] - P[0][1] - P[1][0] + Q_angle);
    P[0][1] -= dt * P[1][1];
    P[1][0] -= dt * P[1][1];
    P[1][1] += Q_bias * dt;

    // Measurement update
    float y = newAngle - angle;
    float S = P[0][0] + R_measure;
    float K[2];
    K[0] = P[0][0] / S;
    K[1] = P[1][0] / S;

    // Update state
    angle += K[0] * y;
    bias  += K[1] * y;

    // Update error covariance matrix
    float P00_temp = P[0][0], P01_temp = P[0][1];
    P[0][0] -= K[0] * P00_temp;
    P[0][1] -= K[0] * P01_temp;
    P[1][0] -= K[1] * P00_temp;
    P[1][1] -= K[1] * P01_temp;
}
