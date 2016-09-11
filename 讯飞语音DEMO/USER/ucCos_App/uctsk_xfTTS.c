

#include <includes.h>          
#include "gpio.h"



/* Private variables ---------------------------------------------------------*/
static  OS_STK         App_TaskXfttsStk[APP_TASK_XFTTS_STK_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void uctsk_xfTTS (void) ;


void  App_xfTTSTaskCreate (void)
{
    CPU_INT08U  os_err;

	os_err = os_err; /* prevent warning... */

	os_err = OSTaskCreate((void (*)(void *)) uctsk_xfTTS,				
                          (void          * ) 0,							
                          (OS_STK        * )&App_TaskXfttsStk[APP_TASK_XFTTS_STK_SIZE - 1],		
                          (INT8U           ) APP_TASK_XFTTS_PRIO  );		
	#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(APP_TASK_XFTTS_PRIO, "Task xfTTS ", &os_err);
	#endif
}
 

void ReadText(char* str, int encoding)
{
	int i;
	int s, ms;
	
	char* p = str;
	int length = strlen(str);
	USART_SendData(USART3,0xFD);				
 	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束	
	USART_SendData(USART3,0x00);				
 	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束	
	USART_SendData(USART3,length + 2);				
 	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束	
	USART_SendData(USART3,0x01);				
 	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束	
	USART_SendData(USART3,encoding);				
 	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束
	
	for ( i = 0; i < length; i++ ) // 依次发送待合成的文本数据
	{ 
		USART_SendData(USART3, *p++);				
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束
	}
}


static void uctsk_xfTTS (void) 
{
 	char* str = "hello owenlzy";
		
	while(1)
	{
		OSTimeDlyHMSM(0, 0, 6, 0);
    ReadText(str, 0x00);
	}
}




/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
