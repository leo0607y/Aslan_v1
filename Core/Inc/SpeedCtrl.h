/*
 * SpeedCtrl.h
 *
 *  Created on: Dec 3, 2025
 *      Author: reoch
 */

#ifndef INC_SPEEDCTRL_H_
#define INC_SPEEDCTRL_H_

#include "stm32f4xx_hal.h"
#include <stdbool.h>

// 外部公開変数
extern float target_speed_l;
extern float target_speed_r;
extern float current_speed_l;
extern float current_speed_r;
extern bool speed_control_enabled;

// 関数プロトタイプ
void SpeedCtrl_Init(void);
void SpeedCtrl_Update(void);
void setTargetSpeed(float speed_l, float speed_r);
void setTarget(float speed);
void enableSpeedControl(bool enable);
void setSpeedGain(float kp, float ki); // ゲイン調整用

#endif /* INC_SPEEDCTRL_H_ */
