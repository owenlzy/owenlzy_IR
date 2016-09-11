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
#include "esp8266.h"


/* Private variables ---------------------------------------------------------*/
static  OS_STK         App_TaskWIFICtrlStk[APP_TASK_WIFI_CTRL_STK_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void uctsk_WIFI_Ctrl            	(void);
static u8 getID							(char *ack);
extern  void  App_ClientTCPCreate (void);



void  App_WIFICtrlTaskCreate (void)
{
    CPU_INT08U  os_err;

	os_err = os_err; /* prevent warning... */

	os_err = OSTaskCreate((void (*)(void *)) uctsk_WIFI_Ctrl,				
                          (void          * ) 0,							
                          (OS_STK        * )&App_TaskWIFICtrlStk[APP_TASK_WIFI_CTRL_STK_SIZE - 1],		
                          (INT8U           ) APP_TASK_WIFI_CTRL_PRIO  );							

	#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(APP_TASK_WIFI_CTRL_PRIO, "Task WIFI Blink", &os_err);
	#endif

}

static void uctsk_WIFI_Ctrl (void) 
{  
	int cpu_sr;  
	INT8U         err;

	MessageBuffer *ack_rcv_p , *cmd_rcv_p;	
	char cmd_rcv[MESSAGE_BUFFER_SIZE] , ack_rcv[MESSAGE_BUFFER_SIZE] ;
	int i ;
	int tcpid ;
	extern OS_EVENT      *Str_Q_WIFI2Ctrl , *Str_Q_Agent2Ctrl , *Sem_TCPSend; 

	while(1){
		/*****��ȡ��Ϣ�����е�����****/
//		printf("\r\nCTRL:[EVENT_TYPE1:%d]\r\n",Str_Q_WIFI2Ctrl->OSEventType);
		cmd_rcv_p=OSQPend(Str_Q_WIFI2Ctrl,0,&err) ;
		strncpy(cmd_rcv,cmd_rcv_p->buffer,MESSAGE_BUFFER_SIZE);
		//��Ϣ���л������ѱ�����,�ͷŵ�
		OS_ENTER_CRITICAL();
		Message_Queue_Back(cmd_rcv_p);
		OS_EXIT_CRITICAL();
		printf("\r\n<wifi ctrl>------Buffer is returned , flag=%d\r\n" , cmd_rcv_p->flag);
		printf("\r\n[ctrl]Command= [%s] err = %d\r\n" , cmd_rcv, err);
		Send_AT(cmd_rcv) ;		   //�������WIFI
//		printf("\r\n[ctrl]send to WIFI moduel OK\r\n");
		/*****��ȡACK��Ϣ����*****/
//		printf("[ctrl] 1 send at to wifi module ok.... waiting for ack...\r\n");
		ack_rcv_p=OSQPend(Str_Q_Agent2Ctrl,0,&err);
//		printf("[ctrl] 1 send at to wifi module ok.... waiting for ack...\r\n");
		strncpy(ack_rcv,ack_rcv_p->buffer,MESSAGE_BUFFER_SIZE);
		//��Ϣ���л������ѱ�����,�ͷŵ�
		OS_ENTER_CRITICAL();
		Message_Queue_Back(ack_rcv_p);
		OS_EXIT_CRITICAL();
		printf("\r\n<wifi ctrl>------Buffer is returned , flag=%d\r\n" , ack_rcv_p->flag);
		printf("\r\n[ctrl]Ack from wifi module = %s err=%d\r\n" ,ack_rcv,err);
		/*****����ACK**
		   	//1.�����TCP����������TCP����
			//2.�����TCP����������TCP����AT+CIPSTATUS
			//3.ά��id-taskid��
			//4.ack ok ,��ȡ�´�ָ��continue
		***/
		if((strstr(ack_rcv , "OK") != NULL)){
			//����AT+CIPSTATUS
			if((strstr(ack_rcv , "AT+CIPSTATUS") != NULL)){
				 //��ȡTCP id
				 tcpid = getID(ack_rcv) ;
				 printf("tcpid:%d\r\n" , tcpid);
				 //��������
				 App_ClientTCPCreate();
			}else if((strstr(ack_rcv , "AT+CIPCLOSE") != NULL)){
				//ɾ������
				OSTaskDelReq(APP_TASK_CLIENT_TCP_PRIO);
			}
			else if((strstr(ack_rcv , "SEND OK") != NULL)){
//				int cnt1,cnt2;				
//			    OS_ENTER_CRITICAL();
//				cnt1 =Sem_TCPSend->OSEventCnt;				
//				OSSemPost(Sem_TCPSend);			//�ź���V����
//				cnt2 = Sem_TCPSend->OSEventCnt;		
//				OS_EXIT_CRITICAL();
//				printf("[wifi_ctrl]1. sem will be V , cnt=%d , i=%d\r\n", cnt1 , i);
//				printf("[wifi_ctrl]2. sem has been V , cnt=%d , i=%d\r\n",cnt2 , i);
			}
			continue;
		//5.ack error,���·���ָ��
		}else if(strstr(ack_rcv , "ERROR") != NULL){
			if(strstr(ack_rcv , "ALREADY CONNECTED") != NULL){
				//��ʾTCP�Ѿ������ϣ������ط�ָ��
				continue;
			}else{
				//�ط�ָ��
				Send_AT(cmd_rcv) ;	
			}
		}else if(strstr(ack_rcv , "SEND FAIL") != NULL){
			   i = OSSemPost(Sem_TCPSend);
		} 
	}

}

static u8 getID(char *ack){
	char *p1 , *p2 ;
	int id = 0 , tetype = 0 ;
	char c1 , c2 ;
	int i ;

	p1 = strstr(ack , "+CIPSTATUS:") ;
	c1 =  *(p1+11);
	id = atoi(&c1);
	
	p2 = strstr(ack , "OK") ;
  	c2 = *(p2-5) ;
	tetype = atoi(&c2) ;
//	printf("[(%c,%x),(%c,%x]" , c1 ,c1 , c2 ,c2) ;
	return (id+tetype);

}




/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
