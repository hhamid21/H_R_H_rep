/* | -------------------------------- HRH Libraries -------------------------
 * | @Created On 07,21,2019
 * | @File Name : at2401_i2c
 * | @Brief : STM32 HAL Driver for All of AT24 eeprom series
 * | @Copyright : 
 * |
 * | @Author :  Heidari Hamidreza
 * |
**/


#include "stm32f1xx_hal.h"
#include "AT2401_I2C.h"

static uint8_t at24_error;

//**********************************************************************************************************
// ************************************   WRITE CHAR FUNCTION   ********************************************
//**********************************************************************************************************
void at2401_HAL_WriteChar(I2C_HandleTypeDef *hi2c,uint8_t MEmAddress, uint8_t wdata)//FOR AT24C01,AT24C02
{
  uint8_t tmp[1];
	/*
	 * Address of eeprom(AT24C01,AT24C02) is 1 to end for example at24c01 address is 1 to 128 or 1 to 256
	 * give MemAddress for the one location you want to Write it
	 */
	tmp[0]=wdata;
 if(HAL_I2C_Mem_Write(hi2c,Standard_Address,(uint16_t)MEmAddress,I2C_MEMADD_SIZE_8BIT,tmp,1,100)!= HAL_OK)
    {
     AT24_Error_Handler(10);
    }
 HAL_Delay(1);	// optinal (if you have inough delay then you should delete this line)
}
//***********************************************************************************************************
void at2401_HAL_WritePChar(I2C_HandleTypeDef *hi2c,uint8_t MEmAddress, uint8_t *wdata)//FOR AT24C01,AT24C02
{
	/*
	 * Address of eeprom(AT24C01,AT24C02) is 1 to end for example at24c01 address is 1 to 128 or 1 to 256
	 * give MemAddress for the one location you want to Write it
	 */
 if(HAL_I2C_Mem_Write(hi2c,Standard_Address,(uint16_t)MEmAddress,I2C_MEMADD_SIZE_8BIT,wdata,1,100)!= HAL_OK)
    {
     AT24_Error_Handler(11);
    }
 HAL_Delay(1);	// optinal (if you have inough delay then you should delete this line)
}
/***********************************************************************************************************
  * @brief       This function handles Reading one Byte (Char) on the specific Address.
  * @param  hi2c         : Pointer to a I2C_HandleTypeDef structure that contains
  *                        the configuration information for the specified I2C.
  * @param	MemAddress   : Internal memory address (WHERE YOU WANNA WRITE TO)
  * @param	rdata       : char parameter you need to read of eeprom
  */
//**********************************************************************************************************
//*************************************   READ CHAR FUNCTION   *********************************************
//**********************************************************************************************************
uint8_t at2401_HAL_ReadChar(I2C_HandleTypeDef *hi2c,uint8_t MEmAddress) //FOR AT24C01,02
{
	uint8_t rdata[1];
	/*
	 * Address of eeprom(AT24C01,AT24C02) is 1 to end for example at24c01 address is 1 to 128 or 1 to 256
	 * give MemAddress for the one location you want to Write it
	 */
if(HAL_I2C_Mem_Read(hi2c,Standard_Address,(uint16_t)MEmAddress,I2C_MEMADD_SIZE_8BIT,rdata,1,100)!= HAL_OK)
    {
     AT24_Error_Handler(20);
    }
	return rdata[0];	
}

//**********************************************************************************************************
//*********************************   READ POINTER CHAR FUNCTION   *****************************************
//**********************************************************************************************************
void at2401_HAL_ReadPChar(I2C_HandleTypeDef *hi2c,uint8_t MEmAddress, uint8_t *rdata) //FOR AT24C01,02
{
	/*
	 * Address of eeprom(AT24C01,AT24C02) is 1 to end for example at24c01 address is 1 to 128 or 1 to 256
	 * give MemAddress for the one location you want to Write it
	 */
if(HAL_I2C_Mem_Read(hi2c,Standard_Address,(uint16_t)MEmAddress,I2C_MEMADD_SIZE_8BIT,rdata,1,100)!= HAL_OK)
    {
     AT24_Error_Handler(21);
    }
}

/**********************************************************************************************************
  * @brief      This function handles Writing Array of Bytes on the specific Address .
  * 					  
  * @param  hi2c           : Pointer to a I2C_HandleTypeDef structure that contains
  *                          the configuration information for the specified I2C.
  * @param	MemAddress     : Internal memory address that you need to save data
  * @param	wdata	         : Pointer to data buffer
  * @param  TxBufferSize   : Amount of data you need to Write
  * @retval
  */
//**********************************************************************************************************
//*************************************   WRITE BYTES FUNCTION   *******************************************
//**********************************************************************************************************
void at2401_HAL_WriteBytes(I2C_HandleTypeDef *hi2c,uint8_t MEmAddress, uint8_t *wdata,uint8_t TxBufferSize)//FOR AT24C01,02
{
	uint8_t comp;
	/*
	 * this program dont need Device Address 
	 * give MemAddress for the location you want to write to for AT24C01 is 1 to 128 and for AT24C02 is 1 to 256
	 * give Data buffer so it can write Data on this location
	 */
comp=8-(MEmAddress%8);
if(TxBufferSize<comp){	
  if(HAL_I2C_Mem_Write(hi2c,Standard_Address,MEmAddress,I2C_MEMADD_SIZE_8BIT,wdata,TxBufferSize,100)!= HAL_OK)
    {
     AT24_Error_Handler(12);
    }
	}
else{
	if(HAL_I2C_Mem_Write(hi2c,Standard_Address,MEmAddress,I2C_MEMADD_SIZE_8BIT,wdata,comp,100)!= HAL_OK)
    {
     AT24_Error_Handler(13);
    }
	TxBufferSize-=comp;
  MEmAddress+=comp;
  wdata+=comp;
	HAL_Delay(1);	// optinal (if you have enough delay then you can delete this line)	
 while(TxBufferSize>8){
   if(HAL_I2C_Mem_Write(hi2c,Standard_Address,MEmAddress,I2C_MEMADD_SIZE_8BIT,wdata,8,100)!= HAL_OK)
    {
     AT24_Error_Handler(14);
    }
	 TxBufferSize-=8;
   MEmAddress+=8;
   wdata+=8;
   HAL_Delay(1);	// optinal (if you have enough delay then you can delete this line)		
	for(uint16_t ff=0;ff<50000;ff++);	
 }
if(HAL_I2C_Mem_Write(hi2c,Standard_Address,MEmAddress,I2C_MEMADD_SIZE_8BIT,wdata,TxBufferSize,100)!= HAL_OK)
    {
     AT24_Error_Handler(15);
    } 
		
	}		
}

/**********************************************************************************************************
  * @brief      This function handles Reading Array of Bytes on the specific Address .
  * 					  
  * @param  hi2c           : Pointer to a I2C_HandleTypeDef structure that contains
  *                          the configuration information for the specified I2C.
  * @param	MemAddress     : Internal memory address that you need to Read data
  * @param	rdata         : Pointer to data buffer
  * @param  TxBufferSize   : Amount of data that you need to read
  * @retval
  */
//**********************************************************************************************************
//**************************************   READ BYTES FUNCTION   *******************************************
//**********************************************************************************************************
void at2401_HAL_ReadBytes(I2C_HandleTypeDef *hi2c,uint8_t MEmAddress, uint8_t *rdata,uint8_t TxBufferSize) //FOR AT24C01,02
{
/*
	 * Address of eeprom(AT24C01,AT24C02) is 1 to end for example at24c01 address is 1 to 128 or 1 to 256
	 * give MemAddress for the location you want to Read it
	 */
if(HAL_I2C_Mem_Read(hi2c,Standard_Address,MEmAddress,I2C_MEMADD_SIZE_8BIT,rdata,TxBufferSize,50)!= HAL_OK)
    {
     AT24_Error_Handler(22);
    }
}
 /**********************************************************************************************************
   * @brief             This function handles Erase Full chip.
   * @param  hi2c       Pointer to a I2C_HandleTypeDef structure that contains
   * @retval
   */
//**********************************************************************************************************
//****************************************   ERASE FUNCTION   **********************************************
//**********************************************************************************************************
void at2401_HAL_EraseMemFull(I2C_HandleTypeDef *hi2c) //FOR AT24C01
{
	/*
	 * this function for erase all of memory in fact all of memory change to 0x00
	 */
	uint8_t EraseBuf[8] ={0};
	for (uint8_t i=0;i<16;i++)
	{
		at2401_HAL_WriteBytes(hi2c,i*8,EraseBuf,8);
	}
}
//**********************************************************************************************************
void at2402_HAL_EraseMemFull(I2C_HandleTypeDef *hi2c) //FOR AT24C02
{
	/*
	 * this function for erase all of memory in fact all of memory change to 0x00
	 */
	uint8_t EraseBuf[8] ={0};
	for (uint8_t i=0;i<32;i++)
	{
		at2401_HAL_WriteBytes(hi2c,i*8,EraseBuf,8);
	}
}
//**********************************************************************************************************
void AT24_Error_Handler(uint8_t er)
{
  at24_error=er;
}
//**********************************************************************************************************

