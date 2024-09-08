//Copright (c) 2018,BT_Combine_speaker
//All rights reserved.

//名称: pwm.c
//说明: pwm程序
//作者: feeling1791 (	Wechat)
//日期: 2018-12-20
//版本: ver1.0 



#include "pwm.h"
#include "ws281x.h"

void PWM_Init(uint16_t arr)
{
     //结构体变量
	    GPIO_InitTypeDef  GPIO_InitStructure;
	    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	    TIM_OCInitTypeDef  TIM_OCInitStructure;
	    DMA_InitTypeDef DMA_InitStructure;
	
	    //使能RCC时钟
    	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);		//使能PORTB时钟	
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);			//使能TIM4时钟    
	    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);				//使能DMA1时钟    
	
	    //初始化GPIO口
					//GPIO_PinRemapConfig(GPIO_FullRemap_TIM4, ENABLE);
    	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;					//GPIOB PB8
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//速度50MHz
    	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//复用推挽输出                               
	    GPIO_Init(GPIOA,&GPIO_InitStructure);				//根据指定的参数初始化GPIO口
	
	    //初始化TIM4_CH3
	    TIM_TimeBaseStructure.TIM_Period=arr;												//自动重装载值					
	    TIM_TimeBaseStructure.TIM_Prescaler=0;											//定时器分频：(0+1)=1,不分频
	    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;				//时钟分割					
	    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		//向上计数模式

	    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);							//根据指定的参数初始化TIM2
						
	    //初始化pwm4 Channel1 PWM模式
	    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 						//选择定时器模式:TIM脉冲宽度调制模式1
 	   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    	TIM_OCInitStructure.TIM_Pulse = 0;													//待装入捕获比较寄存器的脉冲值(此程序不用加初值)
	    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 		//输出极性:TIM输出比较极性高
					
	    TIM_OC1Init(TIM2, &TIM_OCInitStructure);  										//根据指定的参数初始化外设TIM4 Channel3
	    /* 此处为知识点所描述处的对应代码，一定要有。移植时注意0C3代表定时器通道3*/
	    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  						//使能TIM4在CCR3上的预装载寄存器
    	//TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);
					
     TIM_ARRPreloadConfig(TIM2,ENABLE);//						
    	TIM_Cmd(TIM2, ENABLE);  									//失能TIM4
					
					
	   	/* DMA1 Channel5 Config for PWM by TIM4_CH3*/
	    DMA_DeInit(DMA1_Channel5);
	    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&TIM2->CCR1); 				//设置DMA目的地址
  	  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)pixelBuffer;					//设置DMA源地址
 	   DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;													//方向：从内存SendBuff到内存ReceiveBuff  
 	   DMA_InitStructure.DMA_BufferSize = ((PIXEL_NUM+ RESET_NUM) * 24); 															//一次传输大小DMA_BufferSize=SendBuffSize   
  	  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 						//ReceiveBuff地址不增
    	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;											//SendBuff地址自增
  	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//ReceiveBuff数据单位,16bit
  	  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;					//SENDBUFF_SIZE数据单位,16bit
    	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;																//DMA模式：正常模式(传输一次)
  	  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;												//优先级：中 
  	  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 																//内存到内存的传输
					
  	  DMA_Init(DMA1_Channel5, &DMA_InitStructure);																//配置DMA1的5通道(不同定时器的通道不一样)
    
	    TIM_DMACmd(TIM2, TIM_DMA_CC1, ENABLE);//TIM捕获/比较4DMA源
	    TIM_DMACmd(TIM2, TIM_DMA_Update, ENABLE); //TIM4 DMA请求，对应DMA1_CH5

	
}


