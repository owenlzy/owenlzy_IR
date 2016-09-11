/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               uctsk_WIFI_ctrl.c
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
#include "LCD12864G_086P.h"
#include "digitron.h"
#include "ADC.h"

extern volatile double voltageStatus;
extern volatile double sensorStatus[8] ;
extern volatile u16 lightStatus ,humanStatus;
extern volatile u8 infraredStatus ;
extern volatile u16 micStatus[3] ;     //mic 值
extern volatile double laserStaus;   //激光传感器值
extern int wifiConnectStatus;  //WIFI连接的状态0表示未连接，1表示连接成功
extern char usartDebugBuffer[128];

#define MAXOPTION              				 4
#define MAXFILE                        5
#define MAXSENSORSTATUS                2
#define HIGHPOWER                      12.802077
#define LOWPOWER                       9.597501

char *option_list[5] = {
										"1.Run Apps",
										"2.Boot Digitron",
										"3.Sensor Status",
										"4.About Robot",
									 };


extern char *file_list[5];    // can't figure out why can't define it here
/* Private variables ---------------------------------------------------------*/
static  OS_STK         App_TaskLCD12864Stk[APP_TASK_LCD12864_STK_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void uctsk_LCD12864 (void) ;

extern OS_EVENT *Str_Q_LCD12864;
// extern OS_EVENT *Str_Q_Digitron;
STRU_Digitron 	LCD2Digi_Msg;


void  App_LCD12864TaskCreate (void)
{
    CPU_INT08U  os_err;

	os_err = os_err; /* prevent warning... */

	os_err = OSTaskCreate((void (*)(void *))  uctsk_LCD12864,				
                          (void          * ) 0,							
                          (OS_STK        * )&App_TaskLCD12864Stk[APP_TASK_LCD12864_STK_SIZE - 1],		
                          (INT8U           ) APP_TASK_LCD12864_PRIO  );							

	#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(APP_TASK_LCD12864_PRIO, "Task LCD12864", &os_err);
	#endif

}

// void show_list(u8 cur, char** item_list)
// {
// 	if(cur == 0)
// 	{
// 		display_string_8x16(1, 0, item_list[0], 1); 
// 		display_string_8x16(3, 0, item_list[1], 0); 
// 		display_string_8x16(5, 0, item_list[2], 0); 
// // 		display_string_8x16(7, 0, item_list[3], 0); 
// 	}
// 	else if(cur == 1)
// 	{
// 		display_string_8x16(1, 0, item_list[0], 0); 
// 		display_string_8x16(3, 0, item_list[1], 1); 
// 		display_string_8x16(5, 0, item_list[2], 0); 
// // 		display_string_8x16(7, 0, item_list[3], 0); 
// 	}
// 	else if(cur == 2)
// 	{
// 		display_string_8x16(1, 0, item_list[0], 0); 
// 		display_string_8x16(3, 0, item_list[1], 0); 
// 		display_string_8x16(5, 0, item_list[2], 1); 
// // 		display_string_8x16(7, 0, item_list[3], 0); 
// 	}
// 	else if(cur == 3)
// 	{
// 		display_string_8x16(1, 0, item_list[1], 0); 
// 		display_string_8x16(3, 0, item_list[2], 0); 
// 		display_string_8x16(5, 0, item_list[3], 1); 
// // 		display_string_8x16(7, 0, item_list[3], 1); 
// 	}
// // 	else if(cur == 4)
// // 	{
// // 		display_string_8x16(1, 0, item_list[2], 0); 
// // 		display_string_8x16(3, 0, item_list[3], 0); 
// // 		display_string_8x16(5, 0, item_list[4], 1); 
// // // 		display_string_8x16(7, 0, item_list[4], 1); 
// // 	}
// 	//display_string_8x16(7, 0, "Battery", 0);
// 	
// }



// void show_battery(void)
// {
// 	char* bat = "Battery:";
// 	char buf[10];
// //	double voltage = voltageStatus * 10000;
// // 	double power = voltageStatus;
// 	double power;
// 	power = voltageStatus;
// // 	printf("voltageStatus = %2.0lf\n", voltageStatus);
//    power = (power - LOWPOWER) / (HIGHPOWER - LOWPOWER) * 100;
// 	
// 	if(power > 100)
// 		power = 100.0;
// 	else if (power < 0)
// 		power = 1.0;
// 	
// 	sprintf(buf, "%2.0lf", power);
// 	display_string_8x16(7, 0, bat, 0);
// 	display_string_8x16(7, strlen(bat)*8, buf, 0);
// 	display_string_8x16(7, strlen(bat)*8 + strlen(buf)*8, "%", 0);
// 	
//     if(usartDebugBuffer[0] != 0)
//     {
//         sprintf(buf, "%s", usartDebugBuffer);
//         //printf("********************************************");
// #if DEBUG	&& DEBUG_LCD12864		
//         printf("%s", buf);
// #endif 			
//         display_string_8x16(7, 12 * 8, buf, 0);
//         memset(usartDebugBuffer, 0 , sizeof(usartDebugBuffer));
//     }
// // 	if(wifiConnectStatus)
// // 		display_string_8x16(7, 14 * 8, "C", 0);
// }

// void keep_screen(u8 depth, u8 cur, u8 file_cur)
// {
// 			if(depth == 0)
// 			{
// 				show_list(cur, option_list);
// 				show_battery();
// 			}
// 			else if(depth == 1)
// 			{
// 				if(cur == 0)
// 				{
// // 					show_list(file_cur, file_list);
// 					show_battery();
// 				}

// 			}
// 			else if(depth == 2)
// 			{
// 				display_string_8x16(1, 0, "file", 0); 
// 			}			
// }

// void show_window(u8 cur, u8 file_cur, u8 sensor_cur)
// {
// 	char buf[20];

// // extern u16 micStatus[3] ;     //mic 值
// // extern double laserStaus;   //激光传感器值
// 	
// 	if(cur == 0)
// 	{
// // 		display_string_8x16(3, 5 * 8, "window1", 0); 
// 		show_list(file_cur, file_list);
// 		show_battery();
// 	}
// 	else if(cur == 1)
// 		{
// // 		display_string_8x16(3, 5 * 8, "window3", 0); 
// 		LCD2Digi_Msg.first = 2;
// 		LCD2Digi_Msg.second = 3;
// 		LCD2Digi_Msg.third = 4;
// // 		OSQPost(Str_Q_Digitron, &LCD2Digi_Msg); 
// 		display_string_8x16(3, 5 * 8, "Done", 0); 
// 	}	
// 	else if(cur == 2)
// 	{
// // 		display_string_8x16(3, 5 * 8, "window2", 0); 
// 		if(sensor_cur == 0)
// 		{
// 			sprintf(buf, "%d", lightStatus);
// 			display_string_8x16(1, 0, "LightSensor:", 1);
// 			display_string_8x16(1, 12 * 8, buf, 1); 
// 		
// 			sprintf(buf, "%d", humanStatus);
// 			display_string_8x16(3, 0, "HumanSensor:", 0);
// 			display_string_8x16(3, 12 * 8, buf, 0); 
// 		
// 			sprintf(buf, "%d", infraredStatus );
// 			display_string_8x16(5, 0, "Infrared:", 1);
// 			display_string_8x16(5, 9 * 8, buf, 1); 
// 		
// 			sprintf(buf, "%d", micStatus[0] );
// 			display_string_8x16(7, 0, "Mic1:", 0);
// 			display_string_8x16(7, 5 * 8, buf, 0); 
// 		}
// 		else if(sensor_cur == 1)
// 		{
// 			sprintf(buf, "%d", micStatus[1] );
// 			display_string_8x16(1, 0, "Mic2:", 1);
// 			display_string_8x16(1, 5 * 8, buf, 1); 
// 		
// 			sprintf(buf, "%d", micStatus[2] );
// 			display_string_8x16(3, 0, "Mic3:", 0);
// 			display_string_8x16(3, 5 * 8, buf, 0); 
// 			
// 			sprintf(buf, "%2.0lf", laserStaus );
// 			display_string_8x16(5, 0, "LaserStaus:", 1);
// 			display_string_8x16(5, 11 * 8, buf, 1); 
// 		}

// 	}
// 	else if(cur == 3)
// 	{
// 		display_string_8x16(3, 3 * 8, "UWIS ROBOT", 0); 
// 	}
// // 	else if(cur == 4)
// // 		display_string_8x16(3, 5 * 8, "window5", 0); 
// }


// static void uctsk_LCD12864 (void) 
// { 
// 	
// 	//消息队列
// 	INT8U         err;
// 	OS_CPU_SR  cpu_sr = 0;
// 	u8 cur = 0, depth = 0, file_cur = 0, sensor_cur = 0;
// 	extern OS_EVENT  *Str_Q_8x8  ;
// 	MessageBuffer *msg_rcv_p;
// 	char msg_rcv[MESSAGE_BUFFER_SIZE] ;
// // 	
//  	int size;
//  	char str[16];       //这个注释谁写的？？啥意思？？               //LCD任务的长度只能是16位不能只能定义一个指针！！！！！！！！！
// 	
// 	LCD12864_GPIO_Config();    			//LCD 管脚配置
// 	 
// 	LCD12864G_086P_Init();     			//LCD 初始化
// 	
// 	LCD12864G_086P_Clear_Screen();    	//LCD 清屏
// 	
// // 	show_list(cur, option_list);
// // 	show_battery();
// 	while(1)
// 	{
// 		msg_rcv_p=OSQPend(Str_Q_LCD12864,1000,&err);
// 		strncpy(msg_rcv,msg_rcv_p->buffer,MESSAGE_BUFFER_SIZE);
// 		if(msg_rcv_p)
// 		{
// 			OS_ENTER_CRITICAL();
// 			Message_Queue_Back(msg_rcv_p);
// 			OS_EXIT_CRITICAL();
// 		}
// 		
// 		strncpy(msg_rcv,"FFF_LCD12864_hello world!!!\r\n",MESSAGE_BUFFER_SIZE);		
// 		size = strlen(msg_rcv) - 15;              //得到显示字符串长度
// 		
// 		strncpy(str,msg_rcv+13,size);           //得到字符串
// 		str[size] = '\0';                       //手动添加一个'\0'
// 		
// //		printf("LCD command=%s\r\n" , str) ;
// 		
// 		LCD12864G_086P_Clear_Screen();			 //LCD 清屏	
// 		display_string_8x16(1, 0, str, 1);          //LCD 显示
// //		display_string_8x8(1, 0, str);
// 		
// 		
// // 		LCD12864G_086P_Clear_Screen();			 //LCD 清屏
// 		
// // 					show_list(file_cur, file_list);
// // 		show_list(cur, option_list);
// 		
// 		
// // 					show_battery();
// // #if 1 
// // 		if(strstr(msg_rcv, "key_2"))   // up
// // 		{
// // 			if(depth == 0)
// // 			{
// // 				cur = (cur + MAXOPTION - 1) % MAXOPTION;
// // 				show_list(cur, option_list);
// // 				show_battery();
// // 			}
// // 			else if(depth == 1)
// // 			{
// // 				if(cur == 0)
// // 				{
// // 					file_cur = (file_cur + MAXFILE - 1) % MAXFILE;
// // 					show_list(file_cur, file_list);
// // 					show_battery();
// // 				}
// // 				else if(cur == 2)
// // 				{
// // 					sensor_cur = (sensor_cur + MAXSENSORSTATUS - 1) % MAXSENSORSTATUS;
// // 					show_window(cur, file_cur, sensor_cur);
// // 				}
// // 				else
// // 				{
// // 					goto KEEP_SAME;
// // 				}
// // 			}
// // 			else if(depth == 2)
// // 			{
// // 				goto KEEP_SAME;
// // 			}
// // 		}
// // 		else if (strstr(msg_rcv, "key_4")) //left
// // 		{
// // 				goto KEEP_SAME;
// // 		}
// // 		else if (strstr(msg_rcv, "key_6")) //right
// // 		{
// // 				goto KEEP_SAME;
// // 		}
// // 		else if (strstr(msg_rcv, "key_8")) //down
// // 		{
// // 			if(depth == 0)
// // 			{
// // 				cur = (cur + 1) % MAXOPTION;
// // 				show_list(cur, option_list);
// // 				show_battery();
// // 			}
// // 			else if(depth == 1)
// // 			{
// // 				if(cur == 0)
// // 				{
// // 					file_cur = (file_cur + 1) % MAXFILE;
// // 					show_list(file_cur, file_list);
// // 					show_battery();
// // 				}
// // 				else if(cur == 2)
// // 				{
// // 					sensor_cur = (sensor_cur + 1) % MAXSENSORSTATUS;
// // 					show_window(cur, file_cur, sensor_cur);
// // 				}
// // 				else
// // 				{
// // 					goto KEEP_SAME;
// // 				}				
// // 			}
// // 			else if(depth == 2)
// // 			{
// // 					goto KEEP_SAME;
// // 			}			
// // 		}
// // 		
// // 		else if (strstr(msg_rcv, "key_5"))  //OK
// // 		{
// // 			if(depth == 0)
// // 			{
// // 				show_window(cur, file_cur, sensor_cur);
// // 				depth += 1;
// // 			}
// // 			else if(depth == 1 && cur == 0)
// // 			{
// // 				depth += 1;
// // 				display_string_8x16(1, 0, "file", 0); 
// // 			}
// // 		}
// // 		else if (strstr(msg_rcv, "key_0"))  //back 
// // 		{
// // 			if(depth == 0)
// // 			{				
// // 				goto KEEP_SAME;
// // 			}
// // 			else if(depth == 1)
// // 			{
// // 				depth -= 1;
// // 				show_list(cur, option_list);
// // 				show_battery();
// // 			}
// // 			else if(depth == 2)
// // 			{
// // 				depth -= 1;
// // 				show_list(file_cur, file_list);
// // 				show_battery();
// // 			}
// // 		}
// // 		else if (strstr(msg_rcv, "key_*"))  //reset
// // 		{
// // 			cur = 0;
// // 			depth = 0;
// // 			show_list(cur, option_list);
// // 			show_battery();
// // 		}
// // 		else                                  // anything else, keep the same
// // 		{
// // 			goto KEEP_SAME;
// // 		}
// // 		
// // 		
// // KEEP_SAME:			
// // 		{
// // 			if(depth == 0)
// // 			{
// // 				show_list(cur, option_list);
// // 				show_battery();
// // 			}
// // 			else if(depth == 1)
// // 			{
// // 				if(cur == 0)
// // 				{
// // 					show_list(file_cur, file_list);
// // 					show_battery();
// // 				}
// // 				else
// // 				{
// // 					show_window(cur, file_cur, sensor_cur);

// // 				}
// // 			}
// // 			else if(depth == 2)
// // 			{
// // 				display_string_8x16(1, 0, "file", 0); 
// // 			}			
// // 		}
// // 		
// // 		
// // 		msg_rcv_p = NULL;
// // #endif		
// 		
// // 		OSTimeDlyHMSM(0, 0, 1, 0);	

// 	}


// 	OSTimeDlyHMSM(0, 0, 0, 500);	
// }

static void uctsk_LCD12864 (void) 
{ 
	
	//消息队列
	INT8U         err;
	OS_CPU_SR  cpu_sr = 0;
	extern OS_EVENT  *Str_Q_8x8  ;
	MessageBuffer *msg_rcv_p;
	char msg_rcv[MESSAGE_BUFFER_SIZE] ;
	
	int size;
	char str[16];                         //LCD任务的长度只能是16位不能只能定义一个指针！！！！！！！！！
	
	LCD12864_GPIO_Config();    			//LCD 管脚配置
	 
	LCD12864G_086P_Init();     			//LCD 初始化
	
	LCD12864G_086P_Clear_Screen();    	//LCD 清屏
	
	while(1)
	{
		//msg_rcv_p=OSQPend(Str_Q_LCD12864,0,&err);
		//strncpy(msg_rcv,msg_rcv_p->buffer,MESSAGE_BUFFER_SIZE);
		strncpy(msg_rcv,"FFF_LCD12864_hello UWIS!!!\r\n",MESSAGE_BUFFER_SIZE);

		
		
		
		size = strlen(msg_rcv) - 15;              //得到显示字符串长度
		
		strncpy(str,msg_rcv+13,size);           //得到字符串
		str[size] = '\0';                       //手动添加一个'\0'
		
		printf("LCD message=%s\r\n" , str) ;
		
		LCD12864G_086P_Clear_Screen();			 //LCD 清屏	
		display_string_8x16(1, 0, str, 0);       //LCD 显示


		
		OSTimeDlyHMSM(0, 0, 1, 0);	

	}


	OSTimeDlyHMSM(0, 0, 0, 500);	
}
