///* USER CODE BEGIN Header */
///**
//	******************************************************************************
//	* @file           : main.c
//	* @brief          : Main program body
//	******************************************************************************
//	* @attention
//	*
//	* Copyright (c) 2024 STMicroelectronics.
//	* All rights reserved.
//	*
//	* This software is licensed under terms that can be found in the LICENSE file
//	* in the root directory of this software component.
//	* If no LICENSE file comes with this software, it is provided AS-IS.
//	*
//	******************************************************************************
//	*/
///* USER CODE END Header */
///* Includes ------------------------------------------------------------------*/

//	// Inclus�o das Bibliotecas Principais geradas pelo Cube MX

//	#include "main.h"
//	#include "cmsis_os.h"

///* Private includes ----------------------------------------------------------*/
///* USER CODE BEGIN Includes */

//	// Inclus�o da Bilioteca para Funcionamento da Placa PLC01A1

//	#include "x_nucleo_plc01a1.h"

///* USER CODE END Includes */

///* Private typedef -----------------------------------------------------------*/
///* USER CODE BEGIN PTD */

///* USER CODE END PTD */

///* Private define ------------------------------------------------------------*/
///* USER CODE BEGIN PD */

///* USER CODE END PD */

///* Private macro -------------------------------------------------------------*/
///* USER CODE BEGIN PM */

///* USER CODE END PM */

///* Private variables ---------------------------------------------------------*/

//	// Inicializa��o do RTC para funcionamento da Placa 

//		RTC_HandleTypeDef hrtc;
//		
//	// Inicializa��o dos SPI para funcionamento da Placa 

//		SPI_HandleTypeDef hspi1;
//		SPI_HandleTypeDef hspi3;

//	// Comunica��o Serial via USB

//		UART_HandleTypeDef huart2;
//    UART_HandleTypeDef huart1;
//		
//	// Inicializa��o de Thread de Funcionamento

//		osThreadId defaultTaskHandle;
//		osThreadId myTask02Handle;
//		osThreadId myTask03Handle;
//			
//		
///* USER CODE BEGIN PV */

//	static uint8_t* Input_Data; // Detec��o de Entrada na Placa PLC
//	static uint8_t oData = 0; // Inscri��o de Sa�da da Placa PLC
//	static uint8_t* Relay_Status; // Status de Funcionamento das Sa�das da Placa PLC
//	static uint8_t* Current_Limiter_Status; // Status de Funcionamento das Entradas da Placa PLC

//	int qpasso = 0; // Quantidade de Passos dados pelo Motor
//	int horario = 0; // Sinaliza��o do sentido hor�rio de rota��o do motor
//	int antihorario = 0; // Sinaliza��o do sentido antihor�rio de rota��o do motor
//	int habilitado = 0;	 // Sinaliza��o de habilita��o do motor
//	
//	int passo1[8] = {1, 1, 0, 0, 0, 0, 0, 1}; // Sequencia de ativa��o da bobina A+ em half-step
//	int passo2[8] = {0, 1, 1, 1, 0, 0, 0, 0}; // Sequencia de ativa��o da bobina B+ em half-step
//	int passo3[8] = {0, 0, 0, 1, 1, 1, 0, 0}; // Sequencia de ativa��o da bobina A- em half-step
//	int passo4[8] = {0, 0, 0, 0, 0, 1, 1, 1}; // Sequencia de ativa��o da bobina B- em half-step

///* USER CODE END PV */

///* Private function prototypes -----------------------------------------------*/
//	
// // Habilita��o das fun��es de cada funcionalidade habilitada via MX
//	
//		void SystemClock_Config(void);
//		static void MX_GPIO_Init(void);
//		static void MX_SPI1_Init(void);
//    static void MX_USART1_UART_Init(void);
//		static void MX_USART2_UART_Init(void);
//		static void MX_RTC_Init(void);
//		static void MX_SPI3_Init(void);
//		void StartDefaultTask(void const * argument);
//		void StartTask02(void const * argument);
//    void StartTask03(void const * argument);

///* USER CODE BEGIN PFP */
//	
//	// Habilita��o das fun��es de funcionamento da placa PLC

//		static void initializePlc(void);
//		static void PLC_Handler(void);
//		uint8_t* CURRENT_LIMITER_Handler(void);
//		void RELAY_Handler(uint8_t*);

///* USER CODE END PFP */

///* Private user code ---------------------------------------------------------*/
///* USER CODE BEGIN 0 */

///* USER CODE END 0 */

///**
//	* @brief  The application entry point.
//	* @retval int
//	*/
//int main(void)
//{
//	/* USER CODE BEGIN 1 */

//	/* USER CODE END 1 */

//	/* MCU Configuration--------------------------------------------------------*/

//	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
//	
//		// Inicializa��o do HAL
//	
//			HAL_Init();

//	/* USER CODE BEGIN Init */

//	/* USER CODE END Init */

//	/* Configure the system clock */
//	
//		// Inicializa��o do Clock do Sistema
//	
//			SystemClock_Config();

//	/* USER CODE BEGIN SysInit */
//	
//	/* USER CODE END SysInit */

//	/* Initialize all configured peripherals */
//	
//		// Inicializa��o das funcionalidades habilitadas
//	
//			MX_GPIO_Init();
//			MX_SPI1_Init();
//      MX_USART1_UART_Init();
//			MX_USART2_UART_Init();
//			MX_RTC_Init();
//			MX_SPI3_Init();
//	
//	/* USER CODE BEGIN 2 */
//	
//		// Inicializa��o das funcionalidades da placa PLC

//			initializePlc();
//			BSP_RELAY_Reset();

//			BSP_RELAY_EN_Out();
//	
//	/* USER CODE END 2 */

//	/* USER CODE BEGIN RTOS_MUTEX */
//	/* add mutexes, ... */
//	/* USER CODE END RTOS_MUTEX */

//	/* USER CODE BEGIN RTOS_SEMAPHORES */
//	/* add semaphores, ... */
//	/* USER CODE END RTOS_SEMAPHORES */

//	/* USER CODE BEGIN RTOS_TIMERS */
//	/* start timers, add new ones, ... */
//	/* USER CODE END RTOS_TIMERS */

//	/* USER CODE BEGIN RTOS_QUEUES */
//	/* add queues, ... */
//	/* USER CODE END RTOS_QUEUES */

//	/* Create the thread(s) */

//		// Cria��o das Threads de funcionamento por RTOS
//		
//			osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 512);
//			defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

//			osThreadDef(myTask02, StartTask02, osPriorityNormal, 0, 512);
//			myTask02Handle = osThreadCreate(osThread(myTask02), NULL);

//  		/* definition and creation of myTask03 */
//  		osThreadDef(myTask03, StartTask03, osPriorityNormal, 0, 512);
//  		myTask03Handle = osThreadCreate(osThread(myTask03), NULL);

//	/* USER CODE BEGIN RTOS_THREADS */
//	/* add threads, ... */
//	/* USER CODE END RTOS_THREADS */

//	/* Start scheduler */
//	
//		// Inicializa��o das Threads
//		
//			osKernelStart();

//	/* We should never get here as control is now taken by the scheduler */
//	
//	/* Infinite loop */
//	/* USER CODE BEGIN WHILE */
//	
//	while (1) {
//		
//		/* USER CODE END WHILE */

//		/* USER CODE BEGIN 3 */

//	}
//	/* USER CODE END 3 */
//}

///**
//	* @brief System Clock Configuration
//	* @retval None
//	*/
//void SystemClock_Config(void)
//{
//	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
//	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

//	/** Configure the main internal regulator output voltage
//	*/
//	__HAL_RCC_PWR_CLK_ENABLE();
//	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

//	/** Initializes the RCC Oscillators according to the specified parameters
//	* in the RCC_OscInitTypeDef structure.
//	*/
//	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
//	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
//	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
//	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
//	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
//	RCC_OscInitStruct.PLL.PLLM = 16;
//	RCC_OscInitStruct.PLL.PLLN = 336;
//	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
//	RCC_OscInitStruct.PLL.PLLQ = 7;
//	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//	{
//		Error_Handler();
//	}

//	/** Initializes the CPU, AHB and APB buses clocks
//	*/
//	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
//															|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
//	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
//	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

//	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
//	{
//		Error_Handler();
//	}
//}

///**
//	* @brief RTC Initialization Function
//	* @param None
//	* @retval None
//	*/
//static void MX_RTC_Init(void)
//{

//	/* USER CODE BEGIN RTC_Init 0 */

//	/* USER CODE END RTC_Init 0 */

//	RTC_TimeTypeDef sTime = {0};
//	RTC_DateTypeDef sDate = {0};

//	/* USER CODE BEGIN RTC_Init 1 */

//	/* USER CODE END RTC_Init 1 */

//	/** Initialize RTC Only
//	*/
//	hrtc.Instance = RTC;
//	hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
//	hrtc.Init.AsynchPrediv = 127;
//	hrtc.Init.SynchPrediv = 255;
//	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
//	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
//	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
//	if (HAL_RTC_Init(&hrtc) != HAL_OK)
//	{
//		Error_Handler();
//	}

//	/* USER CODE BEGIN Check_RTC_BKUP */

//	/* USER CODE END Check_RTC_BKUP */

//	/** Initialize RTC and set the Time and Date
//	*/
//	sTime.Hours = 0x0;
//	sTime.Minutes = 0x0;
//	sTime.Seconds = 0x0;
//	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
//	sTime.StoreOperation = RTC_STOREOPERATION_RESET;
//	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
//	{
//		Error_Handler();
//	}
//	sDate.WeekDay = RTC_WEEKDAY_MONDAY;
//	sDate.Month = RTC_MONTH_JANUARY;
//	sDate.Date = 0x1;
//	sDate.Year = 0x0;

//	if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
//	{
//		Error_Handler();
//	}

//	/** Enable the WakeUp
//	*/
//	if (HAL_RTCEx_SetWakeUpTimer(&hrtc, 0, RTC_WAKEUPCLOCK_RTCCLK_DIV16) != HAL_OK)
//	{
//		Error_Handler();
//	}
//	/* USER CODE BEGIN RTC_Init 2 */

//	/* USER CODE END RTC_Init 2 */

//}

///**
//	* @brief SPI1 Initialization Function
//	* @param None
//	* @retval None
//	*/
//static void MX_SPI1_Init(void)
//{

//	/* USER CODE BEGIN SPI1_Init 0 */

//	/* USER CODE END SPI1_Init 0 */

//	/* USER CODE BEGIN SPI1_Init 1 */

//	/* USER CODE END SPI1_Init 1 */
//	/* SPI1 parameter configuration*/
//	hspi1.Instance = SPI1;
//	hspi1.Init.Mode = SPI_MODE_SLAVE;
//	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
//	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
//	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
//	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
//	hspi1.Init.NSS = SPI_NSS_SOFT;
//	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
//	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
//	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
//	hspi1.Init.CRCPolynomial = 10;
//	if (HAL_SPI_Init(&hspi1) != HAL_OK)
//	{
//		Error_Handler();
//	}
//	/* USER CODE BEGIN SPI1_Init 2 */

//	/* USER CODE END SPI1_Init 2 */

//}

///**
//	* @brief SPI3 Initialization Function
//	* @param None
//	* @retval None
//	*/
//static void MX_SPI3_Init(void)
//{

//	/* USER CODE BEGIN SPI3_Init 0 */

//	/* USER CODE END SPI3_Init 0 */

//	/* USER CODE BEGIN SPI3_Init 1 */

//	/* USER CODE END SPI3_Init 1 */
//	/* SPI3 parameter configuration*/
//	hspi3.Instance = SPI3;
//	hspi3.Init.Mode = SPI_MODE_SLAVE;
//	hspi3.Init.Direction = SPI_DIRECTION_2LINES;
//	hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
//	hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
//	hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
//	hspi3.Init.NSS = SPI_NSS_SOFT;
//	hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
//	hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
//	hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
//	hspi3.Init.CRCPolynomial = 10;
//	if (HAL_SPI_Init(&hspi3) != HAL_OK)
//	{
//		Error_Handler();
//	}
//	/* USER CODE BEGIN SPI3_Init 2 */

//	/* USER CODE END SPI3_Init 2 */

//}

///**
//  * @brief USART1 Initialization Function
//  * @param None
//  * @retval None
//  */
//static void MX_USART1_UART_Init(void)
//{

//  /* USER CODE BEGIN USART1_Init 0 */

//  /* USER CODE END USART1_Init 0 */

//  /* USER CODE BEGIN USART1_Init 1 */

//  /* USER CODE END USART1_Init 1 */
//  huart1.Instance = USART1;
//  huart1.Init.BaudRate = 115200;
//  huart1.Init.WordLength = UART_WORDLENGTH_8B;
//  huart1.Init.StopBits = UART_STOPBITS_1;
//  huart1.Init.Parity = UART_PARITY_NONE;
//  huart1.Init.Mode = UART_MODE_TX_RX;
//  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
//  if (HAL_UART_Init(&huart1) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /* USER CODE BEGIN USART1_Init 2 */

//  /* USER CODE END USART1_Init 2 */

//}

///**
//	* @brief USART2 Initialization Function
//	* @param None
//	* @retval None
//	*/
//static void MX_USART2_UART_Init(void)
//{

//	/* USER CODE BEGIN USART2_Init 0 */

//	/* USER CODE END USART2_Init 0 */

//	/* USER CODE BEGIN USART2_Init 1 */

//	/* USER CODE END USART2_Init 1 */
//	huart2.Instance = USART2;
//	huart2.Init.BaudRate = 115200;
//	huart2.Init.WordLength = UART_WORDLENGTH_8B;
//	huart2.Init.StopBits = UART_STOPBITS_1;
//	huart2.Init.Parity = UART_PARITY_NONE;
//	huart2.Init.Mode = UART_MODE_TX_RX;
//	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
//	if (HAL_UART_Init(&huart2) != HAL_OK)
//	{
//		Error_Handler();
//	}
//	/* USER CODE BEGIN USART2_Init 2 */

//	/* USER CODE END USART2_Init 2 */

//}

///**
//	* @brief GPIO Initialization Function
//	* @param None
//	* @retval None
//	*/
//static void MX_GPIO_Init(void)
//{
//	GPIO_InitTypeDef GPIO_InitStruct = {0};
///* USER CODE BEGIN MX_GPIO_Init_1 */
///* USER CODE END MX_GPIO_Init_1 */

//	/* GPIO Ports Clock Enable */
//	__HAL_RCC_GPIOC_CLK_ENABLE();
//	__HAL_RCC_GPIOH_CLK_ENABLE();
//	__HAL_RCC_GPIOA_CLK_ENABLE();
//	__HAL_RCC_GPIOB_CLK_ENABLE();

//	/*Configure GPIO pin Output Level */
//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1|GPIO_PIN_7, GPIO_PIN_RESET);

//	/*Configure GPIO pin Output Level */
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_10, GPIO_PIN_RESET);

//	/*Configure GPIO pin Output Level */
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10|GPIO_PIN_4|GPIO_PIN_5, GPIO_PIN_RESET);

//	/*Configure GPIO pin : B1_Pin */
//	GPIO_InitStruct.Pin = B1_Pin;
//	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

//	/*Configure GPIO pins : PC1 PC7 */
//	GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_7;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

//	/*Configure GPIO pins : PA0 PA1 PA10 */
//	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_10;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

//	/*Configure GPIO pin : PB10 */
//	GPIO_InitStruct.Pin = GPIO_PIN_10;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Pull = GPIO_PULLUP;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//	/*Configure GPIO pins : PB4 PB5 */
//	GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

///* USER CODE BEGIN MX_GPIO_Init_2 */
///* USER CODE END MX_GPIO_Init_2 */
//}

///* USER CODE BEGIN 4 */

//	// Fun��o de Inicializa��o das fun��es para funcionamento da Placa PLC

//		static void initializePlc(void)
//		{
//			
//			BSP_Relay_Init(); // Inicializa��o da fun��o de controle das sa�das da placa
//			BSP_CurrentLimiter_Init(); // Inicializa��o da fun��o de controle das entrada da placa
//			
//		}

//		/**
//		* @brief  Handles the relay and current limiter functions and demos
//		* @param  None
//		* @retval None
//		*/
//	
//	// Fun��o de leitura e controle constante das entrada e sa�das da placa
//		
//		static void PLC_Handler(void)
//		{             

//				Input_Data = CURRENT_LIMITER_Handler(); // Leitura das entradas da placa
//					 
//				RELAY_Handler(Input_Data); // Controle das sa�das da placa a partir da leitura das entradas

//		}


//	/** 
//		* @brief  Receive input data from Current Limiter and checks for error status
//		* @param  None
//		* @retval Pointer to input channels state
//	*/
//		
//	
//	// Leitura da entrada da placa
//		
//		uint8_t* CURRENT_LIMITER_Handler(void)
//		{
//			
//			static uint8_t* clData = NULL; // Vari�vel de armazenamento de leitura de entrada
//			
//			clData = BSP_CURRENT_LIMITER_Read(); // Armazenamento da leitura de entradas da placa PLC atrav�s de hexadecimal
//			
//			Current_Limiter_Status = clData; // Transcri��o do status de leitura em caso de problemas em debug
//			
//			return (clData+1); // Retorno do valor das entradas detectadas
//			
//		}

//	/** 
//		* @brief  Select output control function and set outputs state
//		* @param  Pointer to input channels state
//		* @retval None
//	*/
//		
//	
//	// Controle de sa�das da placa
//	
//		void RELAY_Handler(uint8_t* iData)	
//	{		
//		
//		// Caso a Entrada 1 seja acionada para o sentido hor�rio ou antihor�rio, o motor � habilitado 
//		
//			if (*iData == 0x01 || *iData == 0x03) {
//			
//				habilitado = 1;
//				
//				// Se somente a entrada 1 � habilitada, o sentido do motor � colocado em hor�rio, intertravando o sentido antihor�rio
//				
//					if (*iData == 0x01) {
//					
//						horario = 1;
//						antihorario = 0;
//						
//					}
//					
//				// Se a entrada 1 e 2 s�o habilitadas, o sentido do motor � colocado em antihor�rio, intertravando o sentido hor�rio
//				
//					else {
//					
//						horario = 0;
//						antihorario = 1;
//					
//					}
//			
//			}
//		
//		// Caso n�o haja leitura, ou caso haja pulso sendo emitido em outras portas, o motor � ent�o desabilitado
//		
//			else {
//			
//				habilitado = 0;
//				
//				horario = 0;
//				
//				antihorario = 0;
//			
//			}
//		
//	}


//	/**
//	 * @brief  Systick callback implementation for systick interrupt
//	 * @param  None
//	 * @retval None
//	 */
//	

///* USER CODE END 4 */

///* USER CODE BEGIN Header_StartDefaultTask */
///**
//	* @brief  Function implementing the defaultTask thread.
//	* @param  argument: Not used
//	* @retval None
//	*/
///* USER CODE END Header_StartDefaultTask */
//void StartDefaultTask(void const * argument)
//{
//	/* USER CODE BEGIN 5 */
//	/* Infinite loop */
//	for(;;)
//	{
//		
//		// Loop de funcionamento da leitura de entradas e controle de sa�das
//		
//			PLC_Handler();
//			osDelay(100);
//			
//	}
//	/* USER CODE END 5 */
//}

///* USER CODE BEGIN Header_StartTask02 */
///**
//* @brief Function implementing the myTask02 thread.
//* @param argument: Not used
//* @retval None
//*/
///* USER CODE END Header_StartTask02 */
//void StartTask02(void const * argument)
//{
//	/* USER CODE BEGIN StartTask02 */
//	/* Infinite loop */
//	for(;;)
//	{
//		
//		// Se o estado habilitado provindo do reconhecimneto das entradas provindos do CLP for colocado em n�vel l�gico alto, a X-NUCLEO-IHM04A1 tem suas portas habilitadas para funcionamento do motor
//		
//		while (habilitado) {
//		
//			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET); // Habilita��o da Ponte H B da placa IHM04A1
//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);	// Habilita��o da Ponte H A da placa IHM04A1
//			
//			// Quando forem dados 8 passos, o ciclo de passo � reiniciado
//			
//				if (qpasso >= 8) {
//				
//					qpasso = 0;
//				
//				}
//				
//			// Quando o sentido hor�rio do motor for habilitado, a sequencia de habilita��o das chaves em sentido hor�rio � acionada
//		
//				if (horario) {
//					
//					
//					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, passo1[qpasso]);
//					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, passo2[qpasso]);
//					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, passo3[qpasso]);
//					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, passo4[qpasso]);
//				
//					qpasso++;
//					
//					osDelay(50);
//					
//				}
//				
//			// Quando o sentido antihor�rio do motor for habilitado, a sequencia de habilita��o das chaves em sentido antihor�rio � acionada
//			
//				else if (antihorario) {
//				
//					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, passo1[7 - qpasso]);
//					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, passo2[7 - qpasso]);
//					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, passo3[7 - qpasso]);
//					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, passo4[7 - qpasso]);
//					
//					qpasso++;			
//					
//					osDelay(50);
//				
//				}		
//		
//		}
//	 
//			// Caso o modo habilitado n�o esteja acionado, todas as pontes H e as chaves s�o desabilitadas, desligando o motor

//				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
//				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);	
//			
//				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 0);
//				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 0);
//				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, 0);
//				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0);	
//		
//	}
//	/* USER CODE END StartTask02 */
//}

///* USER CODE BEGIN Header_StartTask03 */
///**
//* @brief Function implementing the myTask03 thread.
//* @param argument: Not used
//* @retval None
//*/
///* USER CODE END Header_StartTask03 */
//void StartTask03(void const * argument)
//{
//  /* USER CODE BEGIN StartTask03 */
//  /* Infinite loop */
//  for(;;)
//  {

//		// Se o sinal habilitado estiver acionado, um sinal � enviado via Serial
//		
//			if (habilitado) {
//				
//				// Se o sentido de rota��o do motor for o hor�rio, um sinal � enviado via Serial
//				
//				if (horario) {
//				
//					HAL_UART_Transmit(&huart1, (uint8_t*)"M", 1, 100);
//				
//				}
//				
//				// Se o sentido de rota��o do motor for o antihor�rio, um sinal � enviado via Serial	
//				
//				else if (antihorario) {	
//				
//					HAL_UART_Transmit(&huart1, (uint8_t*)"W", 1, 100);
//				
//				}
//				
//			}
//			
//		// Caso o motor n�o esteja habilitado, um sinal � enviado via Serial
//		
//			else {
//			
//				HAL_UART_Transmit(&huart1, (uint8_t*)"D", 1, 100);
//			
//			}
//			
//		// Delay de transmiss�o
//		
//			osDelay(500);

//  }
//  /* USER CODE END StartTask03 */
//}

///**
//	* @brief  Period elapsed callback in non blocking mode
//	* @note   This function is called  when TIM2 interrupt took place, inside
//	* HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
//	* a global variable "uwTick" used as application time base.
//	* @param  htim : TIM handle
//	* @retval None
//	*/
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//	/* USER CODE BEGIN Callback 0 */

//	/* USER CODE END Callback 0 */
//	if (htim->Instance == TIM2) {
//		HAL_IncTick();
//	}
//	/* USER CODE BEGIN Callback 1 */

//	/* USER CODE END Callback 1 */
//}

///**
//	* @brief  This function is executed in case of error occurrence.
//	* @retval None
//	*/
//void Error_Handler(void)
//{
//	/* USER CODE BEGIN Error_Handler_Debug */
//	/* User can add his own implementation to report the HAL error return state */
//	__disable_irq();
//	while (1)
//	{
//	}
//	/* USER CODE END Error_Handler_Debug */
//}

//#ifdef  USE_FULL_ASSERT

///**
//	* @brief  Reports the name of the source file and the source line number
//	*         where the assert_param error has occurred.
//	* @param  file: pointer to the source file name
//	* @param  line: assert_param error line source number
//	* @retval None
//	*/
//void assert_failed(uint8_t *file, uint32_t line)
//{
//	/* USER CODE BEGIN 6 */
//	/* User can add his own implementation to report the file name and line number,
//		 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
//	/* USER CODE END 6 */
//}
//#endif /* USE_FULL_ASSERT */