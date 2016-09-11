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
#include "esp8266.h"
#include "library_stm32.h"
#include "gpio.h"
#include "pwm.h"
#include "servohand.h"
#include "motor.h"
#include "ultrasonic.h"
#include "adc.h"

#define CCODE_SIZE 180

/* Private variables ---------------------------------------------------------*/
static  OS_STK         App_TaskCheckStk[APP_TASK_Check_STK_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void uctsk_Check	(void) ;
//void transCCode	(char *cStr);

void  App_CheckCreate (void)
{
    CPU_INT08U  os_err;
	os_err = os_err; /* prevent warning... */

	os_err = OSTaskCreate((void (*)(void *)) uctsk_Check,				
                          (void          * ) 0,							
                          (OS_STK        * )&App_TaskCheckStk[APP_TASK_Check_STK_SIZE - 1],		
                          (INT8U           ) APP_TASK_Check_PRIO  );							

	#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(APP_TASK_Check_PRIO, "Task ULTRASONIC Blink", &os_err);
	#endif

}
static void uctsk_Check (void) 
{  
 	OS_CPU_SR 			cpu_sr=0;
	extern OS_EVENT     *Str_Q_Check ;
	extern int 			backvalue ;
	INT8U         		errCheck; 
	
	int 	message_index=0 ;
	u8 		err ;
	extern	MessageBuffer mb[MESSAGE_BUFFER_NUM] ; 					  //��Ϣ���л�����
	MessageBuffer *msg_rcv_p  ;
	
	//�����test����Ϊ����	
	char cCodeBuffer[CCODE_SIZE] = {0} , cCodeMsg[MESSAGE_BUFFER_SIZE] ;
	
	//�����LED �������ĳ�ʼ��
	TIM4_PWM_Init(899,0);
	gpio_motor_init();
	gpio_led_ws2812s_init();
	gpio_voice_init();
	Timerx_Init(9000,160);
	//���ĳ�ʼ��
	printf("\r\n<----Check Running XLIB..........>\r\n");

 	while(1){
		//�ڽ���ִ��֮ǰ���������������		
		memset(cCodeBuffer, 0 , CCODE_SIZE);	
		
		msg_rcv_p = OSQPend(Str_Q_Check,0,&errCheck); 		//�ȴ���Ϣ����		
		
		strncpy(cCodeMsg,msg_rcv_p->buffer,MESSAGE_BUFFER_SIZE);
		OS_ENTER_CRITICAL();
		Message_Queue_Back(msg_rcv_p);
		OS_EXIT_CRITICAL();

		ParseStoryScript(cCodeMsg);				//��ʼ����ִ��
				
 	}	
}

/*********************************************************************************************************
    END FILE
*********************************************************************************************************/
