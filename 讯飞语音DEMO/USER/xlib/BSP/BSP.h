/**
  ******************************************************************************
  * @文件名    
  * @作者 
  * @版本
  * @日期
  * @概要
  ******************************************************************************
  * @注意事项
  *
  *
  *
  ******************************************************************************
  */

#ifndef _BSP_H_
#define _BSP_H_

#ifdef __cplusplus
 extern "C" {
#endif 

/* 头文件				--------------------------------------------------------------*/
#include <stdio.h>
#include <stdarg.h>

#include <RTL.h>
#include <rl_usb.h>

#include "stm32f10x.h"

#include "Serial.h"
	 
/* 数据类型     --------------------------------------------------------------*/
/* 宏定义 	    --------------------------------------------------------------*/	
// #define RedBull_LED
#define FireBull_LED

#ifdef RedBull_LED
  /**
  *  LED1 -> PF6 , LED2 -> PF7 , LED3 -> PF8 , LED4 -> PF9
  */
	#define LED1 GPIO_Pin_6
	#define LED2 GPIO_Pin_7
	#define LED3 GPIO_Pin_8
	#define LED4 GPIO_Pin_9
	
	#define LEDPort			GPIOF
	#define LEDPortRCC  RCC_APB2Periph_GPIOF
#endif

#ifdef FireBull_LED
  /**
  *  LED1 -> PD8 , LED2 -> PD9 , LED3 -> PD10 , LED4 -> PD11
  */
	#define LED1 GPIO_Pin_8
	#define LED2 GPIO_Pin_9
	#define LED3 GPIO_Pin_10
	#define LED4 GPIO_Pin_11
	
	#define LEDPort	GPIOD
	#define LEDPortRCC  RCC_APB2Periph_GPIOD	
#endif

/* 变量 	    	--------------------------------------------------------------*/


/* 函数声明     --------------------------------------------------------------*/
	 
int32_t Printf(char const *const format, ...);	 

void LED_Configuration(void);
void LED_Set(uint8_t num, uint8_t state);

void NVIC_Configuration(void);

void BSP_Init(void);	 
	 
/**
  * @功能
  * @参数
  * @返回值
  */

/**
  * @}
  */
	 
#ifdef __cplusplus
}
#endif	 
	 
#endif /* _BSP_H_ */

/**
 ******************************************************************************
 * 文件末尾
 ******************************************************************************
 */
