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
static  OS_STK         App_TaskUsartAgentStk[APP_TASK_USART_AGENT_STK_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void 	uctsk_WIFI_Usart_Agent      (void);
static u8 		getDataLength				(char *ipd);
static u8 		getAPno						(char *ipd);
static u32 		getAPDataLength				(char *ipd);

static u8 getScriptSize(char *prefix) ;
static u8 getScripti(char *prefix);

void  App_UsartAgentCreate (void)
{
    CPU_INT08U  os_err;

	os_err = os_err; /* prevent warning... */

	os_err = OSTaskCreate((void (*)(void *)) uctsk_WIFI_Usart_Agent,				
                          (void          * ) 0,							
                          (OS_STK        * )&App_TaskUsartAgentStk[APP_TASK_USART_AGENT_STK_SIZE - 1],		
                          (INT8U           ) APP_TASK_USART_AGENT_PRIO  );							

	#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(APP_TASK_USART_AGENT_PRIO, "Task WIFI Blink", &os_err);
	#endif

}



static void uctsk_WIFI_Usart_Agent (void){
	int 	cpu_sr;
	u8 		err ;
	extern  OS_EVENT	 *Sem_WIFIrespose;							  //信号量
	extern 	OS_EVENT     *Str_Q_xlib;			//C语言解释器消息队列
    extern 	OS_EVENT     *Str_Q_CScript;			//C语言解释器消息队列
    
	extern  Buffer 		  cb ;	 									  //环形缓冲区
	extern	MessageBuffer mb[MESSAGE_BUFFER_NUM] ; 					  //消息队列缓冲区
	unsigned char rrrr;						//接收环形缓冲区的数据 
	char 	*atcmd = "EHECKQ\r\n" ;
	char 	recvBuffer[BUFFER_SIZE] ;			//接受缓冲区 Len=128 
	int 	recvLen = 0 , recvIndex = 0 ;
	char	 *rTemp , *cstrs;						//用作保存 判断是否有字串首次出现的地址
	int 	message_index=0 ;
	
	//C 脚本变量
	//script_size： 脚本字节大小
	//script_count：脚本分片总数 ，script_count = script_size/每片大小
	//script_i ：   脚本第几次分片 ， 小于等于script_count
	//script_slice_size: 脚本分片大小
	//u8 script_size , script_count , script_i  , script_slice_size=20;
	//u8 content_size , content_i = 0 ;
    int cur_pices = 0 , cs_size =0 ,last_len =0;
    char *s_size = NULL ,*s_pices=NULL, *s_cntnt = NULL;
    char temp[6] = {0};
	
	//向WIFI发送指令
	int i ;
	
// 	OSTimeDlyHMSM(0, 0, 2, 0);	
	while((USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET)){
		;
	}
	for(i=0;i<strlen(atcmd);i++){
		USART_SendData(UART4, atcmd[i]);//向串口3发送数据
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET)
			;//等待发送结束	 
	}

#if DEBUG	&& DEBUG_USARTAGENT
	printf("check command : first command send ok\r\n");
#endif
	
	servo3_turn(150);
	memset(recvBuffer , 0 , BUFFER_SIZE);
	while(1)
	{
		
		OSSemPend(Sem_WIFIrespose,0,&err);	//信号量P操作 串口读取一个数据，这里取一个数据

		OS_ENTER_CRITICAL();
		rrrr=rb_get(&cb);
		OS_EXIT_CRITICAL();
		//if(rrrr != ' ' || rrrr!=NULL)
			//printf("%c",rrrr);
		
		if(recvIndex >= BUFFER_SIZE)
		{
			recvIndex = 0 ;
			recvBuffer[recvIndex++] = rrrr ;
		}else{			
			recvBuffer[recvIndex++] = rrrr ;
			//在这里判断是变长数据，还是定长数据 strstr(recvBuffer , "FFE")
			if(recvIndex >= 3)
			{
				if(recvBuffer[0]=='F'&&recvBuffer[1]=='F'&&recvBuffer[2]=='E')
				{	//变长 , GPP
					if( strstr(recvBuffer,"\r\n"))
					{	//当接收到此字符串时，表示本次接收完成  然后利用消息队列传递给C解释器任务
						//printf("\r\n{%s}\r\n" , recvBuffer);
						strcat(recvBuffer,"void main(){}$>");
						OS_ENTER_CRITICAL();
						message_index = Message_Queue_Available(mb);
						OS_EXIT_CRITICAL();
						strncpy(mb[message_index].buffer,recvBuffer,BUFFER_SIZE);
						err = OSQPost(Str_Q_xlib,&mb[message_index]);	//发送队列
						memset(recvBuffer , 0 , BUFFER_SIZE);	//一次接收完后，将缓存清空
						recvIndex = 0 ;
					}
				}
				else if(recvBuffer[0]=='F'&&recvBuffer[1]=='F'&&recvBuffer[2]=='F')
				{	//定长 Notepad++ C script
                    //if( strstr(recvBuffer,"\r\n"))
                   if(strstr(recvBuffer,":"))
                   {
                        s_size = strstr(recvBuffer,"_")+1 ;
                        s_pices = strstr(recvBuffer,",")+1 ;
                        s_cntnt = strstr(recvBuffer,":")+1 ;

                        strncpy(temp , s_size,strlen(s_size)-strlen(s_pices)-1);
                        temp[strlen(temp)] = '\0';
                        cs_size = atoi(temp) ;
                        memset(temp,0,6);
                       
                        strncpy(temp , s_pices,strlen(s_pices)-strlen(s_cntnt)-1);
                        temp[strlen(temp)] = '\0';
                        cur_pices = atoi(temp) ;
                        memset(temp,0,6);
                       
                       if(strlen(s_cntnt)==50||strlen(s_cntnt)==52)
                        {	
                          //  printf("\r\nCScript:[%s]",recvBuffer);
                            //当接收到此字符串时，表示本次接收完成  然后利用消息队列传递给C解释器任务
                            //printf("\r\n{%s}\r\n" , recvBuffer);                        
                            OS_ENTER_CRITICAL();
                            message_index = Message_Queue_Available(mb);
                            OS_EXIT_CRITICAL();
                            strncpy(mb[message_index].buffer,recvBuffer,BUFFER_SIZE);
                            err = OSQPost(Str_Q_CScript,&mb[message_index]);	//发送队列
                            memset(recvBuffer , 0 , BUFFER_SIZE);	//一次接收完后，将缓存清空
                            recvIndex = 0 ;
                        }else{
                            last_len = cs_size-(cur_pices-1)*50 ;
                            if(strlen(s_cntnt) == last_len)
                            {	
                              //  printf("\r\nCScript:[%s]",recvBuffer);
                                //当接收到此字符串时，表示本次接收完成  然后利用消息队列传递给C解释器任务
                                //printf("\r\n{%s}\r\n" , recvBuffer);                        
                                OS_ENTER_CRITICAL();
                                message_index = Message_Queue_Available(mb);
                                OS_EXIT_CRITICAL();
                                strncpy(mb[message_index].buffer,recvBuffer,BUFFER_SIZE);
                                err = OSQPost(Str_Q_CScript,&mb[message_index]);	//发送队列
                                memset(recvBuffer , 0 , BUFFER_SIZE);	//一次接收完后，将缓存清空
                                recvIndex = 0 ;
                            }
                        }
                   }
                    
				}else{
					
					if( strstr(recvBuffer,"\r\n"))
					{
#if DEBUG	&& DEBUG_USARTAGENT
						printf("\r\nother data:  %s \r\n",recvBuffer);
#endif						
						memset(recvBuffer , 0 , BUFFER_SIZE);	//一次接收完后，将缓存清空
						recvIndex = 0 ;
					}				
				}
			}
		}
	}
}

/*****************************************************************************


if( strstr(recvBuffer,":")){
    //u8 script_size , script_count , script_i  , script_slice_size=20;
    //检测到:即检测到第一个分片
    script_i = getScripti(recvBuffer);
    //如果是第一个分片且没有开始接收后面的数据，计算出script_size，script_count
    if(content_i == 0 && script_i == 1  ){
            script_size = getScriptSize(recvBuffer);	//脚本大小
            printf("script_size = %d\r\n" , script_size) ;
            script_count = (script_size % script_slice_size == 0) ? script_size / script_slice_size:script_size / script_slice_size+1; //分片数量
            printf("script_count = %d\r\n" , script_count) ;
    }
    //如果还没有开始接收后面的数据，计算出需要接收的数据的大小
    if(content_i == 0){
            content_size = (script_i == script_count)? (script_size - (script_i-1)*20): 20;
            printf("content_size = %d\r\n" , content_size) ;
    }
    content_i++;
    if(content_i == content_size){
        //把这一片发送出去
        printf("SCRIPT:%s\r\n" , recvBuffer) ;
        content_i = 0 ;
        memset(recvBuffer , 0 , BUFFER_SIZE);
    }
}
*****************************************************************************/


static u8 getScriptSize(char *prefix){
	char *p1 , *p2 ;
	int i = 0;
	char num[10];

	p1 = strstr(prefix  , "_") ;	
	p2 = strstr(prefix  , ",") ;
	while(p1 != p2){
		p1++;
		num[i++]=*p1;
	}
	num[i]='\0';
	return atoi(num);
}

static u8 getScripti(char *prefix){
	char *p1 , *p2 ;
	int i = 0;
	char num[10];

	p1 = strstr(prefix  , ",") ;	
	p2 = strstr(prefix  , ":") ;
	while(p1 != p2){
		p1++;
		num[i++]=*p1;
	}
	num[i]='\0';
	return atoi(num);
}

					
static u8 getDataLength(char *ipd){
	char *p1 , *p2 ;
	int id = 0 , tetype = 0 ;
	char c1 , c2 ;
	int i;

	char num[10];

	p1 = strstr(ipd , ",") ;
//	c1 =  *(p1+11);
//	id = atoi(&c1);
	
	p2 = strstr(ipd , ":") ;
	i=0;
	while(p1 != p2){
		p1++;
		num[i++]=*p1;

	}
	num[i]='\0';
//  	c2 = *(p2-5) ;
//	tetype = atoi(&c2) ;
//	printf("[(%c,%x),(%c,%x]" , c1 ,c1 , c2 ,c2) ;
	return atoi(num);
}

static u8 getAPno(char *ipd){
   	char *p1  , *p2 , *p3;
	int i ;
	char num[10];

   	p1 = strstr(ipd , "\r\n+IPD");
   	p2 = strstr(p1 , ",");
   	p3 = p2+1 ;
   	while(*p3!=',')
   	p3++ ;

	i=0;
	while(p2 != p3){
		p2++;
		num[i++]=*p2;
	}
	num[i]='\0';
	return atoi(num);
	
}

static u32 getAPDataLength(char *ipd){
	char *p1 ,*p2;
	int i ;
	char num[10];

	p1 =  strstr(ipd , ":") ;
	p2 = p1 ;
	while(*p2 != ',')
		p2 --;

//	printf("{%c %c %c %c}" , *p2 ,*(p2+1) , *(p2+2) , *(p2+3)) ;
	i=0;
	p2++ ;
	while(p2 != p1){
		num[i++]=*p2;
		p2++ ;
	}
	num[i]='\0';
//	printf("{%s}" , num);
	return atoi(num);

}



/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
