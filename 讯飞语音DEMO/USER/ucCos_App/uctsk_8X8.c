/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               uctsk_WIFI_ctrl.c
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
#include "74h595.h"

/* Private variables ---------------------------------------------------------*/
static  OS_STK         App_Task8X8Stk[APP_TASK_8X8_STK_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void uctsk_8X8 (void) ;

void  App_8X8TaskCreate (void)
{
    CPU_INT08U  os_err;

	os_err = os_err; /* prevent warning... */

	os_err = OSTaskCreate((void (*)(void *))  uctsk_8X8,				
                          (void          * ) 0,							
                          (OS_STK        * )&App_Task8X8Stk[APP_TASK_8X8_STK_SIZE - 1],		
                          (INT8U           ) APP_TASK_8X8_PRIO  );							

	#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(APP_TASK_8X8_PRIO, "Task 8X8 Lattice Blink", &os_err);
	#endif

}


static void uctsk_8X8 (void) 
{ 

	INT8U         err;
	OS_CPU_SR  		cpu_sr = 0;
	extern OS_EVENT  *Str_Q_8x8  ;
	MessageBuffer *msg_rcv_p;
// 	char msg_rcv[MESSAGE_BUFFER_SIZE] ;
	int row ,column;
	
	H595_8x8_Init();
	while(1)
	{
		//printf("fuck everybody\r\n");
		//msg_rcv_p=OSQPend(Str_Q_8x8,0,&err);
		//strncpy(msg_rcv,msg_rcv_p->buffer,MESSAGE_BUFFER_SIZE);
// 		strncpy(msg_rcv,"FFF_8x8_3_3\r\n",MESSAGE_BUFFER_SIZE);
// 		printf("8x8_command = %s" , msg_rcv);
// 		
// 		row = msg_rcv[8]-'0';
// 		column = msg_rcv[10]-'0';
// 		/*******显示函数指定坐标********* = "FFF_8x8_1_1\r\n"***/
// 		display_8x8(row ,column);  

		
		for(row=1;row<9;row++)
			for(column=1;column<9;column++)
			{
				display_8x8(row,column);  
				OSTimeDlyHMSM(0, 0, 1, 0);	
			}
	}

}