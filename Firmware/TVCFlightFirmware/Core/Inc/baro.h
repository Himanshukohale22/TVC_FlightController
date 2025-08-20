/*
 * baro.h
 *
 *  Created on: Jun 30, 2025
 *      Author: if-himanshu
 */

#ifndef INC_BARO_H_
#define INC_BARO_H_

#include "main.h"
#include "stm32g491xx.h"
#include "stdint.h"


#define BARO_ID_REG 0x76 // when SDO is connected to GND 0X77 when to pwr
#define BARO_MEMORY_ADD 0xD0
#define	BARO_RESET 0x0E
#define BARO_STATUS 0xF3
#define BARO_CONFIG 0XF5
#define BARO_PRESS 0XF7   // 0xF7 -- 0XF9
#define BARO_TEMP 0XF4    //
#define BARO_CTLR_MEAS 0XF
#define BARO_ID 0x53

typedef stuct baro(){
	uint16_t pressure;
	uint16_t temp;
	uint16_t altitude;
} baro_data;



int16_t baro_init(I2C_HandleTypeDef *hi2c);
int16_t baro_read(I2C_HandleTypeDef *hi2c, baro_data *data);


#endif /* INC_BARO_H_ */
