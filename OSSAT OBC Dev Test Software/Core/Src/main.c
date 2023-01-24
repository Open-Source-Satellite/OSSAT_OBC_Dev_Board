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
#include "STM32CAN_ContollerAreaNetwork.h"

/**********************************
 * PRM: test buttons to LEDs:
 * One LED flash on off all the time
 * One user button does one LED
 * The other button does the other LED
 * PPS: remove the test.
 **********************************


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
 ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc3;

FDCAN_HandleTypeDef hfdcan1;
FDCAN_HandleTypeDef hfdcan2;

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;
I2C_HandleTypeDef hi2c4;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

SRAM_HandleTypeDef hsram1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC3_Init(void);
static void MX_FDCAN1_Init(void);
static void MX_FDCAN2_Init(void);
static void MX_FMC_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
static void MX_SPI1_Init(void);
static void MX_UART4_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_RTC_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_I2C4_Init(void);
static void UART_Test(UART_HandleTypeDef *huart);
static void GPIO_Test(void);
static void ANALOGUE_Test(void);
static void LVDS_DR_To_MOSI_Test(void);
static void LVDS_DR_To_NSS_Test(void);
static void LVDS_DR_To_SCK_Test(void);
static void LVDS_EXTI9_To_MOSI_Test(void);
static void LVDS_EXTI9_To_NSS_Test(void);
static void LVDS_EXTI9_To_SCK_Test(void);
static void LoopBackIoTest(GPIO_TypeDef *InputGPIOx, uint16_t InputGPIO_Pin, GPIO_TypeDef *OutputGPIOx, uint16_t OutputGPIO_Pin);
static void CAN1_Test(void);
static void CAN2_Test(void);
static void MRAM_CSTest(void);
/* USER CODE BEGIN PFP */

#define MRAM_START_ADDRESS (0xC0000000UL)
#define MRAM_SIZE (0x7FFFFFUL)
#define MRAM_SINGLE_CHIP_SIZE (0x00400000UL)

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

unsigned int HAL_RCC_FDCAN_CLK_ENABLED = 0;

char pcSplashScreen[] = "\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n                  %@@@@@@@@@@@@@@@@@@@@@,\r\n"
"              (@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@.\r\n"
"           @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\r\n"
"        *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@,\r\n"
"      (@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\r\n"
"     @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@               @\r\n"
"   *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@            @@@@\r\n"
"  /@@@@@@@@@@@@@@@@@@@@@#           @@/   @@@     @@@@@@@@\r\n"
"  @@@@@@@@@@@@@@@@@@@&                  @@@@@@@*@@@@@@@@@@@\r\n"
" @@@@@@@@@@@@@@@@@@@                     @@@@@@@@@@@@@@@@@@&\r\n"
" @@@@@@@@@@@@@@@@@@                       @@@@@@@@@@@@@@@@@@\r\n"
" @@@@@@@@@@@@@@@@@@                       @@@@@@@@@@@@@@@@@@\r\n"
" @@@@@@@@@@@@@@@@@@                       @@@@@@@@@@@@@@@@@@\r\n"
" @@@@@@@@@@@@@@@@@@@                     @@@@@@@@@@@@@@@@@@&\r\n"
"  @@@@@@@@@@@@@@@@@@@                  @@@@@@@@@@@@@@@@@@@@\r\n"
"  *@@@@@@@,     @@*   @@&           @@@@@@@@@@@@@@@@@@@@@@\r\n"
"   ,@@@@            @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\r\n"
"     .               @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\r\n"
"                       @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\r\n"
"                    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\r\n"
"                  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%\r\n"
"               @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\r\n"
"                   (@@@@@@@@@@@@@@@@@@@@@\r\n\r\n  OSSAT Dev board OBC Test Program KS-SOF-01541-01\r\n  KISPE Space Systems Ltd.\r\n  list of commands:\r\nA: SRAM Chip Select Test\r\nB: SRAM Write Test\r\nC: SRAM Read Test\r\nH: UART4 test \r\nI: UART2 test\r\nJ: GPIO test\r\nK: Analogue test\r\n";
/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	char pData[1];
	pData[0] = '\0';
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
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

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
    PeriphClkInitStruct.FdcanClockSelection = RCC_FDCANCLKSOURCE_PLL;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }
    /* Peripheral clock enable */
    HAL_RCC_FDCAN_CLK_ENABLED++;
    if(HAL_RCC_FDCAN_CLK_ENABLED==1){
      __HAL_RCC_FDCAN_CLK_ENABLE();
    }

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
    PeriphClkInitStruct.FdcanClockSelection = RCC_FDCANCLKSOURCE_PLL;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    HAL_RCC_FDCAN_CLK_ENABLED++;
    if(HAL_RCC_FDCAN_CLK_ENABLED==1){
      __HAL_RCC_FDCAN_CLK_ENABLE();
    }

  //MX_ADC1_Init();
  //MX_ADC3_Init();
  //MX_FDCAN1_Init();
  //MX_FDCAN2_Init();
  MX_FMC_Init();
  //MX_I2C1_Init();
  //MX_I2C2_Init();
  //MX_SPI1_Init();
  MX_UART4_Init();
  MX_USART2_UART_Init();
  MX_RTC_Init();
  MX_USART3_UART_Init();
  //MX_I2C4_Init();
  GPIO_PinState pinButton1State;
  GPIO_PinState pinButton2State;

  // Initialise the CAN busses
  STM32CAN_Initialise(CAN_PERIPHERAL_FDCAN1, STM32CAN_FDCAN1_MESSAGE_RAM_ADDRESS, CAN_BAUD_1MBPS, 0, 1, 1, 1);
  STM32CAN_Initialise(CAN_PERIPHERAL_FDCAN2, STM32CAN_FDCAN2_MESSAGE_RAM_ADDRESS, CAN_BAUD_1MBPS, 0, 1, 1, 1);
  STM32CAN_RegisterCallbacks(CAN_PERIPHERAL_FDCAN1, CAN1_ISR, 0);
  STM32CAN_RegisterCallbacks(CAN_PERIPHERAL_FDCAN2, CAN2_ISR, 0);
  STM32CAN_Connect(CAN_PERIPHERAL_FDCAN1);
  STM32CAN_Connect(CAN_PERIPHERAL_FDCAN2);

  //HAL_NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
  //HAL_NVIC_EnableIRQ(FDCAN2_IT0_IRQn);
  //HAL_NVIC_EnableIRQ(FDCAN1_IT1_IRQn);
  //HAL_NVIC_EnableIRQ(FDCAN2_IT1_IRQn);

  tsSTM32CAN_Frame tsCANFrame;
  tsCANFrame.iDataLength = 2;
  tsCANFrame.iIdentifier = 0x55;
  tsCANFrame.aData[0] = 0x55;
  tsCANFrame.aData[1] = 0x55;

  STM32CAN_Transmit(CAN_PERIPHERAL_FDCAN1, &tsCANFrame,1);
  STM32CAN_Transmit(CAN_PERIPHERAL_FDCAN2, &tsCANFrame,1);
  /* USER CODE BEGIN 2 */
  // for this board, PC10: USART3_TX, PC11: USART3_RX

  // print the splash screen and wait for something to happen
  HAL_UART_Transmit(&huart3, pcSplashScreen, strlen(pcSplashScreen), 500);
  /* USER CODE END 2 */

  // Toggle between LED1 and LED2
  // LED 1 is PG6
  // LED 2 is PG7
  // LED 3 is PG8
  // USER_BUTTON_1 is PC6
  // USER_BUTTON_2 is PC4
  // Set LED1
  HAL_GPIO_WritePin(GPIOG, 0x0040, GPIO_PIN_SET);

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	pData[0] = '\0';
    // Read the UART3 for a character... wait 1/4 second
	HAL_UART_Receive(&huart3, pData, 1, 250);

	// Toggle LED 1 & 2
	HAL_GPIO_TogglePin(GPIOG, 0x00C0);
	// Read the buttons PC4 & 6

	pinButton2State = HAL_GPIO_ReadPin(GPIOC, 0x0010);
	pinButton1State = HAL_GPIO_ReadPin(GPIOC, 0x0040);

	// set LED 3 if someone is pressing button 2
	if (pinButton2State == GPIO_PIN_RESET)
	{
		// set LED 3
		HAL_GPIO_WritePin(GPIOG, 0x0100, GPIO_PIN_SET);
	}
	// reset LED 3 if someone is pressing button 1
	if (pinButton1State == GPIO_PIN_RESET)
	{
		// reset LED 3
		HAL_GPIO_WritePin(GPIOG, 0x0100, GPIO_PIN_RESET);
	}

	switch (pData[0])
	{
	case 'A':
		HAL_UART_Transmit(&huart3, "Starting Memory CS Decoder Test\r\n", 33, 1000);
		MRAM_CSTest();
		break;
	case 'B':
		HAL_UART_Transmit(&huart3, "Starting MRAM Write Test\r\n", 26, 1000);
		MRAM_WriteTest();
		break;
	case 'C':
		HAL_UART_Transmit(&huart3, "Starting MRAM Read Test\r\n", 25, 1000);
		MRAM_ReadTest();
		break;
	case 'H':
		HAL_UART_Transmit(&huart3, "Starting UART4 Testing, waiting for UART character\r\n", 52, 1000);
		UART_Test(&huart4);
		break;
	case 'I':
		HAL_UART_Transmit(&huart3, "Starting UART2 Testing, waiting for UART character\r\n", 52, 1000);
		UART_Test(&huart2);
		break;
	case 'J':
		HAL_UART_Transmit(&huart3, "Starting GPIO test, pulsing successive outputs\r\n", 49, 1000);
		GPIO_Test();
		break;
	case 'K':
		HAL_UART_Transmit(&huart3, "Starting ADC test, pulsing successive ADC inputs\r\n", 50, 1000);
		ANALOGUE_Test();
		break;
	case 'L':
		HAL_UART_Transmit(&huart3, "Starting PPS test\r\n", 19, 1000);
		PPS_Test();
		break;
	case 'M':
		HAL_UART_Transmit(&huart3, "Starting LVDS, Data Ready to MOSI test. Looping back for 10s\r\n", 62, 1000);
		LVDS_DR_To_MOSI_Test();
		break;
	case 'N':
		HAL_UART_Transmit(&huart3, "Starting LVDS, Data Ready to NSS test. Looping back for 10s\r\n", 61, 1000);
		LVDS_DR_To_NSS_Test();
		break;
	case 'O':
		HAL_UART_Transmit(&huart3, "Starting LVDS, Data Ready to SCK test. Looping back for 10s\r\n", 61, 1000);
		LVDS_DR_To_SCK_Test();
		break;
	case 'P':
		HAL_UART_Transmit(&huart3, "Starting LVDS, EXTI9 to MOSI test. Looping back for 10s\r\n", 56, 1000);
		LVDS_EXTI9_To_MOSI_Test();
		break;
	case 'Q':
		HAL_UART_Transmit(&huart3, "Starting LVDS, EXTI9 to NSS test. Looping back for 10s\r\n", 57, 1000);
		LVDS_EXTI9_To_NSS_Test();
		break;
	case 'R':
		HAL_UART_Transmit(&huart3, "Starting LVDS, EXTI9 to SCK test. Looping back for 10s\r\n", 57, 1000);
		LVDS_EXTI9_To_SCK_Test();
		break;
	default:
		break;
	}

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

unsigned int CAN1_Test_Running = 0;
unsigned int CAN2_Test_Running = 0;


static void CAN1_Test(void)
{
	CAN1_Test_Running = 1;
	HAL_Delay(10000);
	CAN1_Test_Running = 0;

	HAL_UART_Transmit(&huart3, pcSplashScreen, strlen(pcSplashScreen), 1000);
}

static void CAN2_Test(void)
{
	CAN2_Test_Running = 1;
	HAL_Delay(10000);
	CAN2_Test_Running = 0;

	HAL_UART_Transmit(&huart3, pcSplashScreen, strlen(pcSplashScreen), 1000);
}

void CAN1_ISR(void)
{
	unsigned int *pBufferStartAddress;
	unsigned int *piIndexOfFirstFrame;
	unsigned int *piNumFramesToRead;
	unsigned int *piMaxFramesInFIFO;
	unsigned int i = 0;
	STM32CAN_Receive(CAN_PERIPHERAL_FDCAN1, pBufferStartAddress, piIndexOfFirstFrame, piNumFramesToRead, piMaxFramesInFIFO);

	// if the test is running
	if (CAN1_Test_Running == 1)
	{
		tsSTM32CAN_Frame tsCANFrame;
		tsCANFrame.iIdentifier = ((tsSTM32CAN_RXFifoElement*)pBufferStartAddress)->iID;
		tsCANFrame.iDataLength = ((tsSTM32CAN_RXFifoElement*)pBufferStartAddress)->iDLC;
		for (i = 0 ; i < tsCANFrame.iDataLength ; i++ )
		{
			tsCANFrame.aData[i] = ((tsSTM32CAN_RXFifoElement*)pBufferStartAddress)->aDataBytes[i];
		}

		STM32CAN_Transmit(CAN_PERIPHERAL_FDCAN1, &tsCANFrame, 1);
	}
	else
	{
		STM32CAN_Flush(CAN_PERIPHERAL_FDCAN1);
	}
}

void CAN2_ISR(void)
{
	unsigned int *pBufferStartAddress;
	unsigned int *piIndexOfFirstFrame;
	unsigned int *piNumFramesToRead;
	unsigned int *piMaxFramesInFIFO;
	unsigned int i = 0;
	STM32CAN_Receive(CAN_PERIPHERAL_FDCAN2, pBufferStartAddress, piIndexOfFirstFrame, piNumFramesToRead, piMaxFramesInFIFO);

	// if the test is running
	if (CAN2_Test_Running == 1)
	{
		tsSTM32CAN_Frame tsCANFrame;
		tsCANFrame.iIdentifier = ((tsSTM32CAN_RXFifoElement*)pBufferStartAddress)->iID;
		tsCANFrame.iDataLength = ((tsSTM32CAN_RXFifoElement*)pBufferStartAddress)->iDLC;
		for (i = 0 ; i < tsCANFrame.iDataLength ; i++ )
		{
			tsCANFrame.aData[i] = ((tsSTM32CAN_RXFifoElement*)pBufferStartAddress)->aDataBytes[i];
		}

		STM32CAN_Transmit(CAN_PERIPHERAL_FDCAN2, &tsCANFrame, 1);
	}
	else
	{
		STM32CAN_Flush(CAN_PERIPHERAL_FDCAN2);
	}
}

// Perform a test of the decoders that are used to assert the Chip Select signals.
static void MRAM_CSTest(void)
{
	HAL_UART_Transmit(&huart3, "Intialising the FMC to connect the MRAM\r\n", 41, 1000);

	int i = 0;
	int j = 0;
	unsigned short *pMRAMLocation;
	unsigned short actualValue;
	char cCounterChar;

	// FMC bank 1 registers
	int *piFMC_BCR1 = 0x52004000;
	int *piFMC_BTR1 = 0x52004004;

	// pointer to the MRAM
	unsigned short *piMRAM = MRAM_START_ADDRESS;

	unsigned short Value = 0U;
	unsigned int iLoopCounter = 0UL;

    // set the BTR register to 00000000000000000000000100000010 (binary) 0x00000102 (hex)
	// See the above commentry for justification
	*piFMC_BTR1 = 0x00000202UL;
	//*piFMC_BTR1 = 0x0000FF08UL;
	// Set the BCR register to 1000 0000 0010 0000 0001 0000 1001 0001 (binary) 0x80201091 (hex)
	// See the above commentry for justification
	*piFMC_BCR1 = 0x81201091UL;

	// run through all 10 chip selects
	for (i=0 ; i < 10 ; i++)
	{
		// address the next chip
		pMRAMLocation = MRAM_START_ADDRESS + (i* MRAM_SINGLE_CHIP_SIZE);
		HAL_UART_Transmit(&huart3, "Writing to MRAM at chip select #", 32, 1000);
		cCounterChar = i + 48;
		HAL_UART_Transmit(&huart3, &cCounterChar, 1, 1000);
		HAL_UART_Transmit(&huart3, "\r\n", 2, 1000);

		// fill the first 10 locations with content
		for(j=0 ; j < 10 ; j++)
		{
			*pMRAMLocation = (unsigned short)j;
			pMRAMLocation++;
		}

		// reset the pointer to the start address of this chip.
		pMRAMLocation = MRAM_START_ADDRESS + (i* MRAM_SINGLE_CHIP_SIZE);
		// check that the chip is storing the first 10 locations
		for (j=0 ; j < 10 ; j++)
		{
			actualValue = *pMRAMLocation;
			pMRAMLocation++;
			if (actualValue != j)
			{
				HAL_UART_Transmit(&huart3, "Error reading MRAM at chip select #", 35, 1000);
				cCounterChar = i + 48;
				HAL_UART_Transmit(&huart3, &cCounterChar, 1, 1000);
				HAL_UART_Transmit(&huart3, "\r\n", 2, 1000);
				j = 20;
			}
		}
		// if the test was successful.
		if (j == 10)
		{
			HAL_UART_Transmit(&huart3, "Successfully wrote and read data at chip select #", 49, 1000);
			cCounterChar = i + 48;
			HAL_UART_Transmit(&huart3, &cCounterChar, 1, 1000);
			HAL_UART_Transmit(&huart3, "\r\n", 2, 1000);
		}

	}
	HAL_UART_Transmit(&huart3, pcSplashScreen, strlen(pcSplashScreen), 1000);

}

static void UART_Test(UART_HandleTypeDef *huart)
{
	HAL_StatusTypeDef status;
	char pData[1];

	// perform dummy read to flush UART
	HAL_UART_Receive(huart, pData, 1, 0);

	status = HAL_UART_Receive(huart, pData, 1, 10000);
	if (status == HAL_TIMEOUT)
	{
		HAL_UART_Transmit(&huart3, "Nothing received\r\n", 18, 1000);
	}
	else
	{
		HAL_UART_Transmit(&huart3, "Received ", 10, 1000);
		HAL_UART_Transmit(&huart3, pData, 1, 1000);

		HAL_UART_Transmit(&huart3, " echoing on UART\r\n", 18, 1000);
		HAL_UART_Transmit(huart, pData, 1, 1000);
	}

	HAL_UART_Transmit(&huart3, pcSplashScreen, strlen(pcSplashScreen), 1000);
}

static void LVDS_DR_To_MOSI_Test(void)
{
	LoopBackIoTest(GPIOG, 0x8000, GPIOD, 0x0080);
}
static void LVDS_DR_To_NSS_Test(void)
{
	LoopBackIoTest(GPIOG, 0x8000, GPIOG, 0x0400);
}
static void LVDS_DR_To_SCK_Test(void)
{
	LoopBackIoTest(GPIOG, 0x8000, GPIOG, 0x0800);
}
static void LVDS_EXTI9_To_MOSI_Test(void)
{
	LoopBackIoTest(GPIOG, 0x0200, GPIOD, 0x0080);
}
static void LVDS_EXTI9_To_NSS_Test(void)
{
	LoopBackIoTest(GPIOG, 0x0200, GPIOG, 0x0400);
}
static void LVDS_EXTI9_To_SCK_Test(void)
{
	LoopBackIoTest(GPIOG, 0x0200, GPIOG, 0x0800);
}


static void LoopBackIoTest(GPIO_TypeDef *InputGPIOx, uint16_t InputGPIO_Pin, GPIO_TypeDef *OutputGPIOx, uint16_t OutputGPIO_Pin)
{
	unsigned int tEndTime, tStartTime, tCurrentTime;
	tStartTime = HAL_GetTick();
	tEndTime = tStartTime + 10000; // end time is 10 seconds in the future
	tCurrentTime = tStartTime;
	GPIO_PinState PinState;

	// while 10 seconds are not yet up
	while (tCurrentTime < tEndTime)
	{
		// sample the input IO
		PinState = HAL_GPIO_ReadPin(InputGPIOx , InputGPIO_Pin);
		HAL_GPIO_WritePin(OutputGPIOx, OutputGPIO_Pin, PinState);

		// set the output given the input
		tCurrentTime = HAL_GetTick();
	}

	HAL_UART_Transmit(&huart3, pcSplashScreen, strlen(pcSplashScreen), 1000);
}


// a struct to hold the pin mappings.
typedef struct tePinMapping
{
	GPIO_TypeDef *GPIOx;
	uint16_t     GPIO_Pin;
} tePinMapping;

static void GPIO_Test(void)
{
	tePinMapping teGPIOPinMapping[31] =
	{
		{GPIOB, 0x0020},
		{GPIOG, 0x4000},
		{GPIOF, 0x0400},
		{GPIOF, 0x0100},
		{GPIOF, 0x0080},
		{GPIOF, 0x0040},
		{GPIOA, 0x0100},
		{GPIOC, 0x0200},
		{GPIOB, 0x4000},
		{GPIOB, 0x8000},
		{GPIOG, 0x1000},
		{GPIOD, 0x0040},
		{GPIOD, 0x0008},
		{GPIOD, 0x0004},
		{GPIOC, 0x1000},
		{GPIOA, 0x0400},
		{GPIOA, 0x0200},
		{GPIOC, 0x0100},
		{GPIOB, 0x0004},
		{GPIOA, 0x0020},
		{GPIOG, 0x0200},
		{GPIOD, 0x0080},
		{GPIOG, 0x0800},
		{GPIOG, 0x0400},
		{GPIOB, 0x0200},
		{GPIOB, 0x0100},
		{GPIOG, 0x8000},
		{GPIOB, 0x0800},
		{GPIOB, 0x0400},
		{GPIOB, 0x0080},
		{GPIOB, 0x0040}
	};
	int Counter = 0UL;

	HAL_UART_Transmit(&huart3, "GPIO Tests, cycling GPIO lines\r\n", 32, 1000);

	for (Counter = 0UL ; Counter < 31UL ; Counter++)
	{
		char cCounterChar = (Counter%10) + 48; // convert number to ASCII
		// set one output high
		HAL_UART_Transmit(&huart3, "Setting GPIO output ", 20, 1000);
		if ((Counter > 9) && (Counter <= 19))
		{
			HAL_UART_Transmit(&huart3, "1", 1, 1000);
		}
	    if ((Counter > 19) && (Counter <= 29))
		{
	    	HAL_UART_Transmit(&huart3, "2", 1, 1000);
		}
	    if (Counter == 30)
	    {
	    	HAL_UART_Transmit(&huart3, "3", 1, 1000);
	    }
		HAL_UART_Transmit(&huart3, &cCounterChar, 1, 1000);
		HAL_UART_Transmit(&huart3, " high\r\n", 7, 1000);

		HAL_GPIO_WritePin(teGPIOPinMapping[Counter].GPIOx, teGPIOPinMapping[Counter].GPIO_Pin, GPIO_PIN_SET);

		// wait for 0.025 seconds
		HAL_Delay(25);

		// set one output low
		HAL_UART_Transmit(&huart3, "Setting GPIO output ", 20, 1000);
		if ((Counter > 9) && (Counter <= 19))
		{
			HAL_UART_Transmit(&huart3, "1", 1, 1000);
		}
		if ((Counter > 19) && (Counter <= 29))
		{
		   	HAL_UART_Transmit(&huart3, "2", 1, 1000);
		}
		if (Counter == 30)
		{
		   	HAL_UART_Transmit(&huart3, "3", 1, 1000);
		}
		HAL_UART_Transmit(&huart3, &cCounterChar, 1, 1000);
		HAL_UART_Transmit(&huart3, " low\r\n", 6, 1000);

		HAL_GPIO_WritePin(teGPIOPinMapping[Counter].GPIOx, teGPIOPinMapping[Counter].GPIO_Pin, GPIO_PIN_RESET);

		// wait for 0.025 seconds
		HAL_Delay(25);
	}

	HAL_UART_Transmit(&huart3, pcSplashScreen, strlen(pcSplashScreen), 1000);
}

static void ANALOGUE_Test(void)
{
	int Counter;
	tePinMapping teAnalogueInputPinMapping[8] =
		{
			{GPIOF, 0x0800},
			{GPIOA, 0x0040},
			{GPIOB, 0x0002},
			{GPIOC, 0x0020},
			{GPIOC, 0x0004},
			{GPIOC, 0x0008},
			{GPIOC, 0x0001},
			{GPIOC, 0x0002}
		};

	HAL_UART_Transmit(&huart3, "\r\nAnalogue Input Tests, cycling through Inputs\r\n", 48, 1000);

	for (Counter = 0UL ; Counter < 8UL ; Counter++)
	{
		char cCounterChar = Counter + 48; // convert number to ASCII
		// set one output high
		HAL_UART_Transmit(&huart3, "Setting analogue input ", 23, 1000);
		HAL_UART_Transmit(&huart3, &cCounterChar, 1, 1000);
		HAL_UART_Transmit(&huart3, " high\r\n", 7, 1000);

		HAL_GPIO_WritePin(teAnalogueInputPinMapping[Counter].GPIOx, teAnalogueInputPinMapping[Counter].GPIO_Pin, GPIO_PIN_SET);
		// wait for 0.25 seconds
		HAL_Delay(250);
		// set one output low
		HAL_UART_Transmit(&huart3, "Setting analogue input ", 23, 1000);
		HAL_UART_Transmit(&huart3, &cCounterChar, 1, 1000);
		HAL_UART_Transmit(&huart3, " low\r\n", 6, 1000);
		HAL_GPIO_WritePin(teAnalogueInputPinMapping[Counter].GPIOx, teAnalogueInputPinMapping[Counter].GPIO_Pin, GPIO_PIN_RESET);
		// wait for 0.25 seconds
		HAL_Delay(250);
	}
	HAL_UART_Transmit(&huart3, pcSplashScreen, strlen(pcSplashScreen), 1000);

}


void I2C1_Test(void)
{
	HAL_UART_Transmit(&huart3, "I2C1 test, setting clock and data low\r\n", 39, 1000);

	// deassert data
	HAL_GPIO_WritePin(GPIOB, 0x0080, GPIO_PIN_RESET);

	// deassert clock
	HAL_GPIO_WritePin(GPIOB, 0x0040, GPIO_PIN_RESET);

	HAL_Delay(250);
	HAL_UART_Transmit(&huart3, "I2C1 test, setting clock and data high\r\n", 40, 1000);

	// assert clock
	HAL_GPIO_WritePin(GPIOB, 0x0040, GPIO_PIN_SET);
	// assert data
	HAL_GPIO_WritePin(GPIOB, 0x0080, GPIO_PIN_SET);

	HAL_Delay(250);
	HAL_UART_Transmit(&huart3, "I2C1 test, setting clock and data low\r\n", 39, 1000);

	// deassert data
	HAL_GPIO_WritePin(GPIOB, 0x0080, GPIO_PIN_RESET);

	// deassert clock
	HAL_GPIO_WritePin(GPIOB, 0x0040, GPIO_PIN_RESET);


	HAL_UART_Transmit(&huart3, pcSplashScreen, strlen(pcSplashScreen), 1000);
}

void I2C2_Test(void)
{
	HAL_UART_Transmit(&huart3, "I2C2 test, setting clock and data low\r\n", 39, 1000);

	// deassert data
	HAL_GPIO_WritePin(GPIOB, 0x0800, GPIO_PIN_RESET);

	// deassert clock
	HAL_GPIO_WritePin(GPIOB, 0x0400, GPIO_PIN_RESET);

	HAL_Delay(250);
	HAL_UART_Transmit(&huart3, "I2C2 test, setting clock and data high\r\n", 40, 1000);

	// assert clock
	HAL_GPIO_WritePin(GPIOB, 0x0400, GPIO_PIN_SET);
	// assert data
	HAL_GPIO_WritePin(GPIOB, 0x0800, GPIO_PIN_SET);

	HAL_Delay(250);
	HAL_UART_Transmit(&huart3, "I2C2 test, setting clock and data low\r\n", 39, 1000);

	// deassert data
	HAL_GPIO_WritePin(GPIOB, 0x0800, GPIO_PIN_RESET);

	// deassert clock
	HAL_GPIO_WritePin(GPIOB, 0x0400, GPIO_PIN_RESET);

	HAL_UART_Transmit(&huart3, pcSplashScreen, strlen(pcSplashScreen), 1000);
}

void I2C4_Test(void)
{
	HAL_UART_Transmit(&huart3, "I2C4 test, setting clock and data low\r\n", 39, 1000);

	// deassert data
	HAL_GPIO_WritePin(GPIOB, 0x0200, GPIO_PIN_RESET);

	// deassert clock
	HAL_GPIO_WritePin(GPIOB, 0x0100, GPIO_PIN_RESET);

	HAL_Delay(250);
	HAL_UART_Transmit(&huart3, "I2C4 test, setting clock and data high\r\n", 40, 1000);

	// assert clock
	HAL_GPIO_WritePin(GPIOB, 0x0100, GPIO_PIN_SET);
	// assert data
	HAL_GPIO_WritePin(GPIOB, 0x0200, GPIO_PIN_SET);

	HAL_Delay(250);
	HAL_UART_Transmit(&huart3, "I2C4 test, setting clock and data low\r\n", 39, 1000);

	// deassert data
	HAL_GPIO_WritePin(GPIOB, 0x0200, GPIO_PIN_RESET);

	// deassert clock
	HAL_GPIO_WritePin(GPIOB, 0x0100, GPIO_PIN_RESET);

	HAL_UART_Transmit(&huart3, pcSplashScreen, strlen(pcSplashScreen), 1000);
}

void PPS_Test(void)
{
	HAL_UART_Transmit(&huart3, "PPS test, high and low 10 times @ 1Hz\r\n", 39, 1000);

	for (int i = 0 ; i < 10 ; i++)
	{
		HAL_GPIO_WritePin(GPIOC, 0x2000, GPIO_PIN_SET);
		HAL_Delay(500);
		HAL_GPIO_WritePin(GPIOC, 0x2000, GPIO_PIN_RESET);
		HAL_Delay(500);
	}

	HAL_UART_Transmit(&huart3, pcSplashScreen, strlen(pcSplashScreen), 1000);
}


// MRAM Test
// I think the memory will be accessible from address 0x60000000 (Bank 1 - up to 64MB)
// BMAP[1:0] should be 0
// The memory will be addressable in 16 bit mode using ADDR[25:1]

// for MODE 1 SRAM:
// BCR register
// 31 FMCEN 0x1
// 30:26 Reserved 0x000
// 25:24 BMAP set to 01b
// 23:22 Reserved 0x000
// 21 WFDIS Is set to 1 ***I THINK **
// 20 CCLKEN Set to 0 ** I THINK **
// 19 CBURSTRW 0x0 (no effect in Asynchronous mode)
// 18:16 CPSIZE 0x0 (no effect in Asynchronous mode)
// 15 ASYNCWAIT Set to 0 ** I THINK **
// 14 EXTMOD 0x0
// 13 WAITEN 0x0 (no effect in Asynchronous mode)
// 12 WREN Set to 1
// 11 WAITCFG Don’t care
// 10 Reserved 0x0
// 9 WAITPOL Set to 0 ** I THINK **
// 8 BURSTEN 0x0
// 7 Reserved 0x1
// 6 FACCEN Don’t care
// 5:4 01 (16 bit mode)
// 3:2 MTYP 0: SRAM memory type
// 1 MUXE 0x0
// 0 MBKEN 0x1

// BTR register
// 31:30 Reserved 0x0
// 29:28 ACCMOD Don’t care
// 27:24 DATLAT Don’t care
// 23:20 CLKDIV Don’t care
// 19:16 BUSTURN Time 15ns... set to 0 to achieve 15ns.
// 15:8 DATAST 10ns... setting to 1, sets this to 15ns.
// for write accesses, DATAST fmc_ker_ck cycles for read accesses).
// 7:4 ADDHLD Don’t care
// 3:0 ADDSET 15ns is the minimum... Needs to account for address decoders... input clk fmc_ker_ck is 64MHz...
// setting ADDSET to 2 results in 31.25ns
// Duration of the first access phase (ADDSET fmc_ker_ck cycles).
// Minimum value for ADDSET is 0.

// Set the IO for nLB and nUB to low (enabled)... word access.

// nW controlled write access

// 64 MHz: FMC clock drive fmc_ker_ck ** I think **
void MRAM_WriteTest(void)
{
	// initialise the FMC to talk to the MRAM
	HAL_UART_Transmit(&huart3, "Intialising the FMC to connect the MRAM\r\n", 41, 1000);

	// FMC bank 1 registers
	int *piFMC_BCR1 = 0x52004000;

	int *piFMC_BTR1 = 0x52004004;

	// pointer to the MRAM
	unsigned short *piMRAM = MRAM_START_ADDRESS;

	unsigned short Value = 0U;
	unsigned int iLoopCounter = 0UL;



	//HAL_SetFMCMemorySwappingConfig(FMC_SWAPBMAP_SDRAM_SRAM);
    // set the BTR register to 00000000000000000000000100000010 (binary) 0x00000102 (hex)
	// See the above commentry for justification
	*piFMC_BTR1 = 0x00000202UL;
	//*piFMC_BTR1 = 0x0000FF02UL;
	// Set the BCR register to 1000 0010 1010 0000 0001 0000 1001 0001 (binary) 0x80201091 (hex)
	// See the above commentry for justification
	*piFMC_BCR1 = 0x81201091UL;

	// Write a pattern into the memory
	// the board has 8 million bytes of memory. starting at location 0x60000000
	// so the memory extends from 0x60000000 to 0x607A1200
	// Place data into the memory in 1000 byte chunks (pattern 0,1,2.....), start the next chunk at the next number (pattern 1,2,3.....)
	HAL_UART_Transmit(&huart3, "Writing next 1000 bytes\r\n", 25, 1000);
	while (piMRAM < (MRAM_START_ADDRESS + MRAM_SIZE))
	{
		// set the value in the location and move to next 16 bits.
		*piMRAM = Value;
		Value++;
		piMRAM++;
		iLoopCounter++;

		// every 1000 locations, start the sequence again, starting at a value above the last start value.
		if ((iLoopCounter % 1000) == 0)
		{
			HAL_UART_Transmit(&huart3, "Writing next 1000 bytes\r\n", 25, 1000);
			Value = (iLoopCounter / 1000)*1000;
		}
	}

	HAL_UART_Transmit(&huart3, pcSplashScreen, strlen(pcSplashScreen), 1000);
}

// reads the data written to by the write function
void MRAM_ReadTest(void)
{
	// pointer to the MRAM
	unsigned short *piMRAM = MRAM_START_ADDRESS;
	unsigned int bProblem = 0;

	// FMC bank 1 registers
	int *piFMC_BCR1 = 0x52004000;

	int *piFMC_BTR1 = 0x52004004;

	unsigned short ExpectedValue = 0U;
	unsigned short ActualValue = 0U;
	unsigned int iLoopCounter = 0UL;

	// set the BTR register to 00000000000000000000000100000010 (binary) 0x00000102 (hex)
	// See the above commentry for justification
	//*piFMC_BTR1 = 0x00000202UL;
	*piFMC_BTR1 = 0x00000202UL;
	// Set the BCR register to 1000 0000 0010 0000 0001 0000 1001 0001 (binary) 0x80201091 (hex)
	// See the above commentry for justification
	*piFMC_BCR1 = 0x81201091UL;

	// Reading a pattern from the memory
	// the board has 8 million bytes of memory. starting at location 0x60000000
	// so the memory extends from 0x60000000 to 0x607A1200
	// Read data from the memory in 1000 byte chunks (pattern 0,1,2.....), start the next chunk at the next number (pattern 1,2,3.....)
	HAL_UART_Transmit(&huart3, "Reading next 1000 bytes\r\n", 24, 1000);

	while (piMRAM < (MRAM_START_ADDRESS + MRAM_SIZE))
	{
		// set the value in the location and move to next 16 bits.
		ActualValue = *piMRAM;

		// compare actual with expected value
		if (ActualValue != ExpectedValue)
		{
			HAL_UART_Transmit(&huart3, "Error reading data\r\n", 20, 1000);
			bProblem = 1;
		}
		ExpectedValue++;
		piMRAM++;
		iLoopCounter++;

		// every 1000 locations, start the sequence again, starting at a value above the last start value.
		if ((iLoopCounter % 1000) == 0)
		{
			HAL_UART_Transmit(&huart3, "Reading next 1000 bytes\r\n", 25, 1000);
			ExpectedValue = (unsigned short)(iLoopCounter / 1000)*1000;
		}
	}
	if (bProblem == 1)
	{
		HAL_UART_Transmit(&huart3, "ERROR READING MEMORY\r\n", 22, 1000);
	}
	else
	{
		HAL_UART_Transmit(&huart3, "MEMORY ALL OK\r\n", 15, 1000);
	}

	HAL_UART_Transmit(&huart3, pcSplashScreen, strlen(pcSplashScreen), 1000);
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI
                              |RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 9;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOMEDIUM;
  RCC_OscInitStruct.PLL.PLLFRACN = 3072;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_LSE, RCC_MCODIV_1);
  HAL_RCC_MCOConfig(RCC_MCO2, RCC_MCO2SOURCE_HSE, RCC_MCODIV_1);
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.Resolution = ADC_RESOLUTION_16B;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  sConfig.OffsetSignedSaturation = DISABLE;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief ADC3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC3_Init(void)
{

  /* USER CODE BEGIN ADC3_Init 0 */

  /* USER CODE END ADC3_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC3_Init 1 */

  /* USER CODE END ADC3_Init 1 */

  /** Common config
  */
  hadc3.Instance = ADC3;
  hadc3.Init.Resolution = ADC_RESOLUTION_16B;
  hadc3.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc3.Init.LowPowerAutoWait = DISABLE;
  hadc3.Init.ContinuousConvMode = DISABLE;
  hadc3.Init.NbrOfConversion = 1;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc3.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
  hadc3.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc3.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
  hadc3.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_VBAT;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  sConfig.OffsetSignedSaturation = DISABLE;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC3_Init 2 */

  /* USER CODE END ADC3_Init 2 */

}

/**
  * @brief FDCAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_FDCAN1_Init(void)
{

  /* USER CODE BEGIN FDCAN1_Init 0 */

  /* USER CODE END FDCAN1_Init 0 */

  /* USER CODE BEGIN FDCAN1_Init 1 */

  /* USER CODE END FDCAN1_Init 1 */
  hfdcan1.Instance = FDCAN1;
  hfdcan1.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
  hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan1.Init.AutoRetransmission = DISABLE;
  hfdcan1.Init.TransmitPause = DISABLE;
  hfdcan1.Init.ProtocolException = DISABLE;
  hfdcan1.Init.NominalPrescaler = 1;
  hfdcan1.Init.NominalSyncJumpWidth = 1;
  hfdcan1.Init.NominalTimeSeg1 = 2;
  hfdcan1.Init.NominalTimeSeg2 = 2;
  hfdcan1.Init.DataPrescaler = 1;
  hfdcan1.Init.DataSyncJumpWidth = 1;
  hfdcan1.Init.DataTimeSeg1 = 1;
  hfdcan1.Init.DataTimeSeg2 = 1;
  hfdcan1.Init.MessageRAMOffset = 0;
  hfdcan1.Init.StdFiltersNbr = 0;
  hfdcan1.Init.ExtFiltersNbr = 0;
  hfdcan1.Init.RxFifo0ElmtsNbr = 0;
  hfdcan1.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_8;
  hfdcan1.Init.RxFifo1ElmtsNbr = 0;
  hfdcan1.Init.RxFifo1ElmtSize = FDCAN_DATA_BYTES_8;
  hfdcan1.Init.RxBuffersNbr = 0;
  hfdcan1.Init.RxBufferSize = FDCAN_DATA_BYTES_8;
  hfdcan1.Init.TxEventsNbr = 0;
  hfdcan1.Init.TxBuffersNbr = 0;
  hfdcan1.Init.TxFifoQueueElmtsNbr = 0;
  hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  hfdcan1.Init.TxElmtSize = FDCAN_DATA_BYTES_8;
  if (HAL_FDCAN_Init(&hfdcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN FDCAN1_Init 2 */

  /* USER CODE END FDCAN1_Init 2 */

}

/**
  * @brief FDCAN2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_FDCAN2_Init(void)
{

  /* USER CODE BEGIN FDCAN2_Init 0 */

  /* USER CODE END FDCAN2_Init 0 */

  /* USER CODE BEGIN FDCAN2_Init 1 */

  /* USER CODE END FDCAN2_Init 1 */
  hfdcan2.Instance = FDCAN2;
  hfdcan2.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
  hfdcan2.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan2.Init.AutoRetransmission = DISABLE;
  hfdcan2.Init.TransmitPause = DISABLE;
  hfdcan2.Init.ProtocolException = DISABLE;
  hfdcan2.Init.NominalPrescaler = 1;
  hfdcan2.Init.NominalSyncJumpWidth = 1;
  hfdcan2.Init.NominalTimeSeg1 = 2;
  hfdcan2.Init.NominalTimeSeg2 = 2;
  hfdcan2.Init.DataPrescaler = 1;
  hfdcan2.Init.DataSyncJumpWidth = 1;
  hfdcan2.Init.DataTimeSeg1 = 1;
  hfdcan2.Init.DataTimeSeg2 = 1;
  hfdcan2.Init.MessageRAMOffset = 0;
  hfdcan2.Init.StdFiltersNbr = 0;
  hfdcan2.Init.ExtFiltersNbr = 0;
  hfdcan2.Init.RxFifo0ElmtsNbr = 0;
  hfdcan2.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_8;
  hfdcan2.Init.RxFifo1ElmtsNbr = 0;
  hfdcan2.Init.RxFifo1ElmtSize = FDCAN_DATA_BYTES_8;
  hfdcan2.Init.RxBuffersNbr = 0;
  hfdcan2.Init.RxBufferSize = FDCAN_DATA_BYTES_8;
  hfdcan2.Init.TxEventsNbr = 0;
  hfdcan2.Init.TxBuffersNbr = 0;
  hfdcan2.Init.TxFifoQueueElmtsNbr = 0;
  hfdcan2.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  hfdcan2.Init.TxElmtSize = FDCAN_DATA_BYTES_8;
  if (HAL_FDCAN_Init(&hfdcan2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN FDCAN2_Init 2 */

  /* USER CODE END FDCAN2_Init 2 */

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
  hi2c1.Init.Timing = 0x00707CBB;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x00707CBB;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief I2C4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C4_Init(void)
{

  /* USER CODE BEGIN I2C4_Init 0 */

  /* USER CODE END I2C4_Init 0 */

  /* USER CODE BEGIN I2C4_Init 1 */

  /* USER CODE END I2C4_Init 1 */
  hi2c4.Instance = I2C4;
  hi2c4.Init.Timing = 0x00707CBB;
  hi2c4.Init.OwnAddress1 = 0;
  hi2c4.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c4.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c4.Init.OwnAddress2 = 0;
  hi2c4.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c4.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c4.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c4) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c4, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c4, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C4_Init 2 */

  /* USER CODE END I2C4_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable Calibrartion
  */
  if (HAL_RTCEx_SetCalibrationOutPut(&hrtc, RTC_CALIBOUTPUT_1HZ) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES_TXONLY;
  hspi1.Init.DataSize = SPI_DATASIZE_4BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 0x0;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  hspi1.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  hspi1.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  hspi1.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi1.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi1.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  hspi1.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  hspi1.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  hspi1.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  hspi1.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart4.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart4, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart4, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/* FMC initialization function */
static void MX_FMC_Init(void)
{

  /* USER CODE BEGIN FMC_Init 0 */

  /* USER CODE END FMC_Init 0 */

  FMC_NORSRAM_TimingTypeDef Timing = {0};

  /* USER CODE BEGIN FMC_Init 1 */

  /* USER CODE END FMC_Init 1 */

  /** Perform the SRAM1 memory initialization sequence
  */
  hsram1.Instance = FMC_NORSRAM_DEVICE;
  hsram1.Extended = FMC_NORSRAM_EXTENDED_DEVICE;
  /* hsram1.Init */
  hsram1.Init.NSBank = FMC_NORSRAM_BANK1;
  hsram1.Init.DataAddressMux = FMC_DATA_ADDRESS_MUX_DISABLE;
  hsram1.Init.MemoryType = FMC_MEMORY_TYPE_SRAM;
  hsram1.Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_16;
  hsram1.Init.BurstAccessMode = FMC_BURST_ACCESS_MODE_DISABLE;
  hsram1.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram1.Init.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;
  hsram1.Init.WriteOperation = FMC_WRITE_OPERATION_DISABLE;
  hsram1.Init.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;
  hsram1.Init.ExtendedMode = FMC_EXTENDED_MODE_DISABLE;
  hsram1.Init.AsynchronousWait = FMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram1.Init.WriteBurst = FMC_WRITE_BURST_DISABLE;
  hsram1.Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
  hsram1.Init.WriteFifo = FMC_WRITE_FIFO_ENABLE;
  hsram1.Init.PageSize = FMC_PAGE_SIZE_NONE;
  /* Timing */
  Timing.AddressSetupTime = 15;
  Timing.AddressHoldTime = 15;
  Timing.DataSetupTime = 255;
  Timing.BusTurnAroundDuration = 15;
  Timing.CLKDivision = 16;
  Timing.DataLatency = 17;
  Timing.AccessMode = FMC_ACCESS_MODE_A;
  /* ExtTiming */

  if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK)
  {
    Error_Handler( );
  }

  /* USER CODE BEGIN FMC_Init 2 */

  /* USER CODE END FMC_Init 2 */
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
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_10, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_14, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pins : PE0 and PE1*/
  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_RESET);


  /*Configure GPIO pins : PA9 (DI6)*/
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PF6 PF7 PF8 PF10 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pin : PF9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : PA5 PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PC9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PB2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB14 PB15 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PG6 PG7 PG8 PG14 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : PC4 & PC6 */
  GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PC8 PC12 */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PD2 PD3 PD6 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PG9 PG15 */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pin : PG12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /* configure

  /*Configure GPIO pin: PB6,7,8,9,10,11 as GP Output... all the I2C busses.*/
  GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* configure the analogue inputs to be run as outputs */
  // PA6
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // PB1
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // PC0,1,2,3,5
  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 |GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  // PF9,11
  GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  // PG10,11 (NSS SCLK)
  GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  // PD7 (MOSI)
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                            |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9
                            |GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13
                            |GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                            |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_12|GPIO_PIN_13
                            |GPIO_PIN_14|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                            |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                            |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15
                            |GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_FMC;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	/*Configure GPIO pin : PC9 */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF0_MCO;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pin : PA8 */
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF0_MCO;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // initialise CAN2 pins
    GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // initialise CAN1 pins
    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


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
