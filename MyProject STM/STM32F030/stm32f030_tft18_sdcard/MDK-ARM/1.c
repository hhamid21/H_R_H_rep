/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include "HAL_STM_LCD18.h"
 
char buffer[128];
char picx;
volatile uint8_t FatFsCnt = 0;
volatile uint8_t Timer1, Timer2;

void SDTimer_Handler(void)
{  
  if(Timer1 > 0)
    Timer1--;
  
  if(Timer2 > 0)
    Timer2--;
}

ADC_HandleTypeDef hadc;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim14;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
//static void MX_ADC_Init(void);
//static void MX_RTC_Init(void);
static void MX_SPI1_Init(void);
//static void MX_TIM3_Init(void);
//static void MX_TIM14_Init(void);
//static void MX_NVIC_Init(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  uint16_t de=32,tempe;
	uint8_t i=0;
	
	//uint8_t x=0,y=0;
	//uint16_t cv[8]={0x0000,0x001F,0xF800,0x07E0,0x07FF,0xF81F,0xFFE0,0xFFFF};
	//char* buf=" salam dash hamid hahahahahahahahahahahhahahahahah;";
	
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  //MX_ADC_Init();
  //MX_RTC_Init();
  MX_SPI1_Init();
  //MX_TIM3_Init();
  //MX_TIM14_Init();
  MX_FATFS_Init();

  //MX_NVIC_Init();

	FATFS Myfat;
	FIL Myfiles;
	uint8_t Mydata[11]="HELLO WORLD";
	UINT byteCount;
		//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
	/*
	if(f_mount(&Myfat,USERPath,1)== FR_OK)
	{
		f_open(&Myfiles,"test2.txt\0",FA_WRITE|FA_CREATE_ALWAYS);
		f_write(&Myfiles,Mydata,11,&byteCount);
		f_close(&Myfiles);
		f_open(&Myfiles,"test.txt\0",FA_READ);
		f_read(&Myfiles,buffer,20,&byteCount);
		f_close(&Myfiles);
		
	}
*/
	//HAL_TIM_Base_Start_IT(&htim3);
	TFT7735_Init();
	TFT7735_Position(180);
	
	/*
if(f_mount(&Myfat, "", 0) != FR_OK)
    Error_Handler();
  
  // Open file to write 
  if(f_open(&Myfiles, "first.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE) != FR_OK)
    Error_Handler();
  
  // Check free space 
 // if(f_getfree("", &fre_clust, &pfs) != FR_OK)
  //  _Error_Handler(__FILE__, __LINE__);
  
//  total = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
//  free = (uint32_t)(fre_clust * pfs->csize * 0.5);   
    
  // Free space is less than 1kb 
 // if(free < 1)
 //   _Error_Handler(__FILE__, __LINE__);  
  
  // Writing text 
  f_puts("STM32 SD Card I/O Example via SPI\n", &Myfiles);  
  f_puts("Save the world!!!", &Myfiles);
  
  // Close file 
  if(f_close(&Myfiles) != FR_OK)
    Error_Handler();  

// Open file to read 
  if(f_open(&fil, "first.txt", FA_READ) != FR_OK)
   _Error_Handler(__FILE__, __LINE__);
  
  while(f_gets(buffer, sizeof(buffer), &fil))
  {
    //printf("%s", buffer);
		tft_Puts(&LCD,buffer,CYAN, VIOLET);
  }
  
  // Close file 
  if(f_close(&fil) != FR_OK)
    _Error_Handler(__FILE__, __LINE__);     
  
  // Unmount SDCARD 
  if(f_mount(NULL, "", 1) != FR_OK)
    _Error_Handler(__FILE__, __LINE__);  
  
  		*/
	
  while (1)
  {
    /*
	 HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_0|GPIO_PIN_1);
	 HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4);	
	 */
		//if(x>127)  {x=0; de++;}
		//else x++;
		
		//TFT7735_DrawHLine(cv[de],y,x,128);
		//TFT7735_DrawVLine(cv[de],x,y, 160);
		//de++;
		//if(de>8) de=0;
		de++;
		if(de>120) {de=32; }
		//TFT7735_CLS(TFT7735_YELLOW);
		//TFT7735_CLS(TFT7735_YELLOW);
		//TFT7735_Putchar(de,12, 15, TFT7735_BLUE, TFT7735_YELLOW);
		//TFT7735_Putc(de, TFT7735_BLUE, TFT7735_YELLOW);
		//TFT7735_WriteString(8, 40, buf, TFT7735_GREEN, TFT7735_WHITE);
		//TFT7735_Line(0, 0, de,160-de,TFT7735_BLUE); 
		//TFT7735_Rect(10,8,110,50, TFT7735_BLUE , 1);
		//TFT7735_Circle(64, 100, 50,TFT7735_BLUE, 1);
		//TFT7735_Ecllipse(64, 100,50,30,TFT7735_BLUE, 1);
		//TFT7735_Triangle(10, 60, 120, 100, 50, 130,TFT7735_BLUE, 1);
		 picx=0;
   if(f_mount(&Myfat,USERPath,1)== FR_OK)
		{
			TFT7735_SetDisplayWindow(0,0,127,159);
		if(i==1)  f_open(&Myfiles,"p1.bin\0",FA_READ);
		//else if(i==2)  f_open(&Myfiles,"b2.bin\0",FA_READ);
		//else if(i==3)  f_open(&Myfiles,"b3.bin\0",FA_READ);
		//else if(i==4)  f_open(&Myfiles,"b4.bin\0",FA_READ);
		//else if(i==5)  f_open(&Myfiles,"b5.bin\0",FA_READ);
		//else if(i==6)  f_open(&Myfiles,"b6.bin\0",FA_READ);
		//else if(i==7)  f_open(&Myfiles,"b7.bin\0",FA_READ);
		//else if(i==8)  f_open(&Myfiles,"b8.bin\0",FA_READ);
		//else if(i==9)  f_open(&Myfiles,"b9.bin\0",FA_READ);
		//else if(i==10)  f_open(&Myfiles,"b10.bin\0",FA_READ);
		//else if(i==11)  f_open(&Myfiles,"b11.bin\0",FA_READ);
    //else i=0;

    for(uint16_t f=0;f<160;f++)
		   {
			 f_read(&Myfiles,buffer,128,&byteCount); 
        // f_gets(buffer, 128, &Myfiles);       
				 for(uint8_t ff=0;ff<64;ff++)
          {				 
				   tempe=(buffer[2*ff+1]<<8)|buffer[2*ff];
					 TFT7735_WDATA((uint8_t*) &tempe,2);	
          }
			 }			
	f_close(&Myfiles);
	}
			
			
			HAL_Delay(1000);
		}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14
                              |RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None

static void MX_NVIC_Init(void)
{

  HAL_NVIC_SetPriority(RTC_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(RTC_IRQn);

  HAL_NVIC_SetPriority(ADC1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(ADC1_IRQn);

  HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM3_IRQn);

  HAL_NVIC_SetPriority(TIM14_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM14_IRQn);

  HAL_NVIC_SetPriority(SPI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(SPI1_IRQn);
}
  */
/**
  * @brief ADC Initialization Function
  * @param None
  * @retval None

static void MX_ADC_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }

  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  sConfig.Channel = ADC_CHANNEL_VREFINT;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

}
  */
/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None

static void MX_RTC_Init(void)
{

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  RTC_AlarmTypeDef sAlarm = {0};

  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }

  sAlarm.AlarmTime.Hours = 0x0;
  sAlarm.AlarmTime.Minutes = 0x0;
  sAlarm.AlarmTime.Seconds = 0x0;
  sAlarm.AlarmTime.SubSeconds = 0x0;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 0x1;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
}
  */
/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
*/
static void MX_SPI1_Init(void)
{
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
}
  
	
/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None

static void MX_TIM3_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 4799;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 9999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 4999;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_TIM_MspPostInit(&htim3);

}
  */
/**
  * @brief TIM14 Initialization Function
  * @param None
  * @retval None

static void MX_TIM14_Init(void)
{
  htim14.Instance = TIM14;
  htim14.Init.Prescaler = 4799;
  htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim14.Init.Period = 99;
  htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
  {
    Error_Handler();
  }
}
  */
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
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM16 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
 // static uint8_t f=0,ff=0;
//	static uint16_t t2=0,coun;
//	static uint32_t tmp1=0;
	
  if (htim->Instance == TIM16) {
    HAL_IncTick();
		if(FatFsCnt >= 10)
			{
				FatFsCnt = 0;
				SDTimer_Handler();
			}
  }
/*	
  if (htim->Instance == TIM3) {
   coun++;
	 if(coun>99) coun=0;
	 if(coun<50) t2=coun;
   else   t2=99-coun;
   //TIM2->CCR2=ti2*2;
	 TIM3->CCR3=t2*2;
}
  if (htim->Instance == TIM14) {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
		if(ff==0){
		if (tmp1<=t2) {
			tmp1+=350;
			f++;
			if(f>20)
			{
				ff=1;
			}
		}
		else {
				t2+=(50*f);
		}
	}
		else {
			    if (tmp1>=t2) {
			    tmp1-=350;
		    	f--;
					if(f<2)
					 {
						ff=0;
			     }
					}
					else {
							t2-=(50*f);
					}
				}					
		TIM3->ARR = tmp1;
   }
	 */
}

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
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
