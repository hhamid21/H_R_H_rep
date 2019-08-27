/**
  ******************************************************************************
  * @file    TFT7735.c
  * @author  HRH
  * @version V1
  * @date    24-November-2019
  * @brief   This file includes the driver for TFT7735 LCD mounted on the Adafruit
  *          1.8" TFT LCD shield (reference ID 802).
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "HAL_STM_LCD18.h"
//#include "stm32f0xx_hal.h"
//#include "stm32f0xx_hal_spi.h"

static uint8_t ArrayRGB[320] = {0};
SPI_HandleTypeDef lcd_spi;

/********************************* LINK LCD ***********************************/
/**
  * @brief  Initialize the LCD
  */
void TFT7735_IO_Init(void)
{
  GPIO_InitTypeDef  gpiostruct;

  /* LCD_CS_GPIO and LCD_DC_GPIO Periph clock enable */
  LCD_CS_GPIO_CLK_ENABLE();
  LCD_DC_GPIO_CLK_ENABLE();
	LCD_RESET_GPIO_CLK_ENABLE();
  
  /* Configure LCD_CS_PIN pin: LCD Card CS pin */
  gpiostruct.Pin = LCD_CS_PIN;
  gpiostruct.Mode = GPIO_MODE_OUTPUT_PP;
  gpiostruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SD_CS_GPIO_PORT, &gpiostruct);
      
  /* Configure LCD_DC_PIN pin: LCD Card DC pin */
  gpiostruct.Pin = LCD_DC_PIN;
  HAL_GPIO_Init(LCD_DC_GPIO_PORT, &gpiostruct);
	
	/* Configure LCD_RESET_PIN pin: LCD Card RESET pin */
  gpiostruct.Pin = LCD_RESET_PIN;
  HAL_GPIO_Init(LCD_DC_GPIO_PORT, &gpiostruct);

  /* LCD chip select high */
	LCD_CS_HIGH();
	
  /* LCD SPI Config */
  SPIx_Init();
}

/**
  * @brief  Initialize SPI HAL.
  */
static void SPIx_Init(void)
{
  if(HAL_SPI_GetState(&lcd_spi) == HAL_SPI_STATE_RESET)
  {
    /* SPI Config */
    lcd_spi.Instance = SPI1;
      /* SPI baudrate is set to 8 MHz maximum (PCLK2/SPI_BaudRatePrescaler = 64/8 = 8 MHz) 
       to verify these constraints:
          - ST7735 LCD SPI interface max baudrate is 15MHz for write and 6.66MHz for read
            Since the provided driver doesn't use read capability from LCD, only constraint 
            on write baudrate is considered.
          - SD card SPI interface max baudrate is 25MHz for write/read
          - PCLK2 max frequency is 32 MHz 
       */
  lcd_spi.Init.Mode = SPI_MODE_MASTER;
  lcd_spi.Init.Direction = SPI_DIRECTION_2LINES;
  lcd_spi.Init.DataSize = SPI_DATASIZE_8BIT;
  lcd_spi.Init.CLKPolarity = SPI_POLARITY_LOW;
  lcd_spi.Init.CLKPhase = SPI_PHASE_1EDGE;
  lcd_spi.Init.NSS = SPI_NSS_SOFT;
  lcd_spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  lcd_spi.Init.FirstBit = SPI_FIRSTBIT_MSB;
  lcd_spi.Init.TIMode = SPI_TIMODE_DISABLE;
  lcd_spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  lcd_spi.Init.CRCPolynomial = 7;
  lcd_spi.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  lcd_spi.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&lcd_spi) != HAL_OK)
  {
    SPIx_Error ();
  }

  }
}
/********************************* Writ to LCD ***********************************/
/**
  * @brief  SPI Write an amount of data to device
  * @param  Value: value to be written
  * @param  DataLength: number of bytes to write
  */
static void SPIx_WriteData(uint8_t *DataIn, uint16_t DataLength)
{
  HAL_StatusTypeDef status = HAL_OK;
  /* Reset LCD control line CS */
  LCD_CS_LOW();
  /* Set LCD data/command line DC to High */
  LCD_DC_HIGH();
	
  status = HAL_SPI_Transmit(&lcd_spi, DataIn, DataLength, 10);
  
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    SPIx_Error();
  }
	LCD_CS_HIGH();
	
}

/**
  * @brief  SPI Write a byte to device
  * @param  Value: value to be written
  */
static void SPIx_Write(uint8_t Value)
{
  HAL_StatusTypeDef status = HAL_OK;
  //uint8_t data;

  //status = HAL_SPI_TransmitReceive(&lcd_spi, (uint8_t*) &Value, &data, 1, 10);
  status = HAL_SPI_Transmit(&lcd_spi, (uint8_t*) &Value, 1, 10);
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    SPIx_Error();
  }
}

/**
  * @brief  SPI error treatment function
  */
static void SPIx_Error (void)
{
  /* De-initialize the SPI communication BUS */
  HAL_SPI_DeInit(&lcd_spi);

  /* Re-Initiaize the SPI communication BUS */
  SPIx_Init();
}
/********************************* CMD OR DATA to LCD ***********************************/
/**
  * @brief  Write command to select the LCD register.
  * @param  LCDReg: Address of the selected register.
  * @retval None
  */
void TFT7735_CMD(uint8_t LCDReg)
{
	/* Reset LCD control line CS */
  LCD_CS_LOW();
  /* Set LCD data/command line DC to Low */
  LCD_DC_LOW();
  /* Send Command */
  SPIx_Write(LCDReg);
  /* Deselect : Chip Select high */
  LCD_CS_HIGH();
}

/**
* @brief  Write register value.
* @param  pData Pointer on the register value
* @param  Size Size of byte to transmit to the register
*/
void TFT7735_WDATA(uint8_t *pData)
{
  uint16_t counter = 0,Size;
  
	Size=sizeof(pData);
  /* Reset LCD control line CS */
  LCD_CS_LOW();
  /* Set LCD data/command line DC to High */
  LCD_DC_HIGH();
  if (Size == 1)
   {
    /* Only 1 byte to be sent to LCD - general interface can be used */
    /* Send Data */
    SPIx_Write(*pData);
   }
  else
  {
    /* Several data should be sent in a raw */
    /* Direct SPI accesses for optimization */
    for (counter = Size; counter != 0; counter--)
   {
      while(((lcd_spi.Instance->SR) & SPI_FLAG_TXE) != SPI_FLAG_TXE)
   {
 }
      /* Need to invert bytes for LCD*/
      *((__IO uint8_t*)&lcd_spi.Instance->DR) = *(pData+1);

      while(((lcd_spi.Instance->SR) & SPI_FLAG_TXE) != SPI_FLAG_TXE)
{
}
      *((__IO uint8_t*)&lcd_spi.Instance->DR) = *pData;
      counter--;
      pData += 2;
  }  
  
    /* Wait until the bus is ready before releasing Chip select */ 
    while(((lcd_spi.Instance->SR) & SPI_FLAG_BSY) != RESET)
  {
  } 
  } 
/* Deselect : Chip Select high */
  LCD_CS_HIGH();
}
/*********************************  LCD Delay Function ***********************************/
/**
  * @brief  Wait for loop in ms.
  * @param  Delay in ms.
  * @retval None
  */
void LCD_Delay_ms(uint32_t Delay)
{
  HAL_Delay(Delay);
}

void LCD_Delay_us(uint32_t Delay)
{
  while(Delay!=0) Delay--;
}
/********************************* Initialiization LCD ***********************************/
/**
  * @brief  Initialize the TFT7735 LCD Component.
  * @param  None
  * @retval None
  */
void TFT7735_Init(void)
{    
  uint8_t data = 0;
  
	LCD_CS_LOW();
	LCD_Delay_us(200);
	LCD_RESET_LOW();
	LCD_Delay_ms(2);
	LCD_RESET_HIGH();
	LCD_Delay_us(200);
  LCD_CS_HIGH();
	
  /* Initialize TFT7735 low level bus layer -----------------------------------*/
  TFT7735_IO_Init();
  /* Out of sleep mode, 0 args, no delay */
  TFT7735_WREG(LCD_REG_17, 0x00); 
  /* Frame rate ctrl - normal mode, 3 args:Rate = fosc/(1x2+40) * (LINE+2C+2D)*/
  TFT7735_CMD(LCD_REG_177);
  data = 0x01;
  TFT7735_WDATA(&data, 1);
  data = 0x2C;
  TFT7735_WDATA(&data, 1);
  data = 0x2D;
  TFT7735_WDATA(&data, 1);
  /* Frame rate control - idle mode, 3 args:Rate = fosc/(1x2+40) * (LINE+2C+2D) */    
  TFT7735_WREG(LCD_REG_178, 0x01);
  TFT7735_WREG(LCD_REG_178, 0x2C);
  TFT7735_WREG(LCD_REG_178, 0x2D);
  /* Frame rate ctrl - partial mode, 6 args: Dot inversion mode, Line inversion mode */ 
  TFT7735_WREG(LCD_REG_179, 0x01);
  TFT7735_WREG(LCD_REG_179, 0x2C);
  TFT7735_WREG(LCD_REG_179, 0x2D);
  TFT7735_WREG(LCD_REG_179, 0x01);
  TFT7735_WREG(LCD_REG_179, 0x2C);
  TFT7735_WREG(LCD_REG_179, 0x2D);
  /* Display inversion ctrl, 1 arg, no delay: No inversion */
  TFT7735_WREG(LCD_REG_180, 0x07);
  /* Power control, 3 args, no delay: -4.6V , AUTO mode */
  TFT7735_WREG(LCD_REG_192, 0xA2);
  TFT7735_WREG(LCD_REG_192, 0x02);
  TFT7735_WREG(LCD_REG_192, 0x84);
  /* Power control, 1 arg, no delay: VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD */
  TFT7735_WREG(LCD_REG_193, 0xC5);
  /* Power control, 2 args, no delay: Opamp current small, Boost frequency */ 
  TFT7735_WREG(LCD_REG_194, 0x0A);
  TFT7735_WREG(LCD_REG_194, 0x00);
  /* Power control, 2 args, no delay: BCLK/2, Opamp current small & Medium low */  
  TFT7735_WREG(LCD_REG_195, 0x8A);
  TFT7735_WREG(LCD_REG_195, 0x2A);
  /* Power control, 2 args, no delay */
  TFT7735_WREG(LCD_REG_196, 0x8A);
  TFT7735_WREG(LCD_REG_196, 0xEE);
  /* Power control, 1 arg, no delay */
  TFT7735_WREG(LCD_REG_197, 0x0E);
  /* Don't invert display, no args, no delay */
  TFT7735_CMD(LCD_REG_32);
  /* Set color mode, 1 arg, no delay: 16-bit color */
  TFT7735_WREG(LCD_REG_58, 0x05);
  /* Column addr set, 4 args, no delay: XSTART = 0, XEND = 127 */
  TFT7735_CMD(LCD_REG_42);
  data = 0x00;
  TFT7735_WDATA(&data, 1);
  TFT7735_WDATA(&data, 1);
  TFT7735_WDATA(&data, 1);
  data = 0x7F;
  TFT7735_WDATA(&data, 1);
  /* Row addr set, 4 args, no delay: YSTART = 0, YEND = 159 */
  TFT7735_CMD(LCD_REG_43);
  data = 0x00;
  TFT7735_WDATA(&data, 1);
  TFT7735_WDATA(&data, 1);
  TFT7735_WDATA(&data, 1);
  data = 0x9F;
  TFT7735_WDATA(&data, 1);
  /* Magical unicorn dust, 16 args, no delay */
  TFT7735_WREG(LCD_REG_224, 0x02); 
  TFT7735_WREG(LCD_REG_224, 0x1c);  
  TFT7735_WREG(LCD_REG_224, 0x07); 
  TFT7735_WREG(LCD_REG_224, 0x12);
  TFT7735_WREG(LCD_REG_224, 0x37);  
  TFT7735_WREG(LCD_REG_224, 0x32);  
  TFT7735_WREG(LCD_REG_224, 0x29);  
  TFT7735_WREG(LCD_REG_224, 0x2d);
  TFT7735_WREG(LCD_REG_224, 0x29);  
  TFT7735_WREG(LCD_REG_224, 0x25);  
  TFT7735_WREG(LCD_REG_224, 0x2B);  
  TFT7735_WREG(LCD_REG_224, 0x39);  
  TFT7735_WREG(LCD_REG_224, 0x00);  
  TFT7735_WREG(LCD_REG_224, 0x01);  
  TFT7735_WREG(LCD_REG_224, 0x03);  
  TFT7735_WREG(LCD_REG_224, 0x10);
  /* Sparkles and rainbows, 16 args, no delay */
  TFT7735_WREG(LCD_REG_225, 0x03);
  TFT7735_WREG(LCD_REG_225, 0x1d);  
  TFT7735_WREG(LCD_REG_225, 0x07);  
  TFT7735_WREG(LCD_REG_225, 0x06);
  TFT7735_WREG(LCD_REG_225, 0x2E);  
  TFT7735_WREG(LCD_REG_225, 0x2C);  
  TFT7735_WREG(LCD_REG_225, 0x29);  
  TFT7735_WREG(LCD_REG_225, 0x2D);
  TFT7735_WREG(LCD_REG_225, 0x2E);  
  TFT7735_WREG(LCD_REG_225, 0x2E);  
  TFT7735_WREG(LCD_REG_225, 0x37);  
  TFT7735_WREG(LCD_REG_225, 0x3F);  
  TFT7735_WREG(LCD_REG_225, 0x00);  
  TFT7735_WREG(LCD_REG_225, 0x00);  
  TFT7735_WREG(LCD_REG_225, 0x02);  
  TFT7735_WREG(LCD_REG_225, 0x10);
  /* Normal display on, no args, no delay */
  TFT7735_WREG(LCD_REG_19, 0x00);
  /* Main screen turn on, no delay */
  TFT7735_WREG(LCD_REG_41, 0x00);
  /* Memory access control: MY = 1, MX = 1, MV = 0, ML = 0 */
  TFT7735_WREG(LCD_REG_54, 0xC0);
}

/**
  * @brief  Writes to the selected LCD register.
  * @param  LCDReg: Address of the selected register.
  * @param  LCDRegValue: value to write to the selected register.
  * @retval None
  */
void TFT7735_WREG(uint8_t LCDReg, uint8_t LCDRegValue)
{
  TFT7735_CMD(LCDReg);
  TFT7735_WDATA(&LCDRegValue, 1);
}

/**
  * @brief  Enables the Display.
  * @param  None
  * @retval None
  */
void TFT7735_DisplayOn(void)
{
  uint8_t data = 0;
  TFT7735_CMD(LCD_REG_19);
  LCD_Delay_ms(10);
  TFT7735_CMD(LCD_REG_41);
  LCD_Delay_ms(10);
  TFT7735_CMD(LCD_REG_54);
  data = 0xC0;
  TFT7735_WDATA(&data, 1);
}

/**
  * @brief  Disables the Display.
  * @param  None
  * @retval None
  */
void TFT7735_DisplayOff(void)
{
  uint8_t data = 0;
  TFT7735_CMD(LCD_REG_19);
  LCD_Delay_ms(10);
  TFT7735_CMD(LCD_REG_40);
  LCD_Delay_ms(10);
  TFT7735_CMD(LCD_REG_54);
  data = 0xC0;
  TFT7735_WDATA(&data, 1);
}

/**
  * @brief  Sets Cursor position.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @retval None
  */
void TFT7735_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
  uint8_t data = 0;
  TFT7735_CMD(LCD_REG_42);
  data = (Xpos) >> 8;
  TFT7735_WDATA(&data, 1);
  data = (Xpos) & 0xFF;
  TFT7735_WDATA(&data, 1);
  TFT7735_CMD(LCD_REG_43); 
  data = (Ypos) >> 8;
  TFT7735_WDATA(&data, 1);
  data = (Ypos) & 0xFF;
  TFT7735_WDATA(&data, 1);
  TFT7735_CMD(LCD_REG_44);
}
/********************************* Command for execute on LCD ***********************************/
/**
  * @brief  Writes pixel.   
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  RGBCode: the RGB pixel color
  * @retval None
  */
void TFT7735_WritePixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGBCode)
{
  uint8_t data = 0;
  if((Xpos >= TFT7735_LCD_PIXEL_WIDTH) || (Ypos >= TFT7735_LCD_PIXEL_HEIGHT)) 
  {
    return;
  }
  
  /* Set Cursor */
  TFT7735_SetCursor(Xpos, Ypos);
  
  data = RGBCode >> 8;
  TFT7735_WDATA(&data, 1);
  data = RGBCode;
  TFT7735_WDATA(&data, 1);
}  

/**
  * @brief  Sets a display window
  * @param  Xpos:   specifies the X bottom left position.
  * @param  Ypos:   specifies the Y bottom left position.
  * @param  Height: display window height.
  * @param  Width:  display window width.
  * @retval None
  */
void TFT7735_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  uint8_t data = 0;
  /* Column addr set, 4 args, no delay: XSTART = Xpos, XEND = (Xpos + Width - 1) */
  TFT7735_CMD(LCD_REG_42);
  data = (Xpos) >> 8;
  TFT7735_WDATA(&data, 1);
  data = (Xpos) & 0xFF;
  TFT7735_WDATA(&data, 1);
  data = (Xpos + Width - 1) >> 8;
  TFT7735_WDATA(&data, 1);
  data = (Xpos + Width - 1) & 0xFF;
  TFT7735_WDATA(&data, 1);
  /* Row addr set, 4 args, no delay: YSTART = Ypos, YEND = (Ypos + Height - 1) */
  TFT7735_CMD(LCD_REG_43);
  data = (Ypos) >> 8;
  TFT7735_WDATA(&data, 1);
  data = (Ypos) & 0xFF;
  TFT7735_WDATA(&data, 1);
  data = (Ypos + Height - 1) >> 8;
  TFT7735_WDATA(&data, 1);
  data = (Ypos + Height - 1) & 0xFF;
  TFT7735_WDATA(&data, 1);
}

/**
  * @brief  Draws horizontal line.
  * @param  RGBCode: Specifies the RGB color   
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Length: specifies the line length.  
  * @retval None
  */
void TFT7735_DrawHLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  uint16_t counter = 0;
	
  if(Xpos + Length > TFT7735_LCD_PIXEL_WIDTH) return;
  
  /* Set Cursor */
  TFT7735_SetCursor(Xpos, Ypos);
  /*
  for(counter = 0; counter < 2*Length; counter+=2)
  {
   ArrayRGB[counter] = RGBCode;
	 ArrayRGB[counter+1] = RGBCode>>8;
  }
  TFT7735_WDATA( ArrayRGB, Length * 2);
  */
	for(counter = 0; counter < Length; counter++)
  {
	 TFT7735_WDATA((uint8_t*) &RGBCode );
	//	SPIx_WriteData((uint8_t*) &RGBCode , 2);
	}
	
}

/**
  * @brief  Draws vertical line.
  * @param  RGBCode: Specifies the RGB color   
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Length: specifies the line length.  
  * @retval None
  */
void TFT7735_DrawVLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
  uint8_t counter = 0;
  
  if(Ypos + Length > TFT7735_LCD_PIXEL_HEIGHT) return;
  for(counter = 0; counter < Length; counter++)
  {
    TFT7735_WritePixel(Xpos, Ypos + counter, RGBCode);
  }   
}

/**
  * @brief  Gets the LCD pixel Width.
  * @param  None
  * @retval The Lcd Pixel Width

uint16_t TFT7735_GetLcdPixelWidth(void)
{
  return TFT7735_LCD_PIXEL_WIDTH;
}
*/
/**
  * @brief  Gets the LCD pixel Height.
  * @param  None
  * @retval The Lcd Pixel Height

uint16_t TFT7735_GetLcdPixelHeight(void)
{                          
  return TFT7735_LCD_PIXEL_HEIGHT;
}
*/

/**
  * @brief  Displays a bitmap picture loaded in the internal Flash.
  * @param  BmpAddress: Bmp picture address in the internal Flash.
  * @retval None
  */
void TFT7735_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp)
{
  uint32_t index = 0, size = 0;
  
  /* Read bitmap size */
  size = *(volatile uint16_t *) (pbmp + 2);
  size |= (*(volatile uint16_t *) (pbmp + 4)) << 16;
  /* Get bitmap data address offset */
  index = *(volatile uint16_t *) (pbmp + 10);
  index |= (*(volatile uint16_t *) (pbmp + 12)) << 16;
  size = (size - index)/2;
  pbmp += index;
  
  /* Set GRAM write direction and BGR = 0 */
  /* Memory access control: MY = 0, MX = 1, MV = 0, ML = 0 */
  TFT7735_WREG(LCD_REG_54, 0x40);

  /* Set Cursor */
  TFT7735_SetCursor(Xpos, Ypos);  
 
  TFT7735_WDATA((uint8_t*)pbmp, size*2);
 
  /* Set GRAM write direction and BGR = 0 */
  /* Memory access control: MY = 1, MX = 1, MV = 0, ML = 0 */
  TFT7735_WREG(LCD_REG_54, 0xC0);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

