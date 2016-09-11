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
	
#ifndef _SERIAL_H_
#define _SERIAL_H_

/* 头文件				--------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "stm32f10x.h"

/* 数据类型   	--------------------------------------------------------------*/

/* 宏定义 			--------------------------------------------------------------*/
/*
 * 配置
 */
#define _USART1_ENABLE			/* define 启用USART1, 注释掉则不启用, 优化缓存队列空间 */
// #define _USART2_ENABLE			/* define 启用USART2, 注释掉则不启用*/
// #define _USART3_ENABLE			/* define 启用USART3, 注释掉则不启用*/
// #define _UART4_ENABLE				/* define 启用UART4,  注释掉则不启用*/
// #define _UART5_ENABLE				/* define 启用UART5,  注释掉则不启用*/

/*
 *	
 */
#define Serial_TypeDef 			USART_TypeDef				/* USART1, USART2, USART3, UART4, UART5 */
#define Seril_InitTypeDef 	USART_InitTypeDef

/* 变量 				--------------------------------------------------------------*/
/* 函数声明   	--------------------------------------------------------------*/

#ifdef  _USART1_ENABLE	
bool		USART1_putchar (uint8_t c);
int32_t USART1_getchar (void);
bool 		USART1_Get_tx_active (void); 
#endif

#ifdef  _USART2_ENABLE	
bool    USART2_putchar (uint8_t c);
int32_t USART2_getchar (void);
bool    USART2_Get_tx_active (void); 
#endif

#ifdef  _USART3_ENABLE	
bool    USART3_putchar (uint8_t c);
int32_t USART3_getchar (void);
bool    USART3_Get_tx_active (void); 
#endif

#ifdef  _USART4_ENABLE	
bool    UART4_putchar (uint8_t c);
int32_t UART4_getchar (void);
bool    UART4_Get_tx_active (void); 
#endif

#ifdef  _USART5_ENABLE	
bool    UART5_putchar (uint8_t c);
int32_t UART5_getchar (void);
bool    UART5_Get_tx_active (void); 
#endif

int32_t USART_NVICConfig(Serial_TypeDef *Serialx, uint8_t PrePriority, uint8_t SubPriority, FunctionalState state);
int32_t SerialInit(Serial_TypeDef *Serialx, Seril_InitTypeDef *Serialx_InitStruct);

/**
  * @功能
  * @参数
  * @返回值
  */



/**
  * @}
  */

#endif /* _SERIAL_H_ */

/**
 ******************************************************************************
 * 文件末尾
 ******************************************************************************
 */
