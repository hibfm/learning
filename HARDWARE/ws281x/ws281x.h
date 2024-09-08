
#ifndef __WS281X_H
#define __WS281X_H
#include "stm32f10x.h"

#define PIXEL_NUM 24 
#define RESET_NUM 2
    
                         //           "0"                "1"
#define WS_HIGH 51       //  |~~~~|__________|   |~~~~~~~~|______|
#define WS_LOW  25       //   0.35    0.8            0.7    0.6



extern u16 pixelBuffer[PIXEL_NUM + RESET_NUM][24];


void ws281x_init(void);
void ws281x_closeAll(void);
void ws281x_show(void);

uint32_t ws281x_color(uint8_t red, uint8_t green, uint8_t blue);
void ws281x_setPixelColor(uint16_t n ,uint32_t GRBcolor);
void ws281x_setPixelColor_lr(uint16_t n ,uint32_t GRBcolor);
void ws281x_setPixelColor_ringleds(uint16_t n ,uint32_t GRBcolor);



void ws281x_colorWipe( uint32_t c, uint8_t num);
void ws281x_colorWipe_lr( uint32_t c, uint8_t num);
void ws281x_colorWipe_freq(uint8_t type);


uint32_t ws281x_colorTran(uint8_t tran,uint8_t color_mode);
void ws281x_colorblnCtrl(uint8_t color_mode);


void ws281x_setPixelColor_wheel_leds(uint16_t n ,uint32_t GRBcolor); 

void ws281x_display(void);

#endif
