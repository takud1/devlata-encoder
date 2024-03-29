/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
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
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cd4094.h"

#include "NRF905.h"
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
//int _write(int file, char *ptr, int len) {
//	HAL_UART_Transmit(&huart2, (uint8_t*) ptr, len, 50);
//	for (int i = 0; i < len; i++) {
//		ITM_SendChar(*ptr++);
//	}
//	return len;
//}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern void initialise_monitor_handles(void);
NRF905_hw_t NRF905_hw;
NRF905_t NRF905;

int master;
int ret;

char buffer[64];
uint8_t nrf905_payload_buffer[NRF905_MAX_PAYLOAD];

volatile bool tx_flag = false;
volatile int state = 0;

int dr = 0;
int temp_dr = 0;

uint32_t my_address;
uint32_t receiver_address;

#define ADDRESS_MASTER 0xA2B5D154
#define ADDRESS_SLAVE  0xA2B5D154

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	/* USER CODE BEGIN 1 */
	initialise_monitor_handles();
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
	MX_USART2_UART_Init();
	MX_SPI2_Init();
	MX_TIM3_Init();
	/* USER CODE BEGIN 2 */
	uint32_t uid = 0x00;
	for (uint8_t i = 0; i < 3; ++i) {
		uid += *(uint32_t*) (UID_BASE + i * 4);
	}
	srand(uid);

	//  	Pins and ports declaration
	NRF905_hw.gpio[NRF905_HW_GPIO_TXEN].pin = TXEN_Pin;
	NRF905_hw.gpio[NRF905_HW_GPIO_TXEN].port = TXEN_GPIO_Port;
	NRF905_hw.gpio[NRF905_HW_GPIO_TRX_EN].pin = TRX_CE_Pin;
	NRF905_hw.gpio[NRF905_HW_GPIO_TRX_EN].port = TRX_CE_GPIO_Port;
	NRF905_hw.gpio[NRF905_HW_GPIO_PWR].pin = PWR_Pin;
	NRF905_hw.gpio[NRF905_HW_GPIO_PWR].port = PWR_GPIO_Port;

	NRF905_hw.gpio[NRF905_HW_GPIO_CD].pin = CD_Pin;
	NRF905_hw.gpio[NRF905_HW_GPIO_CD].port = CD_GPIO_Port;
	NRF905_hw.gpio[NRF905_HW_GPIO_AM].pin = AM_Pin;
	NRF905_hw.gpio[NRF905_HW_GPIO_AM].port = AM_GPIO_Port;
	NRF905_hw.gpio[NRF905_HW_GPIO_DR].pin = DR_Pin;
	NRF905_hw.gpio[NRF905_HW_GPIO_DR].port = DR_GPIO_Port;

	NRF905_hw.gpio[NRF905_HW_GPIO_CS].pin = SPI_CS_Pin;
	NRF905_hw.gpio[NRF905_HW_GPIO_CS].port = SPI_CS_GPIO_Port;

	NRF905_hw.tim = &htim3;
	NRF905_hw.spi = &hspi2;

	//  address allocation for master and slave
	master = 1; //HAL_GPIO_ReadPin(MODE_GPIO_Port, MODE_Pin);     //Giving Master's address to this device
	if (master == 1) {
		my_address = ADDRESS_MASTER;
		receiver_address = ADDRESS_SLAVE;
	} else {
		my_address = ADDRESS_SLAVE;
		receiver_address = ADDRESS_MASTER;
	}

	if (master == 1) {
		printf("Mode: Master, TX, %08lX\r\n", my_address);
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
	} else {
		printf("Mode: Slave, RX, %08lX\r\n", my_address);
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
	}

	//  NRF initialization and setting address
	NRF905_init(&NRF905, &NRF905_hw);
	NRF905_set_listen_address(&NRF905, receiver_address);

	//  Checking configuration Register values
	printf("Reg conf: ");
	for (int i = 0; i < 10; ++i) {
		uint8_t d = NRF905_read_config_register(&NRF905, i);
		printf("%02X, ", d);
	}
	printf("\r\n");
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {

		if(!HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin)){

			// Data to be sent. Data at 0, 4, and 8 are for authentication at receiver side.
			//Rest of the data is taken as 1 bit for each key press (Total 24 bits i.e 3 bytes).
			//5,6,7 are duplicate of 1,2,3 respectively. At the receiver end we'll check if they are same.
			nrf905_payload_buffer[0] = 0xD3;
			nrf905_payload_buffer[1] = 0b01001100;
			nrf905_payload_buffer[2] = 0b01001010;
			nrf905_payload_buffer[3] = 0b00010101;
			nrf905_payload_buffer[4] = 0x16;
			nrf905_payload_buffer[5] = nrf905_payload_buffer[1];
			nrf905_payload_buffer[6] = nrf905_payload_buffer[2];
			nrf905_payload_buffer[7] = nrf905_payload_buffer[3];
			nrf905_payload_buffer[8] = 0x69;

			ret = 0;

			//		 Sending Data using SPI interrupt
			switch(state){

			case 0:
				//			NRF transmit using interrupt
				//			If Data is there to transmit, state will be 1, else 2.
				state = NRF905_tx_it(&NRF905, my_address, (uint8_t*)nrf905_payload_buffer, NRF905_MAX_PAYLOAD);
				break;

			case 1:
				if(tx_flag == true){     //Checking if the interrupt has occurred
					state++;
					tx_flag = false;
					ret = 1;
				}

				break;

			case 2:
				//Selecting next mode
				state = NRF905_tx_down(&NRF905, NRF905_NEXTMODE_STANDBY);

				break;
			default:
				break;

			}

		}


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

	/** Configure the main internal regulator output voltage
	 */
	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
	{
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

//Callback function for SPI interrupt
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi){

	NRF905_spi_deselect(&NRF905);
	tx_flag = true;

}

// Callback function for DR interrupt
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin){

	if (GPIO_Pin == GPIO_PIN_12){      //verifying if it is generated at appropriate pin

		state = 0;

	}

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
