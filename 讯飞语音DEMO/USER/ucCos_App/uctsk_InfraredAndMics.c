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
      
#include "adc.h"
#include "beep.h"

/* Private variables ---------------------------------------------------------*/
static  OS_STK         App_InfraredAndMicsStk[APP_TASK_INFRARED_MICS_STK_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void uctsk_InfraredAndMics (void) ;
static void infrared_adc(u16 *infrared1 ,u16 *infrared2);
static u16 mic_adc1();
static u16 mic_adc2();
static u16 mic_adc3();
static u8 getDistance(u16 infrared);



void  App_InfraredAndMicsCreate (void)
{
    CPU_INT08U  os_err;

	os_err = os_err; /* prevent warning... */

	os_err = OSTaskCreate((void (*)(void *)) uctsk_InfraredAndMics,				
                          (void          * ) 0,							
                          (OS_STK        * )&App_InfraredAndMicsStk[APP_TASK_INFRARED_MICS_STK_SIZE - 1],		
                          (INT8U           ) APP_TASK_INFRAREDANDMICS_PRIO  );							

	#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(APP_TASK_INFRAREDANDMICS_PRIO, "Task InfraredAndMics Blink", &os_err);
	#endif

}

static void uctsk_InfraredAndMics (void) 
{
	extern u8 infraredStatus ;
    extern u16 micStatus[3] ;
    extern char usartDebugBuffer[128];
	
	u16 infrared1 , infrared2 ;
	//u16 mic1 , mic2 , mic3 ;
	double val ; 
	u8 distance ;
    int count = 1 ;
	while(1)
	{
		infrared_adc(&infrared1 , &infrared2);		//夏普红外测距ADC采集
		//mic_adc(&mic1 , &mic2 , &mic3);						//MIC ADC采集
		
		//mic ADC值
		micStatus[0] = mic_adc1() ;
		micStatus[1] = mic_adc2() ;
		micStatus[2] = mic_adc3() ;
		
		//红外测距测试
		infraredStatus = getDistance(infrared2) ;
		if (count%10 == 0)
			
#if DEBUG	&& DEBUG_INFRARED_MICS	
		//printf("> adc=%d  voltage=%lf  distance=%d\r\n" , infrared1 , infrared1*(3.3/4096) , infraredStatus);
        printf("> adc=%d  voltage=%lf  distance=%d\r\n" , infrared2 , infrared2*(3.3/4096) , infraredStatus);
#endif
//  	printf("MK2 = %d , MK3 = %d ,MK4 =%d\r\n" , mic1,mic2,mic3);
		
		OSTimeDlyHMSM(0, 0, 0, 100);
        count += 1 ;
		
 	}
}
/*测量夏普红外线ADC值*/
static void infrared_adc(u16 *infrared1 ,u16 *infrared2){
		//B版
		*infrared1=Get_Adc_Average(ADC_Channel_1,10);
		*infrared2=Get_Adc_Average(ADC_Channel_15,10);
// 		//A版
// 		*infrared1=Get_Adc_Average(ADC_Channel_4,10);
// 		*infrared2=Get_Adc_Average(ADC_Channel_9,10);
}
//一个函数mic
// static void mic_adc(u16 *mic1 , u16 *mic2 , u16 *mic3){
// 	*mic1=Get_Adc_Average(ADC_Channel_12,10);
// 	*mic2=Get_Adc_Average(ADC_Channel_10,10);
// 	*mic3=Get_Adc_Average(ADC_Channel_11,10);
// }

static u16 mic_adc1(){
	u16 mic1;
	mic1=Get_Adc_Average(ADC_Channel_12,10);
	return mic1;
}

static u16 mic_adc2(){
	u16 mic2;
	mic2=Get_Adc_Average(ADC_Channel_10,10);
	return mic2;
}

static u16 mic_adc3(){
	u16 mic3;
	mic3=Get_Adc_Average(ADC_Channel_11,10);
	return mic3;
}

static u8 getDistance(u16 infrared){
	u8 distance = 80  ;
	
	if(infrared >= 2830) distance = 10 ;
	else if(infrared >= 2260 && infrared < 2830) distance = (2830-infrared)/190 + 10;
	else if(infrared >= 2010 && infrared < 2260) distance = (2260-infrared)/125 + 13;
	else if(infrared >= 1710 && infrared < 2010) distance = (2010-infrared)/100 + 15;
	else if(infrared >= 1560 && infrared < 1710) distance = (1710-infrared)/75  + 18;
	else if(infrared >= 1300 && infrared < 1560) distance = (1560-infrared)/51  + 20;
	else if(infrared >= 1120 && infrared < 1300) distance = (1300-infrared)/36  + 25;
	else if(infrared >= 890  && infrared < 1120) distance = (1120-infrared)/23  + 30;
	else if(infrared >= 760  && infrared < 890 ) distance = (890 -infrared)/13  + 40;
	else if(infrared >= 670  && infrared < 760 ) distance = (760 -infrared)/9   + 50;
	else if(infrared >= 620  && infrared < 670 ) distance = (670 -infrared)/5   + 60;
	else if(infrared >= 570  && infrared < 620 ) distance = (620 -infrared)/5   + 70;
	
	return distance ;
}




/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
