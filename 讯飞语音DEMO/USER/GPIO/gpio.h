#ifndef __GPIO_H
#define __GPIO_H
#include "includes.h"
//#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/4
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
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
