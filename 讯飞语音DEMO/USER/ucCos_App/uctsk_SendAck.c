#include <includes.h> 
#include "gpio.h"
#include "esp8266.h"



/* Private variables ---------------------------------------------------------*/
static  OS_STK         App_TaskSendAckStk[APP_TASK_SEND_ACK_STK_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void uctsk_SendAck            (void);

void  App_SendAckCreate (void)
{
    CPU_INT08U  os_err;

	os_err = os_err; /* prevent warning... */



	os_err = OSTaskCreate((void (*)(void *)) uctsk_SendAck,				
					  (void          * ) 0,							
					  (OS_STK        * )&App_TaskSendAckStk[APP_TASK_SEND_ACK_STK_SIZE - 1],		
					  (INT8U           ) APP_TASK_SEND_ACK_PRIO  );
												

	#if OS_TASK_NAME_EN > 0
	OSTaskNameSet(APP_TASK_SEND_ACK_PRIO, "Task WAVE Blink", &os_err);
	#endif
	
}
static void uctsk_SendAck(void)
{	
	INT8U         		err;
	OS_CPU_SR  			cpu_sr = 0;
	extern OS_EVENT  	*Str_Q_SendAck ;
	MessageBuffer 		*msg_rcv_p  ;
	char 				msg_rcv[MESSAGE_BUFFER_SIZE] ;

	while(1){
		memset(msg_rcv,0,MESSAGE_BUFFER_SIZE);
        
		msg_rcv_p=OSQPend(Str_Q_SendAck,0,&err);//接受消息
		
		strncpy(msg_rcv,msg_rcv_p->buffer,MESSAGE_BUFFER_SIZE);
		OS_ENTER_CRITICAL();
		Message_Queue_Back(msg_rcv_p);
		OS_EXIT_CRITICAL();

#if DEBUG	&& DEBUG_SENDACK	
		printf("\r\n[ sendack = %s" , msg_rcv);
#endif
		
// 		OS_ENTER_CRITICAL();
// 		Message_Queue_Back(msg_rcv_p);
// 		OS_EXIT_CRITICAL();
		
		Send_AT(msg_rcv);	//利用串口4发送数据
	}		
}
/************************************************
	END FILE
************************************************/

