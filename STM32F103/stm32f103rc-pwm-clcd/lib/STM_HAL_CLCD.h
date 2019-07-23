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

#define  CLCD        8    // 8 FOR 8BIT DATA PORT & 4 FOR 4BIT DATA PORT
#define  CLCD_RW     1    // 1 FOR USE RW PIN  &  0 FOR UNUSE RW PIN THATS MEAN RW CONNECT TO GND

#define CLCD_RS_LOW			lcd->CS.port->BRR  = lcd->CS.pin
#define CLCD_RS_HIGH			lcd->CS.port->BSRR = lcd->CS.pin
#if CLCD_RW==1
#define CLCD_RW_LOW			lcd->DC.port->BRR  = lcd->DC.pin
#define CLCD_RW_HIGH			lcd->DC.port->BSRR = lcd->DC.pin
#endif
#define CLCD_EN_LOW			lcd->DIN.port->BRR  = lcd->DIN.pin
#define CLCD_EN_HIGH			lcd->DIN.port->BSRR = lcd->DIN.pin
#if CLCD==8
#define CLCD_D0_LOW			lcd->RST.port->BRR  = lcd->RST.pin
#define CLCD_D0_HIGH			lcd->RST.port->BSRR = lcd->RST.pin
#define CLCD_D1_LOW			lcd->SCK.port->BRR  = lcd->SCK.pin
#define CLCD_D1_HIGH			lcd->SCK.port->BSRR = lcd->SCK.pin
#define CLCD_D2_LOW			lcd->SCK.port->BRR  = lcd->SCK.pin
#define CLCD_D2_HIGH			lcd->SCK.port->BSRR = lcd->SCK.pin
#define CLCD_D3_LOW			lcd->SCK.port->BRR  = lcd->SCK.pin
#define CLCD_D3_HIGH			lcd->SCK.port->BSRR = lcd->SCK.pin
#else
#define CLCD_D4_LOW			lcd->SCK.port->BRR  = lcd->SCK.pin
#define CLCD_D4_HIGH			lcd->SCK.port->BSRR = lcd->SCK.pin
#define CLCD_D5_LOW			lcd->SCK.port->BRR  = lcd->SCK.pin
#define CLCD_D5_HIGH			lcd->SCK.port->BSRR = lcd->SCK.pin
#define CLCD_D6_LOW			lcd->SCK.port->BRR  = lcd->SCK.pin
#define CLCD_D6_HIGH			lcd->SCK.port->BSRR = lcd->SCK.pin
#define CLCD_D7_LOW			lcd->SCK.port->BRR  = lcd->SCK.pin
#define CLCD_D7_HIGH			lcd->SCK.port->BSRR = lcd->SCK.pin
#endif


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
#if CLCD==8 	
	Clcd_pin_TypeDef 	D0;
	Clcd_pin_TypeDef 	D1;
	Clcd_pin_TypeDef	D2;
	Clcd_pin_TypeDef  D3;
#endif
	Clcd_pin_TypeDef  D4;
	Clcd_pin_TypeDef  D5;
	Clcd_pin_TypeDef  D6;
	Clcd_pin_TypeDef  D7;
	Clcd_pin_TypeDef  D8;
} Clcd_TypeDef;




#endif