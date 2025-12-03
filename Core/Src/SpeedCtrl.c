/*
 * SpeedCtrl.c
 *
 *  Created on: Dec 3, 2025
 *      Author: reoch
 */

#include "SpeedCtrl.h"
#include "Encoder.h"
#include "Motor.h"
#include <stdio.h>

// グローバル変数
float target_speed_l = 0.0f;
float target_speed_r = 0.0f;
float current_speed_l = 0.0f;
float current_speed_r = 0.0f;
bool speed_control_enabled = false;

// PI制御パラメータ（左右共通）
static float Kp = 600.0f;
static float Ki = 300.0f;
static float integral_l = 0.0f;
static float integral_r = 0.0f;

#define CONTROL_PERIOD 0.001f // 1ms
#define MAX_PWM 1399.0f
#define MIN_PWM -1399.0f

/**
 * @brief 速度制御初期化
 */
void SpeedCtrl_Init(void)
{
    target_speed_l = 0.0f;
    target_speed_r = 0.0f;
    current_speed_l = 0.0f;
    current_speed_r = 0.0f;
    integral_l = 0.0f;
    integral_r = 0.0f;
    speed_control_enabled = false;

    printf("Speed Control Init: Kp=%.0f Ki=%.0f\r\n", Kp, Ki);
}

/**
 * @brief 速度制御更新（1msごとに呼ぶ）
 */
void SpeedCtrl_Update(void)
{
    static bool prev_enabled = false;

    // エンコーダから現在速度取得
    int16_t enc_l, enc_r;
    getEncoderCnt(&enc_l, &enc_r);
    current_speed_l = (float)enc_l * DISTANCE_PER_CNT * 1000.0f;
    current_speed_r = (float)enc_r * DISTANCE_PER_CNT * 1000.0f;

    // 速度制御ON時に積分リセット
    if (speed_control_enabled && !prev_enabled)
    {
        integral_l = 0.0f;
        integral_r = 0.0f;
    }

    if (speed_control_enabled)
    {
        // PI制御計算（左）
        float error_l = target_speed_l - current_speed_l;
        integral_l += error_l * CONTROL_PERIOD;
        if (integral_l > MAX_PWM / Ki)
            integral_l = MAX_PWM / Ki;
        if (integral_l < MIN_PWM / Ki)
            integral_l = MIN_PWM / Ki;
        float pwm_l = Kp * error_l + Ki * integral_l;

        // PI制御計算（右）
        float error_r = target_speed_r - current_speed_r;
        integral_r += error_r * CONTROL_PERIOD;
        if (integral_r > MAX_PWM / Ki)
            integral_r = MAX_PWM / Ki;
        if (integral_r < MIN_PWM / Ki)
            integral_r = MIN_PWM / Ki;
        float pwm_r = Kp * error_r + Ki * integral_r;

        // PWM制限
        if (pwm_l > MAX_PWM)
            pwm_l = MAX_PWM;
        if (pwm_l < MIN_PWM)
            pwm_l = MIN_PWM;
        if (pwm_r > MAX_PWM)
            pwm_r = MAX_PWM;
        if (pwm_r < MIN_PWM)
            pwm_r = MIN_PWM;

        setMotor((int16_t)pwm_l, (int16_t)pwm_r);
    }

    prev_enabled = speed_control_enabled;
}

/**
 * @brief 目標速度設定（左右個別）
 */
void setTargetSpeed(float speed_l, float speed_r)
{
    target_speed_l = speed_l;
    target_speed_r = speed_r;
}

/**
 * @brief 目標速度設定（両輪同じ）
 */
void setTarget(float speed)
{
    target_speed_l = speed;
    target_speed_r = speed;
}

/**
 * @brief 速度制御ON/OFF
 */
void enableSpeedControl(bool enable)
{
    speed_control_enabled = enable;
}

/**
 * @brief PIゲイン設定（チューニング用）
 */
void setSpeedGain(float kp, float ki)
{
    Kp = kp;
    Ki = ki;
    printf("Speed Gain Updated: Kp=%.0f Ki=%.0f\r\n", Kp, Ki);
}
