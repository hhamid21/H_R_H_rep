/**
  ************************************************************************************************
  * @file    STM_HAL_CLCD.h
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

#ifndef _STM_HAL_CLCD_H_
#define _STM_HAL_CLCD_H_

#include "stm32f1xx_hal.h"

#define  CLCD_PORT   4    // 8 FOR 8BIT DATA PORT & 4 FOR 4BIT DATA PORT
#define  CLCD_RW     0    // 1 FOR USE RW PIN  &  0 FOR UNUSE RW PIN THATS MEAN RW CONNECT TO GND

#define CLCD_RS_LOW			  clcd->RS.port->BRR  = clcd->RS.pin
#define CLCD_RS_HIGH			clcd->RS.port->BSRR = clcd->RS.pin
#if CLCD_RW==1
#define CLCD_RW_LOW			  clcd->RW.port->BRR  = clcd->DC.pin
#define CLCD_RW_HIGH			clcd->RW.port->BSRR = clcd->DC.pin
#endif
#define CLCD_EN_LOW			  clcd->EN.port->BRR  = clcd->EN.pin
#define CLCD_EN_HIGH			clcd->EN.port->BSRR = clcd->EN.pin
#if CLCD_PORT==8
#define CLCD_D0_LOW			  clcd->D0.port->BRR  = dlcd->D0.pin
#define CLCD_D0_HIGH			clcd->D0.port->BSRR = dlcd->D0.pin
#define CLCD_D1_LOW			  clcd->D1.port->BRR  = clcd->D1.pin
#define CLCD_D1_HIGH			clcd->D1.port->BSRR = clcd->D1.pin
#define CLCD_D2_LOW			  clcd->D2.port->BRR  = clcd->D2.pin
#define CLCD_D2_HIGH			clcd->D2.port->BSRR = clcd->D2.pin
#define CLCD_D3_LOW			  clcd->D3.port->BRR  = clcd->D3.pin
#define CLCD_D3_HIGH			clcd->D3.port->BSRR = clcd->D3.pin
#endif
#define CLCD_D4_LOW			  clcd->D4.port->BRR  = clcd->D4.pin
#define CLCD_D4_HIGH			clcd->D4.port->BSRR = clcd->D4.pin
#define CLCD_D5_LOW			  clcd->D5.port->BRR  = clcd->D5.pin
#define CLCD_D5_HIGH			clcd->D5.port->BSRR = clcd->D5.pin
#define CLCD_D6_LOW			  clcd->D6.port->BRR  = clcd->D6.pin
#define CLCD_D6_HIGH			clcd->D6.port->BSRR = clcd->D6.pin
#define CLCD_D7_LOW			  clcd->D7.port->BRR  = clcd->D7.pin
#define CLCD_D7_HIGH			clcd->D7.port->BSRR = clcd->D7.pin



typedef struct {
	GPIO_TypeDef* 	port;
	uint16_t 		pin;
} Clcd_pin_TypeDef;

typedef struct {
	Clcd_pin_TypeDef 	RS;
#if CLCD_RW==1
	Clcd_pin_TypeDef 	RW;
#endif	
	Clcd_pin_TypeDef 	EN;
#if CLCD_PORT==8 	
	Clcd_pin_TypeDef 	D0;
	Clcd_pin_TypeDef 	D1;
	Clcd_pin_TypeDef	D2;
	Clcd_pin_TypeDef  D3;
#endif
	Clcd_pin_TypeDef  D4;
	Clcd_pin_TypeDef  D5;
	Clcd_pin_TypeDef  D6;
	Clcd_pin_TypeDef  D7;
} Clcd_TypeDef;


void Delay_us(uint32_t d_us);
void Clcd_Config_Port(Clcd_pin_TypeDef* prt);
void Clcd_Port_Data(Clcd_TypeDef *clcd,uint8_t lcd_in);
void Clcd_Cmd(Clcd_TypeDef *clcd,uint8_t lcd_in);
void Clcd_Init(Clcd_TypeDef *clcd);
void Clcd_Clear(Clcd_TypeDef *clcd);
void Clcd_home(Clcd_TypeDef *clcd);
void Clcd_Gotoxy(Clcd_TypeDef *clcd,uint8_t lcd_inx, uint8_t lcd_iny);
void Clcd_Cursor_Mode(Clcd_TypeDef *clcd,uint8_t lcd_in);
void Clcd_Blink_Mode(Clcd_TypeDef *clcd,uint8_t lcd_in);
void Clcd_Shift_Mode(Clcd_TypeDef *clcd,uint8_t lcd_in);
void Clcd_Move_Mode(Clcd_TypeDef *clcd,uint8_t lcd_in);
void Clcd_Write_Char(Clcd_TypeDef *clcd,uint8_t lcd_in);
void Clcd_Write_String(Clcd_TypeDef *clcd,char *lcd_in);
void Clcd_Write_Decimal(Clcd_TypeDef *clcd,uint8_t lcd_inx,uint8_t lcd_iny,int32_t lcd_in);


#endif

