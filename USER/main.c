#include "ws281x.h"
#include "delay.h"
#include "sys.h"
#include "pwm.h"
#include "timer.h"
#include "OLED_I2C.h"
#include "stdio.h"
#include "string.h"
#include "usart.h"	

#include "step.h"
#include "RTC.h" 	
void key(void);
uint8_t fTempString[16] = {0};
uint8_t fTempString1[16] = {0};
uint8_t fTempString2[16] = {0};
uint8_t fTempString3[16] = {0};
uint8_t fTempString4[16] = {0};
uint8_t fTempString5[16] = {0};
uint8_t LED_R=50,LED_G=50,LED_B=50,LED_num=1;
int main(void)
{
	uint8_t t,i;
	    SystemInit();        //初始化RCC 设置系统主频为72MHZ
					delay_init(72);
	
     PWM_Init(89);
     ws281x_closeAll(); 
	    delay_ms(1000);
					
		   TIM3_Int_Init(99,7199);  //10ms		
	I2C_Configuration();//oled串口配置
	OLED_Init();//OLED屏初始化	
	RTC_Init();	  			//RTC初始化
	RTC_Set(2024,1,3,14,6,1);  //设置时间	
uart2_init(9600);//蓝牙
	key_GPIO_Init();
	OLED_Fill(0xFF);
	delay_ms(1000);
	OLED_Fill(0x00);	
	    while(1)
     {			
			 			  key( );
				if(t!=calendar.sec)
					{
						t=calendar.sec;
						sprintf((char *)fTempString1,"%d-%d-%d",calendar.w_year,calendar.w_month,calendar.w_date);
						OLED_ShowStr(32,0,fTempString1,2);	  //oled显示
						delay_ms(10);
						sprintf((char *)fTempString,"%d-%d-%d",calendar.hour,calendar.min,calendar.sec);
						OLED_ShowStr(40,2,fTempString,2);	  //oled显示		
					}			 
					sprintf((char *)fTempString2,"R:%d G:%d B:%d",LED_R, LED_G, LED_B);
					OLED_ShowStr(0,4,fTempString2,2);	  //oled显示	
					sprintf((char *)fTempString4,"LED_num:%d",LED_num*2);
					OLED_ShowStr(0,6,fTempString4,2);	  //oled显示
					ws281x_setPixelColor_ringleds(LED_num,ws281x_color(LED_R, LED_G, LED_B));
					ws281x_show();
					
				if(i%100 == 0)
			{
				OLED_CLS();	  //oled显示	
			}
			i++;
     }
}
void USART2_IRQHandler(void)                	//串口2中断服务程序
	{
		char Res;
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收模块返回的数据
		{    
			Res=USART_ReceiveData(USART2);//接收模块的数据;
		//	printf("%d\r\n",Res);
			if(Res==1)
			{
				  LED_num++;
				if(LED_num>=14)
				{
					LED_num=0;
				}
			}		
			if(Res==2)
			{
				  LED_R=LED_R+10;
								if(LED_R>249)
				{
					LED_R=0;
				}
			}			
			if(Res==3)
			{
				 LED_G=LED_G+10;
					if(LED_G>249)
				{
					LED_G=0;
				}
			}			
			if(Res==4)
			{
				  LED_B=LED_B+10;
									if(LED_B>249)
				{
					LED_B=0;
				}
			}
 
    } 
 

} 
void key(void)
{
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12) == 0)	//	检查按键是否按下
			{
				delay_ms(10);	//	消抖
				if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12) == 0)	//	再次判断按键是否按下
					{
							LED_num++;
						if(LED_num>=14)
						{
							LED_num=0;
						}
						 
						while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12) == 0);		//	开关松开检测
					}
			}
						if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13) == 0)	//	检查按键是否按下
			{
				delay_ms(10);	//	消抖
				if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13) == 0)	//	再次判断按键是否按下
					{
 				  LED_R=LED_R+10;
								if(LED_R>249)
				{
					LED_R=0;
				}
						 
						while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13) == 0);		//	开关松开检测
					}
			}
						if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14) == 0)	//	检查按键是否按下
			{
			delay_ms(10);	//	消抖
				if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14) == 0)	//	再次判断按键是否按下
					{
 				 LED_G=LED_G+10;
					if(LED_G>249)
				{
					LED_G=0;
				}
						 
						while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14) == 0);		//	开关松开检测
					}
			}
if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15) == 0)	//	检查按键是否按下
			{
				delay_ms(10);	//	消抖
				if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15) == 0)	//	再次判断按键是否按下
					{
 				  LED_B=LED_B+10;
									if(LED_B>249)
				{
					LED_B=0;
				}
					 
						while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15) == 0);		//	开关松开检测
					}
			}
	
}