/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               app_cfg.h
** Descriptions:            ucosii configuration
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

#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__
					  

/*
*********************************************************************************************************
*                                       MODULE ENABLE / DISABLE
*********************************************************************************************************
*/
#define  OS_VIEW_MODULE                  DEF_DISABLED	     	/* DEF_ENABLED = Present, DEF_DISABLED = Not Present        */


/*
*********************************************************************************************************
*                                              TASKS NAMES
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/	

#define  OS_VIEW_TASK_PRIO              	(                 5)
#define  APP_TASK_START_PRIO             	(                 29) 

/***第一优先级*****/
#define  APP_TASK_USART_AGENT_PRIO		 		(                 10)
#define  APP_TASK_USART_DEBUG_PRIO		 		(                 11)

/***第二优先级*****/
#define  APP_TASK_WIFI_MODE_SWTITCH_PRIO		(                 20)
#define  APP_TASK_xlib_PRIO		     			(                 21)
#define  APP_TASK_SEND_ACK_PRIO		     		(                 22)
#define  APP_TASK_CScriptCPicoc_PRIO            (                 23)
#define  APP_TASK_Avoid_PRIO					(                 25)


/***第三优先级*****/
#define  APP_TASK_TRACKER_PRIO		 	 			(                 30)			/* 循迹传感器  */
#define  APP_TASK_LASER_PRIO		     			(                 31)			/* 激光*/
#define  APP_TASK_KEY_PRIO                (                 32)			/* 按键*/
#define  APP_TASK_MOTOR_PRIO          	 	(                 33)			/* 电机*/
#define  APP_TASK_XFTTS_PRIO              (                 34)     /* 讯飞TTS   */
#define  APP_TASK_DIGITRON_PRIO		        (                 35)			/* 数码管*/
#define  APP_TASK_POWER_MANAGE_PRIO      	(                 36)			/* 电源管理*/
#define  APP_TASK_SENSOR_STATUS_PRIO		  (                 37)			/* 传感器状态表*/
#define  APP_TASK_LIGHTANDHUMAN_PRIO			(                 38)			/* 光线电阻和人体传感器  */
#define  APP_TASK_8X8_PRIO                (                 39)     /* 8*8点阵  */
#define  APP_TASK_LCD12864_PRIO         	(                 40)     /* LCD    */
#define  APP_TASK_INFRAREDANDMICS_PRIO		(                 41)			/*夏普红外线和MIC*/
#define  APP_TASK_LED_WS2812S_PRIO				(                 42)			/*WS2812S 全彩LED灯*/
#define  APP_TASK_LED_PRIO								(                 43)			/*串联LED灯*/



#define  APP_TASK_OSVIEW_TERMINAL_PRIO	 (OS_LOWEST_PRIO - 6)
#define  OS_VIEW_TASK_PRIO               (OS_LOWEST_PRIO - 3)
#define  OS_TASK_TMR_PRIO                (OS_LOWEST_PRIO - 2)



/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*                             Size of the task stacks (# of OS_STK entries)
*********************************************************************************************************
*/
#define  APP_TASK_START_STK_SIZE                          64u
#define  APP_TASK_OSVIEW_TERMINAL_STK_SIZE   			 128u
#define  OS_VIEW_TASK_STK_SIZE                           128u
#define  APP_TASK_MOTOR_STK_SIZE                         128u
#define	 APP_TASK_WIFIMODESWITCH_STK_SIZE				96u
#define	 APP_TASK_USART_AGENT_STK_SIZE					 256u
#define	 APP_TASK_USART_DEBUG_STK_SIZE					 256u
#define	 APP_TASK_TRACKER_STK_SIZE		     		 	 32u
#define	 APP_TASK_SENSOR_STATUS_STK_SIZE		     	96u
#define	 APP_TASK_POWER_MANAGE_STK_SIZE					 32u
#define  APP_KEY_STK_SIZE                               128u
#define	 APP_TASK_SEND_ACK_STK_SIZE					 	 192u
#define	 APP_TASK_xlib_STK_SIZE					 	 	512u
#define  APP_TASK_CScriptCPicoc_STK_SIZE                1050u
#define  APP_TASK_Avoid_STK_SIZE						320u
#define	 APP_TASK_LASER_STK_SIZE					 	 	96u
#define	 APP_TASK_LIGHT_HUMAN_STK_SIZE					 	 	96u      /*   光线电阻  */
#define  APP_TASK_8X8_STK_SIZE                               96u            /*  8*8点阵   */
#define  APP_TASK_LCD12864_STK_SIZE                               128u       
#define  APP_TASK_DIGITRON_STK_SIZE           128u
#define  APP_TASK_XFTTS_STK_SIZE           256u
#define  APP_TASK_INFRARED_MICS_STK_SIZE	96u
#define  APP_TASK_LED_WS2812S_STK_SIZE    64u
#define  APP_TASK_LED_STK_SIZE    				64u

/*
*********************************************************************************************************
*                                            UCOSII MESSAGE
*                             Size of the message
*********************************************************************************************************
*/
 #define  MESSAGE_SIZE                    				 20u   
/*
*********************************************************************************************************
*                                                  LIB
*********************************************************************************************************
*/

#define  uC_CFG_OPTIMIZE_ASM_EN                 DEF_ENABLED
#define  LIB_STR_CFG_FP_EN                      DEF_DISABLED
/*
*********************************************************************************************************
*                                                  DEBUG
*********************************************************************************************************
*/
#define DEBUG 									0
#define DEBUG_USARTAGENT 						0
#define DEBUG_XLIB 								0
#define DEBUG_SENDACK 							0
#define DEBUG_WIFISWITCH 						0
#define DEBUG_SENSORSTATUS					    0
#define DEBUG_LASER								0
#define DEBUG_XFTTS								0
#define DEBUG_LIGHT_HUMAN					    0
#define DEBUG_LCD12864							0
#define DEBUG_INFRARED_MICS					    0
#define DEBUG_CSCRIPT							0
#define DEBUG_USARTDEBUG						0
/*
*********************************************************************************************************
*                                                RING  BUFFER
*********************************************************************************************************
*/

typedef struct {
    int iput  ; /* 环形缓冲区的当前放入位置 */
    int iget  ; /* 缓冲区的当前取出位置 */
    int RB_Count ; /* 环形缓冲区中的元素总数量 */
    unsigned char buffer[256];  /* vector of elements                   */
} Buffer;

unsigned char  rb_get(Buffer *cb);
int rb_put(Buffer *cb , unsigned char z);
void rb_init(Buffer *cb) ;
/*
*********************************************************************************************************
*                                     MESSAGE QUEUE   RING  BUFFER
*********************************************************************************************************
*/
#define MESSAGE_BUFFER_NUM  20
#define MESSAGE_BUFFER_SIZE 128
typedef struct{
	char buffer[MESSAGE_BUFFER_SIZE];	/*内存*/
	char flag;  						/*是否被使用的标志位，1为被占用， 0为未被占用*/
}MessageBuffer;

void  Message_Queue_Init(MessageBuffer mb[] , int size);
int Message_Queue_Available(MessageBuffer mb[]);
int Message_Queue_Back(MessageBuffer *mb);
/*
*********************************************************************************************************
*                                                  SPEED CHANGING BOX
*********************************************************************************************************
*/
#define SPEED_1	24						//单位：cm/s
#define SPEED_2	36
#define SPEED_3	48

#define SPEED_TURN			126		//单位：°/s


/*
*********************************************************************************************************
*                                            STEP MOTOR
*********************************************************************************************************
*/
#define LEFT_MOTOR		GPIOF
#define RIGHT_MOTOR		GPIOE

#define LEFT_MOTOR_IN1  GPIO_Pin_0
#define LEFT_MOTOR_IN2  GPIO_Pin_1
#define LEFT_MOTOR_IN3  GPIO_Pin_2
#define LEFT_MOTOR_IN4  GPIO_Pin_3

#define RIGHT_MOTOR_IN1  GPIO_Pin_14
#define RIGHT_MOTOR_IN2  GPIO_Pin_13
#define RIGHT_MOTOR_IN3  GPIO_Pin_12
#define RIGHT_MOTOR_IN4  GPIO_Pin_11

/*
*********************************************************************************************************
*                                            WIFI Mode
*********************************************************************************************************
*/
#define WIFI_MODE_BIN		0
#define WIFI_MODE_WORK	1
#define WIFI_MODE_LUA		0
#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

