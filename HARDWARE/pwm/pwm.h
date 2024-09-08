#ifndef __PWM_H
#define __PWM_H
#include "stm32f10x.h"

#define TIM2_CCR4_Address 0x40000040



extern void PWM_Init(uint16_t arr);


#endif
