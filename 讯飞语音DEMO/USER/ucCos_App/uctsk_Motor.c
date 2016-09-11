/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               uctsk_Blink.c
** Descriptions:            The uctsk_Blink application function
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
#include "pwm.h"
#include "motor.h"
#include "usart.h"           
#include "gpio.h"

/* Private variables ---------------------------------------------------------*/
static  OS_STK         App_TaskMotorStk[APP_TASK_MOTOR_STK_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void uctsk_Motor            (void);
void motor_init			   (void);
static char * left(char *dst,char *src, int n);

void  App_MotorTaskCreate (void)
{
    CPU_INT08U  os_err;

	os_err = os_err; /* prevent warning... */

	os_err = OSTaskCreate((void (*)(void *)) uctsk_Motor,				
                          (void          * ) 0,							
                          (OS_STK        * )&App_TaskMotorStk[APP_TASK_MOTOR_STK_SIZE - 1],		
                          (INT8U           ) APP_TASK_MOTOR_PRIO  );							

	#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(APP_TASK_MOTOR_PRIO, "Task LED Blink", &os_err);
	#endif

}

static void uctsk_Motor (void) 
{   

	extern OS_EVENT      *Str_Q_Check ;
	extern	MessageBuffer mb[MESSAGE_BUFFER_NUM] ; 
    extern u8           infraredStatus ;
    
	int message_index ;
	char *msg_rcv ;	
	INT8U         err;
	OS_CPU_SR  cpu_sr = 0;


	char data[2]  ,temp[20] ,commandack[40];
	//char *head="commandresult:";
	char head[20];
	int distance ;			//运动距离
	int gear=1;				//档位
	int degree ;		    //转动角度
	
	motor_init() ; //初始化电机
    
    //_42_step_move_forward _42_step_move_left _42_step_move_right

	while(1){
        OSTimeDlyHMSM(0, 0, 5, 0);
//         if(infraredStatus < 40)
//         {
//             _42_step_move_left();
//             OSTimeDlyHMSM(0, 0, 1, 0);
//             if(infraredStatus > 40){
//                 _42_step_move_forward();
//                 OSTimeDlyHMSM(0, 0, 1, 500);
//             }else{
//                  _42_step_move_right();
//                 OSTimeDlyHMSM(0, 0, 2, 500);
//                  if(infraredStatus > 40){
//                     _42_step_move_forward();
//                      OSTimeDlyHMSM(0, 0, 1, 0);
//                  }else{
//                      _42_step_move_right();
//                     OSTimeDlyHMSM(0, 0, 1, 500);
//                  }
//             }
//         }else{
//             _42_step_move_forward();
//             OSTimeDlyHMSM(0, 0, 1, 0);
//        }
	}
    //OSTimeDlyHMSM(0, 0, 5, 0);
	
		while(1){
				//最快速度
// 			right_motor_forward(799);
// 			left_motor_forward(799);
// 			OSTimeDlyHMSM(0, 0, 5, 0);
			  //1档
// 			right_motor_forward(399);
// 			left_motor_forward(399);
// 			OSTimeDlyHMSM(0, 0, 5, 0);
// 			//2档
// 			right_motor_forward(499);
// 			left_motor_forward(499);
// 			OSTimeDlyHMSM(0, 0, 5, 0);
// 			//3档
// 			right_motor_forward(599);
// 			left_motor_forward(599);
// 			OSTimeDlyHMSM(0, 0, 5, 0);
			
// 				right_motor_back(1);
// 				left_motor_back(1);
// 				OSTimeDlyHMSM(0, 0, 5, 0);
			
// 				right_motor_back(399);
// 				left_motor_back(399);
// 				OSTimeDlyHMSM(0, 0, 5, 0);
// 				
// 				right_motor_back(299);
// 				left_motor_back(299);
// 				OSTimeDlyHMSM(0, 0, 5, 0);
// 				
// 				right_motor_back(199);
// 				left_motor_back(199);
// 				OSTimeDlyHMSM(0, 0, 5, 0);
			
		}

	//防碰撞 测试1
	while(1){
		if(infraredStatus >10){
		car_forward_senior(1,10);
		}else{
			car_back_stop();
			car_back_senior(1,10);
			car_left_senior(90);
		}
	}
 while(1){
// 		_42_step_move_forward();
// 		OSTimeDlyHMSM(0, 0, 5, 0);
		right_motor_forward(899);	
		left_motor_forward(899);
		OSTimeDlyHMSM(0, 0, 5, 0);
	  right_motor_forward(450);	
		left_motor_forward(450);
		OSTimeDlyHMSM(0, 0, 5, 0);
		right_motor_back(1);
	  left_motor_back(1);
		OSTimeDlyHMSM(0, 0, 5, 0);
	  right_motor_back(320);
	  left_motor_back(320);
		OSTimeDlyHMSM(0, 0, 5, 0);
}
	
#if DEBUG
		/****************电机动一下************************************/
//	OSTimeDlyHMSM(0, 0, 5, 0);
//	GPIO_SetBits(GPIOA,GPIO_Pin_6);   //右号电机
//	TIM_SetCompare3(TIM4,450); 		  //近满速顺时针转动
//	GPIO_SetBits(GPIOB,GPIO_Pin_6);   //左号电机
//	TIM_SetCompare4(TIM4,450); 		  //近满速顺时针转动
//	OSTimeDlyHMSM(0, 0, 1, 0);
//	//TIM_SetCompare3(TIM4,100); 
//	//TIM_SetCompare4(TIM4,100); 
//	OSTimeDlyHMSM(0, 0, 0, 500);
	while(1){
		//正转--满速
		right_motor_forward(899);	
		left_motor_forward(899);
		OSTimeDlyHMSM(0, 0, 5, 0);
		car_forward_stop();
		OSTimeDlyHMSM(0, 0, 5, 0);
		//反转--满速
		right_motor_back(1);
		left_motor_back(1);
		OSTimeDlyHMSM(0, 0, 5, 0);
		car_forward_stop();
		OSTimeDlyHMSM(0, 0, 5, 0);
		//正转--半速
		right_motor_forward(450);	
		left_motor_forward(450);
		OSTimeDlyHMSM(0, 0, 5, 0);
		car_forward_stop();
		OSTimeDlyHMSM(0, 0, 5, 0);
		//反转--半速
		right_motor_back(125);
		left_motor_back(125);
		OSTimeDlyHMSM(0, 0, 5, 0);
	}
#endif
//	msg_rcv = "\r\n+IPD,15:move_right_10" ;
	while(1){
		
		msg_rcv = OSQPend(Str_Q_Check , 0 , &err) ;
		memset(commandack,0,40);
		memset(temp,0,20);
		memset(head,0,20);
		strcat(head,"commandresult:");
		
		
	 	//向前运动
		if(strstr(msg_rcv , "forward") !=NULL){
			//2.获取运动时间
			left(data ,msg_rcv , 23 ) ;
			distance = atoi(data) ;
			//printf("distance=%d , gear=%d\r\n",distance , gear);
			//3.运动
			car_forward_senior(gear,distance) ;
			//4.运动返回的ACK
// 			strcat(commandack , head);
// 			left(temp,msg_rcv,10) ;
// 			strcat(commandack , temp);
		}else if(strstr(msg_rcv , "back") !=NULL){
			//2.获取运动时间
			left(data ,msg_rcv , 20) ;
			distance = atoi(data) ;
			//3.运动
			car_back_senior(gear,distance) ;
			//4.运动返回的ACK
// 			strcat(commandack , head);
// 			left(temp,msg_rcv,10) ;
// 			strcat(commandack , temp);
		}else if(strstr(msg_rcv , "left") !=NULL){
			left(data ,msg_rcv , 20 ) ;
			degree = atoi(data) ;
			car_left_senior(degree);
			//4.运动返回的ACK
// 			strcat(commandack , head);
// 			left(temp,msg_rcv,10) ;
// 			strcat(commandack , temp);
		}else if(strstr(msg_rcv , "right") !=NULL){
			left(data ,msg_rcv , 21 ) ;
			degree = atoi(data) ;
			car_right_senior(degree);
			//4.运动返回的ACK
// 			strcat(commandack , head);
// 			left(temp,msg_rcv,10) ;
// 			strcat(commandack , temp);
		}else if(strstr(msg_rcv , "gear") !=NULL){
			
			left(data ,msg_rcv , 14 ) ;
			gear=atoi(data);
			//运动返回的ACK
			strcat(commandack , head);
			left(temp,msg_rcv,9) ;
			strcat(commandack , temp);
		}
// 		//返回运动ACK
// 		OS_ENTER_CRITICAL();
// 		message_index = Message_Queue_Available(mb);
// 		OS_EXIT_CRITICAL();
// 		printf("\r\n<ultrasonic>-------Buffer is available , Index=%d\r\n" , message_index);
// 		strncpy(mb[message_index].buffer,commandack,strlen(commandack));
// 		err = OSQPost(Str_Q_SendAck,&mb[message_index]) ;
		OSTimeDlyHMSM(0, 0, 1, 0);
	}
}


//直流电机初始化，包括需要使用到的延时，中断，PWM波，LED灯，串口
void motor_init(void){
	TIM4_PWM_Init(899,0);
  gpio_motor_init();
}

/*从字符串的左边截取n个字符*/
static char * left(char *dst,char *src, int n)
{
    char *p = src ;
    char *q = dst;
    int len = strlen(src);
    int m = len - n ;
  
    p+=n ;
    while(m--)	*(q++) = *(p++);
	*(q++)='\0'; /*有必要吗？很有必要*/
    return q;
}


/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
