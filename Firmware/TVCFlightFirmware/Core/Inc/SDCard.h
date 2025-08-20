/*
 * SDCard.h
 *
 *  Created on: Jun 30, 2025
 *      Author: if-himanshu
 */

#ifndef INC_SDCARD_H_
#define INC_SDCARD_H_

#include "stdio.h"


typedef struct file_data{
    int16_t Accel_X, Accel_Y, Accel_Z;
    int16_t Gyro_X, Gyro_Y, Gyro_Z;
    int16_t Temp;
	uint16_t pressure;
	uint16_t temp;
	uint16_t altitude;
	uint16_t time_stamp;
}data;


// pins defined address reg

// spi init function

int16_t sd_card_init(SPI_HandleTypeDef *SPI);

int16_t sd_card_write(SPI_HandleTypeDef *SPI, data *data);


//spi read function

#endif /* INC_SDCARD_H_ */
