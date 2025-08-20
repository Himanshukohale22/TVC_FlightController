/*
 * bmp.h
 *
 *  Created on: Jul 15, 2025
 *      Author: if-himanshu
 */

//#include "main.h"
#include "main.h"
#include "imu.h"

//#include "main.h"

#ifndef INC_BMP_H_
#define INC_BMP_H_

#define BMP280_ID_ADDR_GND 0x76
#define BMP280_I2C_ADDR_VDD 0x77
#define BMP280_I2C_READ_ADDR 0xEC   //(BMP280_I2C_ADDR_GND << 1)
#define BMP280_I2C_RESET 0x0E
#define BMP280_I2C_STATUS 0xF3
#define BMP280_I2C_PRESSURE 0xF7
#define BMP280_I2C_TEMP 0xF4
#define BMP280_I2C_ID 0x53
#define SENSOR_NAME 'BMP280_BAROMETER'
#define BMP280_CONFIG_VAL    0x27  // Normal mode, oversampling x1
//#define BMP280_REG_PRESS_MSB 0xF7

float sea_level = 1013.25f; // standard sea level pressure

typedef struct BARO_DATA_t{
	float UAV_Pressure;   // in pascles
	float UAV_BARO_Temp;       // in degree's
	float UAV_Altitude;  // in meters
} Baro_data;

typedef struct APOGEE_ALGO_t{
	float Apogee_ALTI;
	float Apogee_ACCEL;
	float Apogee_VEL;
} Apogee_data;


Baro_data BMP280;
Apogee_data APOGEE_DATA;

int16_t Baro_init(I2C_HandleTypeDef *hi2c);
int16_t Baro_read(I2C_HandleTypeDef *hi2c, Baro_data *Data);
int16_t ApogeeDetection(Baro_data *Data, Apogee_data *apogeedata, IMU_raw_Data *imu_data);


#endif /* INC_BMP_H_ */
