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
      
#include "adc.h"
#include "led.h"
#include "human.h"
#include "distance.h"
/* Private variables ---------------------------------------------------------*/
static  OS_STK         App_sensorStatusStk[APP_TASK_SENSOR_STATUS_STK_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void uctsk_sensorStatus (void) ;



void  App_SensorStatusCreate (void)
{
    CPU_INT08U  os_err;

	os_err = os_err; /* prevent warning... */

	os_err = OSTaskCreate((void (*)(void *)) uctsk_sensorStatus,				
                          (void          * ) 0,							
                          (OS_STK        * )&App_sensorStatusStk[APP_TASK_SENSOR_STATUS_STK_SIZE - 1],		
                          (INT8U           ) APP_TASK_SENSOR_STATUS_PRIO  );							

	#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(APP_TASK_SENSOR_STATUS_PRIO, "Task ULTRASONIC Blink", &os_err);
	#endif

}
/*
	每隔1s采集和上传一次光线传感器和人体传感器的值
*/
static void uctsk_sensorStatus (void) 
{
	OS_CPU_SR 			cpu_sr=0;
	u8 err;
	u8 i ;
	char jsonStatus[128] ={ 0 };
	int 	message_index=0 ;
	extern	MessageBuffer mb[MESSAGE_BUFFER_NUM] ; 
	extern OS_EVENT  	*Str_Q_SendAck ;
	
	extern u16 lightStatus ,humanStatus;
	extern u8 infraredStatus ;
    extern u16 micStatus[3] ;
	extern double voltageStatus ;	

	while(1)
	{
		for(i=1;i<=100 ; i++){
			if(i%2 == 0){
				//每隔1s上传一次光线传感器和人体传感器的值
				sprintf(jsonStatus,"{\"ltSensor\":%d,\"huSensor\":%d}\r\n",lightStatus,humanStatus);	
				OS_ENTER_CRITICAL();
				message_index = Message_Queue_Available(mb);
				OS_EXIT_CRITICAL();
				strncpy(mb[message_index].buffer,jsonStatus,128);
				err = OSQPost(Str_Q_SendAck,&mb[message_index]);	//发送队列
				memset(jsonStatus,0,128);	
			}
			if(i == 100){
				//每隔10s上传一次电压的值
				sprintf(jsonStatus,"{\"vtSensor\":%lf}\r\n",voltageStatus);	
				OS_ENTER_CRITICAL();
				message_index = Message_Queue_Available(mb);
				OS_EXIT_CRITICAL();
				strncpy(mb[message_index].buffer,jsonStatus,128);
				err = OSQPost(Str_Q_SendAck,&mb[message_index]);	//发送队列
				memset(jsonStatus,0,128);	
			}
			//每隔100s上传一次夏普红外线和3个MIC的值
			sprintf(jsonStatus,"{\"ifSensor\":%d,\"m1Sensor\":%d,\"m2Sensor\":%d,\"m3Sensor\":%d}\r\n",infraredStatus,micStatus[0],micStatus[1],micStatus[2]);	
			OS_ENTER_CRITICAL();
			message_index = Message_Queue_Available(mb);
			OS_EXIT_CRITICAL();
			strncpy(mb[message_index].buffer,jsonStatus,128);
			err = OSQPost(Str_Q_SendAck,&mb[message_index]);	//发送队列
			memset(jsonStatus,0,128);
			OSTimeDlyHMSM(0, 0, 0, 100);
		}
 	}
}




/*********************************************************************************************************
      


END FILE
*********************************************************************************************************/
