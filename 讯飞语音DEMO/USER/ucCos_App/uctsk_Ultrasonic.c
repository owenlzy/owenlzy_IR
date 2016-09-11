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
#include "usart.h"           
#include "adc.h"
#include "gpio.h"
#include "ultrasonic.h"
#include "esp8266.h"

/* Private variables ---------------------------------------------------------*/
static  OS_STK         App_TaskUltrasonicStk[APP_TASK_ULTRASONIC_STK_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void uctsk_Ultrasonic            (void);
void Ultrasonic_init			   (void);


void  App_UltrasonicTaskCreate (void)
{
    CPU_INT08U  os_err;

	os_err = os_err; /* prevent warning... */

	os_err = OSTaskCreate((void (*)(void *)) uctsk_Ultrasonic,				
                          (void          * ) 0,							
                          (OS_STK        * )&App_TaskUltrasonicStk[APP_TASK_ULTRASONIC_STK_SIZE - 1],		
                          (INT8U           ) APP_TASK_ULTRASONIC_PRIO  );							

	#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(APP_TASK_ULTRASONIC_PRIO, "Task ULTRASONIC Blink", &os_err);
	#endif

}

static void uctsk_Ultrasonic (void) 
{   
	u8 err ;
	OS_CPU_SR  cpu_sr = 0;

	extern OS_EVENT  	 *Sem_TCPSend  , *Str_Q_Client2Ultrasonic ,*Str_Q_SendAck; 
	extern	MessageBuffer mb[MESSAGE_BUFFER_NUM] ; 
	int message_index ;
	int j ;	
	MessageBuffer  *rcv_p;


	u32 UltrasonicWave_Distance ; 	//超声波距离
	//电压检测模块使用
	u16 adcx;
	float voltage , temp;
	char sensor_data[64] ;
										  
	Ultrasonic_init() ; //初始化超声波
// 	Adc_Init();		  	//ADC初始化

	//唤醒传感器
	rcv_p=OSQPend(Str_Q_Client2Ultrasonic,0,&err) ;
	OS_ENTER_CRITICAL();
	Message_Queue_Back(rcv_p);
	OS_EXIT_CRITICAL();
    printf("\r\n<ultrasonic>------Buffer is returned , flag=%d\r\n" , rcv_p->flag);

	while(1)			 
	{
		memset(sensor_data,0,100);
		/******超声波测距*********/
	//	printf("\r\n[ultrasonic]1 hello\r\n");
		UltrasonicWave_Distance = UltrasonicWave_StartMeasure();
	//	printf("\r\n[ultrasonic]2 hello\r\n");
		/******检测电压**********/
// 		adcx=Get_Adc_Average(ADC_Channel_5,10);  		//ADC的值
// 		voltage=(float)adcx*(5.0/1024);
// 		voltage-=0.9;
	//	printf("\r\n[ultrasonic]adcx=%d , voltage=%f\r\n" ,adcx, voltage);
		sprintf(sensor_data ,  "sensor_distance_%d_voltage_%.2f" , UltrasonicWave_Distance,voltage) ;
		
		/********不直接发送给服务器，而是转发给任务发送***********/
		OS_ENTER_CRITICAL();
		message_index = Message_Queue_Available(mb);
		OS_EXIT_CRITICAL();
		printf("\r\n<ultrasonic>-------Buffer is available , Index=%d\r\n" , message_index);
		strncpy(mb[message_index].buffer,sensor_data,strlen(sensor_data));
		err = OSQPost(Str_Q_SendAck,&mb[message_index]) ;
		OSTimeDlyHMSM(0, 0, 3, 0);	
		
//		/******发送心跳给服务器**********/
//		OSSemPend(Sem_TCPSend,0,&err);	//信号量P操作
//		if(err == OS_ERR_NONE)
//			Send_Data(sensor_data);
////		OSSemPost(Sem_TCPSend);
//		OSTimeDlyHMSM(0, 0, 3, 0);	

//		//接收心跳ACK
//		rcv_p=OSQPend(Str_Q_Client2Ultrasonic,0,&err) ;
//		OS_ENTER_CRITICAL();
//		Message_Queue_Back(rcv_p);
//		OS_EXIT_CRITICAL();
//		printf("\r\n------Buffer is returned , flag=%d\r\n" , rcv_p->flag);
//		printf("\r\nhello\r\n");
	}
		
}


//超声波初始化
void Ultrasonic_init(void){
	

	time_init() ;			     //初始化定时器TIM1
    gpio_ultrasonic_init();		 //初始化需要使用的IO
}




/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
