/**
  ******************************************************************************
  * @�ļ���    
  * @���� 
  * @�汾
  * @����
  * @��Ҫ
  ******************************************************************************
  * @ע������
  *
  *
  *
  ******************************************************************************
  */  
	
#ifndef _SERIAL_H_
#define _SERIAL_H_

/* ͷ�ļ�				--------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "stm32f10x.h"

/* ��������   	--------------------------------------------------------------*/

/* �궨�� 			--------------------------------------------------------------*/
/*
 * ����
 */
#define _USART1_ENABLE			/* define ����USART1, ע�͵�������, �Ż�������пռ� */
// #define _USART2_ENABLE			/* define ����USART2, ע�͵�������*/
// #define _USART3_ENABLE			/* define ����USART3, ע�͵�������*/
// #define _UART4_ENABLE				/* define ����UART4,  ע�͵�������*/
// #define _UART5_ENABLE				/* define ����UART5,  ע�͵�������*/

/*
 *	
 */
#define Serial_TypeDef 			USART_TypeDef				/* USART1, USART2, USART3, UART4, UART5 */
#define Seril_InitTypeDef 	USART_InitTypeDef

/* ���� 				--------------------------------------------------------------*/
/* ��������   	--------------------------------------------------------------*/

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
  * @����
  * @����
  * @����ֵ
  */



/**
  * @}
  */

#endif /* _SERIAL_H_ */

/**
 ******************************************************************************
 * �ļ�ĩβ
 ******************************************************************************
 */
