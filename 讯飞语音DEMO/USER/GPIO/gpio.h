#ifndef __GPIO_H
#define __GPIO_H
#include "includes.h"
//#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/4
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//********************************************************************************

void gpio_motor_init(void) ;
void gpio_ultrasonic_init(void);
void gpio_led_ws2812s_init(void);
void gpio_led_init(void);
void gpio_sound_init(void);
void gpio_human_sensor_init(void);
void gpio_voice_init(void);
void gpio_tracker_init(void);
void gpio_LightSensor_init(void);
void gpio_WIFIModeSwitch_init(void);
void gpio_XF_init(void);
int GetXFStatus(void);
#endif
