/*
 * imu.c
 *
 *  Created on: Jul 15, 2025
 *      Author: if-himanshu
 */

#include "imu.h"
#include "main.h"
#include "math.h"

//uint8_t IMU_init(I2C_HandleTypeDef *hi2c){
//	uint8_t check; 	// check buffer
//	uint8_t data;	// data buffer
//	// I2C read for data
//}

// keywords to know
//θ (theta: pitch) and ψ (psi: yaw)

// extern used in .h but have to define again in .c file
IMU_raw_Data raw_data;  // defien it here only once
model_values rp_model_values;
kalman_data rp_fused_values;

// Kalman filter state variables
float theta_n = 0.0f;   // a priori estimation of Theta
float theta_p = 0.0f;   // a posterior estimation of Theta
float phi_n   = 0.0f;   // a priori estimation of Phi
float phi_p   = 0.0f;   // a posterior estimation of Phi

// Covariance variables
float P_theta_n = 0.0f; // a priori covariance of Theta
float P_phi_n   = 0.0f; // a priori covariance of Phi
float P_theta_p = 0.0f; // a posterior covariance of Theta
float P_phi_p   = 0.0f; // a posterior covariance of Phi

// Kalman Gains
float K_theta = 0.0f;   // Kalman gain for Theta
float K_phi   = 0.0f;   // Kalman gain for Phi

// Noise and disturbance covariances
float Q = 0.1f;         // Covariance of disturbance
float R = 4.0f;         // Covariance of measurement noise

// Orientation angles
float theta_pitch = 0.0f;
float psi_yaw     = 0.0f;
float phi_roll    = 0.0f;

// Time delta
float dt = 0.01f;       // Sampling time in seconds (e.g., 10ms)


//resolution constants

float accel_resolution = 16384.0f;  // g
float gyro_resolution = 131.0f;    // degrees per second

//////////////////////////////////////Check for IMU /////////////////////////////////////////

uint8_t IMU_init(I2C_HandleTypeDef *hi2c) {

	// how we check for imu is :  we read the starting data for imu and check i address ID matched
	//							  then we create a data buffer with nothing in it and start writing in i2c peripheral.
    uint8_t check = 0; // declare local variable to store address
    uint8_t data = 0; // declare local variable to store data

    // Check WHO_AM_I register / read register
    if (HAL_I2C_Mem_Read(hi2c, IMU_MPU6050_ADDR, MPU6050_REG_PWR_MGMT_1, 1, &check, 1, HAL_MAX_DELAY) != HAL_OK){
    	printf("unable to read register check wiring ");
        return 1;
    }

    if (check != 0x68) // address of imu to check
        return printf("address is not 0x68, not connected");  // Not detected

    // Wake up the MPU6050
    // disable sleep mode by writing bit '0' in power management register
    data = 0;
    if (HAL_I2C_Mem_Write(hi2c, IMU_MPU6050_ADDR, MPU6050_REG_PWR_MGMT_1, 1, &data, 1, HAL_MAX_DELAY) != HAL_OK){
    	printf("IMU wake up, ready to get orient");
        return 1;
    }

    return 0; // Success
}

//////////////////////////////////////Read IMU Raw values/////////////////////////////////////


uint8_t IMU_read(I2C_HandleTypeDef *hi2c, IMU_raw_Data *data){
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

//converting raw data into meaningfull units for accelaration and velocities

uint8_t IMU_data_units(IMU_raw_Data *data, values_unit *values_data){
	values_data->ax = raw_data.Accel_X / accel_resolution;
	values_data->ay = raw_data.Accel_Y / accel_resolution;
	values_data->az = raw_data.Accel_Z / accel_resolution;   // should 1g=9.8m/s^2

	values_data->gx = raw_data.Gyro_X / gyro_resolution;
	values_data->gy = raw_data.Gyro_Y / gyro_resolution;
	values_data->gz = raw_data.Gyro_Z / gyro_resolution;

	values_data->Temp_c = (raw_data.Temp/ 340.0f) + 36.53; // degrees Celsius

	printf("units values ax:%f,ay:%f,az:%f \n gx:%f,gy:%f,gz:%f \n temp : %f",
			values_data->ax,values_data->ay,values_data->az,
			values_data->gx, values_data->gy, values_data->gz,
			values_data->Temp_c);
	return 0;
}

//////////////////////////////////////calculating model values/////////////////////////////////////

uint8_t IMU_CalculationModel(IMU_raw_Data *data, model_values *model_data) {

    // Check for division by zero in accelerometer
	// This is for ensuring getting values from sensors(not so to be correct)
    if (data->Accel_Z == 0) return 1;

    // --- ACCELEROMETER MODEL (Angle estimation from accel) ---
    model_data->theta_s = atan2f((float)data->Accel_X, (float)data->Accel_Z) * (180.0f / M_PI);  // Pitch
    model_data->phi_s   = atan2f((float)data->Accel_Y, (float)data->Accel_Z) * (180.0f / M_PI);  // Roll/Yaw approx.

    // --- GYROSCOPE MODEL (Angle estimation by integration) ---
    float gyro_x_dps = (float)data->Gyro_X / GYRO_SENSITIVITY; // Roll rate
    float gyro_y_dps = (float)data->Gyro_Y / GYRO_SENSITIVITY; // Pitch rate
//    float gyro_z_dps = (float)data->Gyro_Z / GYRO_SENSITIVITY; // yaw

    // Integrate angular rate over time
    model_data->theta_k += gyro_y_dps * dt;  // Pitch (θ)
    model_data->phi_k   += gyro_x_dps * dt;  // Roll (φ)

    return 0;
}


//////////////////////////////////////Fused values for roll/pitch/yaw /////////////////////////////////////


uint8_t IMU_kalmanFilter(const IMU_raw_Data *data, const model_values *model_data, kalman_data *k) {
    // ---- Roll (Theta) Estimation ----
    k->P_theta_n = k->P_theta_p + k->Q;
    k->K_theta = k->P_theta_n / (k->P_theta_n + k->R);
    k->theta_n = k->theta_p - k->dt * data->Gyro_Y;  // Gyro_Y gives w_y
    k->theta_p = k->theta_n + k->K_theta * (model_data->theta_s - k->theta_n); // theta_s from accelerometer
    k->P_theta_p = (1 - k->K_theta) * k->P_theta_n;

    // ---- Pitch (Phi) Estimation ----
    k->P_phi_n = k->P_phi_p + k->Q;
    k->K_phi = k->P_phi_n / (k->P_phi_n + k->R);
    k->phi_n = k->phi_p + k->dt * data->Gyro_X;  // Gyro_X gives w_x
    k->phi_p = k->phi_n + k->K_phi * (model_data->phi_s - k->phi_n);  // phi_s from accelerometer
    k->P_phi_p = (1 - k->K_phi) * k->P_phi_n;

    return 0; // success
}
