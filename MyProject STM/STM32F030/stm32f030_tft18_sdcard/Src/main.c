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

char buffer[256]={0};
uint16_t tempe;
uint8_t xtemp=0;
char ttemp[3]={0};   
uint8_t buf;


volatile uint8_t FatFsCnt = 0;
volatile uint8_t Timer1, Timer2;
void SDTimer_Handler(void)
{  
  if(Timer1 > 0)
    Timer1--;
  
  if(Timer2 > 0)
    Timer2--;
}


SPI_HandleTypeDef hspi1;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);



int main(void)
{
	uint8_t i=0;
	
	
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_FATFS_Init();

	FATFS Myfat;
	FIL Myfiles;
	DIR MyDir;
	UINT byteCount;

		TFT7735_Init();
	  TFT7735_Position(180,1);
	

if(f_mount(&Myfat, "", 1) != FR_OK)    Error_Handler();			
	
  while (1)
  {
     TFT7735_Bitmap_sd(0,0,128,160,"p1.bin");
		HAL_Delay(500);
		TFT7735_Bitmap_sd(0,0,128,160,"p2.bin");
		HAL_Delay(500);
		TFT7735_Bitmap_sd(0,0,128,160,"p3.bin");
		HAL_Delay(500);
		TFT7735_Bitmap_sd(0,0,128,160,"/c1/c1.bin");
		HAL_Delay(500);
		TFT7735_Bitmap_sd(0,0,128,160,"/c2/c2.bin");
		HAL_Delay(500);
		TFT7735_Bitmap_sd(0,0,128,160,"c3.bin");
		HAL_Delay(500);
		TFT7735_Bitmap_sd(0,0,128,160,"c4.bin");
		HAL_Delay(500);
		TFT7735_Bitmap_sd(0,0,128,160,"c5.bin");
		HAL_Delay(500);
		TFT7735_Bitmap_sd(0,0,128,160,"kh1.bin");
		HAL_Delay(500);
		TFT7735_Bitmap_sd(0,0,128,160,"cat1.bin");
		
   	 i++;
/*
		if(i==1)  f_open(&Myfiles,"c3.bin\0",FA_READ);
		else if(i==2)  f_open(&Myfiles,"p2.bin\0",FA_READ);
		else if(i==3)  f_open(&Myfiles,"p3.bin\0",FA_READ);
		else if(i==4){
			f_opendir(&MyDir,"/c1");
			f_open(&Myfiles,"/c1/c1.bin",FA_READ);			
		}
		else if(i==5){
			f_opendir(&MyDir,"/c2");
			f_open(&Myfiles,"/c2/c2.bin",FA_READ);
		}
		else if(i==6)  f_open(&Myfiles,"c3.bin\0",FA_READ);
		else if(i==7)  f_open(&Myfiles,"c4.bin\0",FA_READ);
		else if(i==8)  f_open(&Myfiles,"c5.bin\0",FA_READ);
		else if(i==9)  f_open(&Myfiles,"kh1.bin\0",FA_READ);
		else if(i==10)  f_open(&Myfiles,"sd1.bin\0",FA_READ);
		else if(i==11)  f_open(&Myfiles,"cat1.bin\0",FA_READ);
    else i=0;

    for(uint16_t f=0;f<160;f++)
		   {
				 f_read(&Myfiles,buffer,256,&byteCount);
        TFT7735_CMD(LCD_REG_43); 
        TFT7735_WDATA((uint8_t*) &f, 2);
        TFT7735_CMD(LCD_REG_44);
				SPIx_WriteData((uint8_t*) buffer, 256);
			}
  f_closedir(&MyDir);			 
	f_close(&Myfiles);
*/
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

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

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
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_1);
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

}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
