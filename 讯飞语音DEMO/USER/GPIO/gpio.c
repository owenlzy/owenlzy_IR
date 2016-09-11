#include "gpio.h"

//直流电机 普通GPIO口初始化 PB0 PB6
void gpio_motor_init(void){
	GPIO_InitTypeDef GPIO_InitStructure ;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOG, ENABLE);
	
	//直流减速电机
	//PB6 接入左测电机的输入口 
	//PB8 接入右侧电机的输入口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_8;					 				
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //设为推挽输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
	GPIO_Init(GPIOB, &GPIO_InitStructure);	                
	//42步进电机
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_13|GPIO_Pin_12|GPIO_Pin_11;					 				
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //设为推挽输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
	GPIO_Init(GPIOE, &GPIO_InitStructure);	 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;					 				
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //设为推挽输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
	GPIO_Init(GPIOF, &GPIO_InitStructure);	
	
	
	//使能
	//PG.1、13为驱动芯片使能，统一置高
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_1|GPIO_Pin_7|GPIO_Pin_8;					 				
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //设为推挽输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
	GPIO_Init(GPIOG, &GPIO_InitStructure);	
	GPIO_SetBits(GPIOG,GPIO_Pin_7);
	GPIO_SetBits(GPIOG,GPIO_Pin_8);
	GPIO_SetBits(GPIOG,GPIO_Pin_11);
	GPIO_SetBits(GPIOG,GPIO_Pin_12);
	GPIO_SetBits(GPIOG,GPIO_Pin_13);
	GPIO_SetBits(GPIOG,GPIO_Pin_1);

}

//超声波 普通GPIO口初始化  PB0 PB1
void gpio_ultrasonic_init(void){
	
// 	GPIO_InitTypeDef GPIO_InitStructure ;

// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;					 //PB10接TRIG
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //设为推挽输出模式
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);	                 

// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				     //PB1接ECH0
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		 //设为输入
// 	GPIO_Init(GPIOB,&GPIO_InitStructure);					
}

//LED  WS2812S
void gpio_led_ws2812s_init(void){

	GPIO_InitTypeDef GPIO_InitStructure ;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;					 //PB10接TRIG
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //设为推挽输出模式
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




//sound 普通GPIO口初始化  PA1
void gpio_sound_init(void){

// 	GPIO_InitTypeDef GPIO_InitStructure ;

// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
//    	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;              //复用推挽输出
//    	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;            //翻转频率50MHz
//    	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//人体传感器  PE9
void gpio_human_sensor_init(void){
 	 GPIO_InitTypeDef GPIO_InitStructure ;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;					 //PA6 TIM_CH1
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		 //设为推挽输出模式
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
 	GPIO_Init(GPIOE, &GPIO_InitStructure);	                    //初始化外设GPIO 
}

//发音模块  PB13 PB13
void gpio_voice_init(void)
{

//     GPIO_InitTypeDef GPIO_InitStructure;
//        
//     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
//        
//     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_13 ;//òy??
//     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//?ù?è
//     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//1¤×÷?￡ê?
//     GPIO_Init(GPIOB,&GPIO_InitStructure);
//     GPIO_SetBits(GPIOB,GPIO_Pin_15 | GPIO_Pin_13);
}

//PB8
void gpio_tracker_init(void){
//  	 GPIO_InitTypeDef GPIO_InitStructure ;

//  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

//  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;					 //PA6 TIM_CH1
//  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		 //设为推挽输出模式
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
//  	GPIO_Init(GPIOB, &GPIO_InitStructure);	                    //初始化外设GPIO 
}
//*****PB 0 1 4 5 6 8 13 15,pA1*********
void gpio_ledarry_init(void){

// 	GPIO_InitTypeDef GPIO_InitStructure ;

// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;					 			//PB10接TRIG
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		    //设为推挽输出模式
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);

// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;					 			//PB10接TRIG
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		    //设为推挽输出模式
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);
// 	GPIO_ResetBits(GPIOB,GPIO_Pin_9);//输出0，关闭蜂鸣器输出

// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;					 			//PB10接TRIG
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		    //设为推挽输出模式
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
//arry PB 7 9 12

//光线传感电阻
void gpio_LightSensor_init(void){

	GPIO_InitTypeDef GPIO_InitStructure ;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);



	//PC4 接入光线电阻的Light_ADC
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;					 //PA6 TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
	GPIO_Init(GPIOC, &GPIO_InitStructure);	                    //初始化外设GPIO 

}



/******************初始化WIFI状态检测管脚**********************/
/*
						PE.10 接上拉电阻和WIFI的GPIO_0，当检测到PE.10为低电平时，WIFI模块处于ROM烧写状态，此时断开STM32与WIFI的串口连接
						xxx   接上拉电阻和WIFI的GPIO_0，当检测到PE.10为低电平时，WIFI模块处于LUA烧写状态，此时断开STM32与WIFI的串口连接
						当检测到这两个管脚都为高时，此时WIFI处于工作状态，连接STM与WIFI的串口连接
*/

void gpio_WIFIModeSwitch_init(void){
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG,ENABLE);
	//WIFI I/O 0
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	//设置为上拉输入
 	GPIO_Init(GPIOE, &GPIO_InitStructure);
	//Lua Check
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	//设置为上拉输入
 	GPIO_Init(GPIOG, &GPIO_InitStructure);
}

/**********************************************************************************************************
			讯飞模块管脚初始化
***********************************************************************************************************/
void gpio_XF_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//使能GPIOC
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;             //Audio_Reset
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;							//Audio_RDY
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		  // 浮空输入 //GPIO_Mode_IPU  上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
}
/************************************************************************************************************
	查看讯飞模块是否正在工作
	返回0：讯飞模块已经完成上一次发声合成
	返回1：讯飞模块正在合成
*************************************************************************************************************/

int GetXFStatus(void)
{
	return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13);            // 低电平ready
}