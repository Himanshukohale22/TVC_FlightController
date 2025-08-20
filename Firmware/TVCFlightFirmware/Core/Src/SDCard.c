/*
 * SDCard.c
 *
 *  Created on: Jun 30, 2025
 *      Author: if-himanshu
 */


#include "main.h"
#include "SDCard.h"
#include "stdio.h"


int16_t sd_card_init(I2C_HandleTypeDef hspi1);


void csv_file(){

	//create file data_logger.csv
	FILE *fp = fopen(data_logger.csv,w);
	//check if open successefully
	if (fp==NULL){
		printf("unable to open csv file");
		return 0;

	}
//	write header
// whenever system get started header should be same

// make inturrupt for one pin to make this header same for all

	fprintf(fp, " time_stamp, Accel_X, Accel_Y, Accel_Z, Gyro_X, Gyro_Y, Gyro_Z, Temp, pressure, altitude ");

	// append data
	fprintf(fp, "%u,%d,%d,%d,%d,%d,%d,%d,%u,%u,%u\n",
	        data.time_stamp, data.Accel_X, data.Accel_Y, data.Accel_Z,
	        data.Gyro_X, data.Gyro_Y, data.Gyro_Z,
	        data.Temp, data.pressure, data.temp, data.altitude);

// close file after write
	fclose(fp);
	return 0;

}


