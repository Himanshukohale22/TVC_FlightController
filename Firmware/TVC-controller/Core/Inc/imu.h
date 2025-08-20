/*
 * imu.h
 *
 *  Created on: Jun 24, 2025
 *      Author: if-himanshu
 */

#ifndef INC_IMU_H_
#define INC_IMU_H_

//defines
#include <stdint.h>
#include "stm32g491xx.h"
#include "main.h"


// MPU6050 I2C address
#define IMU_MPU6050_ADDR     (0x68 << 1)  // 0xD0 (8-bit address for HAL)

// Register addresses
#define MPU6050_REG_PWR_MGMT_1    0x6B
#define MPU6050_REG_ACCEL_XOUT_H  0x3B
#define MPU6050_REG_GYRO_XOUT_H   0x43
#define MPU6050_REG_WHO_AM_I      0x75

// IMU data structure

#define ACCEL_SENSITIVITY   16384.0f  // LSB/g for ±2g
#define GYRO_SENSITIVITY    131.0f    // LSB/(°/s) for ±250°/s
#define TEMP_SENSITIVITY    340.0f
#define TEMP_OFFSET         36.53f
#define GYRO_SENSITIVITY 131.0f  // for ±250°/s

//#define  M_PI 3.14159

// can not do extern and initialization together
//extern to access these variables across project
extern float theta_n;     // a priori estimation of Theta
extern float theta_p;     // a posterior estimation on Theta
extern float phi_n;       // a priori estimation of Phi
extern float phi_p;       // a posterior estimation on Phi

// Covariances
extern float P_theta_n;
extern float P_phi_n;
extern float P_theta_p;
extern float P_phi_p;

// Kalman gains
extern float K_theta;
extern float K_phi;

// Noise parameters
extern float Q;
extern float R;

// Orientation angles
extern float theta_pitch;
extern float psi_yaw;
extern float phi_roll;

// Time step
extern float dt;


//constant values to changes from raw to units
//1 g = 9.80665 m/s² (standard gravity)

extern float accel_resolution;
extern float gyro_resolution;

//enum and struct for data

typedef struct IMU_raw_Data_t {
    int16_t Accel_X, Accel_Y, Accel_Z;
    int16_t Gyro_X, Gyro_Y, Gyro_Z;
    int16_t Temp;
} IMU_raw_Data;

typedef struct IMU_values_unit {
	float ax,ay,az;
	float gx,gy,gz;
	float Temp_c;
} values_unit;

typedef struct model_values_t {
	// accel orientation and rotational angles
	float theta_s;
	float phi_s;

	// gyro orientation and rotational angles
	float theta_k;
	float phi_k;
} model_values;

typedef struct kalman_data_t {
	// fused orientation and rotational angles
    float Q, R, dt;

    float theta_p, theta_n, K_theta, P_theta_p, P_theta_n;
    float phi_p, phi_n, K_phi, P_phi_p, P_phi_n;

} kalman_data;

typedef enum imu_status_e{
    IMU_OK = 0,
    IMU_ERR = 1,
    IMU_READ_FAIL,
    IMU_WRITE_FAIL,
    IMU_TIMEOUT,

} imu_status;

// variable to accesss from anywhere
// this extern for other files in projects to use same variables and assigned data to variables.
extern IMU_raw_Data raw_data;
extern model_values rp_model_values;
extern kalman_data rp_fused_values;


// function initialization
uint8_t IMU_init(I2C_HandleTypeDef *hi2c);   // we can initialize any i2c peripheral with this function
uint8_t IMU_data_units(IMU_raw_Data *data, values_unit *values_data);
uint8_t IMU_read(I2C_HandleTypeDef *hi2c, IMU_raw_Data *data);  // to read data from i2c / raw values
uint8_t IMU_CalculationModel(IMU_raw_Data *data, model_values *model_data);   // model values
uint8_t IMU_kalmanFilter(const IMU_raw_Data *data, const model_values *model_data, kalman_data *kalman_data); // fused values

#endif /* INC_IMU_H_ */

