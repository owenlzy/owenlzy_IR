#include "gpio.h"

//ֱ����� ��ͨGPIO�ڳ�ʼ�� PB0 PB6
void gpio_motor_init(void){
	GPIO_InitTypeDef GPIO_InitStructure ;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOG, ENABLE);
	
	//ֱ�����ٵ��
	//PB6 ���������������� 
	//PB8 �����Ҳ����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_8;					 				
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //��Ϊ�������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
	GPIO_Init(GPIOB, &GPIO_InitStructure);	                
	//42�������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_13|GPIO_Pin_12|GPIO_Pin_11;					 				
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //��Ϊ�������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
	GPIO_Init(GPIOE, &GPIO_InitStructure);	 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;					 				
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //��Ϊ�������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
	GPIO_Init(GPIOF, &GPIO_InitStructure);	
	
	
	//ʹ��
	//PG.1��13Ϊ����оƬʹ�ܣ�ͳһ�ø�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_1|GPIO_Pin_7|GPIO_Pin_8;					 				
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //��Ϊ�������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
	GPIO_Init(GPIOG, &GPIO_InitStructure);	
	GPIO_SetBits(GPIOG,GPIO_Pin_7);
	GPIO_SetBits(GPIOG,GPIO_Pin_8);
	GPIO_SetBits(GPIOG,GPIO_Pin_11);
	GPIO_SetBits(GPIOG,GPIO_Pin_12);
	GPIO_SetBits(GPIOG,GPIO_Pin_13);
	GPIO_SetBits(GPIOG,GPIO_Pin_1);

}

//������ ��ͨGPIO�ڳ�ʼ��  PB0 PB1
void gpio_ultrasonic_init(void){
	
// 	GPIO_InitTypeDef GPIO_InitStructure ;

// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;					 //PB10��TRIG
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //��Ϊ�������ģʽ
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);	                 

// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				     //PB1��ECH0
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		 //��Ϊ����
// 	GPIO_Init(GPIOB,&GPIO_InitStructure);					
}

//LED  WS2812S
void gpio_led_ws2812s_init(void){

	GPIO_InitTypeDef GPIO_InitStructure ;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;					 //PB10��TRIG
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //��Ϊ�������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);
	OSTimeDlyHMSM(0, 0, 0, 50);
}

//LED 
void gpio_led_init(void){
	GPIO_InitTypeDef  GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13|GPIO_Pin_12|GPIO_Pin_11|GPIO_Pin_10|GPIO_Pin_9;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(GPIOD, &GPIO_InitStructure);					 
  GPIO_ResetBits(GPIOD,GPIO_Pin_9);						 
}




//sound ��ͨGPIO�ڳ�ʼ��  PA1
void gpio_sound_init(void){

// 	GPIO_InitTypeDef GPIO_InitStructure ;

// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
//    	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;              //�����������
//    	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;            //��תƵ��50MHz
//    	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//���崫����  PE9
void gpio_human_sensor_init(void){
 	 GPIO_InitTypeDef GPIO_InitStructure ;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;					 //PA6 TIM_CH1
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		 //��Ϊ�������ģʽ
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
 	GPIO_Init(GPIOE, &GPIO_InitStructure);	                    //��ʼ������GPIO 
}

//����ģ��  PB13 PB13
void gpio_voice_init(void)
{

//     GPIO_InitTypeDef GPIO_InitStructure;
//        
//     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
//        
//     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_13 ;//��y??
//     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//?��?��
//     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//1�����?�꨺?
//     GPIO_Init(GPIOB,&GPIO_InitStructure);
//     GPIO_SetBits(GPIOB,GPIO_Pin_15 | GPIO_Pin_13);
}

//PB8
void gpio_tracker_init(void){
//  	 GPIO_InitTypeDef GPIO_InitStructure ;

//  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

//  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;					 //PA6 TIM_CH1
//  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		 //��Ϊ�������ģʽ
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
//  	GPIO_Init(GPIOB, &GPIO_InitStructure);	                    //��ʼ������GPIO 
}
//*****PB 0 1 4 5 6 8 13 15,pA1*********
void gpio_ledarry_init(void){

// 	GPIO_InitTypeDef GPIO_InitStructure ;

// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;					 			//PB10��TRIG
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		    //��Ϊ�������ģʽ
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);

// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;					 			//PB10��TRIG
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		    //��Ϊ�������ģʽ
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);
// 	GPIO_ResetBits(GPIOB,GPIO_Pin_9);//���0���رշ��������

// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;					 			//PB10��TRIG
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		    //��Ϊ�������ģʽ
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
//arry PB 7 9 12

//���ߴ��е���
void gpio_LightSensor_init(void){

	GPIO_InitTypeDef GPIO_InitStructure ;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);



	//PC4 ������ߵ����Light_ADC
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;					 //PA6 TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
	GPIO_Init(GPIOC, &GPIO_InitStructure);	                    //��ʼ������GPIO 

}



/******************��ʼ��WIFI״̬���ܽ�**********************/
/*
						PE.10 �����������WIFI��GPIO_0������⵽PE.10Ϊ�͵�ƽʱ��WIFIģ�鴦��ROM��д״̬����ʱ�Ͽ�STM32��WIFI�Ĵ�������
						xxx   �����������WIFI��GPIO_0������⵽PE.10Ϊ�͵�ƽʱ��WIFIģ�鴦��LUA��д״̬����ʱ�Ͽ�STM32��WIFI�Ĵ�������
						����⵽�������ܽŶ�Ϊ��ʱ����ʱWIFI���ڹ���״̬������STM��WIFI�Ĵ�������
*/

void gpio_WIFIModeSwitch_init(void){
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG,ENABLE);
	//WIFI I/O 0
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	//����Ϊ��������
 	GPIO_Init(GPIOE, &GPIO_InitStructure);
	//Lua Check
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	//����Ϊ��������
 	GPIO_Init(GPIOG, &GPIO_InitStructure);
}

/**********************************************************************************************************
			Ѷ��ģ��ܽų�ʼ��
***********************************************************************************************************/
void gpio_XF_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//ʹ��GPIOC
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;             //Audio_Reset
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;							//Audio_RDY
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		  // �������� //GPIO_Mode_IPU  ��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
}
/************************************************************************************************************
	�鿴Ѷ��ģ���Ƿ����ڹ���
	����0��Ѷ��ģ���Ѿ������һ�η����ϳ�
	����1��Ѷ��ģ�����ںϳ�
*************************************************************************************************************/

int GetXFStatus(void)
{
	return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13);            // �͵�ƽready
}