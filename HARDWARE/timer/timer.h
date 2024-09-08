//Copright (c) 2021,BT_Combine_speaker
//All rights reserved.

//名称: timer.c
//说明: 定时器程序
//作者: feeling1791 (	Wechart)
//日期: 2018-12-20
//版本: ver1.0 


#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"



extern unsigned char color_R[65];
extern unsigned char color_G[65];
extern unsigned char color_B[65];

extern u16 ws281x_led_display_time;
extern u8 ws281x_led_display_mode;

extern u16 ws281x_whirl_speed;
extern u8 ws281x_whirl_index;
extern u16 ws281x_whirl_color_speed;
extern u8 ws281x_whirl_color_index;     //list
extern u8 ws281x_whirl_colortran_index;  




extern u16 ws281x_freq_speed;
extern u8 ws281x_freq_index;
extern u8 ws281x_freq_num;
extern u8 ws281x_freq_color_type;


extern u8 ws281x_bln_mode;
extern u8 ws281x_bln_speed;
extern u16 ws281x_bln_num;
extern u16 ws281x_bln_oldnum;

extern u16 ws281x_crawling_speed;
extern u16 ws281x_crawling_num;

extern u16 ws281x_transition_speed;
extern u16 ws281x_transition_num;

extern u16 ws281x_rainbow_speed;
extern u16 ws281x_rainbow_num;

extern void TIM3_Int_Init(u16 arr,u16 psc); 

#endif
