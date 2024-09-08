#ifndef  __STEP_H__
#define __STEP_H__
 
#include "stm32f10x.h"



void YU_GPIO_Init(void);
void Step_Motor_GPIO_Init(void);
void key_GPIO_Init(void);
void SetMotor_K(void);
void SetMotor_B(void);
void KEY_Exti_Init(void);
void worn_K(void);
void worn_B(void);
void xun_GPIO_Init(void);

#define L1		  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)
#define R1		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)
#define L2		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)
#define R2		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)
#define biz		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)

#endif
