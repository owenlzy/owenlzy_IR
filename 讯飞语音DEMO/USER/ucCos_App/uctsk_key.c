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
      
#include "key.h"
#include "FIFO.h"
#include "usart.h"
#include "exit.h"
#include <string.h>

/* Private variables ---------------------------------------------------------*/
static  OS_STK         App_keyStk[APP_KEY_STK_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void uctsk_key (void) ;

extern QUEUE key_Q ;
extern OS_EVENT *Str_Q_LCD12864;

void   App_keyCreate (void)
{
    CPU_INT08U  os_err;

	os_err = os_err; /* prevent warning... */

	os_err = OSTaskCreate((void (*)(void *)) uctsk_key,				
                          (void          * ) 0,							
                          (OS_STK        * )&App_keyStk[APP_KEY_STK_SIZE - 1],		
                          (INT8U           ) APP_TASK_KEY_PRIO  );							

	#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(APP_TASK_KEY_PRIO, "Task ULTRASONIC Blink", &os_err);
	#endif

}

static void uctsk_key (void) 
{
	
	char pdata;
	const char* prefix = "key_";
	char cmd[10] = {'\0'};
	key_init();
	EXTIX_Init();
	
	TIM5_Configuration();
	//CreateQueue(Q, 10);
	while(1)
	{
// 		scan();
// 		key_up=1;
// 		while(SHQ->first!=NULL)
//         SHQ=del(SHQ);
// 		distance();
// 		human();
// 		printf("sssyyy=%d,%d,%d\r\n", sensorStatus[0],sensorStatus[1],sensorStatus[2]);
// 		OSTimeDlyHMSM(0, 0, 0, 100);
		if(!IsEmptyQueue(&key_Q))
		{
			Dequeue(&key_Q, &pdata);
			strcpy(cmd, prefix);
			strcat(cmd, &pdata);
			//cmd[5] = '\0';
			OSQPost(Str_Q_LCD12864, cmd); 
			//printf("%s\n", cmd);
			//printf("get %c\n", pdata);
		}
		OSTimeDlyHMSM(0, 0, 0, 100);
		
 	}
}




/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
