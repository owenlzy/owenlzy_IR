/**
  ******************************************************************************
  * @文件名    
  * @作者 
  * @版本
  * @日期
  * @概要
  ******************************************************************************
  * @注意事项
  *
  *
  *
  ******************************************************************************
  */  

/* 头文件				--------------------------------------------------------------*/
#include "BSP.h"

/* 数据类型     --------------------------------------------------------------*/
/* 宏定义 	    --------------------------------------------------------------*/
/* 变量 	    	--------------------------------------------------------------*/
/* 函数声明     --------------------------------------------------------------*/


/**
  * @功能 			
  * @参数
  * @返回值
  */
int32_t Printf(char const *const format, ...)
{
	static char buf[83];
	int i,n;
	__va_list  arglist;

	va_start(arglist, format);
	n = vsnprintf(buf, sizeof(buf), format, arglist);
	va_end(arglist);
	
	for(i=0; i<n; i++){
		while(USART1_putchar( buf[i]) == false); /* 若缓冲区已满则等待 */
	}

	return n;	
}

/**
  * @功能				LED GPIO 初始化
  * @参数
  * @返回值
  */
void LED_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd( LEDPortRCC , ENABLE); 	
	
  GPIO_InitStructure.GPIO_Pin =  LED1 | LED2 | LED3 | LED4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(LEDPort, &GPIO_InitStructure);
}

/**
  * @功能				LED 开关设置
  * @参数				num, 0: 全部设置，1~4：1~4 LED
  * @返回值
  */
void LED_Set(uint8_t num, uint8_t state)
{
	switch(num){
		case 0: state ? GPIO_SetBits(LEDPort , LED1 | LED2 | LED3 | LED4) : \
										GPIO_ResetBits(LEDPort , LED1 | LED2 | LED3 | LED4);
						break;
		case 1: state ? GPIO_SetBits(LEDPort , LED1) : GPIO_ResetBits(LEDPort , LED1);
						break;		
		case 2: state ? GPIO_SetBits(LEDPort , LED2) : GPIO_ResetBits(LEDPort , LED2);
						break;
		case 3: state ? GPIO_SetBits(LEDPort , LED3) : GPIO_ResetBits(LEDPort , LED3);
						break;
		case 4: state ? GPIO_SetBits(LEDPort , LED4) : GPIO_ResetBits(LEDPort , LED4);
						break;
	}
}

/**
  * @功能 			系统中断管理, NVIC 只能在特权模式下配置，线程模式访问非法，将导致 Hardfault.
  * @参数
  * @返回值
  */
void NVIC_Configuration(void)
{ 
  /* Configure the NVIC Preemption Priority Bits */  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); /*RTX 启动后配置在非特权模式运行，此时重新配置NVIC 会 HardFault*/ 

// 	#ifdef  VECT_TAB_RAM  
// 	  /* Set the Vector Table base location at 0x20000000 */ 
// 	  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
// 	#else  /* VECT_TAB_FLASH  */
// 	  /* Set the Vector Table base location at 0x08000000 */ 
// 	  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
// 	#endif
	
#ifdef  _USART1_ENABLE
	USART_NVICConfig(USART1, 0, 1, ENABLE);
#endif
	
#ifdef  _USART2_ENABLE
	USART_NVICConfig(USART2, 0, 2, ENABLE);
#endif
	
#ifdef  _USART3_ENABLE
	USART_NVICConfig(USART3, 0, 3, ENABLE);
#endif	
	
#ifdef  _UART4_ENABLE
	USART_NVICConfig(UART4, 0, 4, ENABLE);
#endif
	
#ifdef  _UART5_ENABLE
	USART_NVICConfig(UART5, 0, 5, ENABLE);
#endif

}

/**
  * @功能 			硬件初始化
  * @参数
  * @返回值
  */
void BSP_Init(void)
{	
	Seril_InitTypeDef SerialInitStruct;
	
	/*禁止JTAG, 禁止读flash, SWJ禁止，检查ID等，需先使能复用功能时钟 */
// 	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO , ENABLE); 	
// 	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);										/*DI3 DI4 与JTAG冲突，关闭JTAG调试功能*/

	/* LED 灯初始化 */
	LED_Configuration();
	
	/*串口初始化*/
  SerialInitStruct.USART_BaudRate 	= 115200;
  SerialInitStruct.USART_WordLength = USART_WordLength_8b;
  SerialInitStruct.USART_StopBits 	= USART_StopBits_1;
  SerialInitStruct.USART_Parity 		= USART_Parity_No;
  SerialInitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  SerialInitStruct.USART_Mode 			= USART_Mode_Rx | USART_Mode_Tx;
	
#ifdef _USART1_ENABLE	
	SerialInit(USART1, &SerialInitStruct);
#endif	
#ifdef _USART2_ENABLE	
	SerialInit(USART2, &SerialInitStruct);
#endif	
#ifdef _USART3_ENABLE	
	SerialInit(USART3, &SerialInitStruct);
#endif	
#ifdef _UART4_ENABLE	
	SerialInit(UART4, &SerialInitStruct);
#endif	
#ifdef _UART5_ENABLE	
	SerialInit(UART5, &SerialInitStruct);
#endif	

	NVIC_Configuration();
}

/**
  * @}
  */

/**
 ******************************************************************************
 * 文件末尾
 ******************************************************************************
 */

