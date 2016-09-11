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
#include "servohand.h"

#define BUFFER_SIZE 128

/* Private variables ---------------------------------------------------------*/
static  OS_STK         App_TaskUsartDebugStk[APP_TASK_USART_AGENT_STK_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void 	uctsk_UsartDebug(void);

void  App_UsartDebugCreate (void)
{
    CPU_INT08U  os_err;

	os_err = os_err; /* prevent warning... */

	os_err = OSTaskCreate((void (*)(void *)) uctsk_UsartDebug,				
                          (void          * ) 0,							
                          (OS_STK        * )&App_TaskUsartDebugStk[APP_TASK_USART_DEBUG_STK_SIZE - 1],		
                          (INT8U           ) APP_TASK_USART_DEBUG_PRIO  );							

	#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(APP_TASK_USART_DEBUG_PRIO, "Task WIFI Blink", &os_err);
	#endif

}
static void uctsk_UsartDebug (void){
	int 	cpu_sr;
	u8 		err ;
	extern  OS_EVENT	 *Sem_UsartDebug;							  //�ź���   
	extern  Buffer 		  cb ;	 									  //���λ�����
	extern  char usartDebugBuffer[128] ;
    
    unsigned char rrrr;						//���ջ��λ����������� 
	char 	recvBuffer[BUFFER_SIZE] ;			//���ܻ����� Len=128 
	int 	recvIndex = 0 ;
	
	memset(recvBuffer , 0 , BUFFER_SIZE);
	while(1)
	{		
		OSSemPend(Sem_UsartDebug,0,&err);	//�ź���P���� ���ڶ�ȡһ�����ݣ�����ȡһ������

		OS_ENTER_CRITICAL();
		rrrr=rb_get(&cb);
		OS_EXIT_CRITICAL();
		//if(rrrr != ' ' || rrrr!=NULL)
#if DEBUG	&& DEBUG_USARTDEBUG	
			//printf("%c",rrrr);
#endif		
		
		if(recvIndex >= BUFFER_SIZE)
		{
			recvIndex = 0 ;
			recvBuffer[recvIndex++] = rrrr ;
		}else{			
			recvBuffer[recvIndex++] = rrrr ;
            if( strstr(recvBuffer,"\r\n"))
            {
                memset(usartDebugBuffer,0,128);
                memcpy(usartDebugBuffer,recvBuffer,strlen(recvBuffer)-1);
#if DEBUG	&& DEBUG_USARTDEBUG							
                printf("\r\nUsart:[%s]\r\n",usartDebugBuffer);
#endif							
                memset(recvBuffer , 0 , BUFFER_SIZE);	//һ�ν�����󣬽��������
                recvIndex = 0 ;
            }			
		}
	}
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
