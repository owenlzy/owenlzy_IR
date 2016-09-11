// 	/****************************************Copyright (c)****************************************************
// 	**                                      
// 	**                                 http://www.powermcu.com
// 	**
// 	**--------------File Info---------------------------------------------------------------------------------
// 	** File name:               uctsk_Usart_Agent.c
// 	** Descriptions:            The uctsk_WIFI application function
// 	**
// 	**--------------------------------------------------------------------------------------------------------
// 	** Created by:              AVRman
// 	** Created date:            2010-11-9
// 	** Version:                 v1.0
// 	** Descriptions:            The original version
// 	**
// 	**--------------------------------------------------------------------------------------------------------
// 	** Modified by:             
// 	** Modified date:           
// 	** Version:                 
// 	** Descriptions:            
// 	**
// 	*********************************************************************************************************/

// 	/* Includes ------------------------------------------------------------------*/
// 	#include <includes.h> 
// 	#include "usart.h" 
// 	#include "app_cfg.h" 
// 	#include "esp8266.h"
// 	#include "exit.h"
// 	#include "gpio.h"
// 	#include "adc.h"
// 	#include "RTC.h"

// 	/* Private variables ---------------------------------------------------------*/
// 	static  OS_STK         App_TaskPowerManageStk[APP_TASK_POWER_MANAGE_STK_SIZE];

// 	/* Private function prototypes -----------------------------------------------*/
// 	static void uctsk_Power_Manage            (void);

// 	/************��ʱ��1����ʱ����stopģʽ*****************/
// 	OS_TMR   * tmr1;			
// 	void  tmr1_callback(OS_TMR *ptmr,void *p_arg);
// 	int stop_mode_period_count=0;//������
// 	
// 	/************��ʱ��2������ģ��*****************/
// 	OS_TMR   * tmr2;		
// 	void  tmr2_callback(OS_TMR *ptmr,void *p_arg);
// 	int sound_count=0;	 			//��һ���ƣ������жϴ���
// 	int  statistical_count=0;  		//���ƴ���
// 	
// 	/************��ʱ��2������ģ��2  ��һ�����2�λ��ƺ�3�λ���*****************/
// 	OS_TMR   * tmr3;			
// 	void  tmr3_callback(OS_TMR *ptmr,void *p_arg);
// 	static  u8  flag=0;

// 	/************��ʱ��4������ģ��*****************/
// 	OS_TMR   * tmr4;			
// 	void  tmr4_callback(OS_TMR *ptmr,void *p_arg);
// 	static  int  click_count=0;
// 	static  int  touch_flag=0;
// 	static  int  touch_temp=0;

// 	/************��ʱ��5������ģ��2 �� ��һ������ε��*****************/
// 	OS_TMR   * tmr5;			
// 	void  tmr5_callback(OS_TMR *ptmr,void *p_arg);
// 	static   int  touch2_flag=0;

// 	/************��ʱ��6������ģ��*****************/
// 	OS_TMR   * tmr6;			//�����ʱ��1
// 	void  tmr6_callback(OS_TMR *ptmr,void *p_arg);//�����ʱ���ص�����
// 	static   int light_flag = 0 ;
// 	static   int  light_high_flag=0;
// 	static   int light_flag1 = 0 ;		

// 	
// 		
// 	extern OS_EVENT *Str_Q_PowerManagement ,  *Str_Q_Client2led , *Str_Q_Client2Sound,*Str_Q_Client2Motor;
// 	extern MessageBuffer mb[MESSAGE_BUFFER_NUM] ;

//  
// 	void  App_PowerManageCreate (void)
// 	{
// 		CPU_INT08U  os_err;

// 		os_err = os_err; /* prevent warning... */
// 		os_err = OSTaskCreate((void (*)(void *)) uctsk_Power_Manage,				
// 														(void          * ) 0,							
// 														(OS_STK        * )&App_TaskPowerManageStk[APP_TASK_POWER_MANAGE_STK_SIZE - 1],		
// 														(INT8U           ) APP_TASK_POWER_MANAGE_PRIO  );
// 															

// 		#if OS_TASK_NAME_EN > 0
// 				OSTaskNameSet(APP_TASK_POWER_MANAGE_PRIO, "Task WIFI Blink", &os_err);
// 		#endif	
// 	}

// 	static void uctsk_Power_Manage (void) 
// 	{  
// 		u8  err;
// 		int cpu_sr;  
// 		MessageBuffer *p ;
// 		char  pp[5];
// 		

// 		extern OS_EVENT  *Str_Q_Client2Motor;
// 		extern	MessageBuffer mb[MESSAGE_BUFFER_NUM] ; //��Ϣ���л�����
// 		int message_index ;
// 		char *wakeupcommand="wakeupcmd:move_forward_10" ;

// 		printf("hello");

// 		EXTIX_Init();
// 		Adc_Init(); //���ߴ�����ʹ�� AO --PA.4
// // 		RTC_Init();//rtc ��ʼ��
// 		tmr1=OSTmrCreate(100,100,OS_TMR_OPT_PERIODIC,(OS_TMR_CALLBACK)tmr1_callback,0,"tmr1",&err);		//10sִ��һ��
// 		OSTmrStart(tmr1,&err);//���������ʱ��1
// 		tmr2=OSTmrCreate(1,1,OS_TMR_OPT_PERIODIC,(OS_TMR_CALLBACK)tmr2_callback,0,"tmr2",&err);		//10sִ��һ��
// 	  	tmr3=OSTmrCreate(10,10,OS_TMR_OPT_PERIODIC,(OS_TMR_CALLBACK)tmr3_callback,0,"tmr3",&err);		//10sִ��һ��
// 		tmr4=OSTmrCreate(1,1,OS_TMR_OPT_PERIODIC,(OS_TMR_CALLBACK)tmr4_callback,0,"tmr4",&err);		//10sִ��һ��
// 	  	tmr5=OSTmrCreate(10,10,OS_TMR_OPT_PERIODIC,(OS_TMR_CALLBACK)tmr5_callback,0,"tmr5",&err);		//10sִ��һ��
// 		tmr6=OSTmrCreate(10,10,OS_TMR_OPT_PERIODIC,(OS_TMR_CALLBACK)tmr6_callback,0,"tmr6",&err);		//10sִ��һ��
// 		while(1)
// 		{

// 		  p=OSQPend(Str_Q_PowerManagement,0,&err);//������Ϣ����
// 		  strncpy(pp,p->buffer,10);
// 			//��Ϣ���л������ѱ�����,�ͷŵ�
// 			OS_ENTER_CRITICAL();
// 			Message_Queue_Back(p);
// 			OS_EXIT_CRITICAL();
// 			printf("\r\n<powermanage>------Buffer is returned , flag=%d\r\n" , p->flag);
// 			if (strstr(pp , "1"))//����stop ģʽ
// 			{ 
// 					RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//ʹ��PWR����ʱ��
// 					printf("ok!robot has been stop\r\n");
// // 				  	ALARM0_CNT_Set(5);//��ʱ10s
// 					PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
// 			}
// 			else if(strstr(pp , "2"))//�������ѽ������
// 			{ 
// 				printf("stop  modern is break by touch!\r\n");
// //				click_count=0;
//  				OSTmrStart(tmr4,&err);//���������ʱ��1
// 			}	
// 			else if(strstr(pp , "3"))//�����߽������
// 			{ 
// 				printf("stop  modern is break by light!\r\n");
// 				if(OSTmrStateGet(tmr6,&err)==OS_TMR_STATE_STOPPED)
// 			   		OSTmrStart(tmr6,&err);//���������ʱ��2
// 				light_flag1=0;
// 				light_flag=0;
// 				light_high_flag=0;
// 			}	
// 			else if(strstr(pp , "4"))//mic�ѽ������
// 			{ 
// 				printf("stop  modern is break by mic!\r\n");
// 				sound_count=0;
// 				if(OSTmrStateGet(tmr2,&err)==OS_TMR_STATE_STOPPED)
// 			   		OSTmrStart(tmr2,&err);//���������ʱ��2
// 				
// 			}	
// 		 }
// 										
// 	}
// //��ʱ��1�Ļص�����	----��ʱ����stopģʽ
// void  tmr1_callback(OS_TMR *ptmr,void *p_arg)
// {
// 		int cpu_sr; 
// 		extern	MessageBuffer mb[MESSAGE_BUFFER_NUM] ; 					  //��Ϣ���л�����
// 		int message_index=0 ;
// 		char *p = "1" ;
// 		stop_mode_period_count=stop_mode_period_count+1;
// 		if(stop_mode_period_count==10000)
// 		{
// 				OS_ENTER_CRITICAL();
// 				message_index = Message_Queue_Available(mb);
// 				OS_EXIT_CRITICAL();
// 				strncpy(mb[message_index].buffer, p, sizeof(p));
// 				OSQPost(Str_Q_PowerManagement,&mb[message_index]);
// 		}
// 		else if(stop_mode_period_count>=10000)
// 		{
// 				 stop_mode_period_count=0;	  	//��������0
// 		}
// 		printf("s_count=%d",stop_mode_period_count);
// 			 
// }
// //��ʱ��2�Ļص�����	----����ģ��1
// void  tmr2_callback(OS_TMR *ptmr,void *p_arg)
//  {
// 	 u8 err;									   
// 	 if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==0)
// 		 sound_count++; 
// 	 else 
// 		 sound_count=0;
// 	 if(sound_count>=20)	//20ms�������û���������ж��������ж�����һ�λ���
// 	 {
// 	    statistical_count++;
// 	    sound_count=0;	
// 		flag=0;
// 	    OSTmrStop(tmr2,OS_TMR_OPT_NONE,0,&err);	//�ر������ʱ��2;
// 	    OSTmrStart(tmr3,&err);					//��ʼ������3�����ڼ����ƺ�1s֮���Ƿ��ٴ��л���
//     }	
// }

// //��ʱ��3�Ļص�����	----����ģ��2
// void  tmr3_callback(OS_TMR *ptmr,void *p_arg)
//  {  
//  	  u8  err;
// 	  OS_CPU_SR  cpu_sr = 0;
// 	  char init_message[20] ;
// 	  int message_index ;
// 	  flag++;
// 	 if(OSTmrStateGet(tmr2,&err)==OS_TMR_STATE_STOPPED)//��ʱ����û�п�����
// 	 { 
// 	 	if(flag>=10)//1s��ʱ�Ѿ�����
// 	    {
//          		flag=0;
// 			   	if(statistical_count>3)
// 			   		statistical_count=3;
// 				if(statistical_count==1){
// 					/*************��һ�£���Ӧ�¶���*********************/
// 					memset(init_message,0,20) ;
// 					sprintf(init_message , "%s" , "led_jiong");
// 					OS_ENTER_CRITICAL();
// 					message_index = Message_Queue_Available(mb);
// 					OS_EXIT_CRITICAL();
// 					strncpy(mb[message_index].buffer,init_message,strlen(init_message));
// 					err = OSQPost(Str_Q_Client2led,&mb[message_index]) ;
// 				
// 					memset(init_message,0,20) ;
// 					sprintf(init_message , "%s" , "sound_sound");
// 					OS_ENTER_CRITICAL();
// 					message_index = Message_Queue_Available(mb);
// 					OS_EXIT_CRITICAL();
// 					strncpy(mb[message_index].buffer,init_message,strlen(init_message));
// 					err = OSQPost(Str_Q_Client2Sound,&mb[message_index]) ;
// 				}
//          		printf("statistical_count=%d",statistical_count);
// 			   	statistical_count=0;
// 				OSTmrStop(tmr3,OS_TMR_OPT_NONE,0,&err);//�ر������ʱ��3; 
//         }
//     }
// 	else//��ʱ�����ֿ����ˣ��رռ�����������������
// 	{
//      	flag=0;
//      	OSTmrStop(tmr3,OS_TMR_OPT_NONE,0,&err);//�ر������ʱ��3;
//     }
//  }

// //��ʱ��4�Ļص�����	----����1
//  
// //   void  tmr4_callback(OS_TMR *ptmr,void *p_arg)
// // { 
// // 	////////////////////////�������ȴ���������/////////////////////////
// // 	 u8 err;
// // 	 if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==1)
// // 		 touch_temp++; 
// // 	 else 
// // 		 touch_temp=0;
// // 	 if(touch_temp>=5)	//20ms�������û���������ж��������ж�����һ�λ���
// // 	 {
// // 	    click_count++;
// // 	    touch_temp=0;	
// // 		  touch_flag=0;
// // 		  touch2_flag=0;
// // 	    OSTmrStop(tmr4,OS_TMR_OPT_NONE,0,&err);	//�ر������ʱ��2;
// // 	    OSTmrStart(tmr5,&err);					//��ʼ������3�����ڼ����ƺ�1s֮���Ƿ��ٴ��л���
// // 		   printf("time  is  ok!\r\n");
// //     }	
// // //////////////////////////////////////////////////////////////////////////////////		
// // // 	u8  err; 
// // // 	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==1&&touch_flag==0)
// // // 	{click_count++; touch_flag=1;touch_temp=0;printf("0  is  ok  \r\n");}
// // //   if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0&&touch_flag==1)
// // // 	{click_count++; touch_flag=0;touch_temp=0;printf("1  is  ok  \r\n");}
// // // 	touch_temp++;
// // // 	printf(" click_count=%d \r\n",click_count);
// // // 	printf(" touch_temp=%d \r\n",touch_temp);
// // // 	if(touch_temp>=10)
// // // 	{
// // // 		
// // // 		touch_temp=0;
// // // 		touch_flag=0;
// // // 		touch2_flag=0;
// // // 		OSTmrStop(tmr4,OS_TMR_OPT_NONE,0,&err);//�ر������ʱ��4;
// // // 		OSTmrStart(tmr5,&err);					//��ʼ������5,���ڼ���ε��
// // // 	}
// //  }

// //  //��ʱ��5�Ļص�����	----����2
// // void  tmr5_callback(OS_TMR *ptmr,void *p_arg)
// // { 	
// // 	
// // 	u8  err; 
// // 	 
// // 	OS_CPU_SR  cpu_sr = 0;
// // 	char init_message[20] ;
// // 	int message_index ;
// // 	

// //  	if(OSTmrStateGet(tmr4,&err)==OS_TMR_STATE_STOPPED)//��ʱ��4û�п�����		
// // 	{ 
// // 		  	touch2_flag++; 
// //      	if(touch2_flag>=10)//�������� ��  10   
// // 		{
// //          	touch2_flag=0;
// // 			if(click_count>=3)
// // 					 click_count=3;
// // 			if(click_count==1){
// // 					/*************���һ�£���Ӧ�¶���*********************/
// // 					memset(init_message,0,20) ;
// // 					sprintf(init_message , "%s" , "led_weixiao");
// // 					OS_ENTER_CRITICAL();
// // 					message_index = Message_Queue_Available(mb);
// // 					OS_EXIT_CRITICAL();
// // 					strncpy(mb[message_index].buffer,init_message,strlen(init_message));
// // 					err = OSQPost(Str_Q_Client2led,&mb[message_index]) ;
// // 				
// // 					memset(init_message,0,20) ;
// // 					sprintf(init_message , "%s" , "sound_touched");
// // 					OS_ENTER_CRITICAL();
// // 					message_index = Message_Queue_Available(mb);
// // 					OS_EXIT_CRITICAL();
// // 					strncpy(mb[message_index].buffer,init_message,strlen(init_message));
// // 					err = OSQPost(Str_Q_Client2Sound,&mb[message_index]) ;
// // 			}
// // 			else if(click_count==3)
// // 			 {
// // 				printf("into standby\r\n");
// // //         ALARM0_CNT_Set(5);
// // // 				RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//ʹ��PWR����ʱ��
// // //         PWR_EnterSTANDBYMode();
// //        } 
// //          	printf("click_count=%d\r\n",click_count);
// // 			click_count =0;
// // 			OSTmrStop(tmr5,OS_TMR_OPT_NONE,0,&err);//�ر������ʱ��3; 
// // 		}
// // 	
// //   }
// // 		else
// // 		 {
// //       touch2_flag=0;
// // 		 OSTmrStop(tmr5,OS_TMR_OPT_NONE,0,&err);//�ر������ʱ��3; 
// //      } 
// //  }


//  
// void  tmr4_callback(OS_TMR *ptmr,void *p_arg)
// { 
// 	u8  err; 
// 	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==1&&touch_flag==0)
// 	{ click_count++;touch_flag=1;touch_temp=0;}
// 	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0&&touch_flag==1)
// 	{click_count++;touch_flag=0;touch_temp=0;}
// 	touch_temp++;
// 	printf(" click_count=%d \r\n",click_count);
// 	printf(" touch_temp=%d \r\n",touch_temp);
// 	if(touch_temp>=100)
// 	{
// 		touch_temp=0;
// 		touch_flag=0;
// 		touch2_flag=0;
// 		OSTmrStop(tmr4,OS_TMR_OPT_NONE,0,&err);//�ر������ʱ��4;
// 		OSTmrStart(tmr5,&err);					//��ʼ������5,���ڼ���ε��
// 	}
//  }

//  //��ʱ��5�Ļص�����	----����2
// void  tmr5_callback(OS_TMR *ptmr,void *p_arg)
// { 	
// 	
// 	u8  err; 
// 	 
// 	OS_CPU_SR  cpu_sr = 0;
// 	char init_message[20] ;
// 	int message_index ;
// 	
// 	touch2_flag++; 
//  	if(OSTmrStateGet(tmr4,&err)==OS_TMR_STATE_STOPPED)//��ʱ��4û�п�����		
// 	{ 
//      	if(touch2_flag>=10)
// 		{
//          	touch2_flag=0;
// 			if(click_count>=3)
// 					 click_count=3;
// 			if(click_count==1){
// 					/*************���һ�£���Ӧ�¶���*********************/
// 					memset(init_message,0,20) ;
// 					sprintf(init_message , "%s" , "led_weixiao");
// 					OS_ENTER_CRITICAL();
// 					message_index = Message_Queue_Available(mb);
// 					OS_EXIT_CRITICAL();
// 					strncpy(mb[message_index].buffer,init_message,strlen(init_message));
// 					err = OSQPost(Str_Q_Client2led,&mb[message_index]) ;
// 				
// 					memset(init_message,0,20) ;
// 					sprintf(init_message , "%s" , "sound_touched");
// 					OS_ENTER_CRITICAL();
// 					message_index = Message_Queue_Available(mb);
// 					OS_EXIT_CRITICAL();
// 					strncpy(mb[message_index].buffer,init_message,strlen(init_message));
// 					err = OSQPost(Str_Q_Client2Sound,&mb[message_index]) ;
// 			}
// 			else if(click_count==3)
// 			 {
// 				printf("into standby\r\n");
//          		ALARM0_CNT_Set(5);
//  				RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//ʹ��PWR����ʱ��
//         		PWR_EnterSTANDBYMode();
//        		} 
//          	printf("click_count=%d",click_count);
// 			click_count =0;
// 			OSTmrStop(tmr5,OS_TMR_OPT_NONE,0,&err);//�ر������ʱ��3; 
// 		}
// 	
//   }
// 		else
// 		 {
//       touch2_flag=0;
// 		 OSTmrStop(tmr5,OS_TMR_OPT_NONE,0,&err);//�ر������ʱ��3; 
//      } 
//  }

//  void  tmr6_callback(OS_TMR *ptmr,void *p_arg)
//  {  	u8 err;
// 	   int  light_value;
//  		OS_CPU_SR  cpu_sr = 0;
// 		char init_message[20] ;
// 		int message_index ;
// 		char *wakeupcommand="wakeupcmd:move_forward_20" ;
//     light_value=Get_Adc_Average(ADC_Channel_4,10);
// 	 printf("light_value=%d\n",light_value);
// 		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)==0)
// 		{
//        			light_flag++;
//      	}
// 			else
// 			{
//          light_high_flag++;

//       }
// 		if(light_value<=250)	
// 		{  
// 			  light_flag=0; 
// 			  light_flag1++;//5s����
// 			 if(light_flag1>20)
// 				{
// 						light_flag1=0;
// 								printf("light is  ok!!\r\n");
// 						/*************ǿ������һ�£���Ӧ�¶���*********************/
// 						memset(init_message,0,20) ;
// 						sprintf(init_message , "%s" , "led_huaji");
// 						OS_ENTER_CRITICAL();
// 						message_index = Message_Queue_Available(mb);
// 						OS_EXIT_CRITICAL();
// 						strncpy(mb[message_index].buffer,init_message,strlen(init_message));
// 						err = OSQPost(Str_Q_Client2led,&mb[message_index]) ;
// 						
// 						memset(init_message,0,20) ;
// 						sprintf(init_message , "%s" , "sound_light");
// 						OS_ENTER_CRITICAL();
// 						message_index = Message_Queue_Available(mb);
// 						OS_EXIT_CRITICAL();
// 						strncpy(mb[message_index].buffer,init_message,strlen(init_message));
// 						err = OSQPost(Str_Q_Client2Sound,&mb[message_index]) ;


// 						OS_ENTER_CRITICAL();
// 						message_index = Message_Queue_Available(mb);
// 						OS_EXIT_CRITICAL();
// 						strncpy(mb[message_index].buffer,wakeupcommand,strlen(wakeupcommand));
// 						err = OSQPost(Str_Q_Client2Motor,&mb[message_index]) ;
// 						OSTmrStop(tmr6,OS_TMR_OPT_NONE,0,&err); 
// 				}
//      }
// 		else if(light_high_flag>10)//û�� ���� 1s�رռ�����
// 		{light_high_flag=0; 	OSTmrStop(tmr6,OS_TMR_OPT_NONE,0,&err); }	
//     else if (light_flag>50)//��û�дﵽԤ��ֵ  5s�ر�
// 		{ light_flag=0;	OSTmrStop(tmr6,OS_TMR_OPT_NONE,0,&err); }			
//  }
// 			
// 	/*********************************************************************************************************
// 				END FILE
// 	*********************************************************************************************************/
