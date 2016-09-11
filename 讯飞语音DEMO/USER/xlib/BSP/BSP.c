/**
  ******************************************************************************
  * @�ļ���    
  * @���� 
  * @�汾
  * @����
  * @��Ҫ
  ******************************************************************************
  * @ע������
  *
  *
  *
  ******************************************************************************
  */  

/* ͷ�ļ�				--------------------------------------------------------------*/
#include "BSP.h"

/* ��������     --------------------------------------------------------------*/
/* �궨�� 	    --------------------------------------------------------------*/
/* ���� 	    	--------------------------------------------------------------*/
/* ��������     --------------------------------------------------------------*/


/**
  * @���� 			
  * @����
  * @����ֵ
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
		while(USART1_putchar( buf[i]) == false); /* ��������������ȴ� */
	}

	return n;	
}

/**
  * @����				LED GPIO ��ʼ��
  * @����
  * @����ֵ
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
  * @����				LED ��������
  * @����				num, 0: ȫ�����ã�1~4��1~4 LED
  * @����ֵ
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
  * @���� 			ϵͳ�жϹ���, NVIC ֻ������Ȩģʽ�����ã��߳�ģʽ���ʷǷ��������� Hardfault.
  * @����
  * @����ֵ
  */
void NVIC_Configuration(void)
{ 
  /* Configure the NVIC Preemption Priority Bits */  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); /*RTX �����������ڷ���Ȩģʽ���У���ʱ��������NVIC �� HardFault*/ 

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
  * @���� 			Ӳ����ʼ��
  * @����
  * @����ֵ
  */
void BSP_Init(void)
{	
	Seril_InitTypeDef SerialInitStruct;
	
	/*��ֹJTAG, ��ֹ��flash, SWJ��ֹ�����ID�ȣ�����ʹ�ܸ��ù���ʱ�� */
// 	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO , ENABLE); 	
// 	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);										/*DI3 DI4 ��JTAG��ͻ���ر�JTAG���Թ���*/

	/* LED �Ƴ�ʼ�� */
	LED_Configuration();
	
	/*���ڳ�ʼ��*/
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
 * �ļ�ĩβ
 ******************************************************************************
 */

