/* Includes ------------------------------------------------------------------*/
#include <includes.h>  
#include "gpio.h"

/* Private variables ---------------------------------------------------------*/
static  OS_STK         App_TaskWIFIModeSwitchStk[APP_TASK_WIFIMODESWITCH_STK_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void uctsk_WIFIModeSwitch            (void);

void  App_WIFIModeSwitchTaskCreate (void)
{
  CPU_INT08U  os_err;
	os_err = os_err; /* prevent warning... */
	os_err = OSTaskCreate((void (*)(void *)) uctsk_WIFIModeSwitch,				
                          (void          * ) 0,							
                          (OS_STK        * )&App_TaskWIFIModeSwitchStk[APP_TASK_WIFIMODESWITCH_STK_SIZE - 1],		
                          (INT8U           ) APP_TASK_WIFI_MODE_SWTITCH_PRIO  );							

	#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(APP_TASK_WIFI_MODE_SWTITCH_PRIO, "Task WIFI Mode Switch Blink", &os_err);
	#endif
}

static void uctsk_WIFIModeSwitch (void) 
{  
	GPIO_InitTypeDef GPIO_InitStructure ;
	
	u8 WIFIStatus ;   //WIFIStatus=1 ：工作模式  WIFIStatus=0 ：烧写模式
	u8 LuaStatus  ;		//LuaStatus=1 : 工作模式	 LuaStatus=0 ：烧写模式
	
	u8 bin_count = 0;
	
	
	gpio_WIFIModeSwitch_init();
	
	while(1){
		WIFIStatus = GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_10);

#if DEBUG	&& DEBUG_WIFISWITCH	
		printf("wifi Status = %d\r\n" , WIFIStatus);
#endif
		
		/**接入WIFI bin 文件烧录线**/  
		if(WIFIStatus == WIFI_MODE_BIN){
			bin_count ++ ;
			if(bin_count == 1){
					//串口4 悬空
#if DEBUG	&& DEBUG_WIFISWITCH				
					printf("usart 4 float\r\n");
#endif				
// 					GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				 
// 					GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 
// 					GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
// 					GPIO_Init(GPIOC, &GPIO_InitStructure);	
				  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, DISABLE);
					USART_Cmd(UART4, DISABLE); 
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	
					GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				 
					GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 
					GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
					GPIO_Init(GPIOC, &GPIO_InitStructure);	
					
					GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				 
					GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 	 
					GPIO_Init(GPIOC, &GPIO_InitStructure);		
			}
			if(bin_count > 1000)	bin_count = 1 ;
		}
		
		OSTimeDlyHMSM(0, 0, 1, 0);
	}
}



/*********************************************************************************************************
      END FILE
*********************************************************************************************************/