/*
 * SW.c
 *
 *  Created on: Nov 23, 2025
 *      Author: reoch
 */


#include "SW.h"

uint16_t SWL(uint8_t buttan)
{
    uint16_t ret = 0;
    if (buttan == 'L' && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == 0)
    {
        ret = 1;
        LED1(0);
    }
    else
    {
    	LED1(1);
    }
    return ret;
}

uint16_t SWR(uint8_t buttan)
{
    uint16_t ret = 0;
    if (buttan == 'R' && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8) == 0)
    {
        ret = 2;
        LED2(0);
    }
    else
    {
    	LED2(1);
    }
    return ret;
}
