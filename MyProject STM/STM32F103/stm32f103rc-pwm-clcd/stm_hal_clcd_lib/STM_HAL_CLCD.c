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
	#include "STM_HAL_CLCD.h"
	
static uint8_t d_val;

//************************************   DELAY FUNCTION   ******************************************	

void  Delay_us(uint32_t d_us)
{
	d_us*=d_val;
	while(d_us!=0) d_us--;
}


//***********************************   CONFIGURATION PORT   ****************************************

void  Clcd_Config_Port(Clcd_pin_TypeDef* prt)
{

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

void Clcd_Port_Data(Clcd_TypeDef *clcd,uint8_t lcd_in)
{
#if CLCD_PORT==8 
	if(lcd_in & 0x01)  CLCD_D0_HIGH;
	else CLCD_D0_LOW;
	if(lcd_in & 0x02)  CLCD_D1_HIGH;
	else CLCD_D1_LOW;
	if(lcd_in & 0x04)  CLCD_D2_HIGH;
	else CLCD_D2_LOW;
	if(lcd_in & 0x08)  CLCD_D3_HIGH;
	else CLCD_D3_LOW;
#endif
	if(lcd_in & 0x10)  CLCD_D4_HIGH;
	else CLCD_D4_LOW;
	if(lcd_in & 0x20)  CLCD_D5_HIGH;
	else CLCD_D5_LOW;
	if(lcd_in & 0x40)  CLCD_D6_HIGH;
	else CLCD_D6_LOW;
	if(lcd_in & 0x80)  CLCD_D7_HIGH;
	else CLCD_D7_LOW;
}

//*******************************  This function for send command    **********************************

void Clcd_Cmd(Clcd_TypeDef *clcd,uint8_t lcd_in)
{ 
	CLCD_RS_LOW;             // => RS = 0
	Clcd_Port_Data(clcd,lcd_in);
	CLCD_EN_HIGH;            // => E = 1
  Delay_us(2);
#if CLCD_PORT != 8	
  Delay_us(10);
  CLCD_EN_LOW;             // => E = 0
	Clcd_Port_Data(clcd,lcd_in<<4);
	CLCD_EN_HIGH;            // => E = 1
  Delay_us(2);
#endif	
  CLCD_EN_LOW;             // => E = 0
	Delay_us(10);
}

//**********************  This function define initial condition of clcd   ****************************

void Clcd_Init(Clcd_TypeDef *clcd)
{
	Clcd_Config_Port(&clcd->D4);
	Clcd_Config_Port(&clcd->D5);
	Clcd_Config_Port(&clcd->D6);
	Clcd_Config_Port(&clcd->D7);
	Clcd_Config_Port(&clcd->RS);
	Clcd_Config_Port(&clcd->EN);
#if CLCD_PORT==8
	Clcd_Config_Port(&clcd->D0);
	Clcd_Config_Port(&clcd->D1);
	Clcd_Config_Port(&clcd->D2);
	Clcd_Config_Port(&clcd->D3);
#endif
#if CLCD_RW==1
  Clcd_Config_Port(&clcd->RW);
#endif	
	
	d_val=HAL_RCC_GetSysClockFreq()/1000000;
//	Clcd_Cmd(clcd,0x3C);  //  2-line & font-5*10 & 8-bit data port
//	Clcd_Cmd(clcd,0x38);  //  2-line & font-5*7  & 8-bit data port
//	Clcd_Cmd(clcd,0x34);  //  1-line & font-5*10 & 8-bit data port
//	Clcd_Cmd(clcd,0x30);  //  1-line & font-5*7  & 8-bit data port
//	Clcd_Cmd(clcd,0x2C);  //  2-line & font-5*10 & 4-bit data port
 	Clcd_Cmd(clcd,0x28);    //  2-line & font-5*7  & 4-bit data port
//	Clcd_Cmd(clcd,0x24);  //  1-line & font-5*10 & 4-bit data port
//	Clcd_Cmd(clcd,0x20);  //  1-line & font-5*7  & 4-bit data port
  Delay_us(20);
	
//	Clcd_Cmd(clcd,0x0F);  //  Display-on  & Cursor underline-on   & Cursor blink-on
//	Clcd_Cmd(clcd,0x0E);  //  Display-on  & Cursor underline-on   & Cursor blink-off
//	Clcd_Cmd(clcd,0x0D);  //  Display-on  & Cursor underline-off  & Cursor blink-on
	Clcd_Cmd(clcd,0x0C);    //  Display-on  & Cursor underline-off  & Cursor blink-off
//	Clcd_Cmd(clcd,0x08);  //  Display-off & Cursor underline-off  & Cursor blink-off	
  Delay_us(20);
	
//	Clcd_Cmd(clcd,0x04);    //  Decrement Thats mear left to right  & Shift -off
//	Clcd_Cmd(clcd,0x06);  //  Increment Thats mear right to left  & Shift -off
//	Clcd_Cmd(clcd,0x05);  //  Decrement Thats mear left to right  & Shift -on
//	Clcd_Cmd(clcd,0x07);  //  Increment Thats mear right to left  & Shift -on

//	Clcd_Cmd(clcd,0x10);  //  Cursor move   & Left shift  
//	Clcd_Cmd(clcd,0x14);  //  Cursor move   & Right shift
//	Clcd_Cmd(clcd,0x18);  //  Display shift & Left shift  
//	Clcd_Cmd(clcd,0x14);  //  Display shift & Right shift

	Clcd_Cmd(clcd,0x01);  //  Display clear
  Delay_us(500);
 
//	Lcd_Cmd(0x02);  //  Display & Cursor goto home
	
}

//*********************************  This function is Clear clcd   ************************************

void Clcd_Clear(Clcd_TypeDef *clcd)
{
	
	Clcd_Cmd(clcd,0x01);
	Delay_us(500);
}

//*****************************  This function for cursor goto home   ***********************************

void Clcd_home(Clcd_TypeDef *clcd)
{
	
	Clcd_Cmd(clcd,0x02);
	Delay_us(40);
}

//************************  This function is cursor goto where you want   *******************************

void Clcd_Gotoxy(Clcd_TypeDef *clcd,uint8_t lcd_inx, uint8_t lcd_iny)
{
	if(lcd_inx == 0) Clcd_Cmd(clcd,0x80 + lcd_iny);
	else             Clcd_Cmd(clcd,0xC0 + lcd_iny);
}

//*****************************  This function for cursor on or off   ************************************

void Clcd_Cursor_Mode(Clcd_TypeDef *clcd,uint8_t lcd_in)
{
//  0 for cursor off & 1 for cursor on	
	  Clcd_Cmd(clcd,0x00);
	  Delay_us(20);
	  if(lcd_in==1)   Clcd_Cmd(clcd,0x0E);  //cursor on
    else            Clcd_Cmd(clcd,0x0C);  //cursor off

}

//******************************  This function for blink on or off   ************************************

void Clcd_Blink_Mode(Clcd_TypeDef *clcd,uint8_t lcd_in)
{
//  0 for cursor off & 1 for cursor on	
	  if(lcd_in==1)   Clcd_Cmd(clcd,0x0D);  //blink on
    else            Clcd_Cmd(clcd,0x0C);  //blink off
}

//******************************  This function for shift cursor or display   ************************************

void Clcd_Shift_Mode(Clcd_TypeDef *clcd,uint8_t lcd_in)
{
	  if(lcd_in==0)        Clcd_Cmd(clcd,0x04);  //lcd write to left   (right to left)
    else if(lcd_in==1)   Clcd_Cmd(clcd,0x05);  //lcd write to left and position shift to 20 space
	  else if(lcd_in==2)   Clcd_Cmd(clcd,0x06);  //lcd write to right ( left to right)
    else                 Clcd_Cmd(clcd,0x07);  //lcd write to right and position shift to 20 space	

}

//*******************************  This function for pattern shift display   *************************************

void Clcd_Move_Mode(Clcd_TypeDef *clcd,uint8_t lcd_in)
{
	  if(lcd_in==0)        Clcd_Cmd(clcd,0x10);  //Cursor shift to left 
    else if(lcd_in==1)   Clcd_Cmd(clcd,0x14);  //Cursor shift to right 
	  else if(lcd_in==2)   Clcd_Cmd(clcd,0x18);  //display shift to left with one time run one space to left
    else                 Clcd_Cmd(clcd,0x1C);  //display shift to right with one time run one space to right
}

//*************************************  This function for write char   *******************************************

void Clcd_Write_Char(Clcd_TypeDef *clcd,uint8_t lcd_in)
{	
	 // with this function you can display a char with ascii code
	 CLCD_RS_HIGH;                   //  RS = 1
   Clcd_Port_Data(clcd,lcd_in);        //4-bit high to Data transfer
	 CLCD_EN_HIGH;                   // => E=1
	 Delay_us(2);
#if CLCD_PORT !=8                  	
	 CLCD_EN_LOW;                    // => E=0
	 Delay_us(10); 
	 Clcd_Port_Data(clcd,lcd_in<<4);
	 CLCD_EN_HIGH;                   // => E=1
	 Delay_us(2);
#endif	    
	 CLCD_EN_LOW;                    // => E=0
	 Delay_us(10);
}

//************************************  This function for write string   ******************************************

void Clcd_Write_String(Clcd_TypeDef *clcd,char *lcd_in)
{	
	// with this function you can display a string with stdio.h lib tha is pointer char
  int lcd_ini;
	for(lcd_ini=0;lcd_in[lcd_ini]!='\0';lcd_ini++)
	Clcd_Write_Char(clcd,lcd_in[lcd_ini]);
}	

//************************************  This function for write decimal   ******************************************

void Clcd_Write_Decimal(Clcd_TypeDef *clcd,uint8_t lcd_inx,uint8_t lcd_iny,int32_t lcd_in)
{	
  // with this function you can display a intiger without stdio.h lib
	int8_t lcd_ini,lcd_inj,tmp[17-lcd_iny];
	
	lcd_ini=0;
	do{
		lcd_ini++;
	  tmp[lcd_ini]=lcd_in%10;	
    lcd_in/=10;
	}while(lcd_in != 0);
	Clcd_Gotoxy(clcd,lcd_inx,lcd_iny);	
	for(lcd_inj=lcd_ini;lcd_inj>0;lcd_inj--){
	Clcd_Write_Char(clcd,tmp[lcd_inj]+32);
  }
}
//************************************  end of library  ******************************************

