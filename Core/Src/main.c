/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */
int count = 0;
int period = 500;
uint16_t Dev=0x68;
/* USER CODE END PV */
int minuts = 0;
int seconds =0;
int hours =0;
int date = 0;
	int month = 0;
	int year = 0;
	int day1 = 0;
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM2_Init(void);
int roll1_down = 0;
int roll2_down = 0;
int roll1_top = 0;
int roll2_top = 0;
int roll1_count = 0;
int roll2_count = 0;
int init = 0;
/* USER CODE BEGIN PFP */
enum states_day {
	
	day_winter,
	day_summer,
	night_winter,
	night_summer
	
	
};
enum states_roll
{
	top,
	down,
	go_down,
	go_up
};
enum states_day cur_state_day=day_summer;
enum states_roll cur_state_roll=go_up;
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	count++;
	if (count>period) {
		HAL_GPIO_TogglePin(led_GPIO_Port, led_Pin);
		count = 0;
		
	}
		
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
  MX_I2C1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim2);\
	__enable_irq();
	//time_write(16,15,0, HR_FORMAT_24);
	//date_write(4, 6, 7, 23);
  /* USER CODE END 2 */
	
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		switch (cur_state_day)
		{
			case day_summer:{
				if ((hours > 22) || (hours < 8)){cur_state_day=night_summer;}
				if ((month <= 3) || (month >= 9)) {cur_state_day=day_winter;}
				switch (cur_state_roll)
				{
					case top:{if(HAL_GPIO_ReadPin(proj_GPIO_Port,proj_Pin) == 1){
												HAL_GPIO_WritePin(dir1_GPIO_Port, dir1_Pin, 1);
												HAL_GPIO_WritePin(dir2_GPIO_Port, dir2_Pin, 1);
												HAL_GPIO_WritePin(en1_GPIO_Port, en1_Pin, 1);
												HAL_GPIO_WritePin(en2_GPIO_Port, en2_Pin, 1);
												cur_state_roll = go_down;
											}
					}
					case down:{
						if(HAL_GPIO_ReadPin(proj_GPIO_Port,proj_Pin) == 0)
						{
							HAL_GPIO_WritePin(dir1_GPIO_Port, dir1_Pin, 0);
							HAL_GPIO_WritePin(dir2_GPIO_Port, dir2_Pin, 0);
							HAL_GPIO_WritePin(en1_GPIO_Port, en1_Pin, 1);
							HAL_GPIO_WritePin(en2_GPIO_Port, en2_Pin, 1);
							cur_state_roll = go_up;
						}
					}
					
					case go_up:{
						if (roll1_top||(HAL_GPIO_ReadPin(gerk1_GPIO_Port,gerk1_Pin)== 0))
						{
							HAL_GPIO_WritePin(en1_GPIO_Port, en1_Pin, 0);
						}
						if (roll2_top||(HAL_GPIO_ReadPin(gerk2_GPIO_Port,gerk2_Pin)== 0))
						{
							HAL_GPIO_WritePin(en1_GPIO_Port, en2_Pin, 0);
							roll2_top = 1;
							roll2_count = 0;
						}
						if(roll1_top&&roll2_top){
							cur_state_roll=top;
							init = 1;
						}
						
						if((HAL_GPIO_ReadPin(proj_GPIO_Port,proj_Pin) == 1)&&(init==1))
						{
							HAL_GPIO_WritePin(dir1_GPIO_Port, dir1_Pin, 1);
							HAL_GPIO_WritePin(dir2_GPIO_Port, dir2_Pin, 1);
							HAL_GPIO_WritePin(en1_GPIO_Port, en1_Pin, 1);
							HAL_GPIO_WritePin(en2_GPIO_Port, en2_Pin, 1);
							cur_state_roll = go_down;
						}	
					}
					
					case go_down:{
						if(roll1_down){
							HAL_GPIO_WritePin(en1_GPIO_Port, en1_Pin, 0);
						}
						
						if(roll2_down){
							HAL_GPIO_WritePin(en2_GPIO_Port, en2_Pin, 0);
						}
						
						if(roll1_down && roll2_down){
							cur_state_roll=down;
						}
						
						if(HAL_GPIO_ReadPin(proj_GPIO_Port,proj_Pin) == 0)
						{
							HAL_GPIO_WritePin(dir1_GPIO_Port, dir1_Pin, 0);
							HAL_GPIO_WritePin(dir2_GPIO_Port, dir2_Pin, 0);
							HAL_GPIO_WritePin(en1_GPIO_Port, en1_Pin, 1);
							HAL_GPIO_WritePin(en2_GPIO_Port, en2_Pin, 1);
							cur_state_roll = go_up;
						}
					}
				}
			}
			
			case day_winter:{
				if ((hours > 22) || (hours < 8)){cur_state_day=day_winter;}
				if ((month < 9) || (month > 3)) {cur_state_day=day_summer;}
			}
			
			case night_summer:{
				if ((8 < hours) || (hours < 22)){cur_state_day=day_summer;}
			}
			
			case night_winter:{
				if ((8 < hours) || (hours < 22)){cur_state_day=night_winter;}
			}
		}
	
		//HAL_GPIO_TogglePin(led_GPIO_Port, led_Pin);
		HAL_Delay(1000);
    /* USER CODE END WHILE */
		period = read_time(1)*10;
		minuts =read_time(2);
		hours = read_time(3);
		if (minuts>50||hours>22){
		period = period * 20;}
			date = read_date(1);
	month = read_date(2);
	year = read_date(3);
	day1 = read_date(4);
			if (date>5) {
				period=period*2;
			}
		
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_PLLCLK, RCC_MCODIV_1);
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

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
  htim2.Init.Prescaler = 3599;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 20;
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
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(led_GPIO_Port, led_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, out5v_Pin|en1_Pin|dir1_Pin|step1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, en2_Pin|dir2_Pin|step2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : led_Pin */
  GPIO_InitStruct.Pin = led_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(led_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : kn1_Pin kn2_Pin */
  GPIO_InitStruct.Pin = kn1_Pin|kn2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : gerk1_Pin gerk2_Pin proj_Pin datLight_Pin */
  GPIO_InitStruct.Pin = gerk1_Pin|gerk2_Pin|proj_Pin|datLight_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : out5v_Pin en1_Pin dir1_Pin step1_Pin */
  GPIO_InitStruct.Pin = out5v_Pin|en1_Pin|dir1_Pin|step1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : en2_Pin dir2_Pin step2_Pin */
  GPIO_InitStruct.Pin = en2_Pin|dir2_Pin|step2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
