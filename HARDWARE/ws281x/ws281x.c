//Copright (c) 2018,BT_Combine_speaker
//All rights reserved.

//名称: ws281x.c
//说明: WS281x程序
//作者: feeling1791 (	Wechat)
//日期: 2018-12-20
//版本: ver1.0 

#include "ws281x.h"
#include "delay.h"
#include "timer.h"


u16 pixelBuffer[PIXEL_NUM + RESET_NUM][24] = {0};

void ws281x_show(void)
{		
     DMA_SetCurrDataCounter(DMA1_Channel5,(PIXEL_NUM + RESET_NUM)* 24 );
					
 	   DMA_Cmd(DMA1_Channel5, ENABLE); 
     TIM_Cmd(TIM4, ENABLE);
				

	    while(DMA_GetFlagStatus(DMA1_FLAG_TC5) !=SET); 
     DMA_Cmd(DMA1_Channel5, DISABLE ); 
     DMA_ClearFlag(DMA1_FLAG_TC5); 
     TIM_Cmd(TIM4, DISABLE);							
}

void ws281x_closeAll(void)
{
     uint16_t i;
     uint8_t j;  
     for(i = 0; i < PIXEL_NUM; i++)
     {
         for(j = 0; j < 24; j++)
         {
              pixelBuffer[i][j] = WS_LOW;
         }
     }
					ws281x_show();
}

uint32_t ws281x_color(uint8_t red, uint8_t green, uint8_t blue)
{
     return ((green << 16) | (red << 8) | blue);
}


void ws281x_setPixelColor(uint16_t n ,uint32_t GRBcolor)
{
     uint8_t i;
					if(n < PIXEL_NUM)
     {
          for(i = 0; i < 24; i++)
          {									
										     pixelBuffer[n][i] = ((GRBcolor << i) & 0X800000) ? WS_HIGH : WS_LOW;
          }
     }		
}



/**********************************************************************************************************/
// level
//  8               08   09
//  7            07         10
//  6          06             11
//  5         05               12
//  4         04               13
//  3          03             14
//  2            02         15
//  1               01  16
//  0 off             
void ws281x_setPixelColor_ringleds(uint16_t n ,uint32_t GRBcolor)            //PIXEL_NUM:12/16/2*n Ring leds
{
       uint8_t i,k,p;
							
							for(i = 0; i < 24; i++)
						 {			
													for(k = 0;(k < n) && (n <= (PIXEL_NUM / 2));k++)
													{
													     pixelBuffer[k][i] = ((GRBcolor << i) & 0X800000) ? WS_HIGH : WS_LOW;
													     pixelBuffer[(PIXEL_NUM - 1) - k][i] = ((GRBcolor << i) & 0X800000) ? WS_HIGH : WS_LOW;												
													}
													for(p = n;(p < ( PIXEL_NUM - n)) && (n <= (PIXEL_NUM / 2)) ;p++)
													{
													     pixelBuffer[p][i]  =   WS_LOW;                 																										
													}																																																																			
							}
}

// ws281x_colorWipe_lr(ws281x_color(50, 0, 0), ws281x_freq_num); // red
//  fill the dots one by one with a color on left and right sides at the same time
void ws281x_colorWipe_lr( uint32_t c, uint8_t num)
{
    uint16_t i; 
				if(num > PIXEL_NUM / 2)
				{
				     num = PIXEL_NUM / 2;
				}
    for(i = 0; i < (num + 1); i++)         //
		  {						
								ws281x_setPixelColor_ringleds(i ,c); 
		      ws281x_show();
        //delay_ms(wait);	  //>	0.05ms				will should be using timer		
    }
}


//Freq rising or falling lights
void ws281x_colorWipe_freq(uint8_t type)
{	
														
         	switch(type)
										{
										       case 1: 
                        ws281x_colorWipe_lr(ws281x_color(50, 0, 0), ws281x_freq_num); // red
																								break;
											      case 2: 
                        ws281x_colorWipe_lr(ws281x_color(0, 50, 0), ws281x_freq_num); // green
																								break;																				
																	case 3: 
                        ws281x_colorWipe_lr(ws281x_color(0, 0, 50), ws281x_freq_num); // Blue
																								break;		
																	case 4: 
                        ws281x_colorWipe_lr(ws281x_color(50, 50, 50), ws281x_freq_num); // white
																								break;		
																							
																default:
                        ws281x_colorWipe_lr(ws281x_color(50, 0, 0), ws281x_freq_num); // red
																								break;					
										}   										
}




/***************************************************************************************************/
uint32_t ws281x_colorTran(uint8_t tran,uint8_t color_mode)  //The colour is a transition r/g/b back to r/g/b(color_mode:Red1,Green2,Blue3). space 3
{
     tran = 255 - tran;
     if(tran < 85)
					{
					     switch(color_mode)
										{
										       case 1: 
                        return ws281x_color(tran * 3, 0, 0);
											      case 2: 
                        return ws281x_color(0, tran * 3, 0);																						
																	case 3: 
                        return ws281x_color(0, 0, tran * 3);								
																default:
																        return ws281x_color(tran * 3, 0, 0);						
										}
     }
					
     if(tran < 170) 
					{
          tran -= 85;
										switch(color_mode)
										{
										       case 1: 
                        return ws281x_color(255, 0, 0);
											      case 2: 
                        return ws281x_color(0,255, 0);																						
																	case 3: 
                        return ws281x_color(0, 0, 255);								
																default:
																        return ws281x_color(255, 0, 0);						
										}
     }
     tran -= 170;
					switch(color_mode)
					{
												case 1: 
																			return ws281x_color(255 - tran * 3, 0, 0);
												case 2: 
																			return ws281x_color(0, 255 - tran * 3, 0);																						
												case 3: 
																			return ws281x_color(0, 0, 255 - tran * 3);								
											default:
																			return ws281x_color(255 - tran * 3, 0, 0);																		
					}
}


void ws281x_colorblnCtrl(uint8_t color_mode)   // ws281x_bln_num use timer,mode:red1/green2/blue3 
{
     uint16_t i;

											if( !(ws281x_bln_num == ws281x_bln_oldnum))
											{
															for(i = 0; i < PIXEL_NUM; i++)
															{
																			ws281x_setPixelColor(i ,ws281x_colorTran(ws281x_bln_num,color_mode));
															}
															ws281x_show();
															ws281x_bln_oldnum = ws281x_bln_num;
											}
				
}
/***************************************************************************************************/

void ws281x_setPixelColor_wheel_leds(uint16_t n ,uint32_t GRBcolor)            //n:0~15 whirling leds
{
       uint8_t i,k;
							
							for(i = 0; i < 24; i++)
						 {	
                  for(k = 0;k < PIXEL_NUM;k++)
																		{																									
																									pixelBuffer[k][i]  =   WS_LOW; 																							
																		}    							

													     pixelBuffer[n][i] = ((GRBcolor << i) & 0X800000) ? WS_HIGH : WS_LOW;
																		if( n < PIXEL_NUM/2)
																		{
																		     pixelBuffer[n + (PIXEL_NUM/2)][i] = ((GRBcolor << i) & 0X800000) ? WS_HIGH : WS_LOW;	
																		}
																		else
																		{
																		     pixelBuffer[n - (PIXEL_NUM/2)][i] = ((GRBcolor << i) & 0X800000) ? WS_HIGH : WS_LOW;	
																		}																																																															
							}
							ws281x_show();	
}

void ws281x_display(void)
{
     				if(ws281x_led_display_mode == 1)
									{
									ws281x_colorblnCtrl(ws281x_bln_mode);
									}
									
//									if(ws281x_led_display_mode == 2)
//									{
//									ws281x_colorWipe_freq(ws281x_freq_color_type);
//									}
//									if(ws281x_led_display_mode == 3)
//									{
//													ws281x_setPixelColor_wheel_leds(ws281x_whirl_index,
//																																													ws281x_color(color_R[ws281x_whirl_colortran_index], 
//																																																										color_G[ws281x_whirl_colortran_index], 
//																																																										color_B[ws281x_whirl_colortran_index])) ;	
//									}     
}







/*
void ws281x_setPixelColor_11x44leds(uint16_t n ,uint32_t GRBcolor)            //PIXEL_NUM:11x44 leds  n:0~10
{
       uint8_t i,k,p,q;
							
							for(i = 0; i < 24; i++)
						 {			
													for(k = 0;k < 44;k++)
													{
													      for(q = 0 ;q < (n +1);q++)
																			{
																		      pixelBuffer[q + k*11][i] = ((GRBcolor << i) & 0X800000) ? WS_HIGH : WS_LOW;																						
																	 	}
																			for(p = 1;p < (10-n);p++)
																		 {
																							 pixelBuffer[(n + p) + k*11][i]  =   WS_LOW;                 																										
																		 }																									
													}																																																															
							}
}
*/


