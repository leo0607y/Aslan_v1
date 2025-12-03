/*
 * Motor.h
 *
 *  Created on: Dec 3, 2025
 *      Author: reoch
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "stm32f4xx_hal.h"
#include "tim.h"

#define MAX_COUNTER_PERIOD 1399
#define MIN_COUNTER_PERIOD -1399

extern int16_t motor_l;
extern int16_t motor_r;
extern int16_t mon_rev_l;
extern int16_t mon_rev_r;

void Motor_Init(void);
void motorCtrlFlip(void);
void setMotor(int16_t l, int16_t r);

#endif /* INC_MOTOR_H_ */
