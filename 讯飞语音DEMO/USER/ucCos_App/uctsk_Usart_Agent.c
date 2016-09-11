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
	extern  OS_EVENT	 *Sem_WIFIrespose;							  //�ź���
	extern 	OS_EVENT     *Str_Q_xlib;			//C���Խ�������Ϣ����
    extern 	OS_EVENT     *Str_Q_CScript;			//C���Խ�������Ϣ����
    
	extern  Buffer 		  cb ;	 									  //���λ�����
	extern	MessageBuffer mb[MESSAGE_BUFFER_NUM] ; 					  //��Ϣ���л�����
	unsigned char rrrr;						//���ջ��λ����������� 
	char 	*atcmd = "EHECKQ\r\n" ;
	char 	recvBuffer[BUFFER_SIZE] ;			//���ܻ����� Len=128 
	int 	recvLen = 0 , recvIndex = 0 ;
	char	 *rTemp , *cstrs;						//�������� �ж��Ƿ����ִ��״γ��ֵĵ�ַ
	int 	message_index=0 ;
	
	//C �ű�����
	//script_size�� �ű��ֽڴ�С
	//script_count���ű���Ƭ���� ��script_count = script_size/ÿƬ��С
	//script_i ��   �ű��ڼ��η�Ƭ �� С�ڵ���script_count
	//script_slice_size: �ű���Ƭ��С
	//u8 script_size , script_count , script_i  , script_slice_size=20;
	//u8 content_size , content_i = 0 ;
    int cur_pices = 0 , cs_size =0 ,last_len =0;
    char *s_size = NULL ,*s_pices=NULL, *s_cntnt = NULL;
    char temp[6] = {0};
	
	//��WIFI����ָ��
	int i ;
	
// 	OSTimeDlyHMSM(0, 0, 2, 0);	
	while((USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET)){
		;
	}
	for(i=0;i<strlen(atcmd);i++){
		USART_SendData(UART4, atcmd[i]);//�򴮿�3��������
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET)
			;//�ȴ����ͽ���	 
	}

#if DEBUG	&& DEBUG_USARTAGENT
	printf("check command : first command send ok\r\n");
#endif
	
	servo3_turn(150);
	memset(recvBuffer , 0 , BUFFER_SIZE);
	while(1)
	{
		
		OSSemPend(Sem_WIFIrespose,0,&err);	//�ź���P���� ���ڶ�ȡһ�����ݣ�����ȡһ������

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
			//�������ж��Ǳ䳤���ݣ����Ƕ������� strstr(recvBuffer , "FFE")
			if(recvIndex >= 3)
			{
				if(recvBuffer[0]=='F'&&recvBuffer[1]=='F'&&recvBuffer[2]=='E')
				{	//�䳤 , GPP
					if( strstr(recvBuffer,"\r\n"))
					{	//�����յ����ַ���ʱ����ʾ���ν������  Ȼ��������Ϣ���д��ݸ�C����������
						//printf("\r\n{%s}\r\n" , recvBuffer);
						strcat(recvBuffer,"void main(){}$>");
						OS_ENTER_CRITICAL();
						message_index = Message_Queue_Available(mb);
						OS_EXIT_CRITICAL();
						strncpy(mb[message_index].buffer,recvBuffer,BUFFER_SIZE);
						err = OSQPost(Str_Q_xlib,&mb[message_index]);	//���Ͷ���
						memset(recvBuffer , 0 , BUFFER_SIZE);	//һ�ν�����󣬽��������
						recvIndex = 0 ;
					}
				}
				else if(recvBuffer[0]=='F'&&recvBuffer[1]=='F'&&recvBuffer[2]=='F')
				{	//���� Notepad++ C script
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
                            //�����յ����ַ���ʱ����ʾ���ν������  Ȼ��������Ϣ���д��ݸ�C����������
                            //printf("\r\n{%s}\r\n" , recvBuffer);                        
                            OS_ENTER_CRITICAL();
                            message_index = Message_Queue_Available(mb);
                            OS_EXIT_CRITICAL();
                            strncpy(mb[message_index].buffer,recvBuffer,BUFFER_SIZE);
                            err = OSQPost(Str_Q_CScript,&mb[message_index]);	//���Ͷ���
                            memset(recvBuffer , 0 , BUFFER_SIZE);	//һ�ν�����󣬽��������
                            recvIndex = 0 ;
                        }else{
                            last_len = cs_size-(cur_pices-1)*50 ;
                            if(strlen(s_cntnt) == last_len)
                            {	
                              //  printf("\r\nCScript:[%s]",recvBuffer);
                                //�����յ����ַ���ʱ����ʾ���ν������  Ȼ��������Ϣ���д��ݸ�C����������
                                //printf("\r\n{%s}\r\n" , recvBuffer);                        
                                OS_ENTER_CRITICAL();
                                message_index = Message_Queue_Available(mb);
                                OS_EXIT_CRITICAL();
                                strncpy(mb[message_index].buffer,recvBuffer,BUFFER_SIZE);
                                err = OSQPost(Str_Q_CScript,&mb[message_index]);	//���Ͷ���
                                memset(recvBuffer , 0 , BUFFER_SIZE);	//һ�ν�����󣬽��������
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
						memset(recvBuffer , 0 , BUFFER_SIZE);	//һ�ν�����󣬽��������
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
    //��⵽:����⵽��һ����Ƭ
    script_i = getScripti(recvBuffer);
    //����ǵ�һ����Ƭ��û�п�ʼ���պ�������ݣ������script_size��script_count
    if(content_i == 0 && script_i == 1  ){
            script_size = getScriptSize(recvBuffer);	//�ű���С
            printf("script_size = %d\r\n" , script_size) ;
            script_count = (script_size % script_slice_size == 0) ? script_size / script_slice_size:script_size / script_slice_size+1; //��Ƭ����
            printf("script_count = %d\r\n" , script_count) ;
    }
    //�����û�п�ʼ���պ�������ݣ��������Ҫ���յ����ݵĴ�С
    if(content_i == 0){
            content_size = (script_i == script_count)? (script_size - (script_i-1)*20): 20;
            printf("content_size = %d\r\n" , content_size) ;
    }
    content_i++;
    if(content_i == content_size){
        //����һƬ���ͳ�ȥ
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
