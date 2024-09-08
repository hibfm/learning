//Copright (c) 2021,FFT_Combine
//All rights reserved.

//名称: delay.h
//说明: 函数声明
//作者: Zhang Shaobin
//日期: 2020-12-20
//版本: ver1.0 	
#ifndef __DELAY_H
#define __DELAY_H 			   
#include "stm32f10x.h"
 
extern void delay_init(u8 SYSCLK);
extern void delay_ms(u16 nms);
extern void delay_us(u32 nus);

#endif





























