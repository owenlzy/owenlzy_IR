#include "stm32f10x_it.h"
#include "stm32f10x_flash.h"
#include "USART.h"
#include "stdio.h"
#include "dma.h"
#include "includes.h"
//#include "lwip.h"
unsigned char  memdst[15];

/***�����ڴ浽�ڴ��dma��ʽ****/
void M2M1_Configuration(void)
{
	char memsrc[]="Firebull";
	DMA_InitTypeDef DMA_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����	
  DMA_DeInit(DMA1_Channel4);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
	DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)memdst;         //�����ַ
	DMA_InitStruct.DMA_MemoryBaseAddr = (u32)memsrc;                      //�ڴ��ַ
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;                     //����ΪĿ�ĵ�ַ
	DMA_InitStruct.DMA_BufferSize = sizeof(memsrc)-1;                     //���ݳ���
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Enable;       //�����ַ����ʧ��
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;                //�ڴ��ַ����ʧ��
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//��������ֽڳ���
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;        //�ڴ��ַ�ֽڳ���
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;                        //DMAѭ���ɼ�ģʽ
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;                    //ͨ�����ȼ�
	DMA_InitStruct.DMA_M2M = DMA_M2M_Enable;                           //ʧ���ڴ浽�ڴ�
	DMA_Init(DMA1_Channel4,&DMA_InitStruct);
	DMA_Cmd(DMA1_Channel4,ENABLE);

}
/***�����ڴ浽�����dma��ʽ*****/
void M2M2_Configuration(void)
{
	u32 memsrc=0x0001;
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStruct;
	
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;          //�������
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         //��תƵ��50MHz
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
//	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
	
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);                        //DMA��������������ܣ�
	DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)(&GPIOA->ODR);         //�����ַ��������������ܣ�
	DMA_InitStruct.DMA_MemoryBaseAddr = (u32)memsrc;                      //�ڴ��ַ
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;                     //����ΪĿ�ĵ�ַ
	DMA_InitStruct.DMA_BufferSize =100 ;                     //���ݳ���
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;       //�����ַ����ʧ��(�������������)
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;                //�ڴ��ַ����ʧ��
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//��������ֽڳ���
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;        //�ڴ��ַ�ֽڳ���
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;                        //DMAѭ���ɼ�ģʽ
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;                    //ͨ�����ȼ�
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;                           //ʧ���ڴ浽�ڴ棨������������ܣ�
	DMA_Init(DMA1_Channel4,&DMA_InitStruct);
	DMA_Cmd(DMA1_Channel4,ENABLE);

}
/**********��δ�����ADC_DMA���õĴ���******************/
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
/*********��δ�����ADC���õĴ���**************/
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
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	                     /* ����ģʽ */
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;		 	                 /* ��ͨ��ת�� */
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	                     /* ����ת�� */
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;      /* ת������������ */
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;		             /* �Ҷ��� */
	ADC_InitStructure.ADC_NbrOfChannel = 1;					                 /* ɨ��ͨ���� */
	ADC_Init(ADC2, &ADC_InitStructure);
	ADC_RegularChannelConfig(ADC2, ADC_Channel_10, 1, ADC_SampleTime_71Cycles5);	
	ADC_Cmd(ADC2, ENABLE);                                                   /* Enable ADC1 */                      
	ADC_SoftwareStartConvCmd(ADC2,ENABLE);                                   /* ʹ��ת����ʼ */
	ADC_DMACmd(ADC2, ENABLE);
	ADC_ResetCalibration(ADC2);	
	while(ADC_GetResetCalibrationStatus(ADC2));	
	ADC_StartCalibration(ADC2);	
	while(ADC_GetCalibrationStatus(ADC2));	
}

/***ϵͳʱ�ӳ�ʼ��*****/
void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus;                           //����ö�����ͱ���HSEStartUpStatus
	RCC_DeInit();                                           //��λϵͳʱ��
	RCC_HSEConfig(RCC_HSE_ON);                              //����HSE
	HSEStartUpStatus = RCC_WaitForHSEStartUp();             //�ȴ�HSE����
	if(HSEStartUpStatus == SUCCESS)
	{
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		FLASH_SetLatency(FLASH_Latency_2);
		RCC_HCLKConfig(RCC_SYSCLK_Div1);                      //ѡ��HCLKʱ��ԴΪSYSCLK1��Ƶ
		RCC_PCLK2Config(RCC_HCLK_Div1);                       //ѡ��PCLK2��ʱ��ԴΪHCLK1��Ƶ
		RCC_PCLK1Config(RCC_HCLK_Div2);                       //ѡ��PCLK1��ʱ��ԴΪHCLK2��Ƶ
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);	//ѡ��PLLʱ��ԴΪHSE1��Ƶ��9��Ƶ
		RCC_PLLCmd(ENABLE);                                   //ʹ��PLL
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);   //�ȴ�PLL����ȶ�
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);            //ϵͳʱ��ΪPLL
		while(RCC_GetSYSCLKSource() != 0x08);                 //�ȴ�PLLΪϵͳʱ��Դ
	}
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC
                           |RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOB
                           , ENABLE );
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3 | RCC_APB1Periph_SPI2                     
                           , ENABLE );
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);        //ʹ��DMA1ʱ��
}

/***��ʼ����������*****/
void Init_All_Periph(void)
{
	RCC_Configuration();	                                   //ϵͳʱ������
	//InitDis();                                               //LCD��ʼ����ʾ
	//LED_Configuration();  	                                 //LED�˿�����
	M2M1_Configuration();                                    //dma��ʼ����
}