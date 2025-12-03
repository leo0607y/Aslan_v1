/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include <stdbool.h>
#include <math.h>
#include <string.h>

#include "Encoder.h"
#include "SW.h"
#include "LED.h"
#include "Sensor.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern DMA_HandleTypeDef hdma_adc1;
extern SPI_HandleTypeDef hspi3;

uint32_t timer, timer1, timer2;
uint32_t adc_values[ADC_NUM];
extern volatile uint32_t time_ms;
int lion = 0;
int bayado = 0;
int sw = 0;
int sw2 = 0;

bool calibration_mode = true;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int __io_putchar(int ch)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
	return ch;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM6)
	{ // 1ms
		timer++;
		timer2++;

		Sensor_Update();
		Encoder_Update();
	}
	if (htim->Instance == TIM7)
	{ // 0.1ms
		timer1++;

		Sensor_BufferUpdate();
	}
}
void Init(void)
{
	// タイマー起動（センサー更新に必要）
	HAL_TIM_Base_Start_IT(&htim6); // 1ms timer start
	HAL_TIM_Base_Start_IT(&htim7); // 0.1ms timer start

	LED(LED_RED);
	Sensor_Init(); // ADC DMA開始

	HAL_Delay(500); // ADC安定待ち

	LED(LED_YELLOW);	// キャリブレーション開始を示す
	Sensor_Calibrate(); // 左ボタンを押すまでキャリブレーション

	LED(LED_GREEN); // キャリブレーション完了
	HAL_Delay(500);

	Encoder_Init(); // エンコーダ初期化

	LED(LED_OFF);
	printf("System ready!\r\n");
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_USART1_UART_Init();
	MX_ADC1_Init();
	MX_TIM8_Init();
	MX_TIM3_Init();
	MX_TIM4_Init();
	MX_TIM6_Init();
	MX_TIM7_Init();
	MX_TIM1_Init();
	MX_SPI3_Init();
	/* USER CODE BEGIN 2 */
	Init();

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */
		/* USER CODE BEGIN 3 */
		if (StatusR('R') == 2 && sw2 == 0)
		{
			timer2 = 0;
			sw2 = 1;
		}
		if (StatusR('R') == 2 && timer2 > 20 && sw2 == 1)
		{
			sw2 = 2;
		}
		if (timer2 > 40 && sw2 == 1)
		{
			sw2 = 0;
		}
		if (StatusR('R') == 0 && sw2 == 2)
		{
			bayado = lion;
			printf("Right button released! lion=%d -> bayado=%d\r\n", lion, bayado);
			sw2 = 0;
		}
		if (StatusL('L') == 1 && sw == 0)
		{
			timer = 0;
			sw = 1;
		}
		if (StatusL('L') == 1 && timer > 20 && sw == 1)
		{
			sw = 2;
		}
		if (timer > 40 && sw == 1)
		{
			sw = 0;
		}
		if (StatusL('L') == 0 && sw == 2)
		{
			lion++;
			if (lion >= 8)
				lion = 0;
			sw = 0;
		}

		switch (lion)
		{
		case 0:
			LED(LED_RED);
			break;
		case 1:
			LED(LED_BLUE);
			break;
		case 2:
			LED(LED_GREEN);
			break;
		case 3:
			LED(LED_CYAN);
			break;
		case 4:
			LED(LED_MAGENTA);
			break;
		case 5:
			LED(LED_YELLOW);
			break;
		case 6:
			LED(LED_WHITE);
			break;
		case 7:
			LED(LED_OFF);
			break;
		default:
			break;
		}

		// Current status display
		static uint32_t last_print_time = 0;
		if (timer1 - last_print_time > 1000) // 100ms毎に表示
		{
			printf("lion=%d, bayado=%d\r\n", lion, bayado);
			last_print_time = timer1;
		}

		static int last_bayado = -1;
		if (bayado != last_bayado && bayado >= 0)
		{
			switch (bayado)
			{
			case 0:
				printf("=== Mode 0: RED - Standby ===\r\n");
				break;
			case 1:
				printf("=== Mode 1: BLUE - Running ===\r\n");
				break;
			case 2:
				printf("=== Mode 2: GREEN - Mode 2 ===\r\n");
				break;
			case 3:
				printf("=== Mode 3: CYAN - Mode 3 ===\r\n");
				break;
			case 4:
				printf("=== Mode 4: MAGENTA - Mode 4 ===\r\n");
				break;
			case 5:
				printf("=== Mode 5: YELLOW - Mode 5 ===\r\n");
				break;
			case 6:
				printf("=== Mode 6: WHITE - Mode 6 ===\r\n");
				break;
			case 7:
				printf("=== Mode 7: OFF - Stopped ===\r\n");
				break;
			default:
				printf("=== Mode: Unknown ===\r\n");
				break;
			}
			last_bayado = bayado;
		}
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 12;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	{
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	   ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
