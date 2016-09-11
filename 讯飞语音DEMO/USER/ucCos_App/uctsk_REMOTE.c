
	/****************************************Copyright (c)****************************************************
	**                                      
	**                                 http://www.powermcu.com
	**
	**--------------File Info---------------------------------------------------------------------------------
	** File name:               uctsk_Usart_Agent.c
	** Descriptions:            The uctsk_WIFI application function
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
	#include "app_cfg.h" 
	#include "esp8266.h"
	#include "exit.h"
	#include "gpio.h"
	#include "adc.h"
	#include "remote.h"
#include "app_cfg.h" 
	/* Private variables ---------------------------------------------------------*/
	static  OS_STK         App_TaskREMOTEStk[APP_TASK_REMOTE_STK_SIZE];

	/* Private function prototypes -----------------------------------------------*/
	static void uctsk_REMOTE            (void);
	 int  key=0;
	/****************信号量变量************/ 
	
	
		char *F="manualcmd:move_forward_10";
		char *R="manualcmd:move_right_30";
		char *L="manualcmd:move_left_30";
		char *B="manualcmd:move_back_10 ";
	void  App_REMOTECreate (void)
	{
		CPU_INT08U  os_err;

		os_err = os_err; /* prevent warning... */
		os_err = OSTaskCreate((void (*)(void *)) uctsk_REMOTE,				
														(void          * ) 0,							
														(OS_STK        * )&App_TaskREMOTEStk[APP_TASK_REMOTE_STK_SIZE - 1],		
														(INT8U           ) APP_TASK_REMOTE_PRIO  );
															

		#if OS_TASK_NAME_EN > 0
				OSTaskNameSet(APP_TASK_REMOTE_PRIO, "Task WIFI Blink", &os_err);
		#endif	
	}

	static void uctsk_REMOTE (void) 
	{  
		extern OS_EVENT      *Str_Q_Client2Motor   ;
		extern	MessageBuffer mb[MESSAGE_BUFFER_NUM] ; //消息队列缓冲区
	  int message_index ;
		Remote_Init();//初始化红外遥控

		while(1)
		{	
			u8  err;
		  OS_CPU_SR  cpu_sr = 0;
		  key=Remote_Scan();	
		  if(key!=0)
		   printf("key=%d\r\n",key);
		  switch(key)
		 {
      case 24: //控制前进
				OS_ENTER_CRITICAL();
				message_index = Message_Queue_Available(mb);
				OS_EXIT_CRITICAL();
				strncpy(mb[message_index].buffer,F,MESSAGE_BUFFER_SIZE-strlen(F));
				err = OSQPost(Str_Q_Client2Motor,&mb[message_index]) ;
				break;
    case 74: //控制后退
				OS_ENTER_CRITICAL();
				message_index = Message_Queue_Available(mb);
				OS_EXIT_CRITICAL();
				strncpy(mb[message_index].buffer,B,MESSAGE_BUFFER_SIZE-strlen(B));
				err = OSQPost(Str_Q_Client2Motor,&mb[message_index]) ;
				break;
		case 16: //控制左转
				OS_ENTER_CRITICAL();
				message_index = Message_Queue_Available(mb);
				OS_EXIT_CRITICAL();
				strncpy(mb[message_index].buffer,L,MESSAGE_BUFFER_SIZE-strlen(L));
				err = OSQPost(Str_Q_Client2Motor,&mb[message_index]) ;
				break;
    case 90: //控制右转
				OS_ENTER_CRITICAL();
				message_index = Message_Queue_Available(mb);
				OS_EXIT_CRITICAL();
				strncpy(mb[message_index].buffer,R,MESSAGE_BUFFER_SIZE-strlen(R));
				err = OSQPost(Str_Q_Client2Motor,&mb[message_index]) ;
				break;
     }
       OSTimeDlyHMSM(0, 0, 1, 0);
    }
										
	}
	
			
	/*********************************************************************************************************
				END FILE
	*********************************************************************************************************/
