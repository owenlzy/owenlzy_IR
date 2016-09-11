
#include <includes.h>
#include "stm32f10x.h"
#include "gpio.h"
#include "app_cfg.h"
#include "usart.h"
#include "pwm.h"
#include "encode.h"
#include "adc.h"

#include "BSP.h"
#include "library_stm32.h"




/* Private variables ---------------------------------------------------------*/
static  OS_STK         App_TaskStartStk[APP_TASK_START_STK_SIZE];



/* Private function prototypes -----------------------------------------------*/
#if (OS_VIEW_MODULE == DEF_ENABLED)
extern void  App_OSViewTaskCreate (void);
#endif

static  void  App_TaskCreate		(void);
static  void  App_TaskStart			(void		*p_arg);





/***功能任务***/

 extern  void  App_xfTTSTaskCreate (void);						//讯飞文本发声任务，阅读传入的汉字




/****************消息队列变量************/

OS_EVENT  *Str_Q_XFTTS;
			
void 		*Msg_XFTTS[MESSAGE_SIZE];


/****************环形缓冲区************/
Buffer cb ;
/****************传感器状态值************/
volatile double sensorStatus[8] ;
volatile double voltageStatus ;		
volatile u16 lightStatus ,humanStatus;
volatile u8 infraredStatus ;
volatile u16 micStatus[3] ;     //mic 值
volatile double laserStaus = 0.0f;   //激光传感器值
/****************消息队列缓冲区************/
MessageBuffer mb[MESSAGE_BUFFER_NUM] ;

/**************** 点击转动的校准值设置 *******************/
int move_adjust = 0 ;
int turn_adjust = 0 ;

/************WIFI模块工作模式********************/
u8 WIFIStatus ; //WIFIStatus=1 ：工作模式  WIFIStatus=0 ：烧写模式
u8 LuaStatus  ;		//LuaStatus=1 : 工作模式	 LuaStatus=0 ：烧写模式

/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/
INT32S main (void)
{
		
	
	
    CPU_INT08U  os_err;
    os_err = os_err; /* prevent warning... */
		
	
	/* Note:  由于使用UCOS, 在OS运行之前运行,注意别使能任何中断. */
	CPU_IntDis();                    /* Disable all ints until we are ready to accept them.  */

    OSInit();                        /* Initialize "uC/OS-II, The Real-Time Kernel".         */

	/**************初始化检测WIFI工作模式的管脚****************/
	gpio_WIFIModeSwitch_init();
	WIFIStatus = GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_10);
	LuaStatus  = GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_5);
	/**************初始化串口**********************************/
	uart1_init(19200);	 	//串口1驱动激光传感器
	uart2_init(115200);	 	//串口2和Pcduino通信同时用于串口调试
// 	uart3_init(19200);	 	//串口4驱动激光测距	
	uart3_init(115200);	 	//串口3与讯飞通信	
	if(WIFIStatus == WIFI_MODE_BIN ||LuaStatus == WIFI_MODE_LUA){
// 					printf("%d   %d\r\n" , WIFIStatus , LuaStatus);
					uart4_disable();
					while(1);
	}else
		uart4_init(9600);	 	//串口4 与WIFI串口通信
	
	/***************创建消息队列******************************/

 	Str_Q_XFTTS = OSQCreate(Msg_XFTTS ,MESSAGE_SIZE); 

    /***************消息队列缓冲器初始化******************************/
	Message_Queue_Init(mb , MESSAGE_BUFFER_NUM) ;

	/***************ucosii任务创建****************************/		
	os_err = OSTaskCreateExt((void (*)(void *)) App_TaskStart,  /* Create the start task.                               */
                             (void          * ) 0,
                             (OS_STK        * )&App_TaskStartStk[APP_TASK_START_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_START_PRIO,
                             (INT16U          ) APP_TASK_START_PRIO,
                             (OS_STK        * )&App_TaskStartStk[0],
                             (INT32U          ) APP_TASK_START_STK_SIZE,
                             (void          * )0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
	
#if OS_TASK_NAME_EN > 0
    OSTaskNameSet(APP_TASK_START_PRIO, (CPU_INT08U *)"Start Task", &os_err);
#endif

	OSStart();                                                  /* Start multitasking (i.e. give control to uC/OS-II).  */

	return (0);
}


/*
*********************************************************************************************************
*                                          App_TaskStart()
*
* Description : The startup task.  The uC/OS-II ticker should only be initialize once multitasking starts.
*
* Argument(s) : p_arg       Argument passed to 'App_TaskStart()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************
*/	  
static  void  App_TaskStart (void *p_arg)
{   
	(void)p_arg;
	
	/***************  Init hardware ***************/

    OS_CPU_SysTickInit();                                    /* Initialize the SysTick.                              */

#if (OS_TASK_STAT_EN > 0)
    OSStatInit();                                            /* Determine CPU capacity.                              */
#endif

    App_TaskCreate();                                        /* Create application tasks.                            */

	for(;;)
   	{
      	OSTimeDlyHMSM(0, 1, 0, 0);							 /* Delay One minute */
    }	
}


/*
*********************************************************************************************************
*                                            App_TaskCreate()
*
* Description : Create the application tasks.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : App_TaskStart().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_TaskCreate (void)
{
#if (OS_VIEW_MODULE == DEF_ENABLED)
	App_OSViewTaskCreate();
#endif	
	OS_CPU_SR  cpu_sr = 0;
	 

#if DEBUG	
	printf("********* Hello UWIS Robot **********\r\n");
#endif
	
	/***************初始化环形缓存区******************************/
	rb_init(&cb) ;
	/***************三轴机械手定时器初始化*****************************/
// 	Timerx_Init(9000,160);
	/************蜂鸣器定时器初始化***********************/
	TIM1_PWM_Init(899 , 0);
	/***************键盘缓冲区初始化******************************/
// 	CreateQueue(&key_Q, 100);
	/**************初始化ADC采样**************************/
	Adc_Init();
	
	/***************  启动任务 ***************/
	OS_ENTER_CRITICAL();


 	App_xfTTSTaskCreate ();			//--

    
	OS_EXIT_CRITICAL();
	/**
		每隔10s采集一次电池电压
		ADC与电池电量的换算：voltage=adc*(3.3/4096)*(433/43)
	**/
	 while(1){

		voltageStatus = Get_Adc_Average(ADC_Channel_13,10)*(3.3/4096)*(433.0/43) ; 
// 		printf("voltage = %0.2lf\r\n" , voltageStatus);
		OSTimeDlyHMSM(0, 0, 10, 0);
	 }
}

/*************环形缓存区操作*********************/
/* 环形缓冲区的地址编号计算函数，如果到达唤醒缓冲区的尾部，将绕回到头部。

环形缓冲区的有效地址编号为：0到(NMAX-1)

*/

void rb_init(Buffer *cb){
	cb -> iput = 0 ;
	cb -> iget = 0 ;
	cb -> RB_Count = 0;
}

int addring (int i)
{

        return (i+1) == (256-1) ? 0 : i+1;

}

/* 从环形缓冲区中取一个元素 */

u8 rb_get(Buffer *cb)
{
	int pos;
	if (cb->RB_Count>0){
                      pos = cb->iget;
                      cb->iget = addring(cb->iget);	
                      cb->RB_Count--;
                      return cb->buffer[pos];

	}
	else {
#if DEBUG		
		printf("Buffer is empty. count = %d iput=%d iget=%d\n",cb->RB_Count,cb->iget,cb->iput); 
#endif		
		return 0;
	}
}

 

/* 向环形缓冲区中放入一个元素*/

int rb_put(Buffer *cb , u8 z)
{ 
if (cb->RB_Count<256){

  cb->buffer[cb->iput]=z;
  cb->iput = addring(cb->iput);
  cb->RB_Count++;
  return 1;
}
else {
	// add one at tail 	
    cb->iput = addring(cb->iput);  // already at tail, must at first jump to head.
	cb->buffer[cb->iput]=z;
    //cb->RB_Count++;
	return 0;	// 没有真实的增加一个数据，只是冲加价掉头上的数据，count没有加
}
}

/*************消息队列缓存区操作*********************/
void  Message_Queue_Init(MessageBuffer mb[] , int size){
	int i ;
	for(i=0;i<size;i++){
	 	memset(mb[i].buffer,0,MESSAGE_BUFFER_SIZE);
		mb[i].flag = 0 ;
	} 	
}
int Message_Queue_Available(MessageBuffer mb[]){
	int i ;
	for(i=0;i<MESSAGE_BUFFER_NUM;i++){
		if (mb[i].flag == 0){
			mb[i].flag = 1 ; //掷1
			return i ;
		}
	}
	printf("\r\n------NO AVAILABLE BUFFER , all has been used,please waiting\r\n");
	return -1 ;
}

int Message_Queue_Back(MessageBuffer *mb){
	mb->flag = 0 ; 	//掷0
	memset(mb->buffer,0,MESSAGE_BUFFER_SIZE);
	//printf("\r\n------Buffer is returned , flag=%d\r\n" , mb->flag);
	return 0 ;	
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
