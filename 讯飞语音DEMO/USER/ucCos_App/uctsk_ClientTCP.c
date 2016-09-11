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


/* Private variables ---------------------------------------------------------*/
static  OS_STK         App_TaskClientTCPStk[APP_TASK_CLIENT_TCP_STK_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void uctsk_Client_TCP            (void);
static u8 getAPno						(char *ipd) ;



void  App_ClientTCPCreate (void)
{
    CPU_INT08U  os_err;

	os_err = os_err; /* prevent warning... */



	os_err = OSTaskCreate((void (*)(void *)) uctsk_Client_TCP,				
                          (void          * ) 0,							
                          (OS_STK        * )&App_TaskClientTCPStk[APP_TASK_CLIENT_TCP_STK_SIZE - 1],		
                          (INT8U           ) APP_TASK_CLIENT_TCP_PRIO  );
						  							

	#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(APP_TASK_CLIENT_TCP_PRIO, "Task WIFI Blink", &os_err);
	#endif

	
}

static void uctsk_Client_TCP (void) 
{  
	u8 err ;
	OS_CPU_SR  cpu_sr = 0;

	MessageBuffer *tcp_rcv_p , *tcp_ack_rcv_p;
	char  tcp_rcv[MESSAGE_BUFFER_SIZE], tcp_ack_rcv[MESSAGE_BUFFER_SIZE], online[MESSAGE_BUFFER_SIZE]="car is online";
	extern OS_EVENT  	 *Sem_TCPSend; 
	extern OS_EVENT      *Str_Q_Agent2Client , *Str_Q_Client2led , *Str_Q_Client2Motor , *Str_Q_Client2Ultrasonic , *Str_Q_Client2Sound;
	extern	MessageBuffer mb[MESSAGE_BUFFER_NUM] ; //��Ϣ���л�����
	int message_index ;
	int i , j , no;
	char *p_head;
	char  *data = "login_robot-a123_v44eN8EYiW1O2QUMHBPtT6LsiBtSIEA0uBiMNHA6NZUqXvB19KiJISAHGmwFlEdAFvbZJvRdVllE1eXbIoRvyQ==";
	char init_message[20] ;
	int flag ;

	
	/*************1.1.���͵�½��Ϣ��TCP������*********************/
	OSSemPend(Sem_TCPSend,0,&err);	//�ź���P����
	if(err == OS_ERR_NONE){
		printf("\r\nlogin:sem has been P , cnt=%d , err=%d\r\n",Sem_TCPSend->OSEventCnt , err);
		Send_Data(data);
	}
	OSSemPost(Sem_TCPSend);
	
	/*************2. while�ȴ�TCP��Ϣ������ָ�*********************/
	while(1)
   	{
//		printf("\r\n[client] waiting for message...\r\n");
		i=0;
		flag = 0 ;
		//2.1�ȴ�Agent���͹�������Ϣ����
		tcp_rcv_p=OSQPend(Str_Q_Agent2Client,0,&err) ;
		strncpy(tcp_rcv,tcp_rcv_p->buffer,MESSAGE_BUFFER_SIZE);
		printf("\r\n[client]move command=%s\r\n",tcp_rcv);
		//��Ϣ���л������ѱ�����,�ͷŵ�
		OS_ENTER_CRITICAL();
		Message_Queue_Back(tcp_rcv_p);
		OS_EXIT_CRITICAL();
		printf("\r\n<clientTCP>------Buffer is returned , flag=%d\r\n" , tcp_rcv_p->flag);
		//OSTimeDlyHMSM(0, 0, 3, 0); 			//ÿ3Sģ������������˶�ָ���С��
		//printf("[client_tcp]С�����յ����������͵���Ϣ:%s\r\n" , tcp_rcv);
		/***************�������˶�ָ��******************************/
		if(strstr(tcp_rcv , "move") ||strstr(tcp_rcv , "gear") ){
			 i = 1 ;
		}	
		else if(strstr(tcp_rcv , "true") || strstr(tcp_rcv , "ack_2") || strstr(tcp_rcv , "ack_30")){
		/***************������С������ָ��******************************/
			i = 2 ;
		}else if(strstr(tcp_rcv , "say")){
		/***************������С��˵��ָ��******************************/
			i = 3 ;
		}else if(strstr(tcp_rcv , "led")){
		/***************������С����˸ָ��******************************/
			i = 4 ;
		}

		switch(i){
			case 1:
				/****�������������*****/
				//��������Ϣ���л�����
				p_head = strstr(tcp_rcv,"\r\n+IPD");
				if(p_head == NULL)
					printf("[tcp_client] Error! not find move command IPD! \r\n");

				OS_ENTER_CRITICAL();
				message_index = Message_Queue_Available(mb);
				OS_EXIT_CRITICAL();
				printf("\r\n<clientTCP>-------Buffer is available , Index=%d\r\n" , message_index);
				strncpy(mb[message_index].buffer,p_head,MESSAGE_BUFFER_SIZE-(p_head-tcp_rcv));
				err = OSQPost(Str_Q_Client2Motor,&mb[message_index]) ;	
				//������������ݵ�ACK���˶���Ϣ�����ˣ���Ҫ��������ACK
				if(strstr(tcp_rcv , "ack_2") || strstr(tcp_rcv , "ack_30"))
					flag = 1 ;
				break ;
			case 2:
				/****����������������*****/
				if(strstr(tcp_rcv , "true")){
					/*************���Ϸ���������LED,������Ӧ����*********************/
					memset(init_message,0,20) ;
					sprintf(init_message , "%s" , "led_face");
					OS_ENTER_CRITICAL();
					message_index = Message_Queue_Available(mb);
					OS_EXIT_CRITICAL();
					printf("\r\n<clientTCP>-------Buffer is available , Index=%d\r\n" , message_index);
					strncpy(mb[message_index].buffer,init_message,strlen(init_message));
					err = OSQPost(Str_Q_Client2led,&mb[message_index]) ;
				
					memset(init_message,0,20) ;
					sprintf(init_message , "%s" , "sound_connected");
					OS_ENTER_CRITICAL();
					printf("\r\n<clientTCP>-------Buffer is available , Index=%d\r\n" , message_index);
					message_index = Message_Queue_Available(mb);
					OS_EXIT_CRITICAL();
					strncpy(mb[message_index].buffer,init_message,strlen(init_message));
					err = OSQPost(Str_Q_Client2Sound,&mb[message_index]) ;

					OS_ENTER_CRITICAL();
					message_index = Message_Queue_Available(mb);
					OS_EXIT_CRITICAL();
					printf("\r\n<clientTCP>-------Buffer is available , Index=%d\r\n" , message_index);
					strncpy(mb[message_index].buffer,online,MESSAGE_BUFFER_SIZE);
					err = OSQPost(Str_Q_Client2Ultrasonic,&mb[message_index]);
				}else{
					OS_ENTER_CRITICAL();
					message_index = Message_Queue_Available(mb);
					OS_EXIT_CRITICAL();
					strncpy(mb[message_index].buffer,tcp_rcv,strlen(tcp_rcv));
					err = OSQPost(Str_Q_Client2Ultrasonic,&mb[message_index]);
				}
				break;
			case 3:
				/****������˵������*****/
				//��������Ϣ���л�����
				p_head = strstr(tcp_rcv,"\r\n+IPD");
				OS_ENTER_CRITICAL();
				message_index = Message_Queue_Available(mb);
				OS_EXIT_CRITICAL();
				printf("\r\n<clientTCP>-------Buffer is available , Index=%d\r\n" , message_index);
				strncpy(mb[message_index].buffer,p_head,MESSAGE_BUFFER_SIZE-(p_head-tcp_rcv));
				err = OSQPost(Str_Q_Client2Sound,&mb[message_index]) ;
				//������������ݵ�ACK���˶���Ϣ�����ˣ���Ҫ��������ACK
				if(strstr(tcp_rcv , "ack_2") || strstr(tcp_rcv , "ack_30"))
					flag = 1 ;
				break;
			case 4:
				/****������LED����*****/
				//��������Ϣ���л�����
				p_head = strstr(tcp_rcv,"\r\n+IPD");
				OS_ENTER_CRITICAL();
				message_index = Message_Queue_Available(mb);
				OS_EXIT_CRITICAL();
				printf("\r\n<clientTCP>-------Buffer is available , Index=%d\r\n" , message_index);
				strncpy(mb[message_index].buffer,p_head,MESSAGE_BUFFER_SIZE-(p_head-tcp_rcv));
				err = OSQPost(Str_Q_Client2led,&mb[message_index]) ;
				//������������ݵ�ACK���˶���Ϣ�����ˣ���Ҫ��������ACK
				if(strstr(tcp_rcv , "ack_2") || strstr(tcp_rcv , "ack_30"))
					flag = 1 ;
				break;
			default:
				break;
		}
		if(flag == 1){
			//˵����������ACK��ָ���ں���һ���ˣ���Ҫ��������
			OS_ENTER_CRITICAL();
			message_index = Message_Queue_Available(mb);
			OS_EXIT_CRITICAL();
			printf("\r\n<clientTCP>-------Buffer is available , Index=%d\r\n" , message_index);
			strncpy(mb[message_index].buffer,tcp_rcv,strlen(tcp_rcv));
			err = OSQPost(Str_Q_Client2Ultrasonic,&mb[message_index]);
		}
      	//OSTimeDlyHMSM(0, 0, 1, 0);							 /* Delay One minute */
    }

	
									
}
static u8 getAPno(char *ipd){
   	char *p1  , *p2 ;
	char *p_head ;
	int i ;
	char num[10];

	p_head = strstr(ipd,"+IPD");

   	p1 = strstr(p_head , ",");
   	p2 = p1+1 ;
   	while(*p2!=',')
   		p2++ ;

	i=0;
	while(p1 != p2){
		p1++;
		num[i++]=*p1;
	}
	num[i]='\0';
	return atoi(num);
	
}





/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
