/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 * @author			: Arthur Damasceno
 * @team			: Bombaja - UFSM
 * @date			: 16/06/2020
 ******************************************************************************
 * @attention
 *
 * This code illustrates a may not working prototype of the final Front ECU embedded on
 * BJ-18. Feel free to reproduce it.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FreeRTOS.h"
#include "task.h"
#include <string.h>
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define GPS_in_SIZE 36

const uint8_t UBX_HEADER[] = { 0xB5, 0x62 };
/* USER CODE END PD */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

CAN_HandleTypeDef hcan;

I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;

/* USER CODE BEGIN PV */
TaskHandle_t GPS_Task;
TaskHandle_t Gear_Brake_Task;
TaskHandle_t Gas_Task;
TaskHandle_t Logger_Task;
TaskHandle_t Telemetry_Task;

uint16_t gas[1];
uint8_t k = 0;
uint8_t gear, brake;
uint8_t startLOG=0 , stopLOG=0;
uint8_t bsl[22];

uint8_t buffer[22];

uint8_t GPS_in[GPS_in_SIZE];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_CAN_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART1_UART_Init(void);

/* USER CODE BEGIN PFP */
void GPS_taskF(void *pvParameters);
void Gear_Brake_taskF(void *pvParameters);
void Gas_taskF(void *pvParameters);
void Logger_taskF(void *pvParameters);
void Telemetry_taskF(void *pvParameters);

void CAN_Filter_Config(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	HAL_UART_Receive_IT(&huart2, GPS_in, GPS_in_SIZE);

	BaseType_t checkIfYieldRequired;
	checkIfYieldRequired = xTaskResumeFromISR(GPS_Task);
	portYIELD_FROM_ISR(checkIfYieldRequired);
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_ADC1_Init();
	MX_CAN_Init();
	MX_I2C1_Init();
	MX_USART2_UART_Init();
	MX_USART1_UART_Init();
	CAN_Filter_Config();

	if (HAL_CAN_ActivateNotification(&hcan,
	CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_BUSOFF)
			!= HAL_OK) {
		Error_Handler();
	}

	if (HAL_CAN_Start(&hcan)) {
		Error_Handler();
	}

	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) gas, 1);

	HAL_UART_Receive_IT(&huart2, GPS_in, GPS_in_SIZE);

	xTaskCreate(GPS_taskF, "GPSTask", 128, NULL, 2, &GPS_Task);
	xTaskCreate(Gear_Brake_taskF, "Gear_BrakeTask", 128, NULL, 3,
			&Gear_Brake_Task);
	xTaskCreate(Gas_taskF, "GasTask", 128, NULL, 0, &Gas_Task);
	xTaskCreate(Logger_taskF, "LoggerTask", 128, NULL, 3, &Logger_Task);
	xTaskCreate(Telemetry_taskF, "TelemetryTask", 128, NULL, 1,
			&Telemetry_Task);

	/* Start scheduler */
	vTaskStartScheduler();
	/* We should never get here as control is now taken by the scheduler */

	/* Infinite loop */
	while (1) {
		HAL_Delay(1); /*JUST I CASE*/
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
	PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV8;
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
	hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc1.Init.ContinuousConvMode = ENABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	if (HAL_ADC_Init(&hadc1) != HAL_OK) {
		Error_Handler();
	}
	/** Configure Regular Channel
	 */
	sConfig.Channel = ADC_CHANNEL_4;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
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
static void MX_I2C1_Init(void) {
	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 400000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void) {
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart1) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void) {
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void) {
	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Channel1_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	/* DMA1_Channel6_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
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

	/*Configure GPIO pins : Gear_Pin Brake_Pin */
	GPIO_InitStruct.Pin = Gear_Pin | Brake_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/*Configure CAN Filter */
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

/*GPS Read Task --- THIS TASK NEEDS TEST WITH HARDWARE */
void GPS_taskF(void *pvParameters) {
	while (1) {
		vTaskSuspend(NULL);

		if (GPS_in[0] == UBX_HEADER[0] && GPS_in[1] == UBX_HEADER[1]) {
			uint8_t GPSbuff[4];

			GPSbuff[0] = GPS_in[16];
			GPSbuff[1] = GPS_in[17];
			GPSbuff[2] = GPS_in[12];
			GPSbuff[3] = GPS_in[13];

			buffer[18] = GPS_in[16];
			buffer[19] = GPS_in[17];
			buffer[20] = GPS_in[12];
			buffer[21] = GPS_in[13];
			uint32_t TxMailbox;

			CAN_TxHeaderTypeDef GPSHeader;

			GPSHeader.DLC = 4;
			GPSHeader.StdId = 0x658;
			GPSHeader.IDE = CAN_ID_STD;
			GPSHeader.RTR = CAN_RTR_DATA;

			if (HAL_CAN_AddTxMessage(&hcan, &GPSHeader, GPSbuff, &TxMailbox)
					!= HAL_OK) {
				Error_Handler();
			}
		}
	}
}

/*Gear an Brake Read Task */
void Gear_Brake_taskF(void *pvParameters) {
	while (1) {
		gear = HAL_GPIO_ReadPin(GPIOB, Gear_Pin);
		brake = HAL_GPIO_ReadPin(GPIOB, Brake_Pin);
		uint8_t GBbuff[2];
		GBbuff[0] = gear;
		GBbuff[1] = brake;

		buffer[0] = gear;
		buffer[1] = brake;
		uint32_t TxMailbox;

		CAN_TxHeaderTypeDef GBHeader;

		GBHeader.DLC = 2;
		GBHeader.StdId = 0x657;
		GBHeader.IDE = CAN_ID_STD;
		GBHeader.RTR = CAN_RTR_DATA;

		if (HAL_CAN_AddTxMessage(&hcan, &GBHeader, GBbuff, &TxMailbox)
				!= HAL_OK) {
			Error_Handler();
		}
		vTaskDelay(10); /*100Hz frequency*/
	}
}

/*Gas pedal Read Task */
void Gas_taskF(void *pvParameters) {
	while (1) {
		uint8_t gasbuff[2];
		gasbuff[0] = (uint8_t) (gas[0] >> 8) & 0xFF;
		gasbuff[1] = (uint8_t) gas[0] & 0xFF;

		buffer[2] = gasbuff[0];
		buffer[3] = gasbuff[1];

		uint32_t TxMailbox;

		CAN_TxHeaderTypeDef GasHeader;

		GasHeader.DLC = 2;
		GasHeader.StdId = 0x656;
		GasHeader.IDE = CAN_ID_STD;
		GasHeader.RTR = CAN_RTR_DATA;

		if (HAL_CAN_AddTxMessage(&hcan, &GasHeader, gasbuff, &TxMailbox)
				!= HAL_OK) {
			Error_Handler();
		}
		vTaskDelay(10); /*100Hz frequency*/
	}
}

/*Logger Write Task  --- THIS TASK NEEDS TO IMPLEMENT START AND STOP LOG FUNCTIONS*/
void Logger_taskF(void *pvParameters) {
	while (1) {
		if(startLOG==1){
			startLOG=0;
			bsl[1]  = 0xAA;
			bsl[11] = 0xBB;
			bsl[21] = 0xCC;
			bsl[0]  = 0x00;
			bsl[10] = 0x00;
			bsl[20] = 0x00;
			HAL_I2C_Master_Transmit(&hi2c1, (0x08 << 1), bsl, 22, 10);
		}
		if(stopLOG==1){
			stopLOG=0;
			bsl[0]  = 0xDD;
			bsl[10] = 0xEE;
			bsl[20] = 0xFF;
			bsl[1]  = 0x00;
			bsl[11] = 0x00;
			bsl[21] = 0x00;
			HAL_I2C_Master_Transmit(&hi2c1, (0x08 << 1), bsl, 22, 10);
		}
		HAL_I2C_Master_Transmit(&hi2c1, (0x08 << 1), buffer, 22, 10);
		vTaskDelay(10); /*100Hz frequency*/
	}
}

/*LoRa Transceiver Write Task*/
void Telemetry_taskF(void *pvParameters) {
	while (1) {
		uint8_t sync = 0xAA;
		uint8_t LoRa[23];
		LoRa[0] = sync;
		for (int y = 1; y < 23; y++) {
			LoRa[y] = buffer[y - 1];
		}
		HAL_UART_Transmit(&huart1, LoRa, 23, 100);
		vTaskDelay(100); /*10Hz frequency*/
	}
}

/*CAN Read Callback -- THIS FUNCTION NEEDS TO IMPLEMENT START AND STOP LOG MESSAGES  */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	CAN_RxHeaderTypeDef RxHeader;
	uint8_t rcv_msg[8];
	if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, rcv_msg)
			!= HAL_OK) {
		Error_Handler();
	}

	if (RxHeader.StdId == 0x64E) {
		if (rcv_msg[0] == 0x55) {
			startLOG=1;
		}
	}

	if (RxHeader.StdId == 0x64F) {
		if (rcv_msg[0] == 0x66) {
			stopLOG=1;
		}
	}

	if (RxHeader.StdId == 0x650) {
		buffer[4] = rcv_msg[0];
		buffer[5] = rcv_msg[1];
	}

	if (RxHeader.StdId == 0x651) {
		buffer[6] = rcv_msg[0];
		buffer[7] = rcv_msg[1];
	}

	if (RxHeader.StdId == 0x652) {
		buffer[8] = rcv_msg[0];
		buffer[9] = rcv_msg[1];
	}

	if (RxHeader.StdId == 0x653) {
		buffer[10] = rcv_msg[0];
		buffer[11] = rcv_msg[1];
	}

	if (RxHeader.StdId == 0x654) {
		buffer[12] = rcv_msg[0];
		buffer[13] = rcv_msg[1];
	}

	if (RxHeader.StdId == 0x655) {
		buffer[14] = rcv_msg[0];
		buffer[15] = rcv_msg[1];
		buffer[16] = rcv_msg[2];
		buffer[17] = rcv_msg[3];
	}

}

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM4 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	/* USER CODE BEGIN Callback 0 */

	/* USER CODE END Callback 0 */
	if (htim->Instance == TIM4) {
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/