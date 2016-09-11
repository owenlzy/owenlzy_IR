#include <includes.h> 
#include "gpio.h"
#include "esp8266.h"
#include "usart.h"
#include "string.h"


/* Private variables ---------------------------------------------------------*/
static  OS_STK         App_TaskLaserStk[APP_TASK_LASER_STK_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void uctsk_Laser            ();

extern u16 USART1_RX_STA;   
extern char USART1_RX_BUF[USART1_REC_LEN]; 

void  App_LaserCreate (void)
{
    CPU_INT08U  os_err;

	os_err = os_err; /* prevent warning... */
	os_err = OSTaskCreate((void (*)(void *)) uctsk_Laser,				
					  (void          * ) 0,							
					  (OS_STK        * )&App_TaskLaserStk[APP_TASK_LASER_STK_SIZE - 1],		
					  (INT8U           ) APP_TASK_LASER_PRIO  );
												
	#if OS_TASK_NAME_EN > 0
	OSTaskNameSet(APP_TASK_LASER_PRIO, "Task WAVE Blink", &os_err);
	#endif
	
}

static void uctsk_Laser()
{
	u8 len;
	u8 t=0;
	char  number[11];
	extern double laserStaus;

// 	len=USART3_RX_STA&0x3f;
// 	USART_SendData(USART3,'O');		//打开激光传感器
// 	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
// 	
// 	if(USART3_RX_STA&0x8000)
// 	{
// 		for(t=0;t<len;t++)
// 			USART3_RX_BUF[t]=0;

// 		USART3_RX_STA=0;
// 	}
// 	OSTimeDlyHMSM(0,0,0,300);
// 	while(1)
// 	{
// 		USART_SendData(USART3,'D');		//开始测量
// 		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
// 		
// 		OSTimeDlyHMSM(0,0,2,0);
// 		if(USART3_RX_STA&0x8000)
// 		{
// 			USART3_RX_BUF[7]='\0';		//测量结束
// 			strcpy(number,strstr(USART3_RX_BUF , ":"));//截取字符串
// 			laserStaus = atof(&number[1]);
// 			printf("%lf\r\n",laserStaus);

// 			for(t=0;t<11;t++)
// 				number[t]=0;
// 			for(t=0;t<len;t++)
// 				USART3_RX_BUF[t]=0;
// 			
// 			USART3_RX_STA=0;
// 		}
// 	}

	len=USART1_RX_STA & 0x3f;
	USART_SendData(USART1,'O');		//打开激光传感器
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
	
	if(USART1_RX_STA&0x8000)
	{
		for(t=0;t<len;t++)
			USART1_RX_BUF[t]=0;

		USART1_RX_STA=0;
	}
	OSTimeDlyHMSM(0,0,0,300);
	while(1)
	{
		USART_SendData(USART1,'D');		//开始测量
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
		
		OSTimeDlyHMSM(0,0,2,0);
		if(USART1_RX_STA&0x8000)
		{
			USART1_RX_BUF[7]='\0';		//测量结束
			strcpy(number,strstr(USART1_RX_BUF , ":"));//截取字符串
			laserStaus = atof(&number[1]);
			
#if DEBUG	&& DEBUG_LASER		
			printf("%lf\r\n",laserStaus);
#endif			

			for(t=0;t<11;t++)
				number[t]=0;
			for(t=0;t<len;t++)
				USART1_RX_BUF[t]=0;
			
			USART1_RX_STA=0;
		}
	}
}

  
	
	  






