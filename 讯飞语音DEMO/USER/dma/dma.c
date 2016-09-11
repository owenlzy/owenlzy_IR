#include "stm32f10x_it.h"
#include "stm32f10x_flash.h"
#include "USART.h"
#include "stdio.h"
#include "dma.h"
#include "includes.h"
//#include "lwip.h"
unsigned char  memdst[15];

/***配置内存到内存的dma方式****/
void M2M1_Configuration(void)
{
	char memsrc[]="Firebull";
	DMA_InitTypeDef DMA_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输	
  DMA_DeInit(DMA1_Channel4);   //将DMA的通道1寄存器重设为缺省值
	DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)memdst;         //外设地址
	DMA_InitStruct.DMA_MemoryBaseAddr = (u32)memsrc;                      //内存地址
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;                     //外设为目的地址
	DMA_InitStruct.DMA_BufferSize = sizeof(memsrc)-1;                     //数据长度
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Enable;       //外设地址自增失能
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;                //内存地址自增失能
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设接收字节长度
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;        //内存地址字节长度
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;                        //DMA循环采集模式
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;                    //通道优先级
	DMA_InitStruct.DMA_M2M = DMA_M2M_Enable;                           //失能内存到内存
	DMA_Init(DMA1_Channel4,&DMA_InitStruct);
	DMA_Cmd(DMA1_Channel4,ENABLE);

}
/***配置内存到外设的dma方式*****/
void M2M2_Configuration(void)
{
	u32 memsrc=0x0001;
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStruct;
	
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;          //推挽输出
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         //翻转频率50MHz
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
//	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
	
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);                        //DMA（驱动发光二极管）
	DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)(&GPIOA->ODR);         //外设地址（驱动发光二极管）
	DMA_InitStruct.DMA_MemoryBaseAddr = (u32)memsrc;                      //内存地址
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;                     //外设为目的地址
	DMA_InitStruct.DMA_BufferSize =100 ;                     //数据长度
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;       //外设地址自增失能(驱动发光二极管)
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;                //内存地址自增失能
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设接收字节长度
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;        //内存地址字节长度
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;                        //DMA循环采集模式
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;                    //通道优先级
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;                           //失能内存到内存（驱动发光二极管）
	DMA_Init(DMA1_Channel4,&DMA_InitStruct);
	DMA_Cmd(DMA1_Channel4,ENABLE);

}
/**********这段代码是ADC_DMA配置的代码******************/
u32 AD_Value;
void ADC_DMA_Configuration(void)
{
	  DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr =(u32)&ADC2->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&AD_Value;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 40;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    DMA_Cmd(DMA1_Channel1, ENABLE);
}
/*********这段代码是ADC配置的代码**************/
void ADC2_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2 | RCC_APB2Periph_GPIOC, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);   
	ADC_DeInit(ADC2);
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	                     /* 独立模式 */
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;		 	                 /* 单通道转换 */
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	                     /* 连续转换 */
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;      /* 转换不受外界决定 */
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;		             /* 右对齐 */
	ADC_InitStructure.ADC_NbrOfChannel = 1;					                 /* 扫描通道数 */
	ADC_Init(ADC2, &ADC_InitStructure);
	ADC_RegularChannelConfig(ADC2, ADC_Channel_10, 1, ADC_SampleTime_71Cycles5);	
	ADC_Cmd(ADC2, ENABLE);                                                   /* Enable ADC1 */                      
	ADC_SoftwareStartConvCmd(ADC2,ENABLE);                                   /* 使能转换开始 */
	ADC_DMACmd(ADC2, ENABLE);
	ADC_ResetCalibration(ADC2);	
	while(ADC_GetResetCalibrationStatus(ADC2));	
	ADC_StartCalibration(ADC2);	
	while(ADC_GetCalibrationStatus(ADC2));	
}

/***系统时钟初始化*****/
void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus;                           //定义枚举类型变量HSEStartUpStatus
	RCC_DeInit();                                           //复位系统时钟
	RCC_HSEConfig(RCC_HSE_ON);                              //开启HSE
	HSEStartUpStatus = RCC_WaitForHSEStartUp();             //等待HSE起振
	if(HSEStartUpStatus == SUCCESS)
	{
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		FLASH_SetLatency(FLASH_Latency_2);
		RCC_HCLKConfig(RCC_SYSCLK_Div1);                      //选择HCLK时钟源为SYSCLK1分频
		RCC_PCLK2Config(RCC_HCLK_Div1);                       //选择PCLK2的时钟源为HCLK1分频
		RCC_PCLK1Config(RCC_HCLK_Div2);                       //选择PCLK1的时钟源为HCLK2分频
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);	//选择PLL时钟源为HSE1分频，9倍频
		RCC_PLLCmd(ENABLE);                                   //使能PLL
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);   //等待PLL输出稳定
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);            //系统时钟为PLL
		while(RCC_GetSYSCLKSource() != 0x08);                 //等待PLL为系统时钟源
	}
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC
                           |RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOB
                           , ENABLE );
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3 | RCC_APB1Periph_SPI2                     
                           , ENABLE );
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);        //使能DMA1时钟
}

/***初始化所有外设*****/
void Init_All_Periph(void)
{
	RCC_Configuration();	                                   //系统时钟配置
	//InitDis();                                               //LCD初始化显示
	//LED_Configuration();  	                                 //LED端口配置
	M2M1_Configuration();                                    //dma开始工作
}