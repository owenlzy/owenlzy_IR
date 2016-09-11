// /* Includes ------------------------------------------------------------------*/
// #include <includes.h>          
// #include "adc.h"
// #include "gpio.h"

// static u16          adcx,danger,i; 
// /* Private variables ---------------------------------------------------------*/
// static  OS_STK         App_TasktrackerStk[APP_TASK_TRACKER_STK_SIZE];

// /* Private function prototypes -----------------------------------------------*/
// static void uctsk_tracker (void) ;




// void  App_trackerCreate (void)
// {
//     CPU_INT08U  os_err;

// 	os_err = os_err; /* prevent warning... */

// 	os_err = OSTaskCreate((void (*)(void *)) uctsk_tracker,				
//                           (void          * ) 0,							
//                           (OS_STK        * )&App_TasktrackerStk[APP_TASK_TRACKER_STK_SIZE - 1],		
//                           (INT8U           ) APP_TASK_TRACKER_PRIO  );							

// 	#if OS_TASK_NAME_EN > 0
//     	OSTaskNameSet(APP_TASK_TRACKER_PRIO, "Task ULTRASONIC Blink", &os_err);
// 	#endif

// }
// // void yanshi()
// // {
// // 	for(i=0;i<60000;i++)
// // 	__nop();
// // }
// static void uctsk_tracker (void) 
// {   

// 	extern OS_EVENT  *Str_Q_dis;

// 	
// 	
// 	

// 	printf("tracker sensor start to work\r\n");
// 	Adc_Init();

// 	while(1)
// 	{

//   printf("start adc\r\n");
//   adcx=Get_Adc_Average(ADC_Channel_4,10);
// 		if(adcx>=1000)
// 		{danger=1;}
// 		else
// 		{danger=0;}
// 		 printf("posted=%d\r\n",adcx);
// 		printf("react=%d\r\n",danger);
// 		OSQPost(Str_Q_dis,&danger);
// 		OSTimeDlyHMSM(0, 0, 0, 500);
// 	}

// }

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
#include "adc.h"
#include "gpio.h"
#include "led.h"
#include "pwm.h"

#include "digitron.h"
#include "flash.h"

static u16          adcx,danger; 
extern OS_EVENT      *Str_Q_dis;
INT8U         sensorerr; 


static unsigned char pp;//修改1

/* Private variables ---------------------------------------------------------*/
static  OS_STK         App_TaskLEDStk[APP_TASK_TRACKER_PRIO];

/* Private function prototypes -----------------------------------------------*/
static void uctsk_Tracker (void) ;

extern OS_EVENT *Str_Q_Digitron;

void  App_TrackerTaskCreate (void)
{
    CPU_INT08U  os_err;

	os_err = os_err; /* prevent warning... */

	os_err = OSTaskCreate((void (*)(void *)) uctsk_Tracker,				
                          (void          * ) 0,							
                          (OS_STK        * )&App_TaskLEDStk[APP_TASK_TRACKER_PRIO - 1],		
                          (INT8U           ) APP_TASK_TRACKER_PRIO  );							

	#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(APP_TASK_TRACKER_PRIO, "Task ULTRASONIC Blink", &os_err);
	#endif

}
 
static void uctsk_Tracker (void) 
{
// 	u8 		err ;
// 	
// 	Adc_Init();
// 	printf("> uctsk_LED Task is Running  <\r\n");
// 	while(1)
// 	{
// 		adcx=Get_Adc_Average(ADC_Channel_4,10);
// 		if(adcx>=1000)
// 			danger=1;
// 		else
// 			danger=0;
// 		
// 		printf("> posted = %d   react= %d \r\n ",adcx,danger);
// 		err = OSQPost(Str_Q_dis,&adcx);
// 		
// 		OSTimeDlyHMSM(0, 0, 1, 1);
// 	}
	
	//测试数码管消息队列
//	u32 addr;
	u8 err;
extern STRU_Digitron pMsg;

	int i = 0;
	
	while(1)
	{
		pMsg.first = 0;
		pMsg.second = 2;
		pMsg.third = i%16;
		
//		addr = pMsg;
		printf("hello\n");
 		OSQPost(Str_Q_Digitron, &pMsg); 
		i++;
		OSTimeDlyHMSM(0, 1, 0, 0);
	}
 	

// 测试flash
// uint8_t Tx_Buffer[5] = {1,2,3,4,5};
// uint8_t Rx_Buffer[BufferSize];
// uint32_t DeviceID = 0;
//  uint32_t FlashID = 0;
//     static uint8_t cn_i = 0,i;

//   /* 2M??flash W25X16??? */
// 	SPI_FLASH_Init();

// 	/* Get SPI Flash Device ID */
// 	DeviceID = SPI_FLASH_ReadDeviceID();
// 	
// SPI_Delay(200);
// 	printf("%d\r\n",DeviceID);
// 	/* Get SPI Flash ID */
// 	FlashID = SPI_FLASH_ReadID();
// 	
// 	printf("\r\n FlashID is 0x%X\r\n Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);
// 	
// 	/* Check the SPI Flash ID */
// 	if (FlashID == sFLASH_ID)  /* #define  sFLASH_ID  0XEF4017 */
// 	{	
// 		SPI_FLASH_SectorErase(FLASH_SectorToErase);	
// // 		printf("\r\n ??????FLASH?W25X16 !\r\n");
// 		
// 	 
// 		SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);
// //printf("\r\n ??????:%s \r\n", Tx_Buffer);

// 			printf("write succeed\r\n");
// 				printf("\r\n write data is:%d \r\n", Tx_Buffer[4]);
// 		
//  	while(1)
// 	{
// 		SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
// // 					printf("read succeed\r\n");
// 			OSTimeDlyHMSM(0, 0, 0, 500);
// 		SPI_Delay(200);
// 	printf("\r\n read data is:%d \r\n", Rx_Buffer[3]);
//  	}
// 		/* ????????????????? */
// // 		TransferStatus = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);
// // 		
// // 		if( PASSED == TransferStatus )
// // 		{    
// // 			printf("\r\n 2M??flash(W25X16)????!\n\r");
// // 		}
// // 		else
// // 		{        
// // 			printf("\r\n 2M??flash(W25X32)????!\n\r");
// // 	}
// // 	} if (FlashID == sFLASH_ID)
// // 	{printf("1111111");}
// // 	else
// // 	{    
// // 		printf("\r\n ???? W25X16 ID!\n\r");
// // 	}
//         for(i=0;i<5;i++)Tx_Buffer[i]=cn_i;
//     cn_i++;
//     if(cn_i>99)cn_i =0;
//     
// //	SPI_Flash_PowerDown();  
// 	}
}




/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
