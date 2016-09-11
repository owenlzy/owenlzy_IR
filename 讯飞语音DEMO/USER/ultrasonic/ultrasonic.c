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
	TIM_TimeBaseStructure.TIM_Period=49999;		 					/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
																	/* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
	TIM_TimeBaseStructure.TIM_Prescaler= 71;				    /* ʱ��Ԥ��Ƶ��   ���磺ʱ��Ƶ��=72MHZ/(ʱ��Ԥ��Ƶ+1) */
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 			/* ������Ƶ */
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 		/* ���ϼ���ģʽ */
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);							    /* �������жϱ�־ */
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);



	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM3 �ж�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ� 0 ��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ� 3 ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  //IRQ ͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);   //�ܳ�ʼ�� NVIC �Ĵ���

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

//���������
u32 UltrasonicWave_StartMeasure(void)
{
	OS_CPU_SR cpu_sr=0;
	u32 UltrasonicWave_Distance=0;
    u32 cnnt=0;
	char UltrasonicData[50]   ;

    OS_ENTER_CRITICAL();
    GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	OSTimeDlyHMSM(0, 0, 0, 2);	
    GPIO_SetBits(GPIOB,GPIO_Pin_0); 		  //��>10US�ĸߵ�ƽ�TRIG_PORT,TRIG_PIN��������define����?             
    OSTimeDlyHMSM(0, 0, 0, 20);			   		//��ʱ20US
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	

    while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0);

  	TIM_SetCounter(TIM1,0);
			
    TIM_Cmd(TIM1, ENABLE);
	
    while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==1);
    TIM_Cmd(TIM1, DISABLE);
	cnnt=TIM_GetCounter(TIM1);
   
   //ʱ��Ƶ��=72MHZ	/ (ʱ��Ԥ��Ƶ+1)
   //TIM_TimeBaseStructure.TIM_Period=49999;	��������Ϊ50000��	
   //TIM_TimeBaseStructure.TIM_Prescaler= 71;	Ԥ��Ƶ72����������Ƶ��Ϊ72MHZ/72 = 1MHZ
   //���� = ��������*ÿ�μ�����Ҫʱ��*����/2
   //������(��)= cnt*1/1M * 340(m/s) / 2 
   //����(����) = ����(��) / 100
   UltrasonicWave_Distance=cnnt/58;
   OS_EXIT_CRITICAL();
   return UltrasonicWave_Distance;
}




