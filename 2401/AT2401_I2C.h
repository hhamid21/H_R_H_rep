/*
 * this file is library for AT24C01
 *
 *  Created on: 07,22,2019
 *  Author: Heydari-Hamidreza
 */

#ifndef _AT2401_I2C_H_
#define _AT2401_I2C_H_

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_i2c.h"


#define  Standard_Address      0xA0


// ************************************   WRITE CHAR FUNCTION   ********************************************
void at2401_HAL_WriteChar(I2C_HandleTypeDef *hi2c,uint8_t MEmAddress, uint8_t wdata);   //FOR AT24C01,02
void at2401_HAL_WritePChar(I2C_HandleTypeDef *hi2c,uint8_t MEmAddress, uint8_t *wdata); //FOR AT24C01,02

// *************************************   READ CHAR FUNCTION   ********************************************
uint8_t at2401_HAL_ReadChar(I2C_HandleTypeDef *hi2c,uint8_t MEmAddress);               //FOR AT24C01,02
void at2401_HAL_ReadPChar(I2C_HandleTypeDef *hi2c,uint8_t MEmAddress, uint8_t *rdata); //FOR AT24C01,02


//*************************************   WRITE BYTES FUNCTION   *******************************************
void at2401_HAL_WriteBytes(I2C_HandleTypeDef *hi2c,uint8_t MEmAddress, uint8_t *wdata,uint8_t TxBufferSize);    //FOR AT24C01,02


//**************************************   READ BYTES FUNCTION   *******************************************
void at2401_HAL_ReadBytes(I2C_HandleTypeDef *hi2c,uint8_t MEmAddress, uint8_t *rdata,uint8_t TxBufferSize);    //FOR AT24C01,02

//****************************************   ERASE FUNCTION   **********************************************
void at2401_HAL_EraseMemFull(I2C_HandleTypeDef *hi2c);  //FOR AT24C01
void at2402_HAL_EraseMemFull(I2C_HandleTypeDef *hi2c);  //FOR AT24C02
//**********************************************************************************************************
void AT24_Error_Handler(uint8_t er);
#endif /* AT24_I2C_H_ */
//**********************************************************************************************************
