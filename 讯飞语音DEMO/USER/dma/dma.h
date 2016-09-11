#ifndef __DMA_H
#define __DMA_H

//extern void USART1_SendByte(u16 dat);
//extern void USART1Write(u8* data,u16 len);

//void USART1_Configuration(void);

//extern void USART2_SendByte(u16 dat);
//extern void USART2Write(u8* data,u16 len);

//void USART2_Configuration(void);
//void USART2_SendString(uint8_t *ch);
//#define DIR485_H  GPIOC->BSRR=1<<1
//#define DIR485_L  GPIOC->BRR=1<<1 
extern void M2M1_Configuration(void);
extern void M2M2_Configuration(void);
extern void Init_All_Periph(void) ;//配置所有外设
extern unsigned char  memdst[15];
extern void ADC_DMA_Configuration(void);
extern u32 AD_Value;
extern unsigned char AD_Value_show[20];
extern void ADC2_Configuration(void);
//extern void SD_Init(void);
//extern void USART1_Configuration(void);
#endif