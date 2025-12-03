/*
 * Sensor.h
 *
 *  Created on: Nov 23, 2025
 *      Author: reoch
 */

#ifndef INC_SENSOR_H_
#define INC_SENSOR_H_

#include "stm32f4xx_hal.h"
#include "stdbool.h"
#include "main.h"

#define ADC_NUM 12

extern float coefficient[ADC_NUM];
extern float offset[ADC_NUM];
extern int16_t sensor[ADC_NUM];
extern uint32_t adc_values[ADC_NUM];
extern float values_max[ADC_NUM];
extern float values_min[ADC_NUM];

void Sensor_Init(void);
void Sensor_BufferUpdate(void);
void Sensor_Update(void);
void Sensor_Calibrate(void);

#endif /* INC_SENSOR_H_ */
