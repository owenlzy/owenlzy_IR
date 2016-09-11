#include <includes.h> 
#include "gpio.h"
#include "esp8266.h"



/* Private variables ---------------------------------------------------------*/
static  OS_STK         App_TaskPlayWaveStk[APP_TASK_SAMPLING_STK_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void uctsk_playcontrol            (void);
static void WT588D_delay(unsigned char m , unsigned char n);
static void WT588D_speak(u8 addr , int time) ;



void  App_PlayWaveCreate (void)
{
    CPU_INT08U  os_err;

		os_err = os_err; /* prevent warning... */



		os_err = OSTaskCreate((void (*)(void *)) uctsk_playcontrol,				
                          (void          * ) 0,							
                          (OS_STK        * )&App_TaskPlayWaveStk[APP_TASK_SAMPLING_STK_SIZE - 1],		
                          (INT8U           ) APP_TASK_PLAYWAVE_PRIO  );
						  							

		#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(APP_TASK_PLAYWAVE_PRIO, "Task WAVE Blink", &os_err);
		#endif
	
}
static void uctsk_playcontrol(void)
{	
	INT8U         err;
	OS_CPU_SR  cpu_sr = 0;
	int j;
	extern OS_EVENT  *Str_Q_Client2Sound ,*Sem_TCPSend ;
	MessageBuffer *msg_rcv_p  ;
	char msg_rcv[MESSAGE_BUFFER_SIZE] ;
	char command_tmp[10]="command";


	gpio_voice_init();
	printf("\r\n[sound]sound power on\r\n");
	GPIO_ResetBits(GPIOB , GPIO_Pin_15);
	OSTimeDlyHMSM(0, 0, 0, 5);
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
	OSTimeDlyHMSM(0, 0, 0, 20);
	GPIO_ResetBits(GPIOB,GPIO_Pin_13);
	OSTimeDlyHMSM(0, 0, 0, 5);
	WT588D_speak(0,5) ;		//开机语音

	while(1){
		msg_rcv_p=OSQPend(Str_Q_Client2Sound,0,&err);
		strncpy(msg_rcv,msg_rcv_p->buffer,MESSAGE_BUFFER_SIZE);
		printf("\r\n[%s]" , msg_rcv);
		printf("\r\n[sound] msg_rcv = [%s] \r\n" , msg_rcv);
		//消息队列缓冲区已被用完,释放掉
		OS_ENTER_CRITICAL();
		Message_Queue_Back(msg_rcv_p);
		OS_EXIT_CRITICAL();
	    printf("\r\n<palywave>------Buffer is returned , flag=%d\r\n" , msg_rcv_p->flag);
		
		
		if(strstr(msg_rcv , "poweron")){
			j=0;
		}else if(strstr(msg_rcv , "connected")){
			j=1 ;	
		}else if(strstr(msg_rcv , "clock")){
			j=2 ;	
		}else if(strstr(msg_rcv , "touched")){
			j=3 ;	
		}else if(strstr(msg_rcv , "light")){
			j=5 ;	
		}else if(strstr(msg_rcv , "sound")){
			j=4 ;	
		}
		printf("j=%d",j);


		GPIO_ResetBits(GPIOB , GPIO_Pin_15);
		OSTimeDlyHMSM(0, 0, 0, 5);
		GPIO_SetBits(GPIOB,GPIO_Pin_15);
		OSTimeDlyHMSM(0, 0, 0, 20);
		GPIO_ResetBits(GPIOB,GPIO_Pin_13);
		OSTimeDlyHMSM(0, 0, 0, 5);

		switch(j){
			/****上电发声*****/
			case 0 : 	WT588D_speak(0,5) ;
						break ;
			case 1 : 	WT588D_speak(1,4) ;
						break ;
			case 2 : 	WT588D_speak(2,4) ;
						break ;
			case 3 : 	WT588D_speak(3,5) ;
						break ;
			case 4 : 	
						WT588D_speak(4,3) ;
						break ;
			case 5 : 	WT588D_speak(5,4) ;
						break ;
			default:	break ;

		}
		//发送ACK
		if(strstr(msg_rcv,"+IPD")){
			OSSemPend(Sem_TCPSend,0,&err);	//信号量P操作
			if(err == OS_ERR_NONE)
				Send_Data(command_tmp);
			OSSemPost(Sem_TCPSend);
		}		
	}
		
}


static void WT588D_delay(unsigned char m , unsigned char n){
	unsigned char x = 0x00 ;
	unsigned char y = 0x00 ;
	do{
		x=100;
		do{
			y=n;
			do{
				__nop();
			}while(--y);
		}while(--x);
	}while(--m);
}

static void WT588D_speak(u8 addr , int time){

	int i ;
	for(i=0;i<8;i++){
			GPIO_SetBits(GPIOB , GPIO_Pin_13);
			if(addr & 1)
            {
               WT588D_delay(3,1) ;
			   WT588D_delay(3,1) ;
			   WT588D_delay(3,1) ;
               GPIO_ResetBits(GPIOB , GPIO_Pin_13);
               WT588D_delay(3,1) ;
            }else{

				WT588D_delay(3,1) ;
                GPIO_ResetBits(GPIOB , GPIO_Pin_13);
                WT588D_delay(3,1) ;
				WT588D_delay(3,1) ;
				WT588D_delay(3,1) ; 
			}
			addr>>=1;
		}
	GPIO_SetBits(GPIOB , GPIO_Pin_13);
	OSTimeDlyHMSM(0, 0, time, 0);
}

