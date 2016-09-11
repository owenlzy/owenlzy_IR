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

#define CCODE_SIZE 200

/* Private variables ---------------------------------------------------------*/
static  OS_STK         App_TaskxlibStk[APP_TASK_xlib_STK_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void uctsk_xlib	(void) ;
//void transCCode	(char *cStr);

void  App_xlibCreate (void)
{
    CPU_INT08U  os_err;
	os_err = os_err; /* prevent warning... */

	os_err = OSTaskCreate((void (*)(void *)) uctsk_xlib,				
                          (void          * ) 0,							
                          (OS_STK        * )&App_TaskxlibStk[APP_TASK_xlib_STK_SIZE - 1],		
                          (INT8U           ) APP_TASK_xlib_PRIO  );							

	#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(APP_TASK_xlib_PRIO, "Task ULTRASONIC Blink", &os_err);
	#endif

}
static void uctsk_xlib (void) 
{  
 	u16         		xlib; 
 	OS_CPU_SR 			cpu_sr=0;
	extern OS_EVENT     *Str_Q_xlib ;
	extern OS_EVENT  	*Str_Q_SendAck ;
	extern int 			backvalue ;
	INT8U         		errxlib; 
	
	int 	message_index=0 ;
	u8 		err ;
	extern	MessageBuffer mb[MESSAGE_BUFFER_NUM] ; 					  //消息队列缓冲区
	MessageBuffer *msg_rcv_p  ;
	
	//这里的test仅作为测试	
	char test[] = { 0 };
	char cCodeBuffer[CCODE_SIZE] = {0} , cCodeMsg[MESSAGE_BUFFER_SIZE] ;
	char revalue[64] ={ 0 };
	
	//电机，LED ，声音的初始化
	TIM4_PWM_Init(899,0);
	gpio_motor_init();
	gpio_led_ws2812s_init();
    gpio_led_init();
	gpio_voice_init();
	Timerx_Init(9000,160);
	//测距的初始化
	
#if DEBUG	&& DEBUG_XLIB
	printf("\r\n<----XLIB Running XLIB..........>\r\n");
#endif	
	
	memset(revalue,0,64);
    OSTimeDlyHMSM(0, 0, 5, 0);
    
    //CPicoc(CCODE_SAY_TEST1);
    //CPicoc(CCODE_LAST_TEST2);
   
 	while(1){
		//在解释执行之前，先清空所有数据		
		memset(cCodeBuffer, 0 , CCODE_SIZE);			
		msg_rcv_p = OSQPend(Str_Q_xlib,0,&errxlib); 		//等待消息到来		
		strncpy(cCodeMsg,msg_rcv_p->buffer,MESSAGE_BUFFER_SIZE);
		OS_ENTER_CRITICAL();
		Message_Queue_Back(msg_rcv_p);
		OS_EXIT_CRITICAL();
		
#if DEBUG	&& DEBUG_XLIB
 		printf("\r\n\r\nxlib[%s]\r\n",cCodeMsg);
#endif		
		
        if(cCodeMsg[6]=='H' &&cCodeMsg[7]=='E'&&cCodeMsg[8]=='A')
            continue;
        
		if(strstr(cCodeMsg,"$>")!=NULL)
		{
			ParseStoryScript(cCodeMsg);				//开始解析执行
			sprintf(revalue,"RV_%d\r\n",backvalue);		//给WIFI 返回ACK 并携带上返回值

#if DEBUG	&& DEBUG_XLIB		
			printf("xlib[%s:%s]\r\n",cCodeMsg,revalue);
#endif			
			if(backvalue!=0)
            {
                OS_ENTER_CRITICAL();
                message_index = Message_Queue_Available(mb);
                OS_EXIT_CRITICAL();
                strcpy(mb[message_index].buffer,revalue);
                err = OSQPost(Str_Q_SendAck,&mb[message_index]);	//发送队列
            }
			
		}//else
			//CPicoc(cCodeMsg);
		
		backvalue = 0 ;
		memset(revalue,0,64);	
 	}	
}
/****************************************************************************************



****************************************************************************************/
/*********************************************************************************************************
    END FILE
*********************************************************************************************************/
