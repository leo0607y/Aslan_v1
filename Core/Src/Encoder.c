/*
 * Encorder.c
 *
 *  Created on: Nov 23, 2025
 *      Author: reoch
 */


#include "Encoder.h"
#include "tim.h"

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern int32_t enc_l_total;
extern int32_t enc_r_total;
int32_t enc_l_total = 0;
int32_t enc_r_total = 0;
static int16_t enc_l_cnt, enc_r_cnt;     // カウント値・累積値
static float distance_1ms;               // 1msごとの移動距離
static float distance;              // 10mm単位の距離
static float sab_distance_10mm;          // サブ用10mm距離
static float total_distance;             // 総走行距離
static float goal_judge_distance;        // ゴール判定用距離
static float side_line_judge_distance;   // サイドライン判定用距離
static float distance_cross_line_ignore; // クロスライン無視距離
static float distance_side_line_ignore;  // サイドライン無視距離
static float speed_cnt;                  // 速度カウント
static int16_t enc_l_cnt = 0;
static int16_t enc_r_cnt = 0;

void Encoder_Init(void)
{
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL); // 左エンコーダ開始
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL); // 右エンコーダ開始
    TIM3->CNT = CNT_OFFSET;
    TIM4->CNT = CNT_OFFSET;
}

void Encoder_Update(void)
{
    // 現在のカウント値を取得
    enc_l_cnt = TIM3->CNT - CNT_OFFSET;
    enc_r_cnt = CNT_OFFSET - TIM4->CNT;

    // 累積カウントに加算
    enc_l_total += enc_l_cnt;
    enc_r_total += enc_r_cnt;
    // 距離計算（両輪平均）
    distance_1ms = DISTANCE_PER_CNT * (enc_l_cnt + enc_r_cnt) / 2;
    distance += distance_1ms;
    sab_distance_10mm += distance_1ms;
    total_distance += distance_1ms;
    goal_judge_distance += distance_1ms;
    side_line_judge_distance += distance_1ms;
    distance_cross_line_ignore += distance_1ms;
    distance_side_line_ignore += distance_1ms;

    // カウンタリセット
    TIM3->CNT = CNT_OFFSET;
    TIM4->CNT = CNT_OFFSET;
}

void getEncoderCnt(int16_t *cnt_l, int16_t *cnt_r)
{
    *cnt_l = enc_l_cnt;
    *cnt_r = enc_r_cnt;
}

void getEncoderTotal(int16_t *total_l, int16_t *total_r)
{
    *total_l = (int16_t)((uint16_t)enc_l_total);
    *total_r = (int16_t)((uint16_t)enc_r_total);
}

float getTotalDistance()
{
    return total_distance;
}

float getGoalJudgeDistance()
{
    return goal_judge_distance;
}

float getSideLineJudgeDistance()
{
    return side_line_judge_distance;
}

void setTotalDistance(float distance)
{
    total_distance = distance;
}

float getCrossLineIgnoreDistance(void)
{
    return distance_cross_line_ignore;
}

float getSideLineIgnoreDistance(void)
{
    return distance_side_line_ignore;
}

void clearTotalDistance()
{
    total_distance = 0;
}

void clearGoalJudgeDistance()
{
    goal_judge_distance = 0;
}

void clearSideLineJudgeDistance()
{
    side_line_judge_distance = 0;
}

void clearCrossLineIgnoreDistance(void)
{
    distance_cross_line_ignore = 0;
}

void clearSideLineIgnoreDistance(void)
{
    distance_side_line_ignore = 0;
}

void resetEncoderCnt(void)
{
    TIM3->CNT = CNT_OFFSET;
    TIM4->CNT = CNT_OFFSET;
}

float getDistance(void)
{
    return distance;
}

void clearDistance(void)
{
    distance = 0;
}

float getspeedcount(void)
{
    if (sab_distance_10mm >= 10)
    {
        speed_cnt += 0.1;
        sab_distance_10mm = 0;
    }
    return speed_cnt;
}

void clearspeedcount(void)
{
    speed_cnt = 0.1;
}

void getWheelDistance(float *dist_l, float *dist_r)
{
    *dist_l = (float)enc_l_total * DISTANCE_PER_CNT;
    *dist_r = (float)enc_r_total * DISTANCE_PER_CNT;
}


