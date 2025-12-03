/*
 * SW.c
 *
 *  Created on: Nov 23, 2025
 *      Author: reoch
 */

#include "SW.h"
#include "LED.h"

uint16_t SWL(uint8_t buttan)
{
    uint16_t ret = 0;
    if (buttan == 'L' && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == 0)
    {
        ret = 1;
        LED2(0);
    }
    else
    {
        LED2(1);
    }
    return ret;
}

uint16_t SW_GetState(char sw)
{
    if (sw == 'L')
    {
        return SWL('L');
    }
    else if (sw == 'R')
    {
        return SWR('R');
    }
    return 0;
}
uint16_t SWR(uint8_t buttan)
{
    uint16_t ret = 0;
    if (buttan == 'R' && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8) == 0)
    {
        ret = 2;
        LED1(0);
    }
    else
    {
        LED1(1);
    }
    return ret;
}

uint16_t StatusL(uint8_t buttan)
{
    uint16_t ret = 0;
    if (buttan == 'L' && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == 0)
    {
        ret = 1;
        LED2(0);
    }
    else
    {
        LED2(1);
    }
    return ret;
}

uint16_t StatusR(uint8_t buttan)
{
    uint16_t ret = 0;
    if (buttan == 'R' && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8) == 0)
    {
        ret = 2;
        LED1(0);
    }
    else
    {
        LED1(1);
    }
    return ret;
}
