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

#include "main.h"
#include "fatfs.h"
#include "HAL_STM_LCD18.h"
#include "stdio.h"

char buffile1[15]={'/','k','a','y','o','t','i','/','k','a',0,'.','b','i','n'};
char buffile2[15]={'/','k','a','y','o','t','i','/','k','a',0,'.','b','m','p'};
char stbuf[54];
char databuf[2000];
uint32_t valfil,heightbmp,widthbmp,tmp1,tmpr,tmpb,tmpg,tmpr1,tmpg1,tmpb1;
uint16_t ofset,nread,hb,wb,hbd,wbd;
uint16_t ch,cb;
uint8_t  cplane,ncolor,bsitu,chcl=6,ch1,vers=2,crgb=1,hs1,hs2,ws1,ws2;
uint8_t tt=0;
char buffer[256];


RTC_HandleTypeDef hrtc;

SD_HandleTypeDef hsd;

SPI_HandleTypeDef hspi3;

TIM_HandleTypeDef htim6;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
static void MX_SDIO_SD_Init(void);
static void MX_SPI3_Init(void);
static void MX_TIM6_Init(void);

  FATFS Myfat;
	FIL Myfiles,Mf;
	DIR MyDir;
	UINT byteCount,byteCount1;


int main(void)
{
  uint32_t zk;
	uint8_t i;
	char buff[16];
	
	
	
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_RTC_Init();
  MX_SDIO_SD_Init();
  MX_SPI3_Init();
  MX_TIM6_Init();
  MX_FATFS_Init();

 // HAL_TIM_Base_Start_IT (&htim6);
	
	TFT7735_Init();
	TFT7735_Position(180,0);

  


	if(f_mount(&Myfat, "", 1) != FR_OK)    Error_Handler();	

  while (1)
  {
   /*
		f_opendir(&MyDir,"/p");
		for(i=1;i<71;i+=2){
		buffile[4]=(i/10)+48;
    buffile[5]=(i%10)+48;			
		f_open(&Myfiles,buffile,FA_READ);	
 	  for(uint16_t f=0;f<160;f++)
		   {
		  	f_read(&Myfiles,buffer,256,&byteCount);
				TFT7735_CMD(LCD_REG_43); 
        TFT7735_WDATA((uint8_t*) &f, 2);
        TFT7735_CMD(LCD_REG_44);
				SPIx_WriteData((uint8_t*) buffer, 256);	 
			 }				 
	  f_close(&Myfiles);
		 }	 
	     f_closedir(&MyDir);
		
		*/
		f_opendir(&MyDir,"/kayoti");
		f_open(&Mf,buffile1, FA_CREATE_ALWAYS|FA_WRITE|FA_READ);
		f_open(&Myfiles,buffile2,FA_READ);
		if(buffile1[10]==7)  buffile1[10]++;
		else                 buffile1[10]=0;
		if(buffile2[10]==7)  buffile2[10]++;
		else                 buffile2[10]=0;
		//byteCount=0;
		f_read(&Myfiles,databuf,54,&byteCount);
    // Check Format For BMP
	  if(databuf[0]!=0x42 || databuf[1]!=0x4D) 
			{
			//" print this file is not abmp file"
		  //	return 0;
		  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
		  }
		// Data Of PICTURE	
		valfil=((databuf[5]<<24)|(databuf[4]<<16)|(databuf[3]<<8)|databuf[2]);// Volume Of File
		ofset=(databuf[11]<<8)|databuf[10];// Offset Data 
		heightbmp =(databuf[25]<<24)|(databuf[24]<<16)|(databuf[23]<<8)|databuf[22]; // Height of Picture
		widthbmp = (databuf[21]<<24)|(databuf[20]<<16)|(databuf[19]<<8)|databuf[18]; // Width  of Picture
		cplane = databuf[26]; // Color Plane must be 1
		ncolor = databuf[28]; // Bit Color Per Pixel 
    bsitu  = databuf[30]; // Byte situation 0->BI_RGB 1->RLE8 2->RLE4 3->BI_BITFEILD V2=RGB& V3=RGBA
		// DIB_HEADER
		//if(ofset>54)
		//	{ ofset-=54;
		//    f_read(&Myfiles,databuf,ofset,&byteCount);
		//	}
		// Nomber Of Byte Per Pixel	
		if (ncolor==32) nread=widthbmp*4;
		else if (ncolor==24) nread=widthbmp*3;
		else if (ncolor==16) nread=widthbmp*2;

		// Read Data
    f_close(&Myfiles);
		
    if(	heightbmp>TFT7735_LCD_PIXEL_HEIGHT) {hb= heightbmp%TFT7735_LCD_PIXEL_HEIGHT; hs1=1;}
		if(	widthbmp>TFT7735_LCD_PIXEL_WIDTH)   {wb= widthbmp %TFT7735_LCD_PIXEL_WIDTH;  ws1=1;}
    if( heightbmp>(widthbmp*5/4))     {hbd= heightbmp-(widthbmp*5/4); hs2=1;}
		else if( widthbmp>(heightbmp*5/4)) {wbd= widthbmp-(heightbmp*5/4); ws2=1;}
		else
		{
			 if( heightbmp>=widthbmp) {wbd= widthbmp- (heightbmp*4/5); ws2=2;}
			 else                     {hbd= heightbmp- (widthbmp*4/5); hs2=2;}
		}
		for(ch=0;ch<heightbmp;ch++)
		  {
		    f_open(&Myfiles,buffile2,FA_READ);
			  //byteCount=0;
				f_read(&Myfiles,databuf,ofset,&byteCount);	
				for(uint8_t ff=0;ff<(160-ch);ff++)
				{
				 f_read(&Myfiles,databuf,nread,&byteCount);
				}
			 for(cb=0;cb<widthbmp;cb++)
			 {	
				if(chcl==6)
				{
					switch (bsitu)
				 {
					case 0:
					case 1:	
					  tmpr= databuf[cb*4+2]>>3;
						tmpg= databuf[cb*4+1]>>2;
						tmpb= databuf[cb*4]>>3;
					break;
					
					case 2:
						
					break;
					
					case 3:
						if(ncolor==32)
						{
							if (vers==2)
							{
							 tmpr= databuf[cb*4+2]*databuf[cb*4+3]/256;
						   tmpr>>=3;
							 tmpg= databuf[cb*4+1]*databuf[cb*4+3]/256;
						   tmpg>>=2;
							 tmpb= databuf[cb*4]*databuf[cb*4+3]/256;
						   tmpb>>=3;
							}
							else
							{
							 tmp1 = (databuf[cb*4+3]<<24)|(databuf[cb*4+2]<<16)|(databuf[cb*4+1]<<8)|databuf[cb*4];
							 tmpr= tmp1&0x3FF00000;
							 tmpr>>=20;
							 tmpg= tmp1&0x000FFC00;
							 tmpg>>=10;
							 tmpb= tmp1&0x000003FF;
							 tmpr>>=5;
							 tmpr>>=4;
							 tmpr>>=5;
							}
						}
						else if	(ncolor==24)
								{
									tmpr= databuf[cb*4+2]>>3;
									tmpg= databuf[cb*4+1]>>2;
									tmpb= databuf[cb*4]>>3;
								}
							break;	
					}
				 if(crgb)
					 {
            buffer[cb*2]  = (tmpr<<3)|(tmpg>>3);
						buffer[cb*2+1]= (tmpg<<5)|tmpb;						 
					 }
					else
           {	
						buffer[cb*2]  =(tmpb<<3)|(tmpg>>3);
						buffer[cb*2+1]=(tmpg<<5)|tmpr;
					 }
					
				}
					else if(chcl==4)
					{
						switch (bsitu)
						{
							case 0:
							case 1:	
								tmpr= databuf[cb*4+2]>>4;
								tmpg= databuf[cb*4+1]>>4;
								tmpb= databuf[cb*4]>>4;
							  tmpr1= databuf[cb*8+2]>>4;
								tmpg1= databuf[cb*8+1]>>4;
								tmpb1= databuf[cb*8]>>4;
							  ch++;
					    break;
							
							case 2:
						
							break;
					
							case 3:
								if(ncolor==32)
								{
									if (vers==2)
									{
										tmpr= databuf[cb*4+2]*databuf[cb*4+3]/256;
										tmpr>>=4;
										tmpg= databuf[cb*4+1]*databuf[cb*4+3]/256;
										tmpg>>=4;
										tmpb= databuf[cb*4]*databuf[cb*4+3]/256;
										tmpb>>=4;
										tmpr1= databuf[cb*8+2]*databuf[cb*8+3]/256;
										tmpr1>>=4;
										tmpg1= databuf[cb*8+1]*databuf[cb*8+3]/256;
										tmpg1>>=4;
										tmpb1= databuf[cb*8]*databuf[cb*8+3]/256;
										tmpb1>>=4;
									}
									else
									{
										tmp1 = (databuf[cb*4+3]<<24)|(databuf[cb*4+2]<<16)|(databuf[cb*4+1]<<8)|databuf[cb*4];
										tmpr= tmp1&0x3FF00000;
										tmpr>>=20;
										tmpg= tmp1&0x000FFC00;
										tmpg>>=10;
										tmpb= tmp1&0x000003FF;
										tmpr>>=6;
										tmpr>>=6;
										tmpr>>=6;
										tmp1 = (databuf[cb*8+3]<<24)|(databuf[cb*8+2]<<16)|(databuf[cb*8+1]<<8)|databuf[cb*8];
										tmpr1= tmp1&0x3FF00000;
										tmpr1>>=20;
										tmpg1= tmp1&0x000FFC00;
										tmpg1>>=10;
										tmpb1= tmp1&0x000003FF;
										tmpr1>>=6;
										tmpr1>>=6;
										tmpr1>>=6;
									}
								}									
								else if	(ncolor==24)
								{
									tmpr=  databuf[cb*4+2]>>3;
									tmpg=  databuf[cb*4+1]>>2;
									tmpb=  databuf[cb*4]>>3;
									tmpr1= databuf[cb*8+2]>>3;
									tmpg1= databuf[cb*8+1]>>2;
									tmpb1= databuf[cb*8]>>3;
								}
							ch++;	
							break;	
						}
					if(crgb)
					 {
            buffer[cb*3]  = (tmpr<<4)|tmpg;
						buffer[cb*3+1]= (tmpb<<4)|tmpr1;
						buffer[cb*3+2]= (tmpg1<<4)|tmpb1; 
					 }
					else
           {	
						buffer[cb*3]  = (tmpb<<4)|tmpg;
						buffer[cb*3+1]= (tmpr<<4)|tmpb1;
						buffer[cb*3+2]= (tmpg1<<4)|tmpr1;
					 }	
					}
				}
		
				f_close(&Myfiles);	
    // f_write(&Mf,buffer,256,&byteCount1);				
		if(chcl==6)       f_write(&Mf,buffer,256,&byteCount1);
    else if(chcl==4)	f_write(&Mf,buffer,192,&byteCount1);			
		ch1=160-ch;	
		TFT7735_CMD(LCD_REG_43);			
    TFT7735_WDATA((uint8_t*) &ch, 2);
    TFT7735_CMD(LCD_REG_44);
		SPIx_WriteData((uint8_t*) buffer, 256);	
		}
 // f_close(&Myfiles);
	f_close(&Mf);	
 // f_closedir(&MyDir);
		
HAL_Delay(2000);
		
	TFT7735_CLS(TFT7735_GREEN);
	f_open(&Myfiles,"ham.bin\0",FA_READ);
  for(ch=0;ch<160;ch++)
  {
   f_read(&Myfiles,buffer,256,&byteCount);
	 TFT7735_CMD(LCD_REG_43);			
   TFT7735_WDATA((uint8_t*) &ch, 2);
   TFT7735_CMD(LCD_REG_44);
	 SPIx_WriteData((uint8_t*) buffer, 256);
	}		
	f_close(&Myfiles);
  HAL_Delay(2000);
	TFT7735_CLS(TFT7735_MAGENTA);
	HAL_Delay(6000);
		
	/*	
		//f_opendir(&MyDir,"/p");
		f_open(&Myfiles,"c100.bmp\0",FA_READ);
		f_read(&Myfiles,databuf,54,&byteCount);
    // Check Format For BMP
	  if(databuf[0]!=0x42 || databuf[1]!=0x4D) 
			{
			//" print this file is not abmp file"
		  //	return 0;
		  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
		  }
		// Data Of PICTURE	
		valfil=((databuf[5]<<24)|(databuf[4]<<16)|(databuf[3]<<8)|databuf[2]);// Volume Of File
		ofset=(databuf[11]<<8)|databuf[10];// Offset Data 
		heightbmp =(databuf[25]<<24)|(databuf[24]<<16)|(databuf[23]<<8)|databuf[22]; // Height of Picture
		widthbmp = (databuf[21]<<24)|(databuf[20]<<16)|(databuf[19]<<8)|databuf[18]; // Width  of Picture
		cplane = databuf[26]; // Color Plane must be 1
		ncolor = databuf[28]; // Bit Color Per Pixel 
    bsitu  = databuf[30]; // Byte situation 0->BI_RGB 1->RLE8 2->RLE4 3->BI_BITFEILD V2=RGB& V3=RGBA
		// DIB_HEADER
		if(ofset>54)
			{ ofset-=54;
		    f_read(&Myfiles,databuf,ofset,&byteCount);
			}
		// Nomber Of Byte Per Pixel	
		if(ncolor==32) nread=widthbmp*4;
		else if(ncolor==24) nread=widthbmp*3;
		else if(ncolor==16) nread=widthbmp*2;
		// Read Data	
		for(ch=0;ch<heightbmp;ch++)
		  {
		   f_read(&Myfiles,databuf,nread,&byteCount);
			 for(cb=0;cb<widthbmp;cb++)
			 {	
				if(chcl==6)
				{
					switch (bsitu)
				 {
					case 0:
					case 1:	
					  tmpr= databuf[cb*4+2]>>3;
						tmpg= databuf[cb*4+1]>>2;
						tmpb= databuf[cb*4]>>3;
					break;
					
					case 2:
						
					break;
					
					case 3:
						if(ncolor==32)
						{
							if (vers==2)
							{
							 tmpr= databuf[cb*4+2]*databuf[cb*4+3]/256;
						   tmpr>>=3;
							 tmpg= databuf[cb*4+1]*databuf[cb*4+3]/256;
						   tmpg>>=2;
							 tmpb= databuf[cb*4]*databuf[cb*4+3]/256;
						   tmpb>>=3;
							}
							else
							{
							 tmp1 = (databuf[cb*4+3]<<24)|(databuf[cb*4+2]<<16)|(databuf[cb*4+1]<<8)|databuf[cb*4];
							 tmpr= tmp1&0x3FF00000;
							 tmpr>>=20;
							 tmpg= tmp1&0x000FFC00;
							 tmpg>>=10;
							 tmpb= tmp1&0x000003FF;
							 tmpr>>=5;
							 tmpr>>=4;
							 tmpr>>=5;
							}
						}
						else if	(ncolor==24)
								{
									tmpr= databuf[cb*4+2]>>3;
									tmpg= databuf[cb*4+1]>>2;
									tmpb= databuf[cb*4]>>3;
								}
							break;	
					}
				 if(crgb)
					 {
            buffer[cb*2]  = (tmpr<<3)|(tmpg>>3);
						buffer[cb*2+1]= (tmpg<<5)|tmpb;						 
					 }
					else
           {	
						buffer[cb*2]  =(tmpb<<3)|(tmpg>>3);
						buffer[cb*2+1]=(tmpg<<5)|tmpr;
					 }
					
				}
					else if(chcl==4)
					{
						switch (bsitu)
						{
							case 0:
							case 1:	
								tmpr= databuf[cb*4+2]>>4;
								tmpg= databuf[cb*4+1]>>4;
								tmpb= databuf[cb*4]>>4;
							  tmpr1= databuf[cb*8+2]>>4;
								tmpg1= databuf[cb*8+1]>>4;
								tmpb1= databuf[cb*8]>>4;
							  ch++;
					    break;
							
							case 2:
						
							break;
					
							case 3:
								if(ncolor==32)
								{
									if (vers==2)
									{
										tmpr= databuf[cb*4+2]*databuf[cb*4+3]/256;
										tmpr>>=4;
										tmpg= databuf[cb*4+1]*databuf[cb*4+3]/256;
										tmpg>>=4;
										tmpb= databuf[cb*4]*databuf[cb*4+3]/256;
										tmpb>>=4;
										tmpr1= databuf[cb*8+2]*databuf[cb*8+3]/256;
										tmpr1>>=4;
										tmpg1= databuf[cb*8+1]*databuf[cb*8+3]/256;
										tmpg1>>=4;
										tmpb1= databuf[cb*8]*databuf[cb*8+3]/256;
										tmpb1>>=4;
									}
									else
									{
										tmp1 = (databuf[cb*4+3]<<24)|(databuf[cb*4+2]<<16)|(databuf[cb*4+1]<<8)|databuf[cb*4];
										tmpr= tmp1&0x3FF00000;
										tmpr>>=20;
										tmpg= tmp1&0x000FFC00;
										tmpg>>=10;
										tmpb= tmp1&0x000003FF;
										tmpr>>=6;
										tmpr>>=6;
										tmpr>>=6;
										tmp1 = (databuf[cb*8+3]<<24)|(databuf[cb*8+2]<<16)|(databuf[cb*8+1]<<8)|databuf[cb*8];
										tmpr1= tmp1&0x3FF00000;
										tmpr1>>=20;
										tmpg1= tmp1&0x000FFC00;
										tmpg1>>=10;
										tmpb1= tmp1&0x000003FF;
										tmpr1>>=6;
										tmpr1>>=6;
										tmpr1>>=6;
									}
								}									
								else if	(ncolor==24)
								{
									tmpr=  databuf[cb*4+2]>>3;
									tmpg=  databuf[cb*4+1]>>2;
									tmpb=  databuf[cb*4]>>3;
									tmpr1= databuf[cb*8+2]>>3;
									tmpg1= databuf[cb*8+1]>>2;
									tmpb1= databuf[cb*8]>>3;
								}
							ch++;	
							break;	
						}
					if(crgb)
					 {
            buffer[cb*3]  = (tmpr<<4)|tmpg;
						buffer[cb*3+1]= (tmpb<<4)|tmpr1;
						buffer[cb*3+2]= (tmpg1<<4)|tmpb1; 
					 }
					else
           {	
						buffer[cb*3]  = (tmpb<<4)|tmpg;
						buffer[cb*3+1]= (tmpr<<4)|tmpb1;
						buffer[cb*3+2]= (tmpg1<<4)|tmpr1;
					 }	
					}
				}
		ch1=160-ch;	
		TFT7735_CMD(LCD_REG_43);			
    TFT7735_WDATA((uint8_t*) &ch1, 2);
    TFT7735_CMD(LCD_REG_44);
		SPIx_WriteData((uint8_t*) buffer, 256);	
		}
  f_close(&Myfiles);
 // f_closedir(&MyDir);
	HAL_Delay(2000);
		
	//TFT7735_CLS(TFT7735_YELLOW);	
	f_opendir(&MyDir,"/p");
	f_open(&Myfiles,"/p/p01.bin\0",FA_READ);
  for(ch=0;ch<160;ch++)
  {
   f_read(&Myfiles,buffer,256,&byteCount);
	 TFT7735_CMD(LCD_REG_43);			
   TFT7735_WDATA((uint8_t*) &ch1, 2);
   TFT7735_CMD(LCD_REG_44);
	 SPIx_WriteData((uint8_t*) buffer, 256);
	}		
	f_close(&Myfiles);
  f_closedir(&MyDir);
	
	*/
HAL_Delay(2000);	
  
	
	
	}
	
	
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
	 PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV8;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_RTC_Init(void)
{
  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef DateToUpdate = {0};

  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  sTime.Hours = 11;
  sTime.Minutes = 59;
  sTime.Seconds = 50;

  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  DateToUpdate.WeekDay = RTC_WEEKDAY_WEDNESDAY;
  DateToUpdate.Month = RTC_MONTH_JANUARY;
  DateToUpdate.Date = 16;
  DateToUpdate.Year = 19;

  if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_SDIO_SD_Init(void)
{

  hsd.Instance = SDIO;
  hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
  hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
  hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
  hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
  hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd.Init.ClockDiv = 20;//SDIO_TRANSFER_CLK_DIV;
}

static void MX_SPI3_Init(void)
{
  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_MASTER;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi3.Init.NSS = SPI_NSS_SOFT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_TIM6_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 639;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 499;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  static uint8_t f=0,ff=0;
	static uint16_t t2=0;
	static uint32_t tmp1=0;
	
	
  if (htim->Instance == TIM7) {
    HAL_IncTick();
  }
  if (htim->Instance == TIM6) {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
		if(ff==0){
		if (tmp1<=t2) {
			tmp1+=350;
			f++;
			if(f>25)
			{
				ff=1;
			}
		}
		else {
				t2+=(60*f);
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
							t2-=(60*f);
					}
				}					
			
		TIM6->ARR = tmp1;
  }
}

void Error_Handler(void)
{

}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{ 

}
#endif 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
