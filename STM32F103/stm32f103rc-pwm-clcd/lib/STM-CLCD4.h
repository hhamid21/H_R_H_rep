
//LCD Functions Developed by hamid

#include "stm32f1xx_hal.h"





//LCD Module Connections
//extern bit RS;                                                                   
//extern bit EN;                           
//extern bit D4;
//extern bit D5;
//extern bit D6;
//extern bit D7;
//End LCD Module Connections 

void Lcd_Delay(unsigned int lcd_in)
{
    int lcd_j;
    int lcd_i;
    for(lcd_i=0;lcd_i<lcd_in;lcd_i++)
    {
        for(lcd_j=0;lcd_j<60;lcd_j++)
        {
        }
    }
}



//LCD 4 Bit Interfacing Functions

void Lcd4_Port(unsigned char lcd_in)
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,GPIO_PIN_RESET);
	if(lcd_in & 0x10)
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_SET);
	
	if(lcd_in & 0x20)
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_SET);
	
	if(lcd_in & 0x40)
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_SET);
	
	if(lcd_in & 0x80)
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);
}
void Lcd4_Cmd(unsigned char lcd_in1)
{ 
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_RESET);             // => RS = 0
	Lcd4_Port(lcd_in1);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_SET);             // => E = 1
  Lcd_Delay(2);
  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_RESET);              // => E = 0
	Lcd4_Port(lcd_in1<<4);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_SET);             // => E = 1
  Lcd_Delay(2);
  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_RESET);              // => E = 0
	Lcd_Delay(10);
}

void Lcd4_Clear()
{
	
	Lcd4_Cmd(0x00);
	Lcd4_Cmd(0x01);
	Lcd_Delay(550);
}

void Lcd4_Gotoxy(unsigned char lcd_in, unsigned char lcd_in1)
{
	unsigned char temperar;
	if(lcd_in == 0)
	{
	  if(lcd_in1<16)  temperar = 0x80 +lcd_in1 ;
		else   temperar = 0x80 +15 ;  
		Lcd4_Cmd(temperar); 
	}
	else 
	{
		if(lcd_in1<16)  temperar = 0xC0 +lcd_in1 ;
		else   temperar = 0xC0 +15 ;  
		Lcd4_Cmd(temperar);
	}
}


void Lcd4_Init()
{
	///////////// Reset process from datasheet /////////
	Lcd4_Cmd(0x28);
	Lcd4_Cmd(0x0C);      // Display ON
	// Lcd4_Cmd(0x0E);   // Display ON - Cursor ON
	// Lcd4_Cmd(0x0F);   // Display ON - Cursor ON - Blink ON
	Lcd_Delay(50);
	
}

/*
void Shift_Mode(char lcd_in)
{
	  if(lcd_in==1)   Lcd4_Cmd(0x07);
    if(lcd_in==0)   Lcd4_Cmd(0x05);	

}

void Shift_Pattern(char lcd_in)
{

	  if(lcd_in==1)   Lcd4_Cmd(0x10);
    if(lcd_in==2)   Lcd4_Cmd(0x11);
		if(lcd_in==3)   Lcd4_Cmd(0x12);
    if(lcd_in==4)   Lcd4_Cmd(0x13);	 
}
*/

void Lcd4_Write_Char(unsigned char lcd_in1)
{	
	 HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_SET);              // => RS = 1
   Lcd4_Port(lcd_in1);             //Data transfer
	 HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_SET);              // => E=1
	 Lcd_Delay(2);
	 HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_RESET);            // => E=0
	 Lcd4_Port(lcd_in1<<4);
	 HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_SET);              // => E=1
	 Lcd_Delay(2);
	 HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_RESET);           // => E=0
	Lcd_Delay(10);
}

void Lcd4_Write_String(char *lcd_in)
{
	int lcd_ini;
	for(lcd_ini=0;lcd_in[lcd_ini]!='\0';lcd_ini++)
	 Lcd4_Write_Char(lcd_in[lcd_ini]);
}

//End LCD 4 Bit Interfacing Functions
