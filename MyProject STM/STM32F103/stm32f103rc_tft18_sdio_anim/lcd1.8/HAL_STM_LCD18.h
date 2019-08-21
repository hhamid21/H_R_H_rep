/**
  ******************************************************************************
  * @file    TFT7735.h
  * @author  MCD Application Team
  * @version V1.1.1
  * @date    24-November-2014
  * @brief   This file contains all the functions prototypes for the TFT7735.c
  *          driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HAL_STM_LCD18_H
#define __HAL_STM_LCD18_H


/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

	 
/** 
  * @brief  TFT7735 Size  
  */  
#define  TFT7735_LCD_PIXEL_WIDTH    ((uint16_t)128)
#define  TFT7735_LCD_PIXEL_HEIGHT   ((uint16_t)160)

/**
  * @brief  SD Control Lines management
  */  
#define SD_CS_LOW()       HAL_GPIO_WritePin(SD_CS_GPIO_PORT, SD_CS_PIN, GPIO_PIN_RESET)
#define SD_CS_HIGH()      HAL_GPIO_WritePin(SD_CS_GPIO_PORT, SD_CS_PIN, GPIO_PIN_SET)
    
/**
  * @brief  LCD Control Lines management
  */
#define LCD_CS_LOW()         HAL_GPIO_WritePin(LCD_CS_GPIO_PORT, LCD_CS_PIN, GPIO_PIN_RESET)
#define LCD_CS_HIGH()        HAL_GPIO_WritePin(LCD_CS_GPIO_PORT, LCD_CS_PIN, GPIO_PIN_SET)
#define LCD_DC_LOW()         HAL_GPIO_WritePin(LCD_DC_GPIO_PORT, LCD_DC_PIN, GPIO_PIN_RESET)
#define LCD_DC_HIGH()        HAL_GPIO_WritePin(LCD_DC_GPIO_PORT, LCD_DC_PIN, GPIO_PIN_SET)
#define LCD_RESET_LOW()      HAL_GPIO_WritePin(LCD_RESET_GPIO_PORT, LCD_RESET_PIN, GPIO_PIN_RESET)
#define LCD_RESET_HIGH()     HAL_GPIO_WritePin(LCD_RESET_GPIO_PORT, LCD_RESET_PIN, GPIO_PIN_SET)

/**
  * @brief  SD Control Interface pins (shield D4)
  */
#define SD_CS_PIN                                 GPIO_PIN_2
#define SD_CS_GPIO_PORT                           GPIOC
#define SD_CS_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOC_CLK_ENABLE()
#define SD_CS_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOC_CLK_DISABLE()

/**
  * @brief  LCD Control Interface pins (shield D10)
  */ 
#define LCD_CS_PIN                                 GPIO_PIN_1
#define LCD_CS_GPIO_PORT                           GPIOC
#define LCD_CS_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOC_CLK_ENABLE()
#define LCD_CS_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOC_CLK_DISABLE()

/**
  * @brief  LCD Data/Command Interface pins
  */
#define LCD_DC_PIN                                 GPIO_PIN_0
#define LCD_DC_GPIO_PORT                           GPIOC
#define LCD_DC_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOC_CLK_ENABLE()
#define LCD_DC_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOC_CLK_DISABLE()

/**
  * @brief  LCD Reset Interface pins
  */
#define LCD_RESET_PIN                                 GPIO_PIN_8
#define LCD_RESET_GPIO_PORT                           GPIOB
#define LCD_RESET_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOB_CLK_ENABLE()
#define LCD_RESET_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOB_CLK_DISABLE()

/** 
  * @brief  TFT7735 Registers  
  */ 
#define  LCD_REG_0               0x00 /* No Operation: NOP */
#define  LCD_REG_1               0x01 /* Software reset: SWRESET */
//#define  LCD_REG_4               0x04 /* Read Display ID: RDDID */
//#define  LCD_REG_9               0x09 /* Read Display Statu: RDDST */
//#define  LCD_REG_10              0x0A /* Read Display Power: RDDPM */
//#define  LCD_REG_11              0x0B /* Read Display: RDDMADCTL */
//#define  LCD_REG_12              0x0C /* Read Display Pixel: RDDCOLMOD */  
//#define  LCD_REG_13              0x0D /* Read Display Image: RDDIM */
//#define  LCD_REG_14              0x0E /* Read Display Signal: RDDSM */                           
#define  LCD_REG_16              0x10 /* Sleep in & booster off: SLPIN */ 
#define  LCD_REG_17              0x11 /* Sleep out & booster on: SLPOUT */
#define  LCD_REG_18              0x12 /* Partial mode on: PTLON */ 
#define  LCD_REG_19              0x13 /* Partial off (Normal): NORON */
#define  LCD_REG_32              0x20 /* Display inversion off: INVOFF */
#define  LCD_REG_33              0x21 /* Display inversion on: INVON */
#define  LCD_REG_38              0x26 /* Gamma curve select: GAMSET */
#define  LCD_REG_40              0x28 /* Display off: DISPOFF */
#define  LCD_REG_41              0x29 /* Display on: DISPON */
#define  LCD_REG_42              0x2A /* Column address set: CASET */ 
#define  LCD_REG_43              0x2B /* Row address set: RASET */
#define  LCD_REG_44              0x2C /* Memory write: RAMWR */  
#define  LCD_REG_45              0x2D /* LUT for 4k,65k,262k color: RGBSET */
#define  LCD_REG_46              0x2E /* Memory read: RAMRD*/
#define  LCD_REG_48              0x30 /* Partial start/end address set: PTLAR */ 
#define  LCD_REG_52              0x34 /* Tearing effect line off: TEOFF */ 
#define  LCD_REG_53              0x35 /* Tearing effect mode set & on: TEON */ 
#define  LCD_REG_54              0x36 /* Memory data access control: MADCTL */ 
#define  LCD_REG_56              0x38 /* Idle mode off: IDMOFF */ 
#define  LCD_REG_57              0x39 /* Idle mode on: IDMON */ 
#define  LCD_REG_58              0x3A /* Interface pixel format: COLMOD */
#define  LCD_REG_177             0xB1 /* In normal mode (Full colors): FRMCTR1 */
#define  LCD_REG_178             0xB2 /* In Idle mode (8-colors): FRMCTR2 */   
#define  LCD_REG_179             0xB3 /* In partial mode + Full colors: FRMCTR3 */ 
#define  LCD_REG_180             0xB4 /* Display inversion control: INVCTR */
#define  LCD_REG_192             0xC0 /* Power control setting: PWCTR1 */ 
#define  LCD_REG_193             0xC1 /* Power control setting: PWCTR2 */ 
#define  LCD_REG_194             0xC2 /* In normal mode (Full colors): PWCTR3 */
#define  LCD_REG_195             0xC3 /* In Idle mode (8-colors): PWCTR4 */ 
#define  LCD_REG_196             0xC4 /* In partial mode + Full colors: PWCTR5 */ 
#define  LCD_REG_197             0xC5 /* VCOM control 1: VMCTR1 */ 
#define  LCD_REG_199             0xC7 /* Set VCOM offset control: VMOFCTR */ 
#define  LCD_REG_209             0xD1 /* Set LCM version code: WRID2 */ 
#define  LCD_REG_210             0xD2 /* Customer Project code: WRID3 */ 
#define  LCD_REG_217             0xD9 /* NVM control status: NVCTR1 */
//#define  LCD_REG_218             0xDA /* Read ID1: RDID1 */ 
//#define  LCD_REG_219             0xDB /* Read ID2: RDID2 */ 
//#define  LCD_REG_220             0xDC /* Read ID3: RDID3 */ 
//#define  LCD_REG_222             0xDE /* NVM Read Command: NVCTR2 */ 
#define  LCD_REG_223             0xDF /* NVM Write Command: NVCTR3 */
#define  LCD_REG_224             0xE0 /* Set Gamma adjustment (+ polarity): GAMCTRP1 */                          
#define  LCD_REG_225             0xE1 /* Set Gamma adjustment (- polarity): GAMCTRN1 */ 


// Color definitions
#define	TFT7735_BLACK   0x0000
#define	TFT7735_BLUE    0x001F
#define	TFT7735_RED     0xF800
#define	TFT7735_GREEN   0x07E0
#define TFT7735_CYAN    0x07FF
#define TFT7735_MAGENTA 0xF81F
#define TFT7735_YELLOW  0xFFE0
#define TFT7735_WHITE   0xFFFF
#define TFT7735_COLOR565(r, g, b) (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))

/** @defgroup TFT7735_1.8"_128x160_SPI_LCD_Exported_Functions
  * @{
  */ 
/* LCD Init & IO functions */	
void TFT7735_Init(void);
void TFT7735_IO_Init(void);
void LCD_Delay_ms(uint32_t Delay);
void LCD_Delay_us(uint32_t Delay);

/* SPI Send $ Recive functions */
static void SPIx_Init(void);
static void SPIx_Write(uint8_t Value);
void SPIx_WriteData(uint8_t *Pdata, uint16_t DataLegnth);
static void SPIx_Error (void);

/* LCD Send CMD and DATA */
void TFT7735_CMD(uint8_t Reg);
void TFT7735_WREG(uint8_t LCDReg, uint8_t LCDRegValue);
void TFT7735_WDATA(uint8_t *pData, uint16_t Size);

/* LCD Command Set Function */
void TFT7735_DisplayOn(void);
void TFT7735_DisplayOff(void);
void TFTT7735_InvertColors(uint8_t invert);
void TFT7735_Position(uint16_t Pos,uint8_t RGB);
void TFT7735_SetCursor(uint16_t Xpos, uint16_t Ypos);
void TFT7735_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);

/* LCD Graphical Command  */
void TFT7735_CLS(uint16_t brgb);
void TFT7735_WritePixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGBCode);
void TFT7735_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint16_t RGBCode);
void TFT7735_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint16_t RGBCode);
void TFT7735_Line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t frgb);
void TFT7735_Triangle(uint8_t x1, uint8_t x2, uint8_t x3, uint8_t y1, uint8_t y2, uint8_t y3,  uint16_t frgb, char fill);
void TFT7735_Rect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t frgb , char fill);
void TFT7735_Circle(uint8_t X, uint8_t Y, uint8_t R, uint16_t frgb, char fill);
void TFT7735_Ecllipse(uint8_t Xpos, uint8_t Ypos, uint8_t XRadius, uint8_t YRadius, uint16_t frgb, char fill);

/* LCD Write Text Command  */
void TFT7735_Gotoxy(uint8_t x, uint8_t y);
void TFT7735_Putc(char character, uint16_t frgb, uint32_t brgb);
void TFT7735_Putchar(char character,uint8_t Xpos, uint8_t Ypos, uint16_t frgb, uint32_t brgb);
void TFT7735_WriteString(uint16_t x, uint16_t y, const char* str, uint16_t frgb, uint16_t brgb);
void TFT7735_WString(char* str, uint16_t frgb, uint16_t brgb);

/* LCD Show Picture  */
void TFT7735_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp);
void TFT7735_Bitmap_sd(uint16_t X1pos, uint16_t Y1pos,uint16_t X2pos, uint16_t Y2pos,char* Name);

#endif /* __HAL_STM_LCD18_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
