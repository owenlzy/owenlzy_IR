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
#include "motor.h"

#define CCODE_SIZE 180

/* Private variables ---------------------------------------------------------*/
static  OS_STK      App_TaskAvoidStk[APP_TASK_Avoid_STK_SIZE];
extern  OS_EVENT    *Str_Q_Avoid ;
extern  OS_EVENT    *Str_Q_XFTTS;
extern  u8          infraredStatus ;
extern  int         isOpenAvoidStyle ;

/* Private function prototypes -----------------------------------------------*/
static void uctsk_Avoid	(void) ;
//void transCCode	(char *cStr);

void  App_AvoidCreate (void)
{
    CPU_INT08U  os_err;
	os_err = os_err; /* prevent warning... */

	os_err = OSTaskCreate((void (*)(void *)) uctsk_Avoid,				
                          (void          * ) 0,							
                          (OS_STK        * )&App_TaskAvoidStk[APP_TASK_Avoid_STK_SIZE - 1],		
                          (INT8U           ) APP_TASK_Avoid_PRIO  );							

	#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(APP_TASK_Avoid_PRIO, "Task ULTRASONIC Blink", &os_err);
	#endif

}
static void uctsk_Avoid (void) 
{  
 	char strs[130] = {0};

    while(1)
    {
        if(!isOpenAvoidStyle)
        {
            //printf("\r\n-----------Task Avoid Distns:[[%d]]-----------\r\n",infraredStatus);
            if(infraredStatus < 23)
            {   //让小车左转45度
                
                car_left_senior(60*2);
                
//              memset(strs,0,130);
//              strcat(strs,"FFF_有障碍");             
//              OSQPost(Str_Q_XFTTS,strs);
                
            }else{
                car_back(1);
                OSTimeDlyHMSM(0, 0, 0, 100);
            }
        }else
            continue;
    }   
}

/*********************************************************************************************************
    END FILE
*********************************************************************************************************/
