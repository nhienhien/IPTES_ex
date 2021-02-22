/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "app_x-cube-ai.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "arm_math.h"
#include "feature_extraction.h"
#include "string.h"
//#include "stdio.h"
#include <stdlib.h>
#include "stdint.h"
#include <math.h>
#include "DHT.h"
#include "bh1750.h"
#include "pms5003.h"
#include "math_helper.h"
#include <esp8266_communication.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
//MelSpectrogram
#define HOPLENGTH 		 512
#define FILL_BUFFER_SIZE 1024
#define NFFT             1024
#define NMELS            30
#define NAUDIO 		     16896
#define ADC_BUF_LEN 16896
#define ADCbit		     12
#define SPECTROGRAM_ROWS NMELS
#define SPECTROGRAM_COLS 32
#define n_label 5

//USER DEFINE

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/*
const float32_t firCoeffs32[NUM_TAPS] = {
  -0.0018225230f, -0.0015879294f, +0.0000000000f, +0.0036977508f, +0.0080754303f, +0.0085302217f, -0.0000000000f, -0.0173976984f,
  -0.0341458607f, -0.0333591565f, +0.0000000000f, +0.0676308395f, +0.1522061835f, +0.2229246956f, +0.2504960933f, +0.2229246956f,
  +0.1522061835f, +0.0676308395f, +0.0000000000f, -0.0333591565f, -0.0341458607f, -0.0173976984f, -0.0000000000f, +0.0085302217f,
  +0.0080754303f, +0.0036977508f, +0.0000000000f, -0.0015879294f, -0.0018225230f
};
*/

//////////////////mel//////////////
static float32_t aSpectrogram[SPECTROGRAM_ROWS * SPECTROGRAM_COLS];
static float32_t aColBuffer[SPECTROGRAM_ROWS];
static uint32_t SpectrColIndex;
float32_t aWorkingBuffer1[NFFT];

static arm_rfft_fast_instance_f32 S_Rfft;
static MelFilterTypeDef           S_MelFilter;
static SpectrogramTypeDef         S_Spectr;
static MelSpectrogramTypeDef      S_MelSpectr;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

float32_t data_out[5];
char Tx_Buffer[100];
volatile uint32_t Rx_indx;
float32_t data_in[NAUDIO];
float32_t pBuffer[FILL_BUFFER_SIZE];
char *Rx_Buffer_Get;
uint32_t t1,t2,t;
uint32_t len_Rx_Buffer;
uint32_t ColIndex=0;
float32_t sample, sample2;
uint16_t ADC_buffer[ADC_BUF_LEN];
uint16_t ADC_buffer_sort[ADC_BUF_LEN];
uint16_t ADC_buffer_FIR [ADC_BUF_LEN];
uint32_t ind=0,write=0;;
uint32_t i=0;
uint16_t value_lux = 0;
//uint8_t rawData[5] = {0,0,0,0,0};
extern struct PMS5003_Data PMS5003;
DHT_data d;
float max_output;
int max_output_ind;
int confirm2;
int confirm1;
int send_data=0;
uint8_t Cmd_End[3]={0xFF, 0xFF, 0xFF};
uint16_t Value_Buf [4];

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc3;
DMA_HandleTypeDef hdma_adc3;

CRC_HandleTypeDef hcrc;

I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart7;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_CRC_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_ADC3_Init(void);
static void MX_I2C1_Init(void);
static void MX_UART7_Init(void);
static void MX_UART4_Init(void);
/* USER CODE BEGIN PFP */
static void PowerTodB(float32_t *pSpectrogram);
void Delete_Buffer(char *A);
void Buffer_Substring(char *Rx_Buffer_Get, char *Rx_Buffer, uint32_t start, uint32_t stop);
void Buffer_to_float(char *Rx_Buffer);
void NEXTION_Send(char * ID, uint16_t num);
void NEXTION_Sendfloat(char * ID, float num);
void Sort_ADC_buffer(void);
void Mel_array(void);
void Find_max_output_ind(void);
void Send_Activities_to_NEXTION(void);
void Send_Sensor_to_NEXTION(void);
//MelSpectrogram
static void Preprocessing_Init(void);
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

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

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
  MX_CRC_Init();
  MX_USART1_UART_Init();
  MX_USART6_UART_Init();
  MX_ADC3_Init();
  MX_I2C1_Init();
  MX_UART7_Init();
  MX_UART4_Init();
  MX_X_CUBE_AI_Init();
  /* USER CODE BEGIN 2 */
  Preprocessing_Init();
  SpectrColIndex = 0;
  HAL_ADC_Start_DMA(&hadc3, (uint32_t *)ADC_buffer, ADC_BUF_LEN);
  BH1750_Init();
  BH1750_Start();
  PMS5003_ReceiveStart_IT(&huart6);
  //HAL_TIM_Base_Start_IT(&htim2);
  //ESP_Init("IoT-Research", "Tapit168");
  HAL_Delay(1000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  	/*HAL_UART_Transmit(&huart7,(uint8_t *)"output.val=10",13,1000);
	  	HAL_UART_Transmit(&huart7,Cmd_End,3,100);
	    HAL_Delay(500);

	  	HAL_UART_Transmit(&huart7,(uint8_t *)"output.val=9",12,1000);
	  	HAL_UART_Transmit(&huart7,Cmd_End,3,100);
	    HAL_Delay(500);*/

	  	/*HAL_UART_Transmit(&huart7,(uint8_t *)"temp.txt=\"10\"",17,1000);
	  	HAL_UART_Transmit(&huart7,Cmd_End,3,100);
	  	HAL_Delay(1000);
	  	HAL_UART_Transmit(&huart7,(uint8_t *)"temp.txt=\"9\"",16,1000);
	  	HAL_UART_Transmit(&huart7,Cmd_End,3,100);
	  	HAL_Delay(1000);*/

	    ind=hdma_adc3.Instance->NDTR;
	    write=ADC_BUF_LEN-ind;
	    Sort_ADC_buffer();
	    //lowpass_FIR();
	    Mel_array();
	  	PowerTodB(aSpectrogram);
	  	aiRun(aSpectrogram, data_out);
	  	t1 = HAL_GetTick();
	  	Find_max_output_ind();
	  	Send_Activities_to_NEXTION();
	  	send_data++;
	  	if(send_data==15)
	  	{
	  		value_lux = BH1750_Read();
	  		PMS5003_ReceiveData_IT(&huart6);
	  		d = DHT_getData(DHT11);
	  		Send_Sensor_to_NEXTION();
	  		send_data=0;
	  		/*Value_Buf [0]=d.temp;
	  		Value_Buf [1]=d.hum;
	  		Value_Buf [2]=value_lux;
	  		Value_Buf [3]= PMS5003.pm2p5_ATM;
	  		ESP_Send_Multi("QBICBAHQY14LYJSR", 4, Value_Buf);*/
	  		esp8266_send_value(d.temp, d.hum, value_lux, PMS5003.pm1p0_ATM, PMS5003.pm2p5_ATM, PMS5003.pm10_ATM);
	  	}
	  	//HAL_Delay(200);
	  	t2 = HAL_GetTick();

	  	t = t2 - t1;
    /* USER CODE END WHILE */

  //MX_X_CUBE_AI_Process();
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
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 369;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART6
                              |RCC_PERIPHCLK_UART4|RCC_PERIPHCLK_UART7
                              |RCC_PERIPHCLK_I2C1;
  PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInitStruct.Uart4ClockSelection = RCC_UART4CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Usart6ClockSelection = RCC_USART6CLKSOURCE_PCLK2;
  PeriphClkInitStruct.Uart7ClockSelection = RCC_UART7CLKSOURCE_PCLK1;
  PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
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
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV6;
  hadc3.Init.Resolution = ADC_RESOLUTION_12B;
  hadc3.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc3.Init.ContinuousConvMode = ENABLE;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.NbrOfConversion = 1;
  hadc3.Init.DMAContinuousRequests = ENABLE;
  hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc3) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC3_Init 2 */

  /* USER CODE END ADC3_Init 2 */

}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;
  hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
  hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
  hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

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
  hi2c1.Init.Timing = 0x20303C59;
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
  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

/**
  * @brief UART7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART7_Init(void)
{

  /* USER CODE BEGIN UART7_Init 0 */

  /* USER CODE END UART7_Init 0 */

  /* USER CODE BEGIN UART7_Init 1 */

  /* USER CODE END UART7_Init 1 */
  huart7.Instance = UART7;
  huart7.Init.BaudRate = 9600;
  huart7.Init.WordLength = UART_WORDLENGTH_8B;
  huart7.Init.StopBits = UART_STOPBITS_1;
  huart7.Init.Parity = UART_PARITY_NONE;
  huart7.Init.Mode = UART_MODE_TX_RX;
  huart7.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart7.Init.OverSampling = UART_OVERSAMPLING_16;
  huart7.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart7.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart7) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART7_Init 2 */

  /* USER CODE END UART7_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 9600;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  huart6.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart6.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */
/////////////sắp xếp lại chuỗi dữ liệu//////
void Sort_ADC_buffer()
{
	for(int i=0;i<ind;i++)
		   ADC_buffer_sort[i]=ADC_buffer[i+write];
	for(int i=0;i<write;i++)
		   ADC_buffer_sort[i+ind]=ADC_buffer[i];
}
///////////////mảng mel 32*30 giá trị//////////////////
void Mel_array()
{
	for (ColIndex=0; ColIndex<SPECTROGRAM_COLS; ColIndex++)
	{
		for(int i=0; i<1024; i++)
		  	pBuffer[i]=(ADC_buffer_sort[ColIndex*HOPLENGTH+i]-2047.0)/2047.0;
		MelSpectrogramColumn(&S_MelSpectr, pBuffer, aColBuffer);
		for (uint32_t j = 0; j < NMELS; j++)
		{
		  	aSpectrogram[j * SPECTROGRAM_COLS + ColIndex] = aColBuffer[j];
		}
	}
}
////////////////tìm vị trí và giá trị lớn nhất của output////////////////////
void Find_max_output_ind()
{
	//confirm1=confirm2;
	confirm2=max_output_ind;
	max_output_ind=0;
	max_output=data_out[0];
	for (int i=1; i<n_label;i++)
	{
		if( data_out[i]>max_output)
		{
			max_output=data_out[i];
			max_output_ind=i;
		}
	}

	//if(data_out[1]>max_output)
		//{max_output=data_out[1];max_output_ind=1;}
	//if(data_out[2]>max_output)
		//{max_output=data_out[2];max_output_ind=2;}
	//if(data_out[3]>max_output)
		//{max_output=data_out[3];max_output_ind=3;}
	//if(data_out[4]>max_output)
		//{max_output=data_out[4];max_output_ind=4;}
}
///////////////////hiển thị hoạt động lên màn hình////////////////////
void Send_Activities_to_NEXTION(void)
{

	if(max_output>0.95)
	{
		if((confirm2==max_output_ind)/*&&(confirm2==confirm1)*/)
		{
			switch (max_output_ind)
			{
			case 1:
					HAL_UART_Transmit(&huart7,(uint8_t *)"output.val=9",12,1000);
					HAL_UART_Transmit(&huart7,Cmd_End,3,100);
					//for(int i=0; i<16896;i=i+20)
					//{
						//printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\r\n",ADC_buffer_sort[i],ADC_buffer_sort[i+1],ADC_buffer_sort[i+2],ADC_buffer_sort[i+3],ADC_buffer_sort[i+4],ADC_buffer_sort[i+5],ADC_buffer_sort[i+6],ADC_buffer_sort[i+7],ADC_buffer_sort[i+8],ADC_buffer_sort[i+9],ADC_buffer_sort[i+10],ADC_buffer_sort[i+11],ADC_buffer_sort[i+12],ADC_buffer_sort[i+13],ADC_buffer_sort[i+14],ADC_buffer_sort[i+15],ADC_buffer_sort[i+16],ADC_buffer_sort[i+17],ADC_buffer_sort[i+18],ADC_buffer_sort[i+19]);

					//}
					break;
			case 2:
					HAL_UART_Transmit(&huart7,(uint8_t *)"output.val=10",13,1000);
				  	HAL_UART_Transmit(&huart7,Cmd_End,3,100);
				  	//for(int i=0; i<16896;i=i+20)
				  	//{
				  		//printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\r\n",ADC_buffer_sort[i],ADC_buffer_sort[i+1],ADC_buffer_sort[i+2],ADC_buffer_sort[i+3],ADC_buffer_sort[i+4],ADC_buffer_sort[i+5],ADC_buffer_sort[i+6],ADC_buffer_sort[i+7],ADC_buffer_sort[i+8],ADC_buffer_sort[i+9],ADC_buffer_sort[i+10],ADC_buffer_sort[i+11],ADC_buffer_sort[i+12],ADC_buffer_sort[i+13],ADC_buffer_sort[i+14],ADC_buffer_sort[i+15],ADC_buffer_sort[i+16],ADC_buffer_sort[i+17],ADC_buffer_sort[i+18],ADC_buffer_sort[i+19]);

				  	//}
				  			break;
			case 3:
					HAL_UART_Transmit(&huart7,(uint8_t *)"output.val=11",13,1000);
					HAL_UART_Transmit(&huart7,Cmd_End,3,100);
					break;
			case 4:
					HAL_UART_Transmit(&huart7,(uint8_t *)"output.val=12",13,1000);
					HAL_UART_Transmit(&huart7,Cmd_End,3,100);
					/*for(int i=0; i<16896;i=i+20)
					{
						printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\r\n",ADC_buffer_FIR[i],ADC_buffer_FIR[i+1],ADC_buffer_FIR[i+2],ADC_buffer_FIR[i+3],ADC_buffer_FIR[i+4],ADC_buffer_FIR[i+5],ADC_buffer_FIR[i+6],ADC_buffer_FIR[i+7],ADC_buffer_FIR[i+8],ADC_buffer_FIR[i+9],ADC_buffer_FIR[i+10],ADC_buffer_FIR[i+11],ADC_buffer_FIR[i+12],ADC_buffer_FIR[i+13],ADC_buffer_FIR[i+14],ADC_buffer_FIR[i+15],ADC_buffer_FIR[i+16],ADC_buffer_FIR[i+17],ADC_buffer_FIR[i+18],ADC_buffer_FIR[i+19]);

					}*/
					break;
			default:
		  			HAL_UART_Transmit(&huart7,(uint8_t *)"output.val=8",12,1000);
		  			HAL_UART_Transmit(&huart7,Cmd_End,3,100);
		  			break;
			}
		}
	}
	else
	{
		if((confirm2==max_output_ind)/*&&(confirm2==confirm1)*/)
		{
		HAL_UART_Transmit(&huart7,(uint8_t *)"output.val=7",12,1000);
		HAL_UART_Transmit(&huart7,Cmd_End,3,100);
		}
	}

}

////////////////////gửi giá trị các cảm biến lên màn hình//////////////////
void Send_Sensor_to_NEXTION(void)
{
	///cảm biến cuồng độ ánh sáng///
	NEXTION_Send("ill_val",value_lux);
	///cảm biến bụi min///
	if (PMS5003.state == PMS5003_RECEIVED)
	{
		NEXTION_Send("pm1_val",PMS5003.pm1p0_ATM);
		NEXTION_Send("pm25_val",PMS5003.pm2p5_ATM);
		NEXTION_Send("pm10_val",PMS5003.pm10_ATM);
		/*printf("    PM1.0    =    %d\r\n", PMS5003.pm1p0_ATM);
		printf("    PM2.5    =    %d\r\n", PMS5003.pm2p5_ATM);
		printf("    PM10     =    %d\r\n", PMS5003.pm10_ATM);
		printf("-----------------------------\r\n");*/
	}
	///cảm biến nhiệt độ, độ ẩm///
	if(d.temp!=0)
	{
		NEXTION_Send("temp_val",d.temp);
	}
	if(d.hum!=0)
	{
		NEXTION_Send("hum_val",d.hum);
	}
	//printf("Temp: %2.1f * С, Hum: %2.1f%%\r\n", d.temp, d.hum);
}
/// Xoa buffer
void Delete_Buffer(char *A)
{
	uint32_t i;
	uint32_t len = strlen(A);
	Rx_indx=0;
	for (i=0; i<len; i++)
	A[i]=0;
}
/// Cat chuoi trong chuoi
void Buffer_Substring(char *Rx_Buffer_Get, char *Rx_Buffer, uint32_t start, uint32_t stop)
{
	uint32_t subStrIndex;
	Delete_Buffer(Rx_Buffer_Get);
	if (stop>=start)
	{
		for(subStrIndex = start; subStrIndex<=stop; subStrIndex++)
		{
			Rx_Buffer_Get[subStrIndex-start]=Rx_Buffer[subStrIndex];
		}
	}
}
/////////gửi lên màn hình số nguyên
void NEXTION_Send(char * ID, uint16_t num)
{
	char buf[50];
	int len= sprintf(buf,"%s.txt=\"%d\"",ID,num);
	HAL_UART_Transmit(&huart7,buf,len,1000);
	HAL_UART_Transmit(&huart7,Cmd_End,3,100);
}
/////////gửi lên màn hình số thực
void NEXTION_Sendfloat(char * ID, float num)
{
	char buf[50];
	int len= sprintf(buf,"%s.txt=\"%.1f\"",ID,num);
	HAL_UART_Transmit(&huart7,buf,len,1000);
	HAL_UART_Transmit(&huart7,Cmd_End,3,100);
}
///////////////
#ifdef __GNUC__
	#define putchar_PROTOTYPE int __io_putchar(int ch)
#else
	#define putchar_PROTOTYPE int fputc(int ch, FILE *f)
#endif

	putchar_PROTOTYPE
	{

		HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1, 0xFFFF);

		return ch;
	}
//Preprocessing Init
static void Preprocessing_Init(void)
{
  /* Init RFFT */
  arm_rfft_fast_init_f32(&S_Rfft, 1024);

  /* Init Spectrogram */
  S_Spectr.pRfft    = &S_Rfft;
  S_Spectr.Type     = SPECTRUM_TYPE_POWER;
  S_Spectr.pWindow  = (float32_t *) hannWin_1024;
  S_Spectr.SampRate = 16000;
  S_Spectr.FrameLen = 1024;
  S_Spectr.FFTLen   = 1024;
  S_Spectr.pScratch = aWorkingBuffer1;

  /* Init Mel filter */
  S_MelFilter.pStartIndices = (uint32_t *) melFiltersStartIndices_1024_30;
  S_MelFilter.pStopIndices  = (uint32_t *) melFiltersStopIndices_1024_30;
  S_MelFilter.pCoefficients = (float32_t *) melFilterLut_1024_30;
  S_MelFilter.NumMels       = 30;

  /* Init MelSpectrogram */
  S_MelSpectr.SpectrogramConf = &S_Spectr;
  S_MelSpectr.MelFilter       = &S_MelFilter;
}
//Rx_Buffer to float
void Buffer_to_float(char *Rx_Buffer)
{
	uint32_t i=0, j=0;
	uint32_t start=0, stop=0;
	uint32_t len = strlen(Rx_Buffer);
	Rx_Buffer[len]=44;//44=','
	len += 1;
	while (i<= len)
	{
	   if (Rx_Buffer[i]==44)
	   {
		  stop=i;
		  Buffer_Substring(Rx_Buffer_Get,Rx_Buffer, start, stop-1);
		  data_in[j]=atof(Rx_Buffer_Get);
		  j++;
		  start=stop+1;
		  i=start;
	    }
	i++;
	}
}
//Power to db
static void PowerTodB(float32_t *pSpectrogram)
{
  float32_t max_mel_energy = 0.0f;
  uint32_t i;

  /* Find MelEnergy Scaling factor */
  for (i = 0; i < NMELS * SPECTROGRAM_COLS; i++) {
    max_mel_energy = (max_mel_energy > pSpectrogram[i]) ? max_mel_energy : pSpectrogram[i];
  }

  /* Scale Mel Energies */
  for (i = 0; i < NMELS * SPECTROGRAM_COLS; i++) {
    pSpectrogram[i] /= max_mel_energy;
  }

  /* Convert power spectrogram to decibel */
  for (i = 0; i < NMELS * SPECTROGRAM_COLS; i++) {
    pSpectrogram[i] = 10.0f * log10f(pSpectrogram[i]);
  }

  /* Threshold output to -80.0 dB */
  for (i = 0; i < NMELS * SPECTROGRAM_COLS; i++) {
    pSpectrogram[i] = (pSpectrogram[i] < -80.0f) ? (-80.0f) : (pSpectrogram[i]);
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
