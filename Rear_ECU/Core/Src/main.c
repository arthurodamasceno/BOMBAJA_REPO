/**
 ******************************************************************************
 * @file            : main.c
 * @brief           : Main program body
 * @author			: Arthur Damasceno
 * @team			: Bombaja - UFSM
 * @date			: 16/06/2020
 ******************************************************************************
 * @attention
 *
 * This code illustrates a working prototype of the final Rear ECU embedded on
 * BJ-18. Feel free to reproduce it.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "FLASH_PAGE_F1.h"
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define CSen HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
#define CSdis HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);

#define SCK_H HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
#define SCK_L HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);

#define DWT_CTRL (*(volatile uint32_t*) 0XE0001000)

#define LONG_TIME 0xffff
/* USER CODE END PD */

/* Private variables*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

CAN_HandleTypeDef hcan;

/* USER CODE BEGIN PV */
I2C_HandleTypeDef hi2c2;

TaskHandle_t Start_Task;
TaskHandle_t Bat_Task;
TaskHandle_t Fuel_Task;
TaskHandle_t Speed_Task;
TaskHandle_t RPM_Task;
TaskHandle_t Temp_Task;
TaskHandle_t OD_Task;

SemaphoreHandle_t xSemaphore;

uint16_t analog[4];
uint16_t temp16;
//uint32_t OD = 0;
uint8_t PULSO = 0;
uint8_t p_rpm = 0;
uint8_t p_spd = 0;
uint16_t batval_old = 0;
uint8_t odbuff[1];
uint8_t rpmbuff[2];

float fuel_f=0.0,fuel_f1=0.0,fuel_f2=0.0, temp_f=0.0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_CAN_Init(void);

/* USER CODE BEGIN PFP */
static void MX_I2C2_Init(void);

void Start_taskF(void *pvParameters);
void Bat_taskF(void *pvParameters);
void Fuel_taskF(void *pvParameters);
void Speed_taskF(void *pvParameters);
void RPM_taskF(void *pvParameters);
void Temp_taskF(void *pvParameters);
void OD_taskF(void *pvParameters);
void CAN_Filter_Config(void);
/* USER CODE END PFP */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* MCU Configuration--------------------------------------------------------*/
	__enable_irq();
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	HAL_Delay(2000);
	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_ADC1_Init();
	MX_CAN_Init();

	/* USER CODE BEGIN 0 */
	MX_I2C2_Init();

	DWT_CTRL |= (1 << 0);

	//OD = Flash_Read_NUM(0x0801FC00);

	CAN_Filter_Config();

	if (HAL_CAN_ActivateNotification(&hcan,
	CAN_IT_TX_MAILBOX_EMPTY /*| CAN_IT_RX_FIFO0_MSG_PENDING*/| CAN_IT_BUSOFF)
			!= HAL_OK) {
		Error_Handler();
	}

	if (HAL_CAN_Start(&hcan)) {
		Error_Handler();
	}

	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) analog, 4);

	//SEGGER_SYSVIEW_Conf();
	//SEGGER_SYSVIEW_Start();

	xTaskCreate(Start_taskF, "StartTask", 128, NULL, 7, &Start_Task);

	vTaskStartScheduler();

	/* USER CODE END 0 */

	/* We should never get here as control is now taken by the scheduler */
	/* Infinite loop */
	while (1) {
		HAL_Delay(1); /*JUST IN CASE*/
	}
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
	PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief ADC1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC1_Init(void) {
	ADC_ChannelConfTypeDef sConfig = { 0 };

	/** Common config
	 */
	hadc1.Instance = ADC1;
	hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
	hadc1.Init.ContinuousConvMode = ENABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 4;
	if (HAL_ADC_Init(&hadc1) != HAL_OK) {
		Error_Handler();
	}
	/** Configure Regular Channel
	 */
	sConfig.Channel = ADC_CHANNEL_0;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		Error_Handler();
	}
	/** Configure Regular Channel
	 */
	sConfig.Channel = ADC_CHANNEL_1;
	sConfig.Rank = ADC_REGULAR_RANK_2;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		Error_Handler();
	}
	/** Configure Regular Channel
	 */
	sConfig.Channel = ADC_CHANNEL_8;
	sConfig.Rank = ADC_REGULAR_RANK_3;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		Error_Handler();
	}
	/** Configure Regular Channel
	 */
	sConfig.Channel = ADC_CHANNEL_9;
	sConfig.Rank = ADC_REGULAR_RANK_4;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief CAN Initialization Function
 * @param None
 * @retval None
 */
static void MX_CAN_Init(void) {
	hcan.Instance = CAN1;
	hcan.Init.Prescaler = 9;
	hcan.Init.Mode = CAN_MODE_NORMAL;
	hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan.Init.TimeSeg1 = CAN_BS1_12TQ;
	hcan.Init.TimeSeg2 = CAN_BS2_3TQ;
	hcan.Init.TimeTriggeredMode = DISABLE;
	hcan.Init.AutoBusOff = ENABLE;
	hcan.Init.AutoWakeUp = DISABLE;
	hcan.Init.AutoRetransmission = ENABLE;
	hcan.Init.ReceiveFifoLocked = DISABLE;
	hcan.Init.TransmitFifoPriority = DISABLE;
	if (HAL_CAN_Init(&hcan) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C2_Init(void) {

	/* USER CODE BEGIN I2C1_Init 0 */

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
	hi2c2.Instance = I2C2;
	hi2c2.Init.ClockSpeed = 100000;
	hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c2.Init.OwnAddress1 = 0;
	hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c2.Init.OwnAddress2 = 0;
	hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_ENABLE;
	if (HAL_I2C_Init(&hi2c2) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void) {
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Channel1_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);
	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
	/*Configure GPIO pin : PC13 */
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_14;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pin : PA15 */
	GPIO_InitStruct.Pin = GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : PA12 */
	GPIO_InitStruct.Pin = GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	/*Configure GPIO pin INTERRUPT : PA12 */
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	GPIO_InitStruct.Pin = GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	/*Configure GPIO pin INTERRUPT : PA12 */
	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

	/*Configure GPIO pin : PB3 */
	GPIO_InitStruct.Pin = GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : PB4 */
	GPIO_InitStruct.Pin = GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/*Configure GPIO pin INTERRUPT HANDLER */
//void EXTI15_10_IRQHandler(void) {
//	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
//	p_spd += 1;
//	//BaseType_t checkIfYieldRequired;
//	//checkIfYieldRequired = xTaskResumeFromISR(OD_Task);
//	//portYIELD_FROM_ISR(checkIfYieldRequired);
//	//BaseType_t xHigherPriorityTaskWoken;
//	//xHigherPriorityTaskWoken = pdFALSE;
//	xSemaphoreGiveFromISR(xSemaphore, NULL);
//}
//void EXTI9_5_IRQHandler(void) {
//	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
//	p_rpm += 1;
//}

/*Configure can filters */
void CAN_Filter_Config(void) {
	CAN_FilterTypeDef can_filt;
	can_filt.FilterActivation = ENABLE;
	can_filt.FilterBank = 0;
	can_filt.FilterFIFOAssignment = CAN_RX_FIFO0;
	can_filt.FilterIdHigh = 0x0000;
	can_filt.FilterIdLow = 0x0000;
	can_filt.FilterMaskIdHigh = 0x0000;
	can_filt.FilterMaskIdLow = 0x0000;
	can_filt.FilterMode = CAN_FILTERMODE_IDMASK;
	can_filt.FilterScale = CAN_FILTERSCALE_32BIT;

	if (HAL_CAN_ConfigFilter(&hcan, &can_filt) != HAL_OK) {
		Error_Handler();
	}
}

void Start_taskF(void *pvParameters) {

	xSemaphore = xSemaphoreCreateBinary();
	vTaskDelay(1);
	xTaskCreate(Bat_taskF, "BatTask", 128, NULL, 4, &Bat_Task);
	vTaskDelay(1);
	xTaskCreate(Fuel_taskF, "FuelTask", 128, NULL, 4, &Fuel_Task);
	vTaskDelay(1);
	xTaskCreate(Speed_taskF, "SpeedTask", 128, NULL, 4, &Speed_Task);
	vTaskDelay(1);
	xTaskCreate(Temp_taskF, "TempTask", 128, NULL, 5, &Temp_Task);
	vTaskDelay(1);
	xTaskCreate(RPM_taskF, "RPMTask", 128, NULL, 4, &RPM_Task);
	vTaskDelay(1);
	xTaskCreate(OD_taskF, "ODTask", 128, NULL, 2, &OD_Task);

	if (Start_Task != NULL) {
		vTaskDelete(Start_Task);
	}

}

/*Battery level read task */
void Bat_taskF(void *pvParameters) {
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 1000;  //Ticks to wait since routine starts
	xLastWakeTime = xTaskGetTickCount();
	while (1) {
		uint8_t batbuff[2];
		uint16_t batAD = analog[0];
		float bat_a = 0.0004502626;
		float bat_b = 2.4948762146;
		float bat_v =  30*(((float)batAD * bat_a) + bat_b);
		uint16_t batCAN = (uint16_t) bat_v;

		batbuff[0] = (uint8_t) (batCAN >> 8) & 0xFF;
		batbuff[1] = (uint8_t) batCAN & 0xFF;

		uint32_t TxMailbox;
		CAN_TxHeaderTypeDef BatHeader;

		BatHeader.DLC = 2;
		BatHeader.StdId = 0x650;
		BatHeader.IDE = CAN_ID_STD;
		BatHeader.RTR = CAN_RTR_DATA;

		if (HAL_CAN_AddTxMessage(&hcan, &BatHeader, batbuff, &TxMailbox)
				!= HAL_OK) {
			Error_Handler();
		}
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		vTaskDelayUntil(&xLastWakeTime, xFrequency); /*1Hz frequency*/
	}
}

/*Fuel level read task */
void Fuel_taskF(void *pvParameters) {
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 20;  //Ticks to wait since routine starts
	xLastWakeTime = xTaskGetTickCount();

	while (1) {
		uint8_t fuelbuff[2];
		uint8_t fuelbuff1[4];
		uint8_t fuelbuff2[4];

		uint16_t fuel_a = analog[1];
		fuel_f =  0.998744152176286*fuel_f  + (1-0.998744152176286) * (float)fuel_a;
		fuel_f1 = 0.999874344189209*fuel_f1 + (1-0.999874344189209) * (float)fuel_a;
		fuel_f2 = 0.999987433708342*fuel_f2 + (1-0.999987433708342) * (float)fuel_a;

		uint16_t fuel_c = (uint16_t)fuel_f;
		uint32_t fuel_c1 = (uint32_t)fuel_f1;
		uint32_t fuel_c2 = (uint32_t)fuel_f2;

		fuelbuff[0] = (uint8_t) (fuel_c >> 8) & 0xFF;
		fuelbuff[1] = (uint8_t) fuel_c & 0xFF;

		fuelbuff1[0] = (uint8_t) (fuel_c1 >> 24);
		fuelbuff1[1] = (uint8_t) (fuel_c1 >> 16);
		fuelbuff1[2] = (uint8_t) (fuel_c1 >> 8);
		fuelbuff1[3] = (uint8_t) fuel_c1;

		fuelbuff2[0] = (uint8_t) (fuel_c2 >> 24);
	    fuelbuff2[1] = (uint8_t) (fuel_c2 >> 16);
		fuelbuff2[2] = (uint8_t) (fuel_c2 >> 8);
		fuelbuff2[3] = (uint8_t) fuel_c2;

		uint32_t TxMailbox;

		CAN_TxHeaderTypeDef FuelHeader;

		FuelHeader.DLC = 2;
		FuelHeader.StdId = 0x652;
		FuelHeader.IDE = CAN_ID_STD;
		FuelHeader.RTR = CAN_RTR_DATA;

		if (HAL_CAN_AddTxMessage(&hcan, &FuelHeader, fuelbuff, &TxMailbox)
				!= HAL_OK) {
			Error_Handler();
		}

		uint32_t TxMailbox1;

		CAN_TxHeaderTypeDef FuelHeader1;

		FuelHeader.DLC = 4;
		FuelHeader.StdId = 0x668;
		FuelHeader.IDE = CAN_ID_STD;
		FuelHeader.RTR = CAN_RTR_DATA;

		if (HAL_CAN_AddTxMessage(&hcan, &FuelHeader1, fuelbuff1, &TxMailbox1)
				!= HAL_OK) {
			Error_Handler();
		}

		uint32_t TxMailbox2;

		CAN_TxHeaderTypeDef FuelHeader2;

		FuelHeader.DLC = 4;
		FuelHeader.StdId = 0x669;
		FuelHeader.IDE = CAN_ID_STD;
		FuelHeader.RTR = CAN_RTR_DATA;

		if (HAL_CAN_AddTxMessage(&hcan, &FuelHeader2, fuelbuff2, &TxMailbox2)
				!= HAL_OK) {
			Error_Handler();
		}
		vTaskDelayUntil(&xLastWakeTime, xFrequency); /*50Hz frequency*/
	}
}

/*Speed read task */
void Speed_taskF(void *pvParameters) {
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 20;  //Ticks to wait since routine starts
	xLastWakeTime = xTaskGetTickCount();
	while (1) {
		uint8_t speedbuff[2];
		uint8_t test[1];
		uint8_t dump[1];
		HAL_I2C_Master_Receive(&hi2c2, (0x4 << 1), &test[0], 1, 100);
		if (test[0] != 0xAA) {
			HAL_I2C_Master_Receive(&hi2c2, (0x4 << 1), &dump[0], 1, 100);
		}
		HAL_I2C_Master_Receive(&hi2c2, (0x4 << 1), &speedbuff[0], 1, 100);
		HAL_I2C_Master_Receive(&hi2c2, (0x4 << 1), &speedbuff[1], 1, 100);
		//HAL_I2C_Master_Receive(&hi2c2, (0x4 << 1), &rpmbuff[0], 1, 100);
		//HAL_I2C_Master_Receive(&hi2c2, (0x4 << 1), &rpmbuff[1], 1, 100);
		HAL_I2C_Master_Receive(&hi2c2, (0x4 << 1), &odbuff[0], 1, 100);
		uint32_t TxMailbox;

		CAN_TxHeaderTypeDef SpeedHeader;

		SpeedHeader.DLC = 2;
		SpeedHeader.StdId = 0x653;
		SpeedHeader.IDE = CAN_ID_STD;
		SpeedHeader.RTR = CAN_RTR_DATA;

		if (HAL_CAN_AddTxMessage(&hcan, &SpeedHeader, speedbuff, &TxMailbox)
				!= HAL_OK) {
			Error_Handler();
		}

		vTaskDelayUntil(&xLastWakeTime, xFrequency); /*100Hz frequency*/
	}
}

/*RPM read task */
void RPM_taskF(void *pvParameters) {
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 20;  //Ticks to wait since routine starts
	xLastWakeTime = xTaskGetTickCount();
	while (1) {

		uint8_t test[1];
		uint8_t dump[1];
		HAL_I2C_Master_Receive(&hi2c2, (0x5 << 1), &test[0], 1, 10);
		if (test[0] != 0x55){
			HAL_I2C_Master_Receive(&hi2c2, (0x5 << 1), &dump[0], 1, 10);
		}

		HAL_I2C_Master_Receive(&hi2c2, (0x5 << 1), &rpmbuff[0], 1, 10);
		HAL_I2C_Master_Receive(&hi2c2, (0x5 << 1), &rpmbuff[1], 1, 10);
		uint32_t TxMailbox;


		CAN_TxHeaderTypeDef RPMHeader;

		RPMHeader.DLC = 2;
		RPMHeader.StdId = 0x654;
		RPMHeader.IDE = CAN_ID_STD;
		RPMHeader.RTR = CAN_RTR_DATA;

		if (HAL_CAN_AddTxMessage(&hcan, &RPMHeader, rpmbuff, &TxMailbox)
				!= HAL_OK) {
			Error_Handler();
		}

		vTaskDelayUntil(&xLastWakeTime, xFrequency); /*5Hz frequency*/
	}
}

/*Temperature read task */
void Temp_taskF(void *pvParameters) {
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 200;  //Ticks to wait since routine starts
	xLastWakeTime = xTaskGetTickCount();
	while (1) {
		uint8_t tempdata[16];

		CSen
		for (int i = 0; i < 16; i++) {
			SCK_H
			tempdata[i] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4);
			SCK_L
		}
		CSdis

		if (tempdata[13] == 0) {
			temp16 = tempdata[1] * 2048 + tempdata[2] * 1024 + tempdata[3] * 512
					+ tempdata[4] * 256 + tempdata[5] * 128 + tempdata[6] * 64;
			temp16 = temp16 + tempdata[7] * 32 + tempdata[8] * 16
					+ tempdata[9] * 8 + tempdata[10] * 4 + tempdata[11] * 2
					+ tempdata[12];
			//for (int n = 1; n < 13; n++) {
			//	temp16 += tempdata[n] * (2048 / (1 << (n - 1)));
			//}
		}
		temp_f = 0.7152892160251179* temp_f + (1-0.7152892160251179) * (float)temp16;
		uint16_t tempRx = (uint16_t) temp_f;

		uint8_t tempbuff[2];
		tempbuff[0] = (uint8_t) (tempRx >> 8) & 0xFF;
		tempbuff[1] = (uint8_t) tempRx & 0xFF;

		uint32_t TxMailbox;

		CAN_TxHeaderTypeDef TempHeader;

		TempHeader.DLC = 2;
		TempHeader.StdId = 0x651;
		TempHeader.IDE = CAN_ID_STD;
		TempHeader.RTR = CAN_RTR_DATA;

		if (HAL_CAN_AddTxMessage(&hcan, &TempHeader, tempbuff, &TxMailbox)
				!= HAL_OK) {
			Error_Handler();
		}

		vTaskDelayUntil(&xLastWakeTime, xFrequency); /*5Hz frequency*/
	}
}

/*Odometer read write task */
void OD_taskF(void *pvParameters) {
	TickType_t xLastWakeTime;
		const TickType_t xFrequency = 100;  //Ticks to wait since routine starts
		xLastWakeTime = xTaskGetTickCount();
		while (1) {

			uint32_t TxMailbox;
			CAN_TxHeaderTypeDef odHeader;

			odHeader.DLC = 1;
			odHeader.StdId = 0x666;
			odHeader.IDE = CAN_ID_STD;
			odHeader.RTR = CAN_RTR_DATA;

			if (HAL_CAN_AddTxMessage(&hcan, &odHeader, odbuff, &TxMailbox)
					!= HAL_OK) {
				Error_Handler();
			}
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_6);
			vTaskDelayUntil(&xLastWakeTime, xFrequency); /*10Hz frequency*/
		}
}

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM1 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	/* USER CODE BEGIN Callback 0 */

	/* USER CODE END Callback 0 */
	if (htim->Instance == TIM1) {
		HAL_IncTick();
	}
	/* USER CODE BEGIN Callback 1 */

	/* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
