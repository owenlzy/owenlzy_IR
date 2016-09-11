#include "includes.h"
#include "ultrasonic.h"
#include "stdio.h"
#include "esp8266.h"

void  delayus(u32  time)
{	
	u32  time1;
	time1 = time*72;

	for(;time1!=0;time1--);
}

void  time_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);


	TIM_DeInit(TIM1);
	TIM_TimeBaseStructure.TIM_Period=49999;		 					/* 自动重装载寄存器周期的值(计数值) */
																	/* 累计 TIM_Period个频率后产生一个更新或者中断 */
	TIM_TimeBaseStructure.TIM_Prescaler= 71;				    /* 时钟预分频数   例如：时钟频率=72MHZ/(时钟预分频+1) */
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 			/* 采样分频 */
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 		/* 向上计数模式 */
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);							    /* 清除溢出中断标志 */
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);



	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM3 中断
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //先占优先级 0 级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级 3 级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  //IRQ 通道被使能
	NVIC_Init(&NVIC_InitStructure);   //④初始化 NVIC 寄存器

}

void TIM1_UP_IRQHandler(void)
{
	/*if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_12)==0)
	GPIO_SetBits(GPIOE,GPIO_Pin_12);
	else
	GPIO_ResetBits(GPIOE,GPIO_Pin_12);*/ 
	//printf("\r\nbreak\r\n");
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update ); 
}

//超声波测距
u32 UltrasonicWave_StartMeasure(void)
{
	OS_CPU_SR cpu_sr=0;
	u32 UltrasonicWave_Distance=0;
    u32 cnnt=0;
	char UltrasonicData[50]   ;

    OS_ENTER_CRITICAL();
    GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	OSTimeDlyHMSM(0, 0, 0, 2);	
    GPIO_SetBits(GPIOB,GPIO_Pin_0); 		  //送>10US的高电平RIG_PORT,TRIG_PIN这两个在define中有?             
    OSTimeDlyHMSM(0, 0, 0, 20);			   		//延时20US
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	

    while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0);

  	TIM_SetCounter(TIM1,0);
			
    TIM_Cmd(TIM1, ENABLE);
	
    while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==1);
    TIM_Cmd(TIM1, DISABLE);
	cnnt=TIM_GetCounter(TIM1);
   
   //时钟频率=72MHZ	/ (时钟预分频+1)
   //TIM_TimeBaseStructure.TIM_Period=49999;	计数次数为50000次	
   //TIM_TimeBaseStructure.TIM_Prescaler= 71;	预分频72，即计数器频率为72MHZ/72 = 1MHZ
   //距离 = 计数次数*每次计数需要时间*声速/2
   //即距离(米)= cnt*1/1M * 340(m/s) / 2 
   //距离(厘米) = 距离(米) / 100
   UltrasonicWave_Distance=cnnt/58;
   OS_EXIT_CRITICAL();
   return UltrasonicWave_Distance;
}




