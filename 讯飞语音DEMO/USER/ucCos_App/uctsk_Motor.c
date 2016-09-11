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
	int distance ;			//�˶�����
	int gear=1;				//��λ
	int degree ;		    //ת���Ƕ�
	
	motor_init() ; //��ʼ�����
    
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
				//����ٶ�
// 			right_motor_forward(799);
// 			left_motor_forward(799);
// 			OSTimeDlyHMSM(0, 0, 5, 0);
			  //1��
// 			right_motor_forward(399);
// 			left_motor_forward(399);
// 			OSTimeDlyHMSM(0, 0, 5, 0);
// 			//2��
// 			right_motor_forward(499);
// 			left_motor_forward(499);
// 			OSTimeDlyHMSM(0, 0, 5, 0);
// 			//3��
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

	//����ײ ����1
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
		/****************�����һ��************************************/
//	OSTimeDlyHMSM(0, 0, 5, 0);
//	GPIO_SetBits(GPIOA,GPIO_Pin_6);   //�Һŵ��
//	TIM_SetCompare3(TIM4,450); 		  //������˳ʱ��ת��
//	GPIO_SetBits(GPIOB,GPIO_Pin_6);   //��ŵ��
//	TIM_SetCompare4(TIM4,450); 		  //������˳ʱ��ת��
//	OSTimeDlyHMSM(0, 0, 1, 0);
//	//TIM_SetCompare3(TIM4,100); 
//	//TIM_SetCompare4(TIM4,100); 
//	OSTimeDlyHMSM(0, 0, 0, 500);
	while(1){
		//��ת--����
		right_motor_forward(899);	
		left_motor_forward(899);
		OSTimeDlyHMSM(0, 0, 5, 0);
		car_forward_stop();
		OSTimeDlyHMSM(0, 0, 5, 0);
		//��ת--����
		right_motor_back(1);
		left_motor_back(1);
		OSTimeDlyHMSM(0, 0, 5, 0);
		car_forward_stop();
		OSTimeDlyHMSM(0, 0, 5, 0);
		//��ת--����
		right_motor_forward(450);	
		left_motor_forward(450);
		OSTimeDlyHMSM(0, 0, 5, 0);
		car_forward_stop();
		OSTimeDlyHMSM(0, 0, 5, 0);
		//��ת--����
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
		
		
	 	//��ǰ�˶�
		if(strstr(msg_rcv , "forward") !=NULL){
			//2.��ȡ�˶�ʱ��
			left(data ,msg_rcv , 23 ) ;
			distance = atoi(data) ;
			//printf("distance=%d , gear=%d\r\n",distance , gear);
			//3.�˶�
			car_forward_senior(gear,distance) ;
			//4.�˶����ص�ACK
// 			strcat(commandack , head);
// 			left(temp,msg_rcv,10) ;
// 			strcat(commandack , temp);
		}else if(strstr(msg_rcv , "back") !=NULL){
			//2.��ȡ�˶�ʱ��
			left(data ,msg_rcv , 20) ;
			distance = atoi(data) ;
			//3.�˶�
			car_back_senior(gear,distance) ;
			//4.�˶����ص�ACK
// 			strcat(commandack , head);
// 			left(temp,msg_rcv,10) ;
// 			strcat(commandack , temp);
		}else if(strstr(msg_rcv , "left") !=NULL){
			left(data ,msg_rcv , 20 ) ;
			degree = atoi(data) ;
			car_left_senior(degree);
			//4.�˶����ص�ACK
// 			strcat(commandack , head);
// 			left(temp,msg_rcv,10) ;
// 			strcat(commandack , temp);
		}else if(strstr(msg_rcv , "right") !=NULL){
			left(data ,msg_rcv , 21 ) ;
			degree = atoi(data) ;
			car_right_senior(degree);
			//4.�˶����ص�ACK
// 			strcat(commandack , head);
// 			left(temp,msg_rcv,10) ;
// 			strcat(commandack , temp);
		}else if(strstr(msg_rcv , "gear") !=NULL){
			
			left(data ,msg_rcv , 14 ) ;
			gear=atoi(data);
			//�˶����ص�ACK
			strcat(commandack , head);
			left(temp,msg_rcv,9) ;
			strcat(commandack , temp);
		}
// 		//�����˶�ACK
// 		OS_ENTER_CRITICAL();
// 		message_index = Message_Queue_Available(mb);
// 		OS_EXIT_CRITICAL();
// 		printf("\r\n<ultrasonic>-------Buffer is available , Index=%d\r\n" , message_index);
// 		strncpy(mb[message_index].buffer,commandack,strlen(commandack));
// 		err = OSQPost(Str_Q_SendAck,&mb[message_index]) ;
		OSTimeDlyHMSM(0, 0, 1, 0);
	}
}


//ֱ�������ʼ����������Ҫʹ�õ�����ʱ���жϣ�PWM����LED�ƣ�����
void motor_init(void){
	TIM4_PWM_Init(899,0);
  gpio_motor_init();
}

/*���ַ�������߽�ȡn���ַ�*/
static char * left(char *dst,char *src, int n)
{
    char *p = src ;
    char *q = dst;
    int len = strlen(src);
    int m = len - n ;
  
    p+=n ;
    while(m--)	*(q++) = *(p++);
	*(q++)='\0'; /*�б�Ҫ�𣿺��б�Ҫ*/
    return q;
}


/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
