/*
 * baro.h
 *
 *  Created on: Jun 30, 2025
 *      Author: if-himanshu
 */


#include "main.h"
#include "baro.h"

// read memory
// saperate data using address
//print
// variable that will give real time values at any point and file


int16_t baro_init(I2C_HandleTypeDef hi2c2){
	uint8_t id= 0;  // data buffer

//	HAL_I2C_Mem_Read(hi2c, DevAddress(Device_id), MemAddress(device_memory_add), MemAddSize, pData(buffer to store), Size, Timeout)
	HAL_I2C_Mem_Read(&hi2c2, BARO_ID_REG, BARO_MEMORY_ADD, 1, &id, 1, 100);
	if (id != BARO_ID) return printf("device not detected");

	HAL_I2C_Mem_Write(&hi2c2,BARO_ID_REG , BARO_MEMORY_ADD, 1, BARO_CONFIG, 1, 100);
	HAL_Delay(1000);

	//for pressure and tempreture data from this register and store in another mem register
//	HAL_I2C_Mem_Write(&hi2c2, BARO_ID_REG, BARO_CONFIG, 1, BARO_CTLR_MEAS, 1, 100);
	//	HAL_I2C_Mem_Write(&hi2c2, BARO_ID_REG, BARO_CONFIG, 1, BARO_CTLR_MEAS, 1, 100);

	return 0;

}

int16_t baro_read(I2C_HandleTypeDef hi2c, baro_data *data){
	//create buffer to store raw pressure data
	uint8_t raw_press[6];
	uint8_t raw_temp[10];
	//reading raw data from press register 0xf7
	HAL_I2C_Mem_Read(&hi2c2, BARO_ID_REG, BARO_PRESS, 1, raw_press, 1, 100);
	HAL_I2C_Mem_Read_DMA(&hi2c2, BARO_ID_REG, BARO_TEMP, 1, raw_temp, 1, 100);

	//extracting datafrom register
    int32_t adc_P = ((uint32_t)(raw_press[0]) << 12) | ((uint32_t)(raw_press[1]) << 4) | (raw_press[2] >> 4);
    int32_t adc_T = ((uint32_t)(raw_press[3]) << 12) | ((uint32_t)(raw_press[4]) << 4) | (raw_press[5] >> 4);

    data-> pressure =  adc_P;
    data-> temp = adc_T;

    altitude


	return 0;

}





