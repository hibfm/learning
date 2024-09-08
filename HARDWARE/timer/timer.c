//Copright (c) 2018,BT_Combine_speaker
//All rights reserved.

//名称: timer.c
//说明: 定时器程序
//作者: feeling1791 (	Wechat)
//日期: 2018-12-20
//版本: ver1.0 

#include "timer.h"
#include "ws281x.h"

unsigned char color_R[65] = { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		240, 220, 200, 180, 160, 140, 120, 100, 80, 60, 40, 20, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240 };
unsigned char color_G[65] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 };
unsigned char color_B[65] = { 0, 20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
		240, 220, 200, 180, 160, 140, 120, 100, 80, 60, 40, 20, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };


u16 ws281x_led_display_time = 0;
u8 ws281x_led_display_mode = 1;
extern u8 flag1;


u16 ws281x_freq_speed = 0;
u8 ws281x_freq_index = 0;
u8 ws281x_freq_num = 0;
u8 ws281x_freq_color_type = 1;     //1:red, 2:green,3:blue,: 4:white

u8 ws281x_bln_mode = 1;
u8 ws281x_bln_speed = 0;
u16 ws281x_bln_num = 255;
u16 ws281x_bln_oldnum = 0;



u16 ws281x_whirl_speed_auto = 0;
u8 ws281x_whirl_speed_val_index = 0;
u8 ws281x_whirl_speed_val = 10;


u16 ws281x_whirl_speed = 0;
u8 ws281x_whirl_index = 0;

u16 ws281x_whirl_color_speed = 0;
u8 ws281x_whirl_color_index = 0;     //list
u8 ws281x_whirl_colortran_index = 0;     //0-64-0




u16 ws281x_crawling_speed = 0;
u16 ws281x_crawling_num = 0;

u16 ws281x_transition_speed = 0;
u16 ws281x_transition_num = 0;

u16 ws281x_rainbow_speed = 0;
u16 ws281x_rainbow_num = 0;





void TIM3_Int_Init(u16 arr,u16 psc)
{
     TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	   NVIC_InitTypeDef NVIC_InitStructure;

	   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	   TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	   TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	   TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	   TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	   TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	   NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //先占优先级0级
	   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	   NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	   TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设							 
}

void TIM3_IRQHandler(void)   //TIM3中断   10ms
{

     if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	   {
	        TIM_ClearITPendingBit(TIM3, TIM_IT_Update  ); 
									
									ws281x_led_display_time++;
									if(ws281x_led_display_time > 3000)   //30S 
									{					
										ws281x_led_display_time = 0;
													if(flag1 == 0)
													{
														
													ws281x_led_display_mode++;
   											  if(	ws281x_led_display_mode > 3)
														{
														      ws281x_led_display_mode = 1; 
														}	
													}														
									}
										


          ws281x_whirl_speed_auto++;
											if(ws281x_whirl_speed_auto > 50)   //500ms
											{					 
															ws281x_whirl_speed_auto = 0;
															
															ws281x_whirl_speed_val_index++;
															if(ws281x_whirl_speed_val_index > 6)      //1234321 change speed val
															{
															    ws281x_whirl_speed_val_index = 0; 
															}
															if( ws281x_whirl_speed_val_index < 4)
															{
															    ws281x_whirl_speed_val = ws281x_whirl_speed_val_index + 1;
															}
															else
															{
															    ws281x_whirl_speed_val  = 7 - ws281x_whirl_speed_val_index  ; 
															
															}
																																									
											}


										
									 /*******************************************************************/	
												ws281x_whirl_speed++;		 
											//if(ws281x_whirl_speed > 2)   //20ms add:Rotation speed changes automatically
											if(ws281x_whirl_speed > ws281x_whirl_speed_val)   //20ms add:Rotation speed changes automatically											
											{					 
															ws281x_whirl_speed = 0;
																											
															ws281x_whirl_index++;													
															if(ws281x_whirl_index >= PIXEL_NUM )    //0~15
															{
																					ws281x_whirl_index = 0;			
															}
														//	ws281x_setPixelColor_wheel_leds(ws281x_whirl_index,
														//	                                ws281x_color(color_R[ws281x_whirl_colortran_index], 
															//                                             color_G[ws281x_whirl_colortran_index], 
															//																																													color_B[ws281x_whirl_colortran_index])) ;	
								      // ws281x_show();		
            }
												/*******************************************************************/		
															ws281x_whirl_color_speed++;               //64 color transition
															if(ws281x_whirl_color_speed > 10)       //100ms
															{
															        ws281x_whirl_color_speed = 0;
																							
																							ws281x_whirl_color_index++;
																							if(ws281x_whirl_color_index > 128)
																							{
																							     ws281x_whirl_color_index = 0;
																							}
																							
																							if( ws281x_whirl_color_index <= 64)
																							{
                             ws281x_whirl_colortran_index = ws281x_whirl_color_index;
																							}
																							else
																							{
																							      ws281x_whirl_colortran_index = 128 - ws281x_whirl_color_index;
																							}                        																							
															}																					
									 /*******************************************************************/	
												ws281x_freq_speed++;		 
											if(ws281x_freq_speed > 20)   //200MS 
											{					 
															ws281x_freq_speed = 0;
															
															ws281x_freq_index++;													
															if(ws281x_freq_index > PIXEL_NUM )    //0~16
															{
																					ws281x_freq_index = 0;			
                     ws281x_freq_color_type++;
																					if(ws281x_freq_color_type > 4)
																					{
																					      ws281x_freq_color_type = 1;
																											
																					}
															}

															// 0,1,2,3,4,5,6,7,8,7,6,5,4,3,2,1,0 change
															ws281x_freq_num  =  PIXEL_NUM  - ws281x_freq_index;															
															if(ws281x_freq_num >= (PIXEL_NUM / 2))
															{
																						ws281x_freq_num = PIXEL_NUM - ws281x_freq_num;
															}	


														//	ws281x_colorWipe_freq(ws281x_freq_color_type);

															
											}
															
          /*******************************************************************/	
											ws281x_bln_speed++;
											if(ws281x_bln_speed > 1)   //10ms
											{			 
															ws281x_bln_speed = 0;	

															ws281x_bln_num += 2;	//																														
															if(ws281x_bln_num > 255)
															{												      
																					ws281x_bln_num = 0;
																								
//																									if( ws281x_bln_mode < 3)    //bln times
//																									{
//																															ws281x_bln_mode++;
//																									}
//																									else
//																									{
//																														ws281x_bln_mode = 1;
//																									}																																																																						
															}	
														
										 }
							   /*******************************************************************/	
							
							

	   }
}












