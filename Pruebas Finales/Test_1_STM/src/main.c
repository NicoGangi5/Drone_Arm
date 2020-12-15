/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "stm32f4xx_hal.h"
#include "queue.h"
#include "sd_hal_mpu6050.h"





/* Private function prototypes -----------------------------------------------*/
void vTaskPWM( void *pvParameters );
void vTaskI2C( void *pvParameters );
void vTaskADC( void *pvParameters );


void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_I2C3_Init(void);
static void MX_ADC3_Init(void);


/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;
SD_MPU6050 mpu1;

I2C_HandleTypeDef hi2c3;

ADC_HandleTypeDef hadc3;

QueueHandle_t xQueue1;
QueueHandle_t xQueue2;

TIM_OC_InitTypeDef sConfigOC;



/* USER CODE BEGIN Includes */
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
void HAL_ACD3_MspPostInit(ADC_HandleTypeDef* hadc3Handle);
void HAL_I2C3_MspPostInit(I2C_HandleTypeDef* hi2c3Handle);
void bsp_PWM_Duty(uint32_t Duty);
uint32_t Cambio_ADC_PWM (uint32_t Duty);
uint32_t Blinkea(char Led, uint32_t Delay, uint32_t Flag);
int32_t Acel_Ang (int32_t Acel);
uint32_t Pote_Ang (uint32_t Pote);
void start(void);


int main(void)
{
	HAL_Init();
	SystemClock_Config();

	BSP_LED_Init(LED3);
	BSP_LED_Init(LED4);
	BSP_LED_Init(LED5);
	BSP_LED_Init(LED6);


	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_TIM2_Init();
	MX_I2C3_Init();
	MX_ADC3_Init();


	if(SD_MPU6050_Init(&hi2c3,&mpu1,SD_MPU6050_Device_0,SD_MPU6050_Accelerometer_2G,SD_MPU6050_Gyroscope_250s ) != SD_MPU6050_Result_Ok)    BSP_LED_On(LED4);

	start();

	/* Create a queue capable of containing 10 unsigned long values. */
	xQueue1 = xQueueCreate(1, sizeof(uint32_t) );
	xQueue2 = xQueueCreate(1, sizeof(uint32_t) );
	if( (xQueue1 != NULL) && (xQueue2 != NULL) ){
		/* Create the other task in exactly the same way and at the same priority. */
		xTaskCreate( vTaskPWM, "PWM", 100, NULL, 2, NULL );
		xTaskCreate( vTaskI2C, "I2C", 100, NULL, 1, NULL );
		xTaskCreate( vTaskADC, "ADC3", 100, NULL, 3, NULL );

		/* Start the scheduler so the tasks start executing. */
		vTaskStartScheduler();
	}



	for(;;){
	}
}

//Led Naranja
void vTaskPWM( void *pvParameters){
	uint32_t Duty = 1000;
	int32_t A_x = 0;
	uint32_t Flag = 0;
	uint32_t Set = 0;
	while(1){
		if( (xQueue1 != 0) && (xQueue2 != 0)){
			xQueueReceive(xQueue1, &A_x, 10);
			xQueueReceive(xQueue2, &Set, 10);

			if(A_x > Set){
				if(Duty > 1000){
					Duty = Duty - 1;
					vTaskDelay(pdMS_TO_TICKS(50));
				}
				else{
					Duty = 1000;
				}
			}
			if(A_x < Set){
				if(Duty < 2000){
					Duty = Duty + 1;
					vTaskDelay(pdMS_TO_TICKS(50));
				}
				else{
					Duty = 2000;
				}
			}

			bsp_PWM_Duty(Duty);

			BSP_LED_Off(LED4);
			BSP_LED_Off(LED5);
			BSP_LED_On(LED3);
		}
		else{
			BSP_LED_On(LED6);
		}
	}
}

//Led Verde
void vTaskI2C( void *pvParameters){
	uint32_t Flag = 0;
	int32_t Angulo_x = 0;
	while(1){
		SD_MPU6050_ReadAccelerometer(&hi2c3,&mpu1);
		int16_t a_x = mpu1.Accelerometer_X;
		//int16_t a_y = mpu1.Accelerometer_Y;
		//int16_t a_z = mpu1.Accelerometer_Z;

		Angulo_x = Acel_Ang (a_x);

		xQueueSend(xQueue1, &Angulo_x, 0);

		BSP_LED_Off(LED3);
		BSP_LED_Off(LED5);
		BSP_LED_On(LED4);
	}
}

//Led Rojo
void vTaskADC( void *pvParameters){
	uint32_t Flag = 0;
	uint32_t Valor = 0;
	uint32_t aux = 0;
	while(1){
		HAL_ADC_Start(&hadc3);
		vTaskDelay(pdMS_TO_TICKS(10));
		Valor = HAL_ADC_GetValue(&hadc3);

		aux = Pote_Ang(Valor);


//		aux = Valor * 90;
//		aux = aux / 4085;



		xQueueSend(xQueue2, &aux, 0);

		BSP_LED_Off(LED3);
		BSP_LED_Off(LED3);
		BSP_LED_On(LED5);
	}
}




void start(void){
	#define Vel_start 1270

	for (int32_t i = 0; i < Vel_start; i++){
		bsp_PWM_Duty(i);
		HAL_Delay(1);
	}
	BSP_LED_On(LED6);
	HAL_Delay(500);
	BSP_LED_Off(LED6);
	HAL_Delay(500);
	BSP_LED_On(LED6);
	HAL_Delay(500);
	BSP_LED_Off(LED6);
	HAL_Delay(500);
}




void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}




/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
     PC3   ------> I2S2_SD
     PA4   ------> I2S3_WS
     PA5   ------> SPI1_SCK
     PA6   ------> SPI1_MISO
     PA7   ------> SPI1_MOSI
     PB10   ------> I2S2_CK
     PC7   ------> I2S3_MCK
     PA9   ------> USB_OTG_FS_VBUS
     PA10   ------> USB_OTG_FS_ID
     PA11   ------> USB_OTG_FS_DM
     PA12   ------> USB_OTG_FS_DP
     PC10   ------> I2S3_CK
     PC12   ------> I2S3_SD
     PB6   ------> I2C1_SCL
     PB7   ------> S_TIM4_CH2
     PB9   ------> I2C1_SDA
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10|GPIO_PIN_11|LD4_Pin|LD3_Pin
                          |LD5_Pin|LD6_Pin|GPIO_PIN_0|GPIO_PIN_1
                          |GPIO_PIN_3|Audio_RST_Pin|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin : CS_I2C_SPI_Pin */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CS_I2C_SPI_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(OTG_FS_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PDM_OUT_Pin */
  GPIO_InitStruct.Pin = PDM_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(PDM_OUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : I2S3_WS_Pin */
  GPIO_InitStruct.Pin = I2S3_WS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
  HAL_GPIO_Init(I2S3_WS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SPI1_SCK_Pin SPI1_MISO_Pin SPI1_MOSI_Pin */
  GPIO_InitStruct.Pin = SPI1_SCK_Pin|SPI1_MISO_Pin|SPI1_MOSI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : BOOT1_Pin */
  GPIO_InitStruct.Pin = BOOT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BOOT1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : CLK_IN_Pin */
  GPIO_InitStruct.Pin = CLK_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(CLK_IN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PD10 PD11 LD4_Pin LD3_Pin
                           LD5_Pin LD6_Pin PD0 PD1
                           PD3 Audio_RST_Pin PD6 PD7 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|LD4_Pin|LD3_Pin
                          |LD5_Pin|LD6_Pin|GPIO_PIN_0|GPIO_PIN_1
                          |GPIO_PIN_3|Audio_RST_Pin|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : I2S3_MCK_Pin I2S3_SCK_Pin I2S3_SD_Pin */
  GPIO_InitStruct.Pin = I2S3_MCK_Pin|I2S3_SCK_Pin|I2S3_SD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : VBUS_FS_Pin */
  GPIO_InitStruct.Pin = VBUS_FS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(VBUS_FS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : OTG_FS_ID_Pin OTG_FS_DM_Pin OTG_FS_DP_Pin */
  GPIO_InitStruct.Pin = OTG_FS_ID_Pin|OTG_FS_DM_Pin|OTG_FS_DP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_OverCurrent_Pin */
  GPIO_InitStruct.Pin = OTG_FS_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Audio_SCL_Pin Audio_SDA_Pin */
  GPIO_InitStruct.Pin = Audio_SCL_Pin|Audio_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : MEMS_INT2_Pin */
  GPIO_InitStruct.Pin = MEMS_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MEMS_INT2_GPIO_Port, &GPIO_InitStruct);

}




/* TIM2 init function */
static void MX_TIM2_Init(void)
{

  __HAL_RCC_TIM2_CLK_ENABLE();

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  //TIM_OC_InitTypeDef sConfigOC;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 16 - 1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 20000 - 2;									//AutoReloadRegister
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.RepetitionCounter = 0;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 10000;									//Duty
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim2);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

}

void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(timHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspPostInit 0 */

  /* USER CODE END TIM2_MspPostInit 0 */

    /**TIM2 GPIO Configuration
    PA15     ------> TIM2_CH1
    */
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM2_MspPostInit 1 */

  /* USER CODE END TIM2_MspPostInit 1 */
  }

}

void bsp_PWM_Duty(uint32_t Duty){
	sConfigOC.Pulse = Duty;
	HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
}

#define Vref 3
#define ADC_Res 4095
uint32_t Cambio_ADC_PWM(uint32_t Value){

	uint32_t temp = 0;

	Value = Vref * Value;				//Calculo el Vlsb y lo multiplico por el valor
	Value = Value * 1000;				//Multiplico por 1000 para que sea un valor mas grande
	Value = Value / ADC_Res;			//Primero multiplico porque es un valor muy chico sino
	temp = Value * 6.669;				//Lo pongo en escala de PWM (0 - 20000)

	return temp;

	//Pasar de escala de acelerómetroo a escala de PWM - 0ms a 2ms
	//Para que ande bien hay que hacerlo de 0ms a 1ms y sumarle 1ms = 1ms a 2ms
}

uint32_t Pote_Ang (uint32_t Pote){
	uint32_t aux = 0;

	aux = Pote * 90;
	aux = aux / ADC_Res;
	return aux;
}





/**
  * @brief I2C3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C3_Init(void)
{

  __HAL_RCC_I2C3_CLK_ENABLE();
  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.ClockSpeed = 100000;
  hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /* USER CODE END I2C3_Init 2 */

  HAL_I2C3_MspPostInit(&hi2c3);
}

void HAL_I2C3_MspPostInit(I2C_HandleTypeDef* hi2c3Handle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(hi2c3Handle->Instance==I2C3)
  {
  /* USER CODE BEGIN I2C3_MspPostInit 0 */

  /* USER CODE END I2C3_MspPostInit 0 */

    /**I2C3 GPIO Configuration
    PA8     ------> I2C3_SCL
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /**I2C3 GPIO Configuration
    PC9     ------> I2C3_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USER CODE BEGIN I2C3_MspPostInit 1 */

  /* USER CODE END I2C3_MspPostInit 1 */
  }

}

int32_t Acel_Ang (int32_t Acel){
	int32_t aux = 0;

	if(Acel < -17700){						//Se fija en los límites - 0 grados
		return 0;
	}
	else if (Acel > -4500){					//Se fija en los limites - 90 grados
		return 90;
	}
	else{									//Si esta dentro de los limites:
		aux = Acel + 400;					//Pone los valores entre 0 y -17300
		aux = aux / -195;					// -17300 / 90º		(Unidad de acelerómetro por grado)
		aux = aux - 90;
		aux = aux * -1;
		return aux;
	}
}





/* ADC3 init function */
static void MX_ADC3_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

  __HAL_RCC_ADC3_CLK_ENABLE();

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
    */
  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc3.Init.Resolution = ADC_RESOLUTION_12B;
  hadc3.Init.ScanConvMode = DISABLE;
  hadc3.Init.ContinuousConvMode = DISABLE;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.NbrOfConversion = 1;
  hadc3.Init.DMAContinuousRequests = DISABLE;
  hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
    */
  sConfig.Channel = ADC_CHANNEL_12;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_ACD3_MspPostInit(&hadc3);
}

void HAL_ACD3_MspPostInit(ADC_HandleTypeDef* hadc3Handle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(hadc3Handle->Instance==ADC3)
  {
  /* USER CODE BEGIN ADC3_MspPostInit 0 */

  /* USER CODE END ADC3_MspPostInit 0 */

    /**ADC3 GPIO Configuration
    PC2     ------> ACD3_IN12
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    //GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USER CODE BEGIN ADC3_MspPostInit 1 */

  /* USER CODE END ADC3_MspPostInit 1 */
  }

}





uint32_t Blinkea(char Led, uint32_t Delay, uint32_t Flag){
	if(Flag == 1){
		BSP_LED_Off(Led);
		vTaskDelay(pdMS_TO_TICKS(Delay));
		return 0;
	}
	else if (Flag == 0){
		BSP_LED_On(Led);
		vTaskDelay(pdMS_TO_TICKS(Delay));
		return 1;
	}
}





/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
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
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */
