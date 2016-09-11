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
	extern	MessageBuffer mb[MESSAGE_BUFFER_NUM] ; //消息队列缓冲区
	int message_index ;
	int i , j , no;
	char *p_head;
	char  *data = "login_robot-a123_v44eN8EYiW1O2QUMHBPtT6LsiBtSIEA0uBiMNHA6NZUqXvB19KiJISAHGmwFlEdAFvbZJvRdVllE1eXbIoRvyQ==";
	char init_message[20] ;
	int flag ;

	
	/*************1.1.发送登陆信息给TCP服务器*********************/
	OSSemPend(Sem_TCPSend,0,&err);	//信号量P操作
	if(err == OS_ERR_NONE){
		printf("\r\nlogin:sem has been P , cnt=%d , err=%d\r\n",Sem_TCPSend->OSEventCnt , err);
		Send_Data(data);
	}
	OSSemPost(Sem_TCPSend);
	
	/*************2. while等待TCP消息（控制指令）*********************/
	while(1)
   	{
//		printf("\r\n[client] waiting for message...\r\n");
		i=0;
		flag = 0 ;
		//2.1等待Agent发送过来的消息队列
		tcp_rcv_p=OSQPend(Str_Q_Agent2Client,0,&err) ;
		strncpy(tcp_rcv,tcp_rcv_p->buffer,MESSAGE_BUFFER_SIZE);
		printf("\r\n[client]move command=%s\r\n",tcp_rcv);
		//消息队列缓冲区已被用完,释放掉
		OS_ENTER_CRITICAL();
		Message_Queue_Back(tcp_rcv_p);
		OS_EXIT_CRITICAL();
		printf("\r\n<clientTCP>------Buffer is returned , flag=%d\r\n" , tcp_rcv_p->flag);
		//OSTimeDlyHMSM(0, 0, 3, 0); 			//每3S模拟服务器发送运动指令给小车
		//printf("[client_tcp]小车接收到服务器发送的消息:%s\r\n" , tcp_rcv);
		/***************解析是运动指令******************************/
		if(strstr(tcp_rcv , "move") ||strstr(tcp_rcv , "gear") ){
			 i = 1 ;
		}	
		else if(strstr(tcp_rcv , "true") || strstr(tcp_rcv , "ack_2") || strstr(tcp_rcv , "ack_30")){
		/***************解析是小车上线指令******************************/
			i = 2 ;
		}else if(strstr(tcp_rcv , "say")){
		/***************解析是小车说话指令******************************/
			i = 3 ;
		}else if(strstr(tcp_rcv , "led")){
		/***************解析是小车闪烁指令******************************/
			i = 4 ;
		}

		switch(i){
			case 1:
				/****发送至电机任务*****/
				//拷贝到消息队列缓冲区
				p_head = strstr(tcp_rcv,"\r\n+IPD");
				if(p_head == NULL)
					printf("[tcp_client] Error! not find move command IPD! \r\n");

				OS_ENTER_CRITICAL();
				message_index = Message_Queue_Available(mb);
				OS_EXIT_CRITICAL();
				printf("\r\n<clientTCP>-------Buffer is available , Index=%d\r\n" , message_index);
				strncpy(mb[message_index].buffer,p_head,MESSAGE_BUFFER_SIZE-(p_head-tcp_rcv));
				err = OSQPost(Str_Q_Client2Motor,&mb[message_index]) ;	
				//如果传感器数据的ACK和运动信息交融了，需要单独发送ACK
				if(strstr(tcp_rcv , "ack_2") || strstr(tcp_rcv , "ack_30"))
					flag = 1 ;
				break ;
			case 2:
				/****发送至超声波任务*****/
				if(strstr(tcp_rcv , "true")){
					/*************连上服务器后，让LED,喇叭响应动作*********************/
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
				/****发送至说话任务*****/
				//拷贝到消息队列缓冲区
				p_head = strstr(tcp_rcv,"\r\n+IPD");
				OS_ENTER_CRITICAL();
				message_index = Message_Queue_Available(mb);
				OS_EXIT_CRITICAL();
				printf("\r\n<clientTCP>-------Buffer is available , Index=%d\r\n" , message_index);
				strncpy(mb[message_index].buffer,p_head,MESSAGE_BUFFER_SIZE-(p_head-tcp_rcv));
				err = OSQPost(Str_Q_Client2Sound,&mb[message_index]) ;
				//如果传感器数据的ACK和运动信息交融了，需要单独发送ACK
				if(strstr(tcp_rcv , "ack_2") || strstr(tcp_rcv , "ack_30"))
					flag = 1 ;
				break;
			case 4:
				/****发送至LED任务*****/
				//拷贝到消息队列缓冲区
				p_head = strstr(tcp_rcv,"\r\n+IPD");
				OS_ENTER_CRITICAL();
				message_index = Message_Queue_Available(mb);
				OS_EXIT_CRITICAL();
				printf("\r\n<clientTCP>-------Buffer is available , Index=%d\r\n" , message_index);
				strncpy(mb[message_index].buffer,p_head,MESSAGE_BUFFER_SIZE-(p_head-tcp_rcv));
				err = OSQPost(Str_Q_Client2led,&mb[message_index]) ;
				//如果传感器数据的ACK和运动信息交融了，需要单独发送ACK
				if(strstr(tcp_rcv , "ack_2") || strstr(tcp_rcv , "ack_30"))
					flag = 1 ;
				break;
			default:
				break;
		}
		if(flag == 1){
			//说明超声波的ACK和指令融合在一起了，需要单独发送
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
