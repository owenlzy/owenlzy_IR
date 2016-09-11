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
#include <includes.h>
#include "esp8266.h"
#include "library_stm32.h"
#include "gpio.h"
#include "pwm.h"
#include "servohand.h"
#include "motor.h"
#include "ultrasonic.h"
#include "adc.h"


/* Private variables ---------------------------------------------------------*/
static  OS_STK         App_TaskCScriptStk[APP_TASK_CScriptCPicoc_STK_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void uctsk_CScriptCPicoc	(void) ;
static int parse_str(int *forPices,char *destns,char *strs);
//void transCCode	(char *cStr);

void  App_CScriptCPicocCreate (void)
{
    CPU_INT08U  os_err;
	os_err = os_err; /* prevent warning... */

	os_err = OSTaskCreate((void (*)(void *)) uctsk_CScriptCPicoc,				
                          (void          * ) 0,							
                          (OS_STK        * )&App_TaskCScriptStk[APP_TASK_CScriptCPicoc_STK_SIZE - 1],		
                          (INT8U           ) APP_TASK_CScriptCPicoc_PRIO  );							

	#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(APP_TASK_CScriptCPicoc_PRIO, "Task ULTRASONIC Blink", &os_err);
	#endif

}
static void uctsk_CScriptCPicoc (void) 
{  
 	OS_CPU_SR 			cpu_sr=0;
	extern OS_EVENT     *Str_Q_CScript ;
	extern int 			backvalue ;
	INT8U         		errxlib; 
	
	int 	message_index=0 ,forPices = 0;//forPices表示前一片的值
	u8 		err ;
	extern	MessageBuffer mb[MESSAGE_BUFFER_NUM] ; 					  //消息队列缓冲区
	MessageBuffer *msg_rcv_p  ;
    
    char cscript_buff[128] = { 0 };
    char cs_strs[1000] = { 0 } ;

#if DEBUG && DEBUG_CSCRIPT
    printf("\r\n---------------- For CScript CPicoc Running -------------------\r\n");
#endif 

    while(1)
    {
        memset(cscript_buff , 0 , 128);
        
        msg_rcv_p = OSQPend(Str_Q_CScript,0,&errxlib); 		//等待消息到来		
		strncpy(cscript_buff,msg_rcv_p->buffer,MESSAGE_BUFFER_SIZE);
		OS_ENTER_CRITICAL();
		Message_Queue_Back(msg_rcv_p);
		OS_EXIT_CRITICAL();
        
        if(parse_str(&forPices,cs_strs,cscript_buff))
        {
            forPices =0 ;
#if DEBUG	&& DEBUG_CSCRIPT				
            printf("\r\n> Running CScript..[%s]....",cs_strs);
#endif					
            CPicoc(cs_strs);
            memset(cs_strs,0,1000);
            forPices =0 ;
        }
    }
}
static int parse_str(int *forPices,char *destns,char *strs)
{
	extern OS_EVENT  	*Str_Q_SendAck ;
    OS_CPU_SR 			cpu_sr=0;
    u8 		err ;
	extern	MessageBuffer mb[MESSAGE_BUFFER_NUM] ; 					  //消息队列缓冲区
	MessageBuffer *msg_rcv_p  ;
    int 	message_index=0 ,sindex =0 ,dlen =0;
    
    int  cs_size = 0 , curt_pices = 0 ,cur_len = 0,count=0 ;
    char *s_size = NULL ,*s_pices=NULL, *s_cntnt = NULL;
    char temp[6] = { 0 } ;// stemp[23]={0};
    
    s_size = strstr(strs,"_")+1 ;
    s_pices = strstr(strs,",")+1 ;
    s_cntnt = strstr(strs,":")+1 ;

    //获取当前传送的字节数
    cur_len = strlen(s_cntnt);
    //获取总的字节数
    strncpy(temp , s_size,strlen(s_size)-strlen(s_pices)-1);
    temp[strlen(temp)] = '\0';
    cs_size = atoi(temp) ;
    memset(temp,0,6);

    strncpy(temp , s_pices,strlen(s_pices)-strlen(s_cntnt)-1);
    temp[strlen(temp)] = '\0';
    curt_pices = atoi(temp) ;
    memset(temp,0,6);

  //  printf("\r\nn---------------CScriptApp[---Pices Len:{%d:%s}---]",cur_len,s_cntnt);
    
    dlen = strlen(destns) ;
    while(sindex < strlen(s_cntnt)){
        //if(dlen ==0)
            *(destns+dlen+sindex) = *(s_cntnt+sindex);
        //else
        //    *(destns+dlen+sindex-1) = *(s_cntnt+sindex);
        sindex += 1 ;
    }

#if DEBUG && DEBUG_CSCRIPT   
    printf("\r\nCScriptApp:[%d:{%s}]\r\n",curt_pices,destns);
#endif
		
    if(strlen(destns) == cs_size){
#if DEBUG	&& DEBUG_CSCRIPT		
        printf("\r\nCStr Len = %d  curt_lend=%d",strlen(destns),cs_size);
#endif			
        return 1;
    }
    else{
#if DEBUG	&& DEBUG_CSCRIPT		
        printf("\r\nCStr Len = %d  curt_lend=%d",strlen(destns),cs_size);
#endif			
        return 0;
    }
    
}


/*********************************************************************************************************
        if(cur_len < 20)
    {
        if(((curt_pices-1)*20 + cur_len) == cs_size)
            return 1 ;
        else 
            return 0;
    }else if(cur_len == 20){
        if(curt_pices*20 == cs_size)
            return 1;
        else
            return 0;
    }
    END FILE
*********************************************************************************************************/
