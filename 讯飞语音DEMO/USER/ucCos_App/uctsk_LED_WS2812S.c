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
#include "led_ws2812s.h"

/* Private variables ---------------------------------------------------------*/
static  OS_STK         App_TaskLEDStk[APP_TASK_LED_WS2812S_STK_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void uctsk_LED_WS2812S (void) ;



void  App_LEDWS2812STaskCreate (void)
{
    CPU_INT08U  os_err;

	os_err = os_err; /* prevent warning... */

	os_err = OSTaskCreate((void (*)(void *)) uctsk_LED_WS2812S,				
                          (void          * ) 0,							
                          (OS_STK        * )&App_TaskLEDStk[APP_TASK_LED_WS2812S_STK_SIZE - 1],		
                          (INT8U           ) APP_TASK_LED_WS2812S_PRIO  );							

	#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(APP_TASK_LED_WS2812S_PRIO, "Task ULTRASONIC Blink", &os_err);
	#endif

}


static void uctsk_LED_WS2812S (void) 
{  
		u8 num,color ;
		gpio_led_ws2812s_init();
		while(1){
			for(num=0 ; num<=4 ;num++)
				for(color=0;color<3;color++){
					flash_ws2812s(num,color) ;
					OSTimeDlyHMSM(0, 0, 1, 0);
				}
		}
}




/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
