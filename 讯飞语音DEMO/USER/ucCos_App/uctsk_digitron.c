

/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               uctsk_digitron.c
** Descriptions:            The uctsk_digitron application function
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
#include "digitron.h"


/* Private variables ---------------------------------------------------------*/
static  OS_STK         App_TaskDigitronStk[APP_TASK_DIGITRON_STK_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void uctsk_digitron (void) ;

void  App_DigitronTaskCreate (void)
{
  CPU_INT08U  os_err;

	os_err = os_err; /* prevent warning... */

	os_err = OSTaskCreate((void (*)(void *)) uctsk_digitron,				
                          (void          * ) 0,							
                          (OS_STK        * )&App_TaskDigitronStk[APP_TASK_DIGITRON_STK_SIZE - 1],		
                          (INT8U           ) APP_TASK_DIGITRON_PRIO  );							

	#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(APP_TASK_DIGITRON_PRIO, "Task DIGITRON ", &os_err);
	#endif

}
 
static void uctsk_digitron (void) 
{
	STRU_Digitron 	*pMsg;
    STRU_Digitron 	showMsg={0};
	u8 				err;
	OS_CPU_SR  		cpu_sr = 0;
	extern OS_EVENT *Str_Q_Digitron;
	
	int first = DISPLAY_1 + DISPLAY_DOT,         // 1 + 16
			second = DISPLAY_2 + DISPLAY_DOT,        // 2 + 16
			third = DISPLAY_3 + DISPLAY_DOT,         // 3 + 16
			count = 0, flag = 0;
	H595_Init();

//	OS_ENTER_CRITICAL();
	
	while(1)
	{
/***************发送消息队列方法************************
// addr = & pMsg;   
// OSQPost(xxxxxxxxxxxx, addr); 
// addr类型为INT32U;pMsg结构体变量;xxxxxxxxxx?窍⒍恿?
********************************************************/		
		//printf("digitron\n");
		pMsg = (STRU_Digitron*) OSQPend(Str_Q_Digitron, 10, &err);
        
		if (pMsg != (STRU_Digitron *)0) 
		{
			first = pMsg->first;
			second = pMsg->second;
			third = pMsg->third;
		}		
		while(flag < 10)
		{
// 			count++;
// 			if(count == 100 ){
// 				flag++;
// 				count = 0;
// 			}
			flag++;
			digitron_display( CS_1 , first);
			OSTimeDlyHMSM(0, 0, 0, 5);
		
			digitron_display( CS_2 , second);
			OSTimeDlyHMSM(0, 0, 0, 5);
		
			digitron_display( CS_3 , third);
			OSTimeDlyHMSM(0, 0, 0, 5);
		}
		write_smg(0x00);
		flag = 0;
	}
//	OS_EXIT_CRITICAL();
}




/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
