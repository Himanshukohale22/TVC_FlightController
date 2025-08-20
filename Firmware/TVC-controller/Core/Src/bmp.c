/*
 * bmp.c
 *
 *  Created on: Jul 15, 2025
 *      Author: if-himanshu
 */

#include "math.h"
#include "bmp.h"
#include "main.h"
#include "imu.h"

int16_t Baro_init(I2C_HandleTypeDef *hi2c){
	uint8_t id= 0;  // data buffer

    HAL_I2C_Mem_Read(hi2c, BMP280_I2C_READ_ADDR, BMP280_I2C_ADDR_VDD, 1, &id, 1, 100);
    if (id != BMP280_I2C_ID) {
    	printf("Device not found");
        printf("BMP280 not detected! ID = 0x%02X\n", id);
        return -1;
    }

    uint8_t config = BMP280_CONFIG_VAL;
	HAL_I2C_Mem_Write(hi2c, BMP280_I2C_READ_ADDR, BMP280_I2C_PRESSURE, 1, &config, 1, 100);
	HAL_Delay(1000);

	return 0;

}

int16_t Baro_read(I2C_HandleTypeDef *hi2c, Baro_data *Data){
	//create buffer to store raw pressure data
	uint8_t raw_press[6];
	//reading raw data from press register 0xf7
	HAL_I2C_Mem_Read(hi2c, BMP280_ID_ADDR_GND, BMP280_I2C_PRESSURE, 1, raw_press, 1, 100);

	//extracting datafrom register
    int32_t adc_P = ((uint32_t)(raw_press[0]) << 12) | ((uint32_t)(raw_press[1]) << 4) | (raw_press[2] >> 4);
    int32_t adc_T = ((uint32_t)(raw_press[3]) << 12) | ((uint32_t)(raw_press[4]) << 4) | (raw_press[5] >> 4);

    // Final values for pressure and temp
    BMP280.UAV_Pressure = adc_P;
    BMP280.UAV_BARO_Temp = adc_T;
    BMP280.UAV_Altitude = 44330.0f * (1.0f - powf((adc_P / sea_level), 0.1903f));

	return 0;
}

int16_t ApogeeDetection(Baro_data *Data, Apogee_data *apogeedata, IMU_raw_Data *imu_data){
	//model values
	//kalman filter
	//final values for vel/accel/alti and final shoot-up pwm
	return 0;
}


