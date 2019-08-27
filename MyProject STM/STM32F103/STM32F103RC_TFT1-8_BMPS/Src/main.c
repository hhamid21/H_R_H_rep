/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

#include "main.h"
#include "fatfs.h"
#include "HAL_STM_LCD18.h"
#include "stdio.h"

//static char buffile1[8]={'d','1',0,0,'.','b','m','p'};
//static char buffile2[8]={'d','2',0,0,'.','b','m','p'};
static char buffile2[7]={'k',0,0,'.','b','m','p'};
//char buffile3[16]={'/','d','a','n','c','e','1','/','d','1',0,0,'.','b','i','n'};
char stbuf[54];
uint32_t tmpe[10]={0};
char databuf[16100];
uint32_t valfil,heightbmp,widthbmp,tmp1,tmpr,tmpb,tmpg,tmpr1,tmpg1,tmpb1;
uint16_t ofset,nread,hws=0,f1;
uint16_t ch,ch1,ch2=0,ch3,cb,cb1,cb2=0,conh,conw,writsitu,hs2,ws2;
uint32_t  hs1,ws1;
int16_t  hbd,wbd;
uint8_t  cplane,ncolor,bsitu,chcl=6,vers=2,crgb=1;
uint8_t  tt=0,f2,f3,confil=0;
char buffer[320];


RTC_HandleTypeDef hrtc;

SD_HandleTypeDef hsd;

SPI_HandleTypeDef hspi3;

TIM_HandleTypeDef htim6;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
static void MX_SDIO_SD_Init(void);
static void MX_SPI3_Init(void);
static void MX_TIM6_Init(void);

  FATFS Myfat;
	FIL Myfiles,Mf;
	DIR MyDir;
	UINT byteCount,byteCount1;


int main(void)
{
  //uint32_t zk;
	//uint8_t i;
	//char buff[16];
	
	
	
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_RTC_Init();
  MX_SDIO_SD_Init();
  MX_SPI3_Init();
  MX_TIM6_Init();
  MX_FATFS_Init();

 // HAL_TIM_Base_Start_IT (&htim6);
	
	TFT7735_Init();
	TFT7735_Position(180,0);
  TFT7735_CLS_UD(TFT7735_YELLOW);
  HAL_Delay(2000);


	if(f_mount(&Myfat, "", 1) != FR_OK)    Error_Handler();	

  while (1)
  {
		//TFT7735_CLS(TFT7735_YELLOW);
		//HAL_Delay(2000);
		
		//buffile1[3]=(confil%10)+48;
		buffile2[2]=(confil%10)+48;
		//buffile3[11]=(confil%10)+48;
		//buffile1[2]=(confil/10)+48;  
		buffile2[1]=(confil/10)+48;
    //buffile3[10]=(confil/10)+48;		
		confil++;
		if(confil>94) return 0;
		
		
	//	f_opendir(&MyDir,"/dance1");
		buffile2[5]='i';
		buffile2[6]='n';
		f_open(&Mf,buffile2, FA_WRITE|  FA_CREATE_ALWAYS);
		buffile2[5]='m';
		buffile2[6]='p';
		f_open(&Myfiles,buffile2,FA_READ);
		f_read(&Myfiles,databuf,54,&byteCount);
    // Check Format For BMP
	  if(databuf[0]!=0x42 || databuf[1]!=0x4D) 
			{
			//" print this file is not abmp file"
		  //	return 0;
		  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
		  }
		// Data Of PICTURE	
		valfil=((databuf[5]<<24)|(databuf[4]<<16)|(databuf[3]<<8)|databuf[2]);// Volume Of File
		ofset=(databuf[11]<<8)|databuf[10];// Offset Data 
		heightbmp =(databuf[25]<<24)|(databuf[24]<<16)|(databuf[23]<<8)|databuf[22]; // Height of Picture
		widthbmp = (databuf[21]<<24)|(databuf[20]<<16)|(databuf[19]<<8)|databuf[18]; // Width  of Picture
		cplane = databuf[26]; // Color Plane must be 1
		ncolor = databuf[28]; // Bit Color Per Pixel 
    bsitu  = databuf[30]; // Byte situation 0->BI_RGB 1->RLE8 2->RLE4 3->BI_BITFEILD V2=RGB& V3=RGBA
		// DIB_HEADER
		if(ofset>54)
			{ ofset-=54;
		    f_read(&Myfiles,databuf,ofset,&byteCount);
			}
		// Nomber Of Byte Per Pixel	
		if (ncolor==32) nread=widthbmp*4;
		else if (ncolor==24) nread=widthbmp*3;
		else if (ncolor==16) nread=widthbmp*2;
    f_close(&Myfiles);
		//f_closedir(&MyDir);
		// Write dimension & situation of bit	
		if(chcl==8)
		 {
			writsitu=888;
			f_write(&Mf,(uint8_t*)&writsitu,2,&byteCount1);
		 }
    else if(chcl==6)
		 {
			writsitu=565;
			f_write(&Mf,(uint8_t*)&writsitu,2,&byteCount1);
		 }
    else if(chcl==4)
		 {
			writsitu=444;
			f_write(&Mf,(uint8_t*)&writsitu,2,&byteCount1);
		 }
    		
		if(heightbmp>=widthbmp)
		 {
			hbd = heightbmp- TFT7735_LCD_PIXEL_HEIGHT;
			wbd = widthbmp - TFT7735_LCD_PIXEL_WIDTH;
			ws1= (widthbmp*10000)/TFT7735_LCD_PIXEL_WIDTH ;
			hs1= (heightbmp*10000)/TFT7735_LCD_PIXEL_HEIGHT;
			TFT7735_Position(180,0); 
		 }
		else 
		 {
			wbd = heightbmp- TFT7735_LCD_PIXEL_HEIGHT;
			hbd = widthbmp - TFT7735_LCD_PIXEL_WIDTH;
			hs1= (heightbmp*10000)/TFT7735_LCD_PIXEL_WIDTH ;
			ws1= (widthbmp*10000)/TFT7735_LCD_PIXEL_HEIGHT;
      TFT7735_Position(270,0);			 
		 }
		if    (hbd==0 && wbd==0) hws=1;
		else if(hbd==0 && wbd<0) hws=2;
		else if(hbd<0 && wbd==0) hws=3;
		else if(hbd<0 && wbd<0)  hws=4;
		else 	
		{
			if(hs1>=ws1)  hws=5;
			else   				hws=6;
		}
// Read Data
    switch (hws)
		{
			case 1:
			case 2:
			case 3:
      case 4:				
        
			  if(heightbmp>widthbmp)
				 {
					hs2=heightbmp;
					ws2=widthbmp;
					TFT7735_Position(180,0);
				 }
				else
         {
					ws2=heightbmp;
					hs2=widthbmp;
					TFT7735_Position(270,0);
				 }
				TFT7735_SetDisplayWindow(0,0,ws2,hs2); 
			  f_write(&Mf,(uint8_t*)&ws2,2,&byteCount1); 
			  f_write(&Mf,(uint8_t*)&hs2,2,&byteCount1);
				for(ch=0;ch<hs2;ch++)
				{
					f_open(&Myfiles,buffile2,FA_READ);
					f_read(&Myfiles,databuf,ofset,&byteCount);	
					for(f2=0;f2<(hs2-ch);f2++)
						{
						f_read(&Myfiles,databuf,nread,&byteCount);
						}
					for(cb=0;cb<ws2;cb++)
					 {	
						if(chcl==6)
						{
						 switch (bsitu)
						 {
							case 0:
							case 1:
								if(ncolor==32)
									{
										tmpr= databuf[cb*4+2]>>3;
										tmpg= databuf[cb*4+1]>>2;
										tmpb= databuf[cb*4]>>3;
									}
								else if(ncolor==24)
									{
										tmpr= databuf[cb*3+2]>>3;
										tmpg= databuf[cb*3+1]>>2;
										tmpb= databuf[cb*3]>>3;
									}	
							break;

							case 2:
							break;
					
							case 3:
								if(ncolor==32)
									{
										if (vers==2)
										{
											tmpr= databuf[cb*4+2]*databuf[cb*4+3]/256;
											tmpr>>=3;
											tmpg= databuf[cb*4+1]*databuf[cb*4+3]/256;
											tmpg>>=2;
											tmpb= databuf[cb*4]*databuf[cb*4+3]/256;
											tmpb>>=3;
										}
										else	
										{
											tmp1 = (databuf[cb*4+3]<<24)|(databuf[cb*4+2]<<16)|(databuf[cb*4+1]<<8)|databuf[cb*4];
											tmpr= tmp1&0x3FF00000;
											tmpr>>=20;
											tmpg= tmp1&0x000FFC00;
											tmpg>>=10;
											tmpb= tmp1&0x000003FF;
											tmpr>>=5;
											tmpr>>=4;
											tmpr>>=5;
										}
									}
								else if	(ncolor==24)
									{
										tmpr= databuf[cb*3+2]>>3;
										tmpg= databuf[cb*3+1]>>2;
										tmpb= databuf[cb*3]>>3;
									}
							break;	
							}
						if(crgb)
						 {
							buffer[cb*2]  = (tmpr<<3)|(tmpg>>3);
							buffer[cb*2+1]= (tmpg<<5)|tmpb;						 
						 }
						else
						 {	
							buffer[cb*2]  =(tmpb<<3)|(tmpg>>3);
							buffer[cb*2+1]=(tmpg<<5)|tmpr;
					   }
						}
					else if(chcl==4)
					 {
						switch (bsitu)
						 {
							case 0:
							case 1:
								if(ncolor==32)
									{
										tmpr=  databuf[cb*4+2]>>4;
										tmpg=  databuf[cb*4+1]>>4;
										tmpb=  databuf[cb*4]>>4;
										tmpr1= databuf[(cb+1)*4+2]>>4;
										tmpg1= databuf[(cb+1)*4+1]>>4;
										tmpb1= databuf[(cb+1)*4]>>4;
									}
								else if(ncolor==24)
									{
										tmpr=  databuf[cb*3+2]>>4;
										tmpg=  databuf[cb*3+1]>>4;
										tmpb=  databuf[cb*3]>>4;
										tmpr1= databuf[(cb+1)*3+2]>>4;
										tmpg1= databuf[(cb+1)*3+1]>>4;
										tmpb1= databuf[(cb+1)*3]>>4;
									}
							  ch++;
					    break;
							
							case 2:
							break;
					
							case 3:
								if(ncolor==32)
								{
									if (vers==2)
									{
										tmpr= databuf[cb*4+2]*databuf[cb*4+3]/256;
										tmpr>>=4;
										tmpg= databuf[cb*4+1]*databuf[cb*4+3]/256;
										tmpg>>=4;
										tmpb= databuf[cb*4]*databuf[cb*4+3]/256;
										tmpb>>=4;
										tmpr1= databuf[(cb+1)*4+2]*databuf[(cb+1)*4+3]/256;
										tmpr1>>=4;
										tmpg1= databuf[(cb+1)*4+1]*databuf[(cb+1)*4+3]/256;
										tmpg1>>=4;
										tmpb1= databuf[(cb+1)*4]*databuf[(cb+1)*4+3]/256;
										tmpb1>>=4;
									}
									else
									{
										tmp1 = (databuf[cb*4+3]<<24)|(databuf[cb*4+2]<<16)|(databuf[cb*4+1]<<8)|databuf[cb*4];
										tmpr= tmp1&0x3FF00000;
										tmpr>>=20;
										tmpg= tmp1&0x000FFC00;
										tmpg>>=10;
										tmpb= tmp1&0x000003FF;
										tmpr>>=6;
										tmpr>>=6;
										tmpr>>=6;
										tmp1 = (databuf[(cb+1)*4+3]<<24)|(databuf[(cb+1)*4+2]<<16)|(databuf[(cb+1)*4+1]<<8)|databuf[(cb+1)*4];
										tmpr1= tmp1&0x3FF00000;
										tmpr1>>=20;
										tmpg1= tmp1&0x000FFC00;
										tmpg1>>=10;
										tmpb1= tmp1&0x000003FF;
										tmpr1>>=6;
										tmpr1>>=6;
										tmpr1>>=6;
									}
								}									
								else if	(ncolor==24)
								{
									tmpr=  databuf[cb*3+2]>>3;
									tmpg=  databuf[cb*3+1]>>2;
									tmpb=  databuf[cb*3]>>3;
									tmpr1= databuf[(cb+1)*3+2]>>3;
									tmpg1= databuf[(cb+1)*3+1]>>2;
									tmpb1= databuf[(cb+1)*3]>>3;
								}
							ch++;	
							break;	
						}
					 if(crgb)
						 {
							buffer[cb*3]  = (tmpr<<4)|tmpg;
							buffer[cb*3+1]= (tmpb<<4)|tmpr1;
							buffer[cb*3+2]= (tmpg1<<4)|tmpb1; 
						 }
						else
						 {	
							buffer[cb*3]  = (tmpb<<4)|tmpg;
							buffer[cb*3+1]= (tmpr<<4)|tmpb1;
							buffer[cb*3+2]= (tmpg1<<4)|tmpr1;
						 }	
						}
				 }
			f_close(&Myfiles);	
			if(chcl==6)       f_write(&Mf,buffer,ws2*2,&byteCount1);
			else if(chcl==4)	f_write(&Mf,buffer,(ws2*3)/4,&byteCount1);			
			TFT7735_CMD(LCD_REG_43);			
			TFT7735_WDATA((uint8_t*) &ch, 2);
			TFT7735_CMD(LCD_REG_44);
			SPIx_WriteData((uint8_t*) buffer, 256);	
		 }	
			
			break;
			
			case 5:
			case 6:
				
			  if(heightbmp>=widthbmp)
				 { 
					if(hws==5)		
						{
						 ws2=(widthbmp*TFT7735_LCD_PIXEL_HEIGHT)/heightbmp;
						 hs2=TFT7735_LCD_PIXEL_HEIGHT;
						}
					else if(hws==6)		
						{
						 ws2=TFT7735_LCD_PIXEL_WIDTH;
						 hs2=(heightbmp*TFT7735_LCD_PIXEL_WIDTH)/widthbmp;
						}
					}
				else
				 {
					if(hws==5)		
						{
						 ws2=(widthbmp*TFT7735_LCD_PIXEL_WIDTH)/heightbmp;
						 hs2=TFT7735_LCD_PIXEL_WIDTH;	
						}
					else if(hws==6)		
						{
						 ws2=TFT7735_LCD_PIXEL_HEIGHT;
						 hs2=(heightbmp*TFT7735_LCD_PIXEL_HEIGHT)/widthbmp;
						}  
				 }
					//TFT7735_SetCursor(ws2,hs2);
					//writwh=(ws2<<16)|hs2;
				TFT7735_SetDisplayWindow(0,0,ws2,hs2);
			  f_write(&Mf,(uint8_t*)&ws2,2,&byteCount1);
				f_write(&Mf,(uint8_t*)&hs2,2,&byteCount1);
        ch2=0;
        conh=0;					
  			for(ch=0;ch<heightbmp;ch++)
				{
				 if(heightbmp>=widthbmp)
				  { 
				 	 if(hws==5)		
						{
						ch1=(((conh+1)*heightbmp)/TFT7735_LCD_PIXEL_HEIGHT)-ch2;
					  ch2+=ch1;
						}
					 else if(hws==6)		
						{
						ch1=(((conh+1)*widthbmp)/TFT7735_LCD_PIXEL_WIDTH)-ch2;
						ch2+=ch1;
						}
					}
				 else
				  {
					 if(hws==5)		
						{
						 ch1=(((conh+1)*heightbmp)/TFT7735_LCD_PIXEL_WIDTH)-ch2;
						 ch2+=ch1;
						}
					 else if(hws==6)		
						{
						 ch1=(((conh+1)*widthbmp)/TFT7735_LCD_PIXEL_HEIGHT)-ch2;
						 ch2+=ch1;
						}	  
					}	

					f_open(&Myfiles,buffile2,FA_READ);
					f_read(&Myfiles,databuf,ofset,&byteCount);	
					for(f1=0;f1<(heightbmp-ch);f1++)
						{
						 if (f1==(heightbmp-(ch+ch1))) {ch3=nread*ch1; f1+=20;}
						 else ch3=nread;
						 f_read(&Myfiles,databuf,ch3,&byteCount);
						}
					conw=0;
          cb2=0;						
					for(cb=0;cb<widthbmp;cb++)
					 {
						if(heightbmp>=widthbmp)
						 { 
							if(hws==5)		
							{
							 cb1=(((conw+1)*heightbmp)/TFT7735_LCD_PIXEL_HEIGHT)-cb2;
							 cb2+=cb1;
							}
							else if(hws==6)		
							{
							 cb1=(((conw+1)*widthbmp)/TFT7735_LCD_PIXEL_WIDTH)-cb2;
						   cb2+=cb1;
							}
						 }
						else
						 {
							if(hws==5)		
							{
							 cb1=(((conw+1)*heightbmp)/TFT7735_LCD_PIXEL_WIDTH)-cb2;
							 cb2+=cb1;
							}
							else if(hws==6)		
							{
							 cb1=(((conw+1)*widthbmp)/TFT7735_LCD_PIXEL_HEIGHT)-cb2;
							 cb2+=cb1;
							}	  
						 } 
 
						for(f2=0;f2<10;f2++)
						{
							tmpe[f2]=0;
						}
						 if(ncolor==32)
									{
									 for(f2=0;f2<ch1;f2++)
										{
											for(f3=0;f3<cb1;f3++)  {tmpe[0]+=databuf[nread*f2+(cb+f3)*4+3];}
											for(f3=0;f3<cb1;f3++)  {tmpe[1]+=databuf[nread*f2+(cb+f3)*4+2];}
											for(f3=0;f3<cb1;f3++)  {tmpe[2]+=databuf[nread*f2+(cb+f3)*4+1];}
											for(f3=0;f3<cb1;f3++)  {tmpe[3]+=databuf[nread*f2+(cb+f3)*4];}
										}
										tmpe[0]/=(ch1*cb1);
										tmpe[1]/=(ch1*cb1);
										tmpe[2]/=(ch1*cb1);
										tmpe[3]/=(ch1*cb1);
									}
								else if(ncolor==24)
									{
										for(f2=0;f2<ch1;f2++)
										{
											for(f3=0;f3<cb1;f3++)  {tmpe[0]+=databuf[nread*f2+(cb+f3)*3+2];}
											for(f3=0;f3<cb1;f3++)  {tmpe[1]+=databuf[nread*f2+(cb+f3)*3+1];}
											for(f3=0;f3<cb1;f3++)  {tmpe[2]+=databuf[nread*f2+(cb+f3)*3];}
										}
										tmpe[0]/=(ch1*cb1);
										tmpe[1]/=(ch1*cb1);
										tmpe[2]/=(ch1*cb1);
									}
						if(cb1>0) cb+=(cb1-1);		
						if(chcl==6)
						{
						 switch (bsitu)
						 {
							case 0:
							case 1:	
								if(ncolor==32)
									{
										tmpr= tmpe[1]>>3;
										tmpg= tmpe[2]>>2;
										tmpb= tmpe[3]>>3;
									}
								else if(ncolor==24)
									{
										tmpr= tmpe[0]>>3;
										tmpg= tmpe[1]>>2;
										tmpb= tmpe[2]>>3;
									}	
							break;

							case 2:
							break;
					
							case 3:
								if(ncolor==32)
									{
										if (vers==2)
										{
											tmpr= tmpe[1]*tmpe[0]/256;
											tmpr>>=3;
											tmpg= tmpe[2]*tmpe[0]/256;
											tmpg>>=2;
											tmpb= tmpe[3]*tmpe[0]/256;
											tmpb>>=3;
										}
										else	
										{
											tmp1 = (tmpe[0]<<24)|(tmpe[1]<<16)|(tmpe[2]<<8)|tmpe[3];
											tmpr= tmp1&0x3FF00000;
											tmpr>>=20;
											tmpg= tmp1&0x000FFC00;
											tmpg>>=10;
											tmpb= tmp1&0x000003FF;
											tmpr>>=5;
											tmpr>>=4;
											tmpr>>=5;
										}
									}
								else if	(ncolor==24)
									{
										tmpr= tmpe[0]>>3;
										tmpg= tmpe[1]>>2;
										tmpb=tmpe[2]>>3;
									}
							break;	
							}
						if(crgb)
						 {
							buffer[conw*2]  = (tmpr<<3)|(tmpg>>3);
							buffer[conw*2+1]= (tmpg<<5)|tmpb;						 
						 }
						else
						 {	
							buffer[conw*2]  =(tmpb<<3)|(tmpg>>3);
							buffer[conw*2+1]=(tmpg<<5)|tmpr;
					   }
						}
					else if(chcl==4)
					 {
						if(heightbmp>=widthbmp)
						 { 
							if(hws==5)		
							{
							 cb1=(((conw+1)*heightbmp)/TFT7735_LCD_PIXEL_HEIGHT)-cb2;
							 cb2+=cb1;
							}
							else if(hws==6)		
							{
							 cb1=(((conw+1)*widthbmp)/TFT7735_LCD_PIXEL_WIDTH)-cb2;
						   cb2+=cb1;
							}
						 }
						else
						 {
							if(hws==5)		
							{
							 cb1=(((conw+1)*heightbmp)/TFT7735_LCD_PIXEL_WIDTH)-cb2;
							 cb2+=cb1;
							}
							else if(hws==6)		
							{
							 cb1=(((conw+1)*widthbmp)/TFT7735_LCD_PIXEL_HEIGHT)-cb2;
							 cb2+=cb1;
							}	  
						 }  
						 if(ncolor==32)
									{
									 for(f2=0;f2<ch1;f2++)
										{
											for(f3=0;f3<cb1;f3++)  {tmpe[4]+=databuf[nread*f2+(cb+1+f3)*4+3];}
											for(f3=0;f3<cb1;f3++)  {tmpe[5]+=databuf[nread*f2+(cb+1+f3)*4+2];}
											for(f3=0;f3<cb1;f3++)  {tmpe[6]+=databuf[nread*f2+(cb+1+f3)*4+1];}
											for(f3=0;f3<cb1;f3++)  {tmpe[7]+=databuf[nread*f2+(cb+1+f3)*4];}
										}
										tmpe[4]/=(ch1*cb1);
										tmpe[5]/=(ch1*cb1);
										tmpe[6]/=(ch1*cb1);
										tmpe[7]/=(ch1*cb1);
									}
								else if(ncolor==24)
									{
										for(f2=0;f2<ch1;f2++)
										{
											for(f3=0;f3<cb1;f3++)  {tmpe[3]+=databuf[nread*f2+(cb+1+f3)*3+2];}
											for(f3=0;f3<cb1;f3++)  {tmpe[4]+=databuf[nread*f2+(cb+1+f3)*3+1];}
											for(f3=0;f3<cb1;f3++)  {tmpe[5]+=databuf[nread*f2+(cb+1+f3)*3];}
										}
										tmpe[3]/=(ch1*cb1);
										tmpe[4]/=(ch1*cb1);
										tmpe[5]/=(ch1*cb1);
									}
							if(cb1>0) cb+=(cb1-1);
						switch (bsitu)
						 {
							case 0:
							case 1:
								if(ncolor==32)
									{
										tmpr= tmpe[1]>>4;
										tmpg= tmpe[2]>>4;
										tmpb= tmpe[3]>>4;
										tmpr1=tmpe[5]>>4;
										tmpg1=tmpe[6]>>4;
										tmpb1=tmpe[7]>>4;
									}
								else if(ncolor==24)
									{
										tmpr= tmpe[0]>>4;
										tmpg= tmpe[1]>>4;
										tmpb= tmpe[2]>>4;
										tmpr1=tmpe[3]>>4;
										tmpg1=tmpe[4]>>4;
										tmpb1=tmpe[5]>>4;
									}		
					    break;
							
							case 2:
							break;
					
							case 3:
								if(ncolor==32)
								{
									if (vers==2)
									{
										tmpr= tmpe[1]*tmpe[0]/256;
										tmpr>>=4;
										tmpg= tmpe[2]*tmpe[0]/256;
										tmpg>>=4;
										tmpb= tmpe[3]*tmpe[0]/256;
										tmpb>>=4;
										tmpr1= tmpe[5]*tmpe[4]/256;
										tmpr1>>=4;
										tmpg1= tmpe[6]*tmpe[4]/256;
										tmpg1>>=4;
										tmpb1= tmpe[7]*tmpe[4]/256;
										tmpb1>>=4;
									}
									else
									{
										tmp1 = (tmpe[0]<<24)|(tmpe[1]<<16)|(tmpe[2]<<8)|tmpe[3];
										tmpr= tmp1&0x3FF00000;
										tmpr>>=20;
										tmpg= tmp1&0x000FFC00;
										tmpg>>=10;
										tmpb= tmp1&0x000003FF;
										tmpr>>=6;
										tmpr>>=6;
										tmpr>>=6;
										tmp1 = (tmpe[4]<<24)|(tmpe[5]<<16)|(tmpe[6]<<8)|tmpe[7];
										tmpr1= tmp1&0x3FF00000;
										tmpr1>>=20;
										tmpg1= tmp1&0x000FFC00;
										tmpg1>>=10;
										tmpb1= tmp1&0x000003FF;
										tmpr1>>=6;
										tmpr1>>=6;
										tmpr1>>=6;
									}
								}									
								else if	(ncolor==24)
								{
									tmpr=  tmpe[0]>>3;
									tmpg=  tmpe[1]>>2;
									tmpb=  tmpe[2]>>3;
									tmpr1= tmpe[3]>>3;
									tmpg1= tmpe[4]>>2;
									tmpb1= tmpe[5]>>3;
								}	
							break;	
						}
					 if(crgb)
						 {
							buffer[conw*3]  = (tmpr<<4)|tmpg;
							buffer[conw*3+1]= (tmpb<<4)|tmpr1;
							buffer[conw*3+2]= (tmpg1<<4)|tmpb1; 
						 }
						else
						 {	
							buffer[conw*3]  = (tmpb<<4)|tmpg;
							buffer[conw*3+1]= (tmpr<<4)|tmpb1;
							buffer[conw*3+2]= (tmpg1<<4)|tmpr1;
						 }	
						}
					conw++; 
				 }
			if(ch1>0)ch+=(ch1-1);
			f_close(&Myfiles);	 
			if(chcl==6)       f_write(&Mf,buffer,ws2*2,&byteCount1);
			else if(chcl==4)	f_write(&Mf,buffer,(ws2*3)/4,&byteCount1);
    
			if(heightbmp>=widthbmp)  
			{
				TFT7735_CMD(LCD_REG_43);			
			  TFT7735_WDATA((uint8_t*) &conh, 2);
			  TFT7735_CMD(LCD_REG_44);
			  SPIx_WriteData((uint8_t*) buffer, ws2*2);
			}
      else  
			{				 
				TFT7735_CMD(LCD_REG_43);			
				TFT7735_WDATA((uint8_t*) &conh, 2);
				TFT7735_CMD(LCD_REG_44);
				SPIx_WriteData((uint8_t*) buffer, ws2*2);
			}
      conh++;					 
		 }	
		break;
		}

	f_close(&Mf);
	//f_closedir(&MyDir);
  HAL_Delay(2000);
		
	TFT7735_CLS_LR(TFT7735_GREEN);
	//f_opendir(&MyDir,"/dance1");
    buffile2[5]='i';
		buffile2[6]='n';		
	f_open(&Myfiles,buffile2,FA_READ);
	f_read(&Myfiles,buffer,6,&byteCount);	
  for(ch=0;ch<hs2;ch++)
  {
   f_read(&Myfiles,buffer,ws2*2,&byteCount);
	 TFT7735_CMD(LCD_REG_43);			
   TFT7735_WDATA((uint8_t*) &ch, 2);
   TFT7735_CMD(LCD_REG_44);
	 SPIx_WriteData((uint8_t*) buffer, ws2*2);
	}		
	f_close(&Myfiles);
	//f_closedir(&MyDir);
  HAL_Delay(1000);
	TFT7735_CLS_LR(TFT7735_MAGENTA);
	HAL_Delay(1000);
	}
	
	
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
	// PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV8;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_RTC_Init(void)
{
  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef DateToUpdate = {0};

  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  sTime.Hours = 11;
  sTime.Minutes = 59;
  sTime.Seconds = 50;

  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  DateToUpdate.WeekDay = RTC_WEEKDAY_WEDNESDAY;
  DateToUpdate.Month = RTC_MONTH_JANUARY;
  DateToUpdate.Date = 16;
  DateToUpdate.Year = 19;

  if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_SDIO_SD_Init(void)
{

  hsd.Instance = SDIO;
  hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
  hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
  hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
  hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
  hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd.Init.ClockDiv = 15;//SDIO_TRANSFER_CLK_DIV;
}

static void MX_SPI3_Init(void)
{
  hspi3.Instance = SPI3;
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
  hspi3.Init.CRCPolynomial = 15;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_TIM6_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 639;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 499;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  static uint8_t f=0,ff=0;
	static uint16_t t2=0;
	static uint32_t tmp1=0;
	
	
  if (htim->Instance == TIM7) {
    HAL_IncTick();
  }
  if (htim->Instance == TIM6) {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
		if(ff==0){
		if (tmp1<=t2) {
			tmp1+=350;
			f++;
			if(f>25)
			{
				ff=1;
			}
		}
		else {
				t2+=(60*f);
		}
	}
		else {
			    if (tmp1>=t2) {
			    tmp1-=350;
		    	f--;
					if(f<2)
					 {
						ff=0;
			     }
					}
					else {
							t2-=(60*f);
					}
				}					
			
		TIM6->ARR = tmp1;
  }
}

void Error_Handler(void)
{

}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{ 

}
#endif 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
