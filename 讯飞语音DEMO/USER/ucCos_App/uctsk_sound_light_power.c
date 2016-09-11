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
#include "usart.h"         
#include "adc.h"


/* Private variables ---------------------------------------------------------*/
static  OS_STK         App_Tasksound_light_powerStk[APP_TASK_sound_light_power_STK_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void uctsk_sound_light_power            (void);

extern int sensorStatus[8] ;

void   App_power_sound_lightCreate(void)
{
    CPU_INT08U  os_err;

	os_err = os_err; /* prevent warning... */

	os_err = OSTaskCreate((void (*)(void *)) uctsk_sound_light_power ,				
                          (void          * ) 0,							
                          (OS_STK        * )&App_Tasksound_light_powerStk[APP_TASK_sound_light_power_STK_SIZE - 1],		
                          (INT8U           ) APP_TASK_sound_light_power_PRIO  );							

	#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(APP_TASK_sound_light_power_PRIO, "Task ULTRASONIC Blink", &os_err);
	#endif

}

static void uctsk_sound_light_power  (void) 
{   
	u32 light=0,sound=0,power=0;
	
	Adc_Init();
	while(1)
	{
		 power = Get_Adc_Average(ADC_Channel_4,10);
		 sound = Get_Adc_Average(ADC_Channel_5,10);
		 light = Get_Adc_Average(ADC_Channel_6,10);
		 sensorStatus[0] =power;
		 sensorStatus[1] =sound;
		 sensorStatus[2] =light;
 		 //printf("light=%d,sound=%d,power=%d\r\n",light,sound,power);
		 OSTimeDlyHMSM(0, 0, 1, 0);
	}
}





/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
