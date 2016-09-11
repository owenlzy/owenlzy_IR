#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "includes.h" 


#define USART1_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收

#define USART2_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART2_RX 			1		//使能（1）/禁止（0）串口1接收

#define USART3_REC_LEN  			2048  	//定义最大接收字节数 200
#define EN_USART3_RX 			1		//使能（1）/禁止（0）串口1接收

#define USART4_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART4_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern char  USART3_RX_BUF[USART3_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART3_RX_STA; 
// extern  char  	USART_RX_BUF3[USART4_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
// extern 	u16 	USART_RX_STA3;         		//接收状态标记

// extern u8 fStar=0;
// extern u8 fClose=0;
//extern u8 flag;        		//接收状态标记	
//如果想串口中断接收，请不要注释以下宏定义
void uart1_init(u32 bound);
void uart2_init(u32 bound);
void uart3_init(u32 bound);
void uart4_init(u32 bound);
void uart4_disable();
#endif


