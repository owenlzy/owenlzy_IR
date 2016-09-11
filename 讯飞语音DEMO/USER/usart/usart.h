#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "includes.h" 


#define USART1_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����

#define USART2_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART2_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����

#define USART3_REC_LEN  			2048  	//�����������ֽ��� 200
#define EN_USART3_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����

#define USART4_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART4_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern char  USART3_RX_BUF[USART3_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART3_RX_STA; 
// extern  char  	USART_RX_BUF3[USART4_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
// extern 	u16 	USART_RX_STA3;         		//����״̬���

// extern u8 fStar=0;
// extern u8 fClose=0;
//extern u8 flag;        		//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart1_init(u32 bound);
void uart2_init(u32 bound);
void uart3_init(u32 bound);
void uart4_init(u32 bound);
void uart4_disable();
#endif


