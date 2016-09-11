
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





/***��������***/

 extern  void  App_xfTTSTaskCreate (void);						//Ѷ���ı����������Ķ�����ĺ���




/****************��Ϣ���б���************/

OS_EVENT  *Str_Q_XFTTS;
			
void 		*Msg_XFTTS[MESSAGE_SIZE];


/****************���λ�����************/
Buffer cb ;
/****************������״ֵ̬************/
volatile double sensorStatus[8] ;
volatile double voltageStatus ;		
volatile u16 lightStatus ,humanStatus;
volatile u8 infraredStatus ;
volatile u16 micStatus[3] ;     //mic ֵ
volatile double laserStaus = 0.0f;   //���⴫����ֵ
/****************��Ϣ���л�����************/
MessageBuffer mb[MESSAGE_BUFFER_NUM] ;

/**************** ���ת����У׼ֵ���� *******************/
int move_adjust = 0 ;
int turn_adjust = 0 ;

/************WIFIģ�鹤��ģʽ********************/
u8 WIFIStatus ; //WIFIStatus=1 ������ģʽ  WIFIStatus=0 ����дģʽ
u8 LuaStatus  ;		//LuaStatus=1 : ����ģʽ	 LuaStatus=0 ����дģʽ

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
		
	
	/* Note:  ����ʹ��UCOS, ��OS����֮ǰ����,ע���ʹ���κ��ж�. */
	CPU_IntDis();                    /* Disable all ints until we are ready to accept them.  */

    OSInit();                        /* Initialize "uC/OS-II, The Real-Time Kernel".         */

	/**************��ʼ�����WIFI����ģʽ�Ĺܽ�****************/
	gpio_WIFIModeSwitch_init();
	WIFIStatus = GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_10);
	LuaStatus  = GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_5);
	/**************��ʼ������**********************************/
	uart1_init(19200);	 	//����1�������⴫����
	uart2_init(115200);	 	//����2��Pcduinoͨ��ͬʱ���ڴ��ڵ���
// 	uart3_init(19200);	 	//����4����������	
	uart3_init(115200);	 	//����3��Ѷ��ͨ��	
	if(WIFIStatus == WIFI_MODE_BIN ||LuaStatus == WIFI_MODE_LUA){
// 					printf("%d   %d\r\n" , WIFIStatus , LuaStatus);
					uart4_disable();
					while(1);
	}else
		uart4_init(9600);	 	//����4 ��WIFI����ͨ��
	
	/***************������Ϣ����******************************/

 	Str_Q_XFTTS = OSQCreate(Msg_XFTTS ,MESSAGE_SIZE); 

    /***************��Ϣ���л�������ʼ��******************************/
	Message_Queue_Init(mb , MESSAGE_BUFFER_NUM) ;

	/***************ucosii���񴴽�****************************/		
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
	
	/***************��ʼ�����λ�����******************************/
	rb_init(&cb) ;
	/***************�����е�ֶ�ʱ����ʼ��*****************************/
// 	Timerx_Init(9000,160);
	/************��������ʱ����ʼ��***********************/
	TIM1_PWM_Init(899 , 0);
	/***************���̻�������ʼ��******************************/
// 	CreateQueue(&key_Q, 100);
	/**************��ʼ��ADC����**************************/
	Adc_Init();
	
	/***************  �������� ***************/
	OS_ENTER_CRITICAL();


 	App_xfTTSTaskCreate ();			//--

    
	OS_EXIT_CRITICAL();
	/**
		ÿ��10s�ɼ�һ�ε�ص�ѹ
		ADC���ص����Ļ��㣺voltage=adc*(3.3/4096)*(433/43)
	**/
	 while(1){

		voltageStatus = Get_Adc_Average(ADC_Channel_13,10)*(3.3/4096)*(433.0/43) ; 
// 		printf("voltage = %0.2lf\r\n" , voltageStatus);
		OSTimeDlyHMSM(0, 0, 10, 0);
	 }
}

/*************���λ���������*********************/
/* ���λ������ĵ�ַ��ż��㺯����������﻽�ѻ�������β�������ƻص�ͷ����

���λ���������Ч��ַ���Ϊ��0��(NMAX-1)

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

/* �ӻ��λ�������ȡһ��Ԫ�� */

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

 

/* ���λ������з���һ��Ԫ��*/

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
	return 0;	// û����ʵ������һ�����ݣ�ֻ�ǳ�Ӽ۵�ͷ�ϵ����ݣ�countû�м�
}
}

/*************��Ϣ���л���������*********************/
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
			mb[i].flag = 1 ; //��1
			return i ;
		}
	}
	printf("\r\n------NO AVAILABLE BUFFER , all has been used,please waiting\r\n");
	return -1 ;
}

int Message_Queue_Back(MessageBuffer *mb){
	mb->flag = 0 ; 	//��0
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
