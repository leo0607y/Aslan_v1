/*
 * LED.h
 *
 *  Created on: Nov 23, 2025
 *      Author: reoch
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#include "stm32f4xx_hal.h"

typedef enum
{
    LED_RED,
    LED_GREEN,
    LED_BLUE,
    LED_WHITE,
    LED_YELLOW,
    LED_CYAN,
    LED_MAGENTA,
    LED_OFF
} led_color_t;

void LED(led_color_t c);
void LED_SetColor(led_color_t c); // エイリアス

void LED1(int state1);
void LED2(int state2);
void LED3(int state3);
void LED4(int state4);
void LED5(int state5);
void LED6(int state6);

#endif /* INC_LED_H_ */
