#ifndef __ENCODE_H
#define __ENCODE_H
//#include "sys.h"
 #include "includes.h"
 #include "stm32f10x.h"
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
#define buffsize 256
extern u16 round_1;
extern u16  round_2;

void tim4_time5_init(void);
void TIM3_Int_Init(u16 arr,u16 psc);
void TIM2_Int_Init(void);
void TIM6_Int_Init(void);
#endif