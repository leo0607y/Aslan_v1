/*
 * Motor.c
 *
 *  Created on: Dec 3, 2025
 *      Author: reoch
 */

#include "Motor.h"

int16_t motor_l = 0;
int16_t motor_r = 0;
int16_t mon_rev_l = 0;
int16_t mon_rev_r = 0;

/**
 * @brief モータ初期化
 */
void Motor_Init(void)
{
    // PWM出力開始
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1); // 右モータPWM
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3); // 左モータPWM

    // 初期状態は停止
    setMotor(0, 0);
    motorCtrlFlip();
}

/**
 * @brief モータ制御反映
 */
void motorCtrlFlip(void)
{
    int16_t motor_pwm_l, motor_pwm_r;

    // 左モータ制御
    if (motor_l >= 0)
    {
        motor_pwm_l = motor_l;
        __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_3, motor_pwm_l);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET); // 正転
    }
    else
    {
        motor_pwm_l = motor_l * (-1);
        __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_3, motor_pwm_l);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET); // 逆転
    }

    // 右モータ制御
    if (motor_r >= 0)
    {
        motor_pwm_r = motor_r;
        __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, motor_pwm_r);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET); // 正転
    }
    else
    {
        motor_pwm_r = motor_r * (-1);
        __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, motor_pwm_r);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET); // 逆転
    }

    mon_rev_l = motor_pwm_l;
    mon_rev_r = motor_pwm_r;
}

/**
 * @brief モータ出力値を設定
 * @param l 左モータのPWM値 (-1399 ~ 1399)
 * @param r 右モータのPWM値 (-1399 ~ 1399)
 */
void setMotor(int16_t l, int16_t r)
{
    // 左モータの範囲制限
    if (l >= MAX_COUNTER_PERIOD)
        l = MAX_COUNTER_PERIOD;
    else if (l <= MIN_COUNTER_PERIOD)
        l = MIN_COUNTER_PERIOD;

    // 右モータの範囲制限
    if (r >= MAX_COUNTER_PERIOD)
        r = MAX_COUNTER_PERIOD;
    else if (r <= MIN_COUNTER_PERIOD)
        r = MIN_COUNTER_PERIOD;

    motor_l = l;
    motor_r = r;
}
