#include "encode.h"
u16  round_1=0;
u16  round_2=0;
u8 err;



void TIM2_Int_Init(void)
{
	/* 定义 TIM_TimeBase 初始化结构体 TIM_TimeBaseStructure */
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	/* 定义 TIM_OCInit 初始化结构体 TIM_OCInitStructure */
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = 0xFF;                       //重装载值20000
	TIM_TimeBaseStructure.TIM_Prescaler = 8;                        //预分频0
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                    //时钟分割0
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	    //向上计数模式
	TIM_TimeBaseInit(TIM2 , &TIM_TimeBaseStructure);                //初始化时基单元

	 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState =  TIM_OutputNState_Disable;
  TIM_OCInitStructure.TIM_Pulse = 0xFF;  /* Duty cycle: 50%*/
  TIM_OCInitStructure.TIM_OCPolarity =	TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCNPolarity =	TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCIdleState =	TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);

	TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设
}

void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM3, ENABLE);  //使能TIMx					 
}

//void TIM6_Int_Init(void)
//{
//    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //时钟使能
//	
//	//定时器TIM6初始化
//	TIM_TimeBaseStructure.TIM_Period = 9000;
//	TIM_TimeBaseStructure.TIM_Prescaler =0;
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向下计数模式
//	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
// 
//	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE ); //使能指定的TIM6中断,允许更新3中断
//
//	//中断优先级NVIC设置
//	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;  //TIM6中断
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级3级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
//	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
//
//
//	TIM_Cmd(TIM6, ENABLE);  //使能TIMx					 
//}


void tim4_time5_init(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_ICInitTypeDef TIM_ICInitStructure;   
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

//   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3|RCC_APB1Periph_TIM5, ENABLE);
//   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);

//   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//   GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//   GPIO_Init(GPIOA, &GPIO_InitStructure);

//   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//   GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//   GPIO_Init(GPIOA, &GPIO_InitStructure);

  	TIM_TimeBaseStructure.TIM_Period =64000 ;
  	TIM_TimeBaseStructure.TIM_Prescaler = 0;
  	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
  
   	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);
	TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);
    TIM_ICStructInit(&TIM_ICInitStructure);
  	TIM_ICInitStructure.TIM_ICFilter = 6;
  	TIM_ICInit(TIM5, &TIM_ICInitStructure);
	TIM_ICInit(TIM3, &TIM_ICInitStructure);

	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);  
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);
    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
		
	TIM_SetCounter(TIM5, 0);
	TIM_SetCounter(TIM3, 0);   
	TIM_Cmd(TIM3, ENABLE);
	TIM_Cmd(TIM5, ENABLE);
	  
}
void TIM3_IRQHandler(void)   //TIM3中断
{
//	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
//		{
//			TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
//			TIM_SetCompare2(TIM2, Stream_Buff[SBNo][Out_Data_Offset++]); 
//			if(Out_Data_Offset >= buffsize)
//			{	
//	      SBNo = 1 - SBNo;           
//				Out_Data_Offset = 0; 			  
//				readNextBytes = 1; //需要读下一个文件流
//			}
//		}
}
//void TIM5_IRQHandler(void)   //TIM3中断
//{
//	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
//		{
//		TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
//		//LED1=!LED1;
//		round_2=round_2+1;
//		}
//}
//void TIM6_IRQHandler(void)
//{	 
//	 if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
//  {	 
//		TIM_SetCompare2(TIM2, Stream_Buff[SBNo][Out_Data_Offset++]); 
//	  if(Out_Data_Offset >= buffsize)
//	  {	
//	      SBNo = 1 - SBNo;           
//		  Out_Data_Offset = 0; 			  
//		  readNextBytes = 1; //需要读下一个文件流
//	  }
//	  /* Clear TIM2 update interrupt */
//		TIM_ClearITPendingBit(TIM6, TIM_IT_Update); 
//	}			
//}
