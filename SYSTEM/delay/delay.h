//Copright (c) 2021,FFT_Combine
//All rights reserved.

//����: delay.h
//˵��: ��������
//����: Zhang Shaobin
//����: 2020-12-20
//�汾: ver1.0 	
#ifndef __DELAY_H
#define __DELAY_H 			   
#include "stm32f10x.h"
 
extern void delay_init(u8 SYSCLK);
extern void delay_ms(u16 nms);
extern void delay_us(u32 nus);

#endif





























