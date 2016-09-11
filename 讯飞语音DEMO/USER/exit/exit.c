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

	/******************键盘******************************/
	//GPIOE.3/4/5/6   中断线以及中断初始化配置   下降沿触发
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line3;	//KEY2
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource5);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource6); 
 	EXTI_InitStructure.EXTI_Line=EXTI_Line6;
  	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	/******************键盘******************************/
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//使能按键KEY1所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级1 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//使能按键KEY0所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	NVIC_InitStructure.NVIC_IRQChannel =  EXTI9_5_IRQn; //使能按键所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //从优先级2级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}

//外部中断3服务程序 
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
		EXTI_ClearITPendingBit(EXTI_Line3);  //清除LINE3上的中断标志位 
	}
}
 
//外部中断4服务程序
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
		EXTI_ClearITPendingBit(EXTI_Line4);  //清除LINE2上的中断标志位  
	}
}

//外部中断5-9服务程序
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
		EXTI_ClearITPendingBit(EXTI_Line5);  //清除LINE3上的中断标志位  
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
		EXTI_ClearITPendingBit(EXTI_Line6);  //清除LINE3上的中断标志位  
	}
}

