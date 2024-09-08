//Copright (c) 2018,BT_Combine_speaker
//All rights reserved.

//����: pwm.c
//˵��: pwm����
//����: feeling1791 (	Wechat)
//����: 2018-12-20
//�汾: ver1.0 



#include "pwm.h"
#include "ws281x.h"

void PWM_Init(uint16_t arr)
{
     //�ṹ�����
	    GPIO_InitTypeDef  GPIO_InitStructure;
	    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	    TIM_OCInitTypeDef  TIM_OCInitStructure;
	    DMA_InitTypeDef DMA_InitStructure;
	
	    //ʹ��RCCʱ��
    	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);		//ʹ��PORTBʱ��	
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);			//ʹ��TIM4ʱ��    
	    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);				//ʹ��DMA1ʱ��    
	
	    //��ʼ��GPIO��
					//GPIO_PinRemapConfig(GPIO_FullRemap_TIM4, ENABLE);
    	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;					//GPIOB PB8
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//�ٶ�50MHz
    	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//�����������                               
	    GPIO_Init(GPIOA,&GPIO_InitStructure);				//����ָ���Ĳ�����ʼ��GPIO��
	
	    //��ʼ��TIM4_CH3
	    TIM_TimeBaseStructure.TIM_Period=arr;												//�Զ���װ��ֵ					
	    TIM_TimeBaseStructure.TIM_Prescaler=0;											//��ʱ����Ƶ��(0+1)=1,����Ƶ
	    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;				//ʱ�ӷָ�					
	    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		//���ϼ���ģʽ

	    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);							//����ָ���Ĳ�����ʼ��TIM2
						
	    //��ʼ��pwm4 Channel1 PWMģʽ
	    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 						//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
 	   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    	TIM_OCInitStructure.TIM_Pulse = 0;													//��װ�벶��ȽϼĴ���������ֵ(�˳����üӳ�ֵ)
	    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 		//�������:TIM����Ƚϼ��Ը�
					
	    TIM_OC1Init(TIM2, &TIM_OCInitStructure);  										//����ָ���Ĳ�����ʼ������TIM4 Channel3
	    /* �˴�Ϊ֪ʶ�����������Ķ�Ӧ���룬һ��Ҫ�С���ֲʱע��0C3����ʱ��ͨ��3*/
	    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  						//ʹ��TIM4��CCR3�ϵ�Ԥװ�ؼĴ���
    	//TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);
					
     TIM_ARRPreloadConfig(TIM2,ENABLE);//						
    	TIM_Cmd(TIM2, ENABLE);  									//ʧ��TIM4
					
					
	   	/* DMA1 Channel5 Config for PWM by TIM4_CH3*/
	    DMA_DeInit(DMA1_Channel5);
	    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&TIM2->CCR1); 				//����DMAĿ�ĵ�ַ
  	  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)pixelBuffer;					//����DMAԴ��ַ
 	   DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;													//���򣺴��ڴ�SendBuff���ڴ�ReceiveBuff  
 	   DMA_InitStructure.DMA_BufferSize = ((PIXEL_NUM+ RESET_NUM) * 24); 															//һ�δ����СDMA_BufferSize=SendBuffSize   
  	  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 						//ReceiveBuff��ַ����
    	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;											//SendBuff��ַ����
  	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//ReceiveBuff���ݵ�λ,16bit
  	  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;					//SENDBUFF_SIZE���ݵ�λ,16bit
    	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;																//DMAģʽ������ģʽ(����һ��)
  	  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;												//���ȼ����� 
  	  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 																//�ڴ浽�ڴ�Ĵ���
					
  	  DMA_Init(DMA1_Channel5, &DMA_InitStructure);																//����DMA1��5ͨ��(��ͬ��ʱ����ͨ����һ��)
    
	    TIM_DMACmd(TIM2, TIM_DMA_CC1, ENABLE);//TIM����/�Ƚ�4DMAԴ
	    TIM_DMACmd(TIM2, TIM_DMA_Update, ENABLE); //TIM4 DMA���󣬶�ӦDMA1_CH5

	
}


