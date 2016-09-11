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
#include "gpio.h"
#include "led.h"

/* Private variables ---------------------------------------------------------*/
static  OS_STK         App_TaskLEDStk[APP_TASK_LED_STK_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void uctsk_LED (void) ;



void  App_LEDTaskCreate (void)
{
    CPU_INT08U  os_err;

	os_err = os_err; /* prevent warning... */

	os_err = OSTaskCreate((void (*)(void *)) uctsk_LED,				
                          (void          * ) 0,							
                          (OS_STK        * )&App_TaskLEDStk[APP_TASK_LED_STK_SIZE - 1],		
                          (INT8U           ) APP_TASK_LED_PRIO  );							

	#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(APP_TASK_LED_PRIO, "Task ULTRASONIC Blink", &os_err);
	#endif

}


static void uctsk_LED (void) 
{  
	u8 i=1 ;
	gpio_led_init();
	
	write_6xled(0x1f);
	write_6xled(0x10);
    
	OSTimeDlyHMSM(0, 0, 1, 0);
	while(1){
		flash_6xled(i++);
		OSTimeDlyHMSM(0, 0, 1, 0);
		if(i>6)	i=1;
	}
}




/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
