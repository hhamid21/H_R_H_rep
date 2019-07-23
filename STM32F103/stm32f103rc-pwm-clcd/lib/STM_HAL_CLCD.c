/**
  ************************************************************************************************
  * @file    STM_HAL_CLCD.c
  * @author  Heidari Hamid R.
  * @brief   Header file of CLCD with HAL function. 
  *************************************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by H_R_H 
  *
  **************************************************************************************************
  */
	
	#include "stm32f1xx_hal.h"
	#include "stm32f1xx_hal_rcc.h"
	
static uint8_t d_val;

//************************************   DELAY FUNCTION   ******************************************	

void  Delay_us_Handler(uint16_t d_us)
{
	d_us*=d_val;
	while(d_us!=0) d_us--;
}

void  Delay_ms_Handler(uint32_t d_ms)
{
	d_ms=d_ms*d_val*1000;
	while(d_ms!=0) d_ms--;
}

//***********************************   CONFIGURATION PORT   ****************************************

void  Clcd_Config_Port(Clcd_pin_TypeDef* prt){

		GPIO_InitTypeDef GPIO_IStruct = {0};
	
	if (prt->port == GPIOA)	      __HAL_RCC_GPIOA_CLK_ENABLE();		
	else if(prt->port == GPIOB)		__HAL_RCC_GPIOB_CLK_ENABLE();	
	else if(prt->port == GPIOC)   __HAL_RCC_GPIOC_CLK_ENABLE(); 	
	else if(prt->port == GPIOD) 	__HAL_RCC_GPIOD_CLK_ENABLE();	
	else if(prt->port == GPIOE) 	__HAL_RCC_GPIOE_CLK_ENABLE();	
	
//	else if(prt->port == GPIOF)		__HAL_RCC_GPIOF_CLK_ENABLE();	
//	else if(prt->port == GPIOG)		__HAL_RCC_GPIOG_CLK_ENABLE();

	GPIO_IStruct.Pin = prt->pin;
  GPIO_IStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_IStruct.Pull = GPIO_NOPULL;
  GPIO_IStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(prt->port, &GPIO_IStruct);
}

//***********************************   WRITE DATA TO LCD   *****************************************

void Lcd_Port_Data(unsigned char lcd_in)
{
#if CLCD==8 
  CLCD_D0_LOW;
	CLCD_D1_LOW;
	CLCD_D2_LOW;
	CLCD_D3_LOW;
	if(lcd_in & 0x01)  CLCD_D0_HIGH;
	if(lcd_in & 0x02)  CLCD_D1_HIGH;
	if(lcd_in & 0x04)  CLCD_D2_HIGH;
	if(lcd_in & 0x08)  CLCD_D3_HIGH;
#endif
  CLCD_D4_LOW
  CLCD_D5_LOW
  CLCD_D6_LOW
  CLCD_D7_LOW
	if(lcd_in & 0x10)  CLCD_D4_HIGH;
	if(lcd_in & 0x20)  CLCD_D5_HIGH;
	if(lcd_in & 0x40)  CLCD_D6_HIGH;
	if(lcd_in & 0x80)  CLCD_D7_HIGH;
}

//***********************************   WRITE DATA TO LCD   *****************************************

void Clcd_Init()
{
	///////////// Reset process from datasheet /////////
	Lcd4_Cmd(0x28);
	Lcd4_Cmd(0x0C);      // Display ON
	// Lcd4_Cmd(0x0E);   // Display ON - Cursor ON
	// Lcd4_Cmd(0x0F);   // Display ON - Cursor ON - Blink ON
	Lcd_Delay(50);
	
}



