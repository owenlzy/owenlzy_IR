/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               uctsk_Ultrasonic.c
** Descriptions:            The uctsk_Ultrasonic application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-9
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <includes.h>          
#include "adc.h"
#include "gpio.h"

#define S0     GPIO_Pin_9 
#define S1     GPIO_Pin_8  
#define S2     GPIO_Pin_6  
#define S3     GPIO_Pin_7 
#define E0     GPIO_Pin_5                       
#define out    GPIO_Pin_4 

#define   S0_0    GPIO_ResetBits(GPIOB,S0);
#define   S0_1    GPIO_SetBits(GPIOB,S0);

#define   S1_0    GPIO_ResetBits(GPIOB,S1);
#define   S1_1    GPIO_SetBits(GPIOB,S1);

#define   S2_0    GPIO_ResetBits(GPIOB,S2);
#define   S2_1    GPIO_SetBits(GPIOB,S2);

#define   S3_0    GPIO_ResetBits(GPIOB,S3);
#define   S3_1    GPIO_SetBits(GPIOB,S3);
/* Private variables ---------------------------------------------------------*/
static  OS_STK         App_TaskLightAndHumanStk[APP_TASK_LIGHT_HUMAN_STK_SIZE];



/* Private function prototypes -----------------------------------------------*/
static void uctsk_LightAndHumanSensor (void) ;

void  App_LightAndHumanTaskCreate (void)
{
    CPU_INT08U  os_err;

	os_err = os_err; /* prevent warning... */

	os_err = OSTaskCreate((void (*)(void *)) uctsk_LightAndHumanSensor,				
                          (void          * ) 0,							
                          (OS_STK        * )&App_TaskLightAndHumanStk[APP_TASK_LIGHT_HUMAN_STK_SIZE - 1],		
                          (INT8U           ) APP_TASK_LIGHTANDHUMAN_PRIO  );							

	#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(APP_TASK_LIGHTANDHUMAN_PRIO, "Task LightAndHuman Blink", &os_err);
	#endif

}

/*
************************光线传感器***************************
************************使用PC4/ADC12_IN14*******************
*/
static void uctsk_LightAndHumanSensor (void) 
{   
	
	extern OS_EVENT      *Str_Q;
	extern u16 lightStatus , humanStatus ;
	
	int 	cpu_sr;
	u8 		err ;
	extern	MessageBuffer mb[MESSAGE_BUFFER_NUM] ; 					  //消息队列缓冲区
	int 	message_index=0 ;
	extern 	OS_EVENT     *Str_Q_SendAck;			//C语言解释器消息队列
	char message[10]="SV_hello\r\n";

	gpio_human_sensor_init();//人体传感器管脚初始化
	while(1)
	{
		lightStatus = Get_Adc_Average(ADC_Channel_14,10);
		humanStatus = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9);

#if DEBUG && DEBUG_LIGHT_HUMAN
		printf("> lightSensor=%d , humanSensor=%d \r\n",lightStatus , humanStatus);
#endif
		
		OSTimeDlyHMSM(0, 0, 1, 0);
//		printf("lightSensor=%d\r\n",lightSensor);
		
// 		//定时发送给服务器
// 		OS_ENTER_CRITICAL();
// 		message_index = Message_Queue_Available(mb);
// 		OS_EXIT_CRITICAL();
// 		strncpy(mb[message_index].buffer,message,10);
// 		err = OSQPost(Str_Q_SendAck,&mb[message_index]);	//发送队列
// 		
// 		OSTimeDlyHMSM(0, 0, 0, 100);
		
	}
	
}


/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
