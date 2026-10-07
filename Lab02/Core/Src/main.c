/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lab_exercise.h"
#include "functions.h"
#include "software_timer.h"
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
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */
/* Lab counters: TIM2 tick = 10 ms, with 8 MHz / 8000 / 10. */
const int MAX_LED = 4;
int index_led = 0;
#if LAB_EXERCISE == 1 || LAB_EXERCISE == 2 || LAB_EXERCISE == 3
int counter_7seg = 50;  /* 500 ms per digit */
#else
int counter_7seg = 25;  /* 250 ms per digit: four-digit scan = 1 Hz */
#endif
int counter_dot = 100; /* 1000 ms, independent of the display scan */
#if LAB_EXERCISE == 5
/* Ex5 comparison test: restore the original callback counters. */
int counter = 25;
static uint8_t idx = 0;
#endif
#if LAB_EXERCISE >= 5
int hour = 15, minute = 8, second = 50;
#endif
#if LAB_EXERCISE >= 9
int index_led_matrix = 0;
#endif
#if LAB_EXERCISE == 10
int shift_count = 0;
#endif
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  /* D1 and DOT are active-low: start with both LED outputs OFF. */
  HAL_GPIO_WritePin(led_red_GPIO_Port, led_red_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(dot_GPIO_Port, dot_Pin, GPIO_PIN_SET);
  clearEnableVsLED();

#if LAB_EXERCISE == 1
  led_buffer[0] = 1;
  led_buffer[1] = 2;
#elif LAB_EXERCISE == 2
  led_buffer[0] = 1;
  led_buffer[1] = 2;
  led_buffer[2] = 3;
  led_buffer[3] = 0;
#elif LAB_EXERCISE >= 5
  updateClockBuffer(led_buffer, hour, minute, second);
#endif
#if LAB_EXERCISE == 5
  /* Original Ex5 startup: first digit is displayed by TIM2 after 250 ms. */
  index_led = 0;
#else
  update7SEG(0);
  index_led = 1;
#endif

#if LAB_EXERCISE >= 6
  setTimer0(1000);
#endif
#if LAB_EXERCISE >= 8
  setTimer1(250);
#endif
#if LAB_EXERCISE >= 9
  setTimer2(10);  /* One column per tick: 8 * 10 ms = 80 ms per frame. */
#endif
#if LAB_EXERCISE == 10
  setTimer3(500);  /* Animation speed is independent of matrix refresh. */
#endif

  HAL_TIM_Base_Start_IT(&htim2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
#if LAB_EXERCISE == 5
    /* Ex5: the clock skeleton from the lab uses HAL_Delay(1000). */
    second++;
    if (second >= 60) { second = 0; minute++; }
    if (minute >= 60) { minute = 0; hour++; }
    if (hour >= 24) { hour = 0; }
    updateClockBuffer(led_buffer, hour, minute, second);
    HAL_Delay(1000);
#elif LAB_EXERCISE == 6
    /* Ex6: the software-timer LED example, preserving Ex5 above. */
    if (timer0_flag == 1)
    {
      HAL_GPIO_TogglePin(led_red_GPIO_Port, led_red_Pin);
      setTimer0(2000);
    }
#elif LAB_EXERCISE >= 7
    /* Ex7: replace the clock delay with a software-timer flag. */
    if (timer0_flag == 1)
    {
      second++;
      if (second >= 60) { second = 0; minute++; }
      if (minute >= 60) { minute = 0; hour++; }
      if (hour >= 24) { hour = 0; }
      updateClockBuffer(led_buffer, hour, minute, second);
      HAL_GPIO_TogglePin(dot_GPIO_Port, dot_Pin);
      HAL_GPIO_TogglePin(led_red_GPIO_Port, led_red_Pin);
      setTimer0(1000);
    }
#endif

#if LAB_EXERCISE >= 8
    /* Ex8: move display processing from the interrupt into main. */
    if (flags[1] == 1)
    {
      update7SEG(index_led);
      index_led++;
      if (index_led >= MAX_LED) index_led = 0;
      setTimer1(250);
    }
#endif
#if LAB_EXERCISE >= 9
    /* Ex9: scan the A matrix in main, using a separate software timer. */
    if (flags[2] == 1)
    {
      updateLEDMatrix(index_led_matrix);
      index_led_matrix++;
      if (index_led_matrix >= MAX_LED_MATRIX) index_led_matrix = 0;
      setTimer2(10);
    }
#endif
#if LAB_EXERCISE == 10
    /* Ex10: move A left every 500 ms; reload the original after 8 shifts. */
    if (flags[3] == 1)
    {
      shiftLeft(matrix_buffer);
      shift_count++;
      if (shift_count >= 8)
      {
        for (int i = 0; i < 8; i++) matrix_buffer[i] = A_pattern[i];
        shift_count = 0;
      }
      setTimer3(500);
    }
#endif
    /* Ex1..4: all processing is in the interrupt; main stays empty. */
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, em0_Pin|em1_Pin|dot_Pin|led_red_Pin
                          |en0_Pin|en1_Pin|en2_Pin|en3_Pin
                          |em2_Pin|em3_Pin|em4_Pin|em5_Pin
                          |em6_Pin|em7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, seg0_Pin|seg1_Pin|seg2_Pin|row2_Pin
                          |row3_Pin|row4_Pin|row5_Pin|row6_Pin
                          |row7_Pin|seg3_Pin|seg4_Pin|seg5_Pin
                          |seg6_Pin|row0_Pin|row1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : em0_Pin em1_Pin dot_Pin led_red_Pin
                           en0_Pin en1_Pin en2_Pin en3_Pin
                           em2_Pin em3_Pin em4_Pin em5_Pin
                           em6_Pin em7_Pin */
  GPIO_InitStruct.Pin = em0_Pin|em1_Pin|dot_Pin|led_red_Pin
                          |en0_Pin|en1_Pin|en2_Pin|en3_Pin
                          |em2_Pin|em3_Pin|em4_Pin|em5_Pin
                          |em6_Pin|em7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : seg0_Pin seg1_Pin seg2_Pin row2_Pin
                           row3_Pin row4_Pin row5_Pin row6_Pin
                           row7_Pin seg3_Pin seg4_Pin seg5_Pin
                           seg6_Pin row0_Pin row1_Pin */
  GPIO_InitStruct.Pin = seg0_Pin|seg1_Pin|seg2_Pin|row2_Pin
                          |row3_Pin|row4_Pin|row5_Pin|row6_Pin
                          |row7_Pin|seg3_Pin|seg4_Pin|seg5_Pin
                          |seg6_Pin|row0_Pin|row1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* Ex1..3: 50 ticks per digit. Ex4..7: 25 ticks per digit.
 * Ex2/3 full scan: 4 * 500 ms = 2 s = 0.5 Hz.
 * Ex4/5 full scan: 4 * 250 ms = 1 s = 1 Hz.
 * Ex1..4: DOT/LED have their own 100-tick (1-second) counter.
 * Ex5 comparison test: no DOT/LED toggling.
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance != TIM2) return;

#if LAB_EXERCISE >= 6
  timer_run();
#endif

#if LAB_EXERCISE == 5
  /* Original Ex5 callback, isolated from the other exercises. */
  counter--;
  if (counter == 0)
  {
    counter = 25;
    clearEnableVsLED();
    display7seg(idx, led_buffer[idx]);
    idx = (idx + 1) % 4;
  }
#elif LAB_EXERCISE <= 7
  counter_7seg--;
  if (counter_7seg <= 0)
  {
#if LAB_EXERCISE <= 3
    counter_7seg = 50;
#else
    counter_7seg = 25;
#endif

#if LAB_EXERCISE <= 2
    /* Ex1/2: select a digit and write its segment value directly. */
    display7seg(index_led, led_buffer[index_led]);
#else
    /* Ex3 introduces update7SEG(); Ex4..7 reuse the same function. */
    update7SEG(index_led);
#endif
    index_led++;
#if LAB_EXERCISE == 1
    if (index_led >= 2) index_led = 0;
#else
    if (index_led >= MAX_LED) index_led = 0;
#endif
  }
#endif

#if LAB_EXERCISE <= 4
  counter_dot--;
  if (counter_dot <= 0)
  {
    counter_dot = 100;
    HAL_GPIO_TogglePin(led_red_GPIO_Port, led_red_Pin);
#if LAB_EXERCISE >= 2
    HAL_GPIO_TogglePin(dot_GPIO_Port, dot_Pin);
#endif
  }
#elif LAB_EXERCISE == 6
  /* Ex6 keeps display/DOT scanning in the ISR; PA5's timer example is in main. */
  counter_dot--;
  if (counter_dot <= 0)
  {
    counter_dot = 100;
    HAL_GPIO_TogglePin(dot_GPIO_Port, dot_Pin);
  }
#endif
  /* Ex8..10: the ISR only services software timers. */
}

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

#ifdef  USE_FULL_ASSERT
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
