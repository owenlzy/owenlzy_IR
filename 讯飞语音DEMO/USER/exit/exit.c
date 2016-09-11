#include "exit.h"
#include "FIFO.h"
#include <includes.h> 

extern 	QUEUE key_Q ;

static void delay_ms(uint16_t ms)    
{ 
	uint16_t i,j; 
	for( i = 0; i < ms; i++ )
	{ 
		for( j = 0; j < 3333; j++ );
	}
} 	


void EXTIX_Init(void)
{	  
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	/******************����******************************/
	//GPIOE.3/4/5/6   �ж����Լ��жϳ�ʼ������   �½��ش���
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line3;	//KEY2
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource5);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource6); 
 	EXTI_InitStructure.EXTI_Line=EXTI_Line6;
  	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	/******************����******************************/
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//ʹ�ܰ���KEY1���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//�����ȼ�1 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//ʹ�ܰ���KEY0���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//�����ȼ�0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	NVIC_InitStructure.NVIC_IRQChannel =  EXTI9_5_IRQn; //ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}

//�ⲿ�ж�3������� 
void EXTI3_IRQHandler(void)
{

	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
	{
		TIM_Cmd(TIM5, DISABLE);
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0)==0)
		{
					//printf("input :3 \n");
			Enqueue(&key_Q, '3');
		}
		
		else if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)==0)
		{
					//printf("input :2 \n");
			Enqueue(&key_Q, '2');
		}
	
		else if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)==0)
		{
					//printf("input :1\n");
			Enqueue(&key_Q, '1');
		}
		delay_ms(600);

		TIM_Cmd(TIM5, ENABLE);
		EXTI_ClearITPendingBit(EXTI_Line3);  //���LINE3�ϵ��жϱ�־λ 
	}
}
 
//�ⲿ�ж�4�������
void EXTI4_IRQHandler(void)
{

	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
	{
		TIM_Cmd(TIM5, DISABLE);
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0)==0)
		{
			  
				Enqueue(&key_Q, '6');
			//printf("input :6 \n");
		}
		
		else if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)==0)
		{
			  
				Enqueue(&key_Q, '5');
			//printf("input :5 \n");
		}
	
		else if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)==0)
		{
			  
				Enqueue(&key_Q, '4');
			//printf("input :4 \n");
		}
		delay_ms(600);

		TIM_Cmd(TIM5, ENABLE);
		EXTI_ClearITPendingBit(EXTI_Line4);  //���LINE2�ϵ��жϱ�־λ  
	}
}

//�ⲿ�ж�5-9�������
void  EXTI9_5_IRQHandler(void)
{
	int cpu_sr; 
	if(EXTI_GetITStatus(EXTI_Line5) != RESET)
	{
		TIM_Cmd(TIM5, DISABLE);
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0)==0)
		{
					//printf("input 9 \n");
			Enqueue(&key_Q, '9');
		}
		
		else if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)==0)
		{
					//printf("input 8\n");
			Enqueue(&key_Q, '8');
		}
	
		else if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)==0)
		{
					//printf("input 7\n");
			Enqueue(&key_Q, '7');
		}
		delay_ms(600);
		TIM_Cmd(TIM5, ENABLE);
		EXTI_ClearITPendingBit(EXTI_Line5);  //���LINE3�ϵ��жϱ�־λ  
	}
	
	else if(EXTI_GetITStatus(EXTI_Line6) != RESET)
	{
		TIM_Cmd(TIM5, DISABLE);
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0)==0)
		{
					//printf("input :* \n");
			Enqueue(&key_Q, '*');
		}
		
		else if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)==0)
		{
					//printf("input :0 \n");
			Enqueue(&key_Q, '0');
		}
	
		else if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)==0)
		{
					//printf("input :# \n");
			Enqueue(&key_Q, '#');
		}
		delay_ms(600);

		TIM_Cmd(TIM5, ENABLE);
		EXTI_ClearITPendingBit(EXTI_Line6);  //���LINE3�ϵ��жϱ�־λ  
	}
}

