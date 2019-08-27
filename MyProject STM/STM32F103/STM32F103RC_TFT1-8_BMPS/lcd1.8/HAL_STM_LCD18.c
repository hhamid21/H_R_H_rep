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
#include "font.h"
#include "fatfs.h"
//#include "stm32f0xx_hal.h"
//#include "stm32f0xx_hal_spi.h"


volatile uint8_t x_text=0, y_text=0;
static uint16_t Position=0;

extern SPI_HandleTypeDef hspi3;

//SPI_HandleTypeDef lcd_spi;
//extern SPI_HandleTypeDef hspi3; 
/********************************* LINK LCD Define Reset,DC,CS  pin ***********************************
  * @brief  Initialize the LCD
  */
void TFT7735_IO_Init(void)
{
  GPIO_InitTypeDef  gpiostruct;

  /* LCD_CS_GPIO and LCD_DC_GPIO Periph clock enable */
  LCD_CS_GPIO_CLK_ENABLE();
  LCD_DC_GPIO_CLK_ENABLE();
	LCD_RESET_GPIO_CLK_ENABLE();
	SD_CS_GPIO_CLK_ENABLE();
  
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

/* Configure LCD_RESET_PIN pin: LCD Card RESET pin */
  gpiostruct.Pin = SD_CS_PIN;
  HAL_GPIO_Init(SD_CS_GPIO_PORT, &gpiostruct);
	
  /* LCD chip select high */
	LCD_CS_HIGH();
	SD_CS_HIGH();
  /* LCD SPI Config */
	SPIx_Init();
//	MX_SPI1_Init();
}

/*************************************   Initialize SPI HAL    **************************************
  * @brief  Initialize SPI HAL.
  */
static void SPIx_Init(void)
{
  if(HAL_SPI_GetState(&hspi3) == HAL_SPI_STATE_RESET)
  {
    /* SPI Config */
    hspi3.Instance = SPI1;
      /* SPI baudrate is set to 8 MHz maximum (PCLK2/SPI_BaudRatePrescaler = 64/8 = 8 MHz) 
       to verify these constraints:
          - ST7735 LCD SPI interface max baudrate is 15MHz for write and 6.66MHz for read
            Since the provided driver doesn't use read capability from LCD, only constraint 
            on write baudrate is considered.
          - SD card SPI interface max baudrate is 25MHz for write/read
          - PCLK2 max frequency is 32 MHz 
       */
  hspi3.Init.Mode = SPI_MODE_MASTER;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi3.Init.NSS = SPI_NSS_SOFT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    SPIx_Error ();
  }

  }
}
/********************************* Writ data more than 1 byte to LCD **********************************
  * @brief  SPI Write a byte to device
  * @param  Value: value to be written
  * @retval None
  */
void SPIx_WriteData(uint8_t *Pdata, uint16_t DataLegnth)
{
  HAL_StatusTypeDef status = HAL_OK;

	/* Reset LCD control line CS */
  LCD_CS_LOW();
  /* Set LCD data/command line DC to Low */
  LCD_DC_HIGH();
  status = HAL_SPI_Transmit(&hspi3, Pdata, DataLegnth,10);

  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    SPIx_Error();
  }
	/* Deselect : Chip Select high */
  LCD_CS_HIGH();
}
/********************************* send 1 byte data to lcd with spi connection *******************************
  * @brief  SPI Write a byte to device
  * @param  Value: value to be written
  */
static void SPIx_Write(uint8_t Value)
{
  HAL_StatusTypeDef status = HAL_OK;

  status = HAL_SPI_Transmit(&hspi3, (uint8_t*) &Value, 1, 10);
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    SPIx_Error();
  }
}

/********************************* Error on spi Connection *******************************
  * @brief  SPI error treatment function
  */
static void SPIx_Error (void)
{
  /* De-initialize the SPI communication BUS */
  //HAL_SPI_DeInit(&hspi3);
  HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_1);
  /* Re-Initiaize the SPI communication BUS */
  //SPIx_Init();
}
/*************************************** CMD OR DATA to LCD but 1 value ***************************************
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

/************************************* Command for Write register value *****************************************
* @brief  Write register value.
* @param  pData Pointer on the register value
* @param  Size Size of byte to transmit to the register
*/
void TFT7735_WDATA(uint8_t *pData, uint16_t Size)
{
  uint16_t counter = 0;
  
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
      while(((hspi3.Instance->SR) & SPI_FLAG_TXE) != SPI_FLAG_TXE)
   {
 }
      /* Need to invert bytes for LCD*/
      *((__IO uint8_t*)&hspi3.Instance->DR) = *(pData+1);

      while(((hspi3.Instance->SR) & SPI_FLAG_TXE) != SPI_FLAG_TXE)
{
}
      *((__IO uint8_t*)&hspi3.Instance->DR) = *pData;
      counter--;
      pData += 2;
  }  
  
    /* Wait until the bus is ready before releasing Chip select */ 
    while(((hspi3.Instance->SR) & SPI_FLAG_BSY) != RESET)
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
  
	/* Initialize TFT7735 Reset & DC & CS Pins & spi Connection-------------------------*/
  TFT7735_IO_Init();
	/* Hardware Reset TFT7735  ---------------------------------------------------------*/
	LCD_CS_LOW();
	LCD_RESET_LOW();
	LCD_Delay_ms(2);
	LCD_RESET_HIGH();
	LCD_Delay_ms(120);
  LCD_CS_HIGH();
  /* Out of sleep mode, 0 args, no delay */
  TFT7735_WREG(LCD_REG_17, 0x00); 
  /* Frame rate ctrl - normal mode, 3 args:Rate = fosc/(1x2+40) * (LINE+2C+2D)*/
  //TFT7735_CMD(LCD_REG_177);
  //data = 0x01;
  //TFT7735_WDATA(&data, 1);
  //data = 0x2C;
	//TFT7735_WDATA(&data, 1);
  //data = 0x2D;
  //TFT7735_WDATA(&data, 1);
  /* Frame rate control - idle mode, 3 args:Rate = fosc/(1x2+40) * (LINE+2C+2D) */    
  //TFT7735_WREG(LCD_REG_178, 0x01);//01
  //TFT7735_WREG(LCD_REG_178, 0x2C);//2c
  //TFT7735_WREG(LCD_REG_178, 0x2D);//2d
  /* Frame rate ctrl - partial mode, 6 args: Dot inversion mode, Line inversion mode */ 
  //TFT7735_WREG(LCD_REG_179, 0x01);
  //TFT7735_WREG(LCD_REG_179, 0x2C);
  //TFT7735_WREG(LCD_REG_179, 0x2D);
  //TFT7735_WREG(LCD_REG_179, 0x01);
  //TFT7735_WREG(LCD_REG_179, 0x2C);
  //TFT7735_WREG(LCD_REG_179, 0x2D);
  /* Display inversion ctrl, 1 arg, no delay: No inversion */
  //TFT7735_WREG(LCD_REG_180, 0x07);
  /* Power control, 3 args, no delay: -4.6V , AUTO mode */
  //TFT7735_WREG(LCD_REG_192, 0xA2);
  //TFT7735_WREG(LCD_REG_192, 0x02);
  //TFT7735_WREG(LCD_REG_192, 0x84);
  /* Power control, 1 arg, no delay: VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD */
  //TFT7735_WREG(LCD_REG_193, 0xC5);
  /* Power control, 2 args, no delay: Opamp current small, Boost frequency */ 
  //TFT7735_WREG(LCD_REG_194, 0x0A);
  //TFT7735_WREG(LCD_REG_194, 0x00);
  /* Power control, 2 args, no delay: BCLK/2, Opamp current small & Medium low */  
  //TFT7735_WREG(LCD_REG_195, 0x8A);
  //TFT7735_WREG(LCD_REG_195, 0x2A);
  /* Power control, 2 args, no delay */
  //TFT7735_WREG(LCD_REG_196, 0x8A);
  //TFT7735_WREG(LCD_REG_196, 0xEE);
  /* Power control, 1 arg, no delay */
  //TFT7735_WREG(LCD_REG_197, 0x0E);
  /* 0x33->invert 0x32->Don't invert display, no args, no delay */
  //TFT7735_CMD(LCD_REG_32);
  /* Set color mode, 1 arg, no delay:0x03->12bit 0x05->16bit 0x06->18bit color */
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
  /* Magical unicorn dust, 16 args, no delay 
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
	*/
  /* Sparkles and rainbows, 16 args, no delay 
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
	*/
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

/************************************* Command for set Display On *****************************************
  * @brief  Enables the Display.
  * @param  None
  * @retval None
  */
void TFT7735_DisplayOn(void)
{
  uint8_t data;
	TFT7735_CMD(LCD_REG_19);
  LCD_Delay_ms(10);
  TFT7735_CMD(LCD_REG_41);
  LCD_Delay_ms(10);
  TFT7735_CMD(LCD_REG_54);
  data=0xC0;
	TFT7735_WDATA(&data, 1);
}

/************************************* Command for set Display Off *****************************************
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
/********************************* Command for set dimension of start point **********************************
  * @brief  Sets Cursor position.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @retval None
  */
void TFT7735_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
  TFT7735_CMD(LCD_REG_42);
	TFT7735_WDATA((uint8_t*) &Xpos, 2);
  TFT7735_CMD(LCD_REG_43); 
  TFT7735_WDATA((uint8_t*) &Ypos, 2);
  TFT7735_CMD(LCD_REG_44);
}
/********************************* Command for set dimension of display **********************************
  * @brief  Sets a display window
  * @param  Xpos:   specifies the X bottom left position.
  * @param  Ypos:   specifies the Y bottom left position.
  * @param  Height: display window height.
  * @param  Width:  display window width.
  * @retval None
  */
void TFT7735_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  uint16_t data = 0;

  /* Column addr set, 4 args, no delay: XSTART = Xpos, XEND = (Xpos + Width - 1) */
  data = Xpos + Width - 1;
	TFT7735_CMD(LCD_REG_42);
  TFT7735_WDATA((uint8_t*) &Xpos, 2);
  TFT7735_WDATA((uint8_t*) &data, 2);
  /* Row addr set, 4 args, no delay: YSTART = Ypos, YEND = (Ypos + Height - 1) */
  data = Ypos + Height - 1 ;
  TFT7735_CMD(LCD_REG_43);
  TFT7735_WDATA((uint8_t*) &Ypos, 2);
  TFT7735_WDATA((uint8_t*) &data, 2);
	TFT7735_CMD(LCD_REG_44);
}


/********************************* Command for Clear Screen *****************************************
  * @brief  Clear Screen
  * @param  brgb:  Color of Background.
  * @retval None
  */
void TFT7735_CLS_UD(uint16_t brgb)
{
TFT7735_SetDisplayWindow(0,0,TFT7735_LCD_PIXEL_WIDTH,TFT7735_LCD_PIXEL_HEIGHT);
  
	
	for(uint8_t f=0;f<160;f++)
	{
	  TFT7735_SetCursor(0,f);
    TFT7735_CMD(LCD_REG_44); 
		for(uint8_t counter = 0; counter < 128; counter++)
		{
		TFT7735_WDATA((uint8_t*) &brgb,2);
		}	
  }
}
/********************************* Command for Clear Screen *****************************************
  * @brief  Clear Screen
  * @param  brgb:  Color of Background.
  * @retval None
  */
void TFT7735_CLS_LR(uint16_t brgb)
{
TFT7735_SetDisplayWindow(0,0,TFT7735_LCD_PIXEL_HEIGHT ,TFT7735_LCD_PIXEL_WIDTH);
  
	
	for(uint8_t f=0;f<128;f++)
	{
	  TFT7735_SetCursor(0,f);
    TFT7735_CMD(LCD_REG_44); 
		for(uint8_t counter = 0; counter < 160; counter++)
		{
		TFT7735_WDATA((uint8_t*) &brgb,2);
		}	
  }
}
/********************************* Command for set Position of display **********************************
  * @brief  set Position of display
  * @param  Pos: Position is 0 or 90 or 180 or 270 ( up and down or left and right
  *					Normal mode 0xC0 90->0xE0 180->0x00 270->0x20
  * @param  RGB: 0 for RGB and 1 for BGR 
  * @retval None
  */
void TFT7735_Position(uint16_t Pos,uint8_t RGB)// Normal mode 0xC0 90->0xE0 180->0x00 270->0x20
{										
	switch (Pos)																		
	{	
	case 0:
	Position=0;
	if(RGB) TFT7735_WREG(0x36,0xC8);
  else    TFT7735_WREG(0x36,0xC0);
	break;
	case 90:
	Position=90;
	if(RGB) TFT7735_WREG(0x36,0xEC);
  else    TFT7735_WREG(0x36,0xE4);
	break;
	case 180:
	Position=180;
	if(RGB) TFT7735_WREG(0x36,0x1C);
	else    TFT7735_WREG(0x36,0x14);
	break;
	case 270:
	Position=270;
	if(RGB) TFT7735_WREG(0x36,0x38);
 	else    TFT7735_WREG(0x36,0x30);
	break;
	default:
	Position=0;
	TFT7735_WREG(0x36,0xC0);			
 }
}
/************************************   Command for Invert Color   ***************************************
  * @brief  Invert Color
  * @param  invert: value for inversion  0 for Invert Off & 1 for Invert On
  * @retval None
  */
void TFTT7735_InvertColors(uint8_t invert) 
{
 if(invert==1) TFT7735_CMD(LCD_REG_33 ); 
 else          TFT7735_CMD(LCD_REG_32 );
}
/********************************* Command for set point for text on display **********************************
  * @brief  Sets a point for start text
  * @param  x: x is 0 to 16  
  * @param  y: y is 0 to 10
  * @retval None
  */
void TFT7735_Gotoxy(uint8_t x, uint8_t y)
{
if( Position==0 || Position==180){
  if(x>16 || y>10) 
  {
    y+=x/16;
    x%=16;
  }
}
else{
  if(x>20 || y>8) 
  {
    y+=x/20;
    x%=20;
  }
}
  x_text=x;
  y_text=y;
}
/********************************* Command for Write a char based on ASCII **********************************
  * @brief  Write a char based on ASCII
  * @param  frgb: Front Color (Text color)
  * @param  brgb: Back Color (Background color)
  * @retval None
  */
void TFT7735_Putc(char character, uint16_t frgb, uint32_t brgb)
{  
  int8_t wcount, hcount;
  char char_data;

  TFT7735_SetDisplayWindow(x_text*8, y_text*16, 8, 16);
  for(hcount=0; hcount<16; hcount++)
  {
    char_data = font8x16[character-32][hcount];
   
   	for(wcount=7; wcount>-1; wcount--)
			{
				if (char_data&(1<<wcount))   TFT7735_WDATA((uint8_t*) &frgb, 2);
				else			                   TFT7735_WDATA((uint8_t*) &brgb, 2);
			}
  }
if(Position==0 || Position==180){	
  if(++x_text>16)
  {            
    x_text=0;
    if(++y_text>10) y_text=0;
  }
}
else{
  if(++x_text>20)
  {            
    x_text=0;
    if(++y_text>8) y_text=0;
  }
}
}
/********************************* Command for Write a char based on ASCII **********************************
  * @brief  Write a char based on ASCII with position
  * @param  character: based on ASCII code
  * @param  Xpos: x position
	* @param  Ypos: Y position
	* @param  frgb: Front Color (Text color)
  * @param  brgb: Back Color (Background color)
  * @retval None
  */
void TFT7735_Putchar(char character,uint8_t Xpos, uint8_t Ypos, uint16_t frgb, uint32_t brgb)
{  
  int8_t wcount, hcount;
  char char_data;
  
  if((Xpos >= TFT7735_LCD_PIXEL_WIDTH) || (Ypos >= TFT7735_LCD_PIXEL_HEIGHT))    return;
  for(hcount=0; hcount<16; hcount++)
  {
    TFT7735_SetCursor(Xpos, Ypos+hcount);
		char_data = font8x16[character-32][hcount];
   
   	for(wcount=7; wcount>-1; wcount--)
	{
      if (char_data & (1<<wcount))   TFT7735_WDATA((uint8_t*) &frgb, 2);
	    else  	    TFT7735_WDATA((uint8_t*) &brgb, 2);
	}
  }
}
/********************************* Command for Write String based on ASCII **********************************
  * @brief  Write String based on ASCII with position 
  * @param  str : string based on ASCII code
	* @param  frgb: Front Color (Text color)
  * @param  brgb: Back Color (Background color)
  * @retval None
  * @Note   start point can be define with GOTOXY Command
  */
void TFT7735_WString(char* str, uint16_t frgb, uint16_t brgb) 
{
    while(*str) {
            TFT7735_Putc(*str++,frgb,brgb);
           }
}
/********************************* Command for Write String based on ASCII **********************************
  * @brief  Write String based on ASCII with position
  * @param  str : string based on ASCII code
  * @param  Xpos: x position
	* @param  Ypos: Y position
	* @param  frgb: Front Color (Text color)
  * @param  brgb: Back Color (Background color)
  * @retval None
  */
void TFT7735_WriteString(uint16_t x, uint16_t y, const char* str, uint16_t frgb, uint16_t brgb) 
{
  if((x >= TFT7735_LCD_PIXEL_WIDTH) || (y >= TFT7735_LCD_PIXEL_HEIGHT))    return;
	while(*str) {
      if(x + 8 >= TFT7735_LCD_PIXEL_WIDTH ) {
        x = 0;
        y += 16;
        if(y + 16 >= TFT7735_LCD_PIXEL_HEIGHT)   y=0;
		}
		else x+=8;
        TFT7735_Putchar(*str++,x,y,frgb,brgb);
    }
}

/********************************* Command for set graphic pixel ***********************************
  * @brief  Writes pixel.   
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  RGBCode: the RGB pixel color
  * @retval None
  */
void TFT7735_WritePixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGBCode)
{
  if((Xpos >= TFT7735_LCD_PIXEL_WIDTH) || (Ypos >= TFT7735_LCD_PIXEL_HEIGHT)) 
  {
    return;
  }
  /* Set Cursor */
  TFT7735_SetCursor(Xpos, Ypos);
  TFT7735_WDATA((uint8_t*) &RGBCode, 2);
}
/********************************* Command for Draw Horizental Line ***********************************
  * @brief  Draws horizontal line.
  * @param  RGBCode: Specifies the RGB color   
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Length: specifies the line length.  
  * @retval None
  */
void TFT7735_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint16_t RGBCode)
{
  uint16_t counter = 0;
	
  if(Xpos + Length > TFT7735_LCD_PIXEL_WIDTH) return;
  
  /* Set Cursor */
  TFT7735_SetCursor(Xpos, Ypos);

	for(counter = 0; counter < Length; counter++)
  {
	 TFT7735_WDATA((uint8_t*) &RGBCode,2);
	}
}
/********************************* Command for Draw Vertical Line ***********************************
  * @brief  Draws vertical line.
  * @param  RGBCode: Specifies the RGB color   
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Length: specifies the line length.  
  * @retval None
  */
void TFT7735_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint16_t RGBCode)
{
  uint8_t counter = 0;
	uint16_t yp;
  
  if(Ypos + Length > TFT7735_LCD_PIXEL_HEIGHT) return;

	TFT7735_SetCursor(Xpos, Ypos);
  for(counter = 0; counter < Length; counter++)
  {
    yp=Ypos+counter;
		TFT7735_CMD(LCD_REG_43); 
		TFT7735_WDATA((uint8_t*) &yp, 2);
		TFT7735_CMD(LCD_REG_44);
		TFT7735_WDATA((uint8_t*) &RGBCode, 2);
  }   
}
/********************************* Command for Draw Line  to every side  ***********************************
  * @brief  Draws line.
  * @param  frgb: Specifies the RGB color   
  * @param  x0  : specifies the x0 position.
  * @param  y0  : specifies the y0 position.
  * @param  x1  : specifies the x1 position.
  * @param  y1  : specifies the y1 position.  
  * @retval None
  */
void TFT7735_Line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t frgb) 
{
  int16_t dy=y1-y0;
  int16_t dx=x1-x0;
  int8_t stepx, stepy;
  if(dy<0) { 
	dy=-dy; 
	stepy=-1; 
	} 
	else   stepy=1;  
  if(dx<0) {
  dx=-dx;  
	stepx=-1;  
	} 
	else   stepx=1; 
  dy<<=1; 
  dx<<=1;
  TFT7735_WritePixel(x0,y0,frgb);	
  if(dx>dy) 
  {
    int16_t fraction=dy-(dx>>1); 
    while(x0!=x1) 
    {
      if(fraction>=0) 
      {
        y0+=stepy;
        fraction-=dx; 
      }
      x0+=stepx;
      fraction+=dy; 
			TFT7735_WritePixel(x0,y0,frgb);
    }
  } 
  else 
  {
    int16_t fraction=dx-(dy>>1);
    while (y0 != y1) 
    {
      if(fraction>=0) 
      {
        x0+=stepx;
        fraction-=dy;
      }
      y0+=stepy;
      fraction+=dx;
			TFT7735_WritePixel(x0,y0,frgb);
    }
  }
}

/**
  * @brief  Fills a triangle (between 3 points).
  * @param  Points: Pointer to the points array
  * @param  x1: Point 1 X position
  * @param  y1: Point 1 Y position
  * @param  x2: Point 2 X position
  * @param  y2: Point 2 Y position
  * @param  x3: Point 3 X position
  * @param  y3: Point 3 Y position
  * @retval None
  */
void TFT7735_Triangle(uint8_t x1, uint8_t x2, uint8_t x3, uint8_t y1, uint8_t y2, uint8_t y3,  uint16_t frgb, char fill)
{ 
  int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
  yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
  curpixel = 0;
  
if(fill){	
  if(x2>=x1) deltax = x2 - x1;   		    /* The difference between the x's */
	else			 deltax = x1 - x2;      	  /* The difference between the x's */
	if(y2>=y1) deltay = y2 - y1;        	/* The difference between the y's */
	else			 deltay = y1 - y2;      	  /* The difference between the y's */
  x = x1;                          	    /* Start x off at the first pixel */
  y = y1;                       				/* Start y off at the first pixel */
  
  if (x2 >= x1){xinc1 = 1; xinc2 = 1;}  /* The x-values are increasing */
  else { xinc1 = -1; xinc2 = -1;}       /* The x-values are decreasing */
  if (y2 >= y1){yinc1 = 1; yinc2 = 1;}  /* The y-values are increasing */
  else  {yinc1 = -1; yinc2 = -1;}       /* The y-values are decreasing */
  if (deltax >= deltay)         /* There is at least one x-value for every y-value */
  {
    xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
    yinc2 = 0;                  /* Don't change the y for every iteration */
    den = deltax;
    num = deltax / 2;
    numadd = deltay;
    numpixels = deltax;         /* There are more x-values than y-values */
  }
  else                          /* There is at least one y-value for every x-value */
  {
    xinc2 = 0;                  /* Don't change the x for every iteration */
    yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
    den = deltay;
    num = deltay / 2;
    numadd = deltax;
    numpixels = deltay;         /* There are more y-values than x-values */
  }
  
  for (curpixel = 0; curpixel <= numpixels; curpixel++)
  {
    TFT7735_Line(x,y,x3,y3,frgb);
    num += numadd;              /* Increase the numerator by the top of the fraction */
    if (num >= den)             /* Check if numerator >= denominator */
    {
      num -= den;               /* Calculate the new numerator value */
      x += xinc1;               /* Change the x as appropriate */
      y += yinc1;               /* Change the y as appropriate */
    }
    x += xinc2;                 /* Change the x as appropriate */
    y += yinc2;                 /* Change the y as appropriate */
  }
}
else{
	TFT7735_Line(x1,y1,x2,y2,frgb);
	TFT7735_Line(x2,y2,x3,y3,frgb);
	TFT7735_Line(x1,y1,x3,y3,frgb);
}

}

/********************************* Command for Draw rectangle  ***********************************
  * @brief  Draws rectangle.
  * @param  frgb: Specifies the RGB color   
  * @param  x0  : specifies the x0 position.
  * @param  y0  : specifies the y0 position.
  * @param  x1  : specifies the x1 position.
  * @param  y1  : specifies the y1 position. 
  * @param  fill: 0 for empty and 1 for fill. 
  * @retval None
  */
void TFT7735_Rect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t frgb , char fill)
{
  uint16_t counter;
  if(fill)
  {
    TFT7735_SetDisplayWindow(x0,y0,x1,y1);
    for(counter=0;counter<((y1-y0+1)*(x1-x0+1));counter++)
    {		
      TFT7735_WDATA((uint8_t*) &frgb, 2);
    }
  }
	else{
	TFT7735_Line(x0,y0,x0,y1,frgb);	
  TFT7735_Line(x0,y1,x1,y1,frgb);
  TFT7735_Line(x1,y1,x1,y0,frgb);
  TFT7735_Line(x1,y0,x0,y0,frgb);
  	}	
}
/************************************ Command for Draw Circle ***************************************
  * @brief  Displays a bitmap picture loaded in the internal Flash.
  * @param  xcen: x of Center 0 to 128
  * @param  ycen: y of Center 0 to 160
  * @param  rad : radius of circle  0 to 64
  * @param  fill: 1 for fill 0 for empty
  * @retval None
  */
void TFT7735_Circle(uint8_t X, uint8_t Y, uint8_t R, uint16_t frgb, char fill)
{
	uint16_t x=0, y=R;
	int16_t d, e=0;

	while(y > 0){
		if(fill)
    {
			TFT7735_DrawHLine(X-x,Y+y, 2*x,frgb);
			TFT7735_DrawHLine(X-x,Y-y, 2*x,frgb);
    }
    else
    {
			TFT7735_WritePixel(X + x, Y + y,frgb);
			TFT7735_WritePixel(X + x, Y - y,frgb);
      TFT7735_WritePixel(X - x, Y + y,frgb);
      TFT7735_WritePixel(X - x, Y - y,frgb);
    }
		e = 2 * (d + y) - 1;
		if ((d<0)&&(e<=0)){
			d += 2*(++x) + 1;
			continue;
		}
		e = 2 * (d-x) - 1;
		if ((d>0)&&(e>0)){
			d += 1 - 2* (--y);
			continue;
		}
		x++;
		d += 2*(x-y);
		y--;
	}
	if(fill) TFT7735_DrawHLine(X-x,Y, 2*R,frgb);
  else{  
	TFT7735_WritePixel(X + x,Y,frgb);
  TFT7735_WritePixel(X - x,Y,frgb);
	}	
}

/**
  * @brief  Draws an ellipse on LCD.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  XRadius: Ellipse X radius
  * @param  YRadius: Ellipse Y radius
	* @param  frgb: RGB code color
  * @param  fill: fill ecllipse 0 for empty and 1 for fill
  * @retval None
  */
void TFT7735_Ecllipse(uint8_t Xpos, uint8_t Ypos, uint8_t XRadius, uint8_t YRadius, uint16_t frgb, char fill)
{
  int16_t x = 0, y = -YRadius, err = 2 - 2*XRadius, e2;
  float  K = 0, rad1 = 0, rad2 = 0;
  
  rad1 = XRadius;
  rad2 = YRadius;
  
  K = (float)(rad2/rad1);
  
  do {
		  if(fill)
				{
					TFT7735_DrawHLine(Xpos-(x/K),Ypos+y, 2*(x/K) + 1,frgb);
					TFT7735_DrawHLine(Xpos-(x/K),Ypos-y, 2*(x/K) + 1,frgb);
				}
			else
				{
					TFT7735_WritePixel(Xpos-(x/K),Ypos+y,frgb);		
					TFT7735_WritePixel(Xpos+(x/K),Ypos+y,frgb);
					TFT7735_WritePixel(Xpos+(x/K),Ypos-y,frgb);
					TFT7735_WritePixel(Xpos-(x/K),Ypos-y,frgb);
				}
    e2 = err;
    if (e2 <= x) {
      err += ++x*2+1;
      if (-y == x && e2 <= y) e2 = 0;
    }
    if (e2 > y) err += ++y*2+1;     
  }
  while (y <= 0);
}
/********************************* Command for Displays a bitmap picture ***********************************
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
/********************************* Command for Displays a bitmap picture ***********************************
  * @brief  Displays a bitmap picture loaded in the internal Flash With spi Direct Connection.
  * @param  BmpAddress: Bmp picture address in the internal Flash.
  * @retval None
  * @Note   Attention for This Function You Should be at First Mounted drive with f_mount(&Myfat1,"",0)
  */
void TFT7735_Bitmap_sd(uint16_t X1pos, uint16_t Y1pos,uint16_t X2pos, uint16_t Y2pos, char* Name)
{
 	//FATFS Myfat1;
	FIL Myfiles1;
	UINT byteCount1;
	char buffer1[256];
	uint16_t tmp;
	
	tmp=(X2pos-X1pos)*2;
	
	TFT7735_SetDisplayWindow(X1pos,Y1pos,X2pos,Y2pos);
  //if(f_mount(&Myfat1,"",0)== FR_OK)
	//	{
			f_open(&Myfiles1, Name,FA_READ);
			for(uint16_t f=0;f<(Y2pos-Y1pos);f++)
				{
				 f_read(&Myfiles1,buffer1,tmp,&byteCount1);
				 TFT7735_CMD(LCD_REG_43); 
         TFT7735_WDATA((uint8_t*) &f, 2);
         TFT7735_CMD(LCD_REG_44);
				 SPIx_WriteData((uint8_t*) buffer1,tmp);
				}
			f_close(&Myfiles1);
		//}
//f_mount(0, "", 0);
//free(Myfat1);		
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

