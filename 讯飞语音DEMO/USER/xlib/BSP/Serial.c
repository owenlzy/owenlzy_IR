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
#include "Serial.h"

/* ��������     --------------------------------------------------------------*/
struct buf_st  {
  uint8_t in;
  uint8_t out;
  uint8_t buf [256];
};

/* �궨�� 			--------------------------------------------------------------*/
/* ���� 				--------------------------------------------------------------*/

#ifdef  _USART1_ENABLE
static struct 	buf_st USART1_rbuf;
static struct 	buf_st USART1_tbuf;
static bool  		USART1_tx_active;
#endif

#ifdef  _USART2_ENABLE
static struct 	buf_st USART2_rbuf;
static struct 	buf_st USART2_tbuf;
static bool  		USART2_tx_active;
#endif

#ifdef  _USART3_ENABLE
static struct 	buf_st USART3_rbuf;
static struct 	buf_st USART3_tbuf;
static bool  		USART3_tx_active;
#endif

#ifdef  _UART4_ENABLE
static struct 	buf_st UART4_rbuf;
static struct 	buf_st UART4_tbuf;
static bool  		UART4_tx_active;
#endif

#ifdef  _UART5_ENABLE
static struct 	buf_st UART5_rbuf;
static struct 	buf_st UART5_tbuf;
static bool  		UART5_tx_active;
#endif

/* ��������     --------------------------------------------------------------*/
#ifdef  _USART1_ENABLE

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define GETCHAR_PROTOTYPE int __io_getchar(int ch)
#else

#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)

#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __GNUC__ */

/**
  * @����
  * @����
  * @����ֵ
 
PUTCHAR_PROTOTYPE
{
	while(USART1_putchar(ch) == false)  __nop (); 	
  return ch;
}
*/ 

/**
  * @����
  * @����
  * @����ֵ
  */
GETCHAR_PROTOTYPE
{
  /* �Ӵ��ڶ�ȡһ���ֽ� */
  volatile struct buf_st *p = &USART1_rbuf;
	
	while((p->in) == (p->out)) __nop ();	
  return (p->buf[p->out++]);
}


static int32_t USART1_HalInit(Seril_InitTypeDef* Serialx_InitStruct);

#endif

#ifdef  _USART2_ENABLE
static int32_t USART2_HalInit(Seril_InitTypeDef* Serialx_InitStruct);
#endif

#ifdef  _USART3_ENABLE
static int32_t USART3_HalInit(Seril_InitTypeDef* Serialx_InitStruct);
#endif

#ifdef  _UART4_ENABLE
static int32_t UART4_HalInit(Seril_InitTypeDef* Serialx_InitStruct);
#endif

#ifdef  _UART5_ENABLE
static int32_t UART5_HalInit(Seril_InitTypeDef* Serialx_InitStruct);
#endif

/**
  * @����
  * @����
  * @����ֵ
  */
#ifdef  _USART1_ENABLE

static int32_t USART1_HalInit(Seril_InitTypeDef* Serialx_InitStruct)
{
   GPIO_InitTypeDef 		GPIO_InitStructure;

  /* ʹ������ʱ�� 			*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

	/* ���� GPIO ���Ź��� */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;/* ���츴����� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	  
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; /* ���ջ��������� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* USART1 ��λ	*/
	USART_Cmd(USART1, DISABLE);
	USART_DeInit(USART1);
	
	/* ���ò����ʵ� 		*/
	USART_Init(USART1, Serialx_InitStruct);
	
	/* �����ж� 				*/	
// 	USART_ITConfig(USART1, USART_IT_TXE , ENABLE); /* ���ͻ�����ж� */
	USART_ITConfig(USART1, USART_IT_RXNE , ENABLE);
	
	/* ʹ������ 				*/
  USART_Cmd(USART1, ENABLE);
	
	return 0;
}

/**
  * @����
  * @����
  * @����ֵ
  */
bool USART1_putchar (uint8_t c) 
{
  struct buf_st *p = &USART1_tbuf;

  /* дһ���ֽڵ�����1 */
  if ((uint8_t)(p->in + 1) == p->out) {
    /* ���ڷ��ͻ������� */
    return (false);
  }
	
  NVIC_DisableIRQ(USART1_IRQn);			/*��ֹ����1�ж�*/
  __nop ();
  if (USART1_tx_active == false) {
    /* �������δ���ͣ���ֱ��д�봮�����ݼĴ��� */
    USART1->DR   = (uint8_t)c;
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
    USART1_tx_active = true;
  }
  else {
    /* ���һ���ֽڵ����ͻ��� */
    p->buf [p->in++] = c;
  }
  NVIC_EnableIRQ(USART1_IRQn);			/*ʹ�ܴ���1�ж�*/
	
  return (true);	
}

/**
  * @����
  * @����
  * @����ֵ
  */
int USART1_getchar (void) {
  /* �Ӵ��ڶ�ȡһ���ֽ� */
  struct buf_st *p = &USART1_rbuf;

  if (p->in == p->out) {
    /* ���ڻ���Ϊ�� */
    return (-1); /* EOF */
  }
		
  return (p->buf[p->out++]);
}

/**
  * @����
  * @����
  * @����ֵ
  */
bool USART1_Get_tx_active (void) {
  /* ��鴮�ڷ����Ƿ���Ч         */
  /* �����ͻ���Ϊ��ʱ��tx_active = false */
  return (USART1_tx_active);
}

/**
  * @����			���ڷ��ͽ����жϽӿ� 
  * @����
  * @����ֵ
  
void USART1_IRQHandler(void)
{
  struct buf_st *p;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
		p = &USART1_rbuf;
    if ((uint8_t)(p->in + 1) != p->out) {
      p->buf [p->in++] = (uint8_t)USART1->DR;
    }		
	}
	
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET){
    p = &USART1_tbuf;
    if (p->in != p->out) {
      USART1->DR = p->buf [p->out++];
    } else {
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
      USART1_tx_active = false;
    }
	}
}
*/

#endif
/**
  * @����
  * @����
  * @����ֵ
  */
#ifdef  _USART2_ENABLE

static int32_t USART2_HalInit(Seril_InitTypeDef* Serialx_InitStruct)
{
  GPIO_InitTypeDef 		GPIO_InitStructure;

  /* ʹ������ʱ�� 			*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* ���� GPIO ���Ź��� */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;/* ���츴����� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	  
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; /* ���ջ��������� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* USART2 ��λ 		*/
	USART_Cmd(USART2, DISABLE);
	USART_DeInit(USART2);
	
	/* ��ʼ�������ʵ� 		*/
	USART_Init(USART2, Serialx_InitStruct);
	
	/* �����ж� 					*/
// 	USART_ITConfig(USART2, USART_IT_TXE , ENABLE);
	USART_ITConfig(USART2, USART_IT_RXNE , ENABLE);	
	
	/* ʹ������ 					*/
  USART_Cmd(USART2, ENABLE);	
	
	return 0;
}


/**
  * @����
  * @����
  * @����ֵ
  */
bool USART2_putchar (uint8_t c) 
{
  struct buf_st *p = &USART2_tbuf;

  /* дһ���ֽڵ�����1 */
  if ((uint8_t)(p->in + 1) == p->out) {
    /* ���ڷ��ͻ������� */
    return (false);
  }
  NVIC_DisableIRQ(USART2_IRQn);			/*��ֹ����1�ж�*/
  __nop ();
  if (USART2_tx_active == false) {
    /* �������δ���ͣ���ֱ��д�봮�����ݼĴ��� */
    USART2->DR   = (uint8_t)c;
    USART2->CR1 |= 0x0080;			/*ʹ�ܷ����ж�*/
    USART2_tx_active = true;
  }
  else {
    /* ���һ���ֽڵ����ͻ��� */
    p->buf [p->in++] = c;
  }
  NVIC_EnableIRQ(USART2_IRQn);			/*ʹ�ܴ���1�ж�*/
  return (true);	
}

/**
  * @����
  * @����
  * @����ֵ
  */
int USART2_getchar (void) 
{
  /* �Ӵ��ڶ�ȡһ���ֽ� */
  struct buf_st *p = &USART2_rbuf;

  if (p->in == p->out) {
    /* ���ڻ���Ϊ�� */
    return (-1); /* EOF */
  }
  return (p->buf[p->out++]);
}

/**
  * @����
  * @����
  * @����ֵ
  */
bool USART2_Get_tx_active (void) 
{
  /* ��鴮�ڷ����Ƿ���Ч         */
  /* �����ͻ���Ϊ��ʱ��tx_active = false */
  return (USART2_tx_active);
}

/**
  * @����
  * @����
  * @����ֵ
  */
void USART2_IRQHandler(void)
{
  struct buf_st *p;

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){
		p = &USART2_rbuf;
    if ((uint8_t)(p->in + 1) != p->out) {
      p->buf [p->in++] = (uint8_t)USART2->DR;
    }		
	}
	
	if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET){
    p = &USART2_tbuf;
    if (p->in != p->out) {
      USART2->DR = p->buf [p->out++];
    } else {
			USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
      USART2_tx_active = false;
    }
	}
}


#endif

/**
  * @����
  * @����
  * @����ֵ
  */
#ifdef  _USART3_ENABLE

static int32_t USART3_HalInit(Seril_InitTypeDef* Serialx_InitStruct)
{
  GPIO_InitTypeDef 		GPIO_InitStructure;

  /* ʹ������ʱ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	/* ����GPIO���� */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;/* ���츴����� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	  
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; /* ���ջ��������� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* USART3 ��λ */
	USART_Cmd(USART3, DISABLE);
	USART_DeInit(USART3);
	
	/* ��ʼ�������ʵ� 		*/
	USART_Init(USART3, Serialx_InitStruct);
	
	/* �����ж� 					*/
// 	USART_ITConfig(USART3, USART_IT_TXE , ENABLE);
	USART_ITConfig(USART3, USART_IT_RXNE , ENABLE);	
	
	/* ʹ������ */
  USART_Cmd(USART3, ENABLE);	
		
	return 0;
}

/**
  * @����
  * @����
  * @����ֵ
  */
bool USART3_putchar (uint8_t c) 
{
  struct buf_st *p = &USART3_tbuf;

  /* дһ���ֽڵ�����1 */
  if ((uint8_t)(p->in + 1) == p->out) {
    /* ���ڷ��ͻ������� */
    return (false);
  }
  NVIC_DisableIRQ(USART3_IRQn);			/*��ֹ����1�ж�*/
  __nop ();
  if (USART3_tx_active == false) {
    /* �������δ���ͣ���ֱ��д�봮�����ݼĴ��� */
    USART3->DR   = (uint8_t)c;
    USART3->CR1 |= 0x0080;			/*ʹ�ܷ����ж�*/
    USART3_tx_active = true;
  }
  else {
    /* ���һ���ֽڵ����ͻ��� */
    p->buf [p->in++] = c;
  }
  NVIC_EnableIRQ(USART3_IRQn);			/*ʹ�ܴ���1�ж�*/
  return (true);	
}

/**
  * @����
  * @����
  * @����ֵ
  */
int USART3_getchar (void) {
  /* �Ӵ��ڶ�ȡһ���ֽ� */
  struct buf_st *p = &USART3_rbuf;

  if (p->in == p->out) {
    /* ���ڻ���Ϊ�� */
    return (-1); /* EOF */
  }
  return (p->buf[p->out++]);
}

/**
  * @����
  * @����
  * @����ֵ
  */
bool USART3_Get_tx_active (void) {
  /* ��鴮�ڷ����Ƿ���Ч         */
  /* �����ͻ���Ϊ��ʱ��tx_active = false */
  return (USART3_tx_active);
}

/**
  * @����
  * @����
  * @����ֵ
  */
void USART3_IRQHandler(void)
{
  struct buf_st *p;

	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET){
		p = &USART3_rbuf;
    if ((uint8_t)(p->in + 1) != p->out) {
      p->buf [p->in++] = (uint8_t)USART3->DR;
    }		
	}
	
	if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET){
    p = &USART3_tbuf;
    if (p->in != p->out) {
      USART3->DR = p->buf [p->out++];
    } else {
			USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
      USART3_tx_active = false;
    }
	}
}

#endif

/**
  * @����
  * @����
  * @����ֵ
  */
#ifdef  _UART4_ENABLE

static int32_t UART4_HalInit(Seril_InitTypeDef* Serialx_InitStruct)
{
  GPIO_InitTypeDef 		GPIO_InitStructure;

  /* ʹ������ʱ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

	/* ���� GPIO ���� */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;/* ���츴����� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	  
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; /* ���ջ��������� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* UART4 ��λ 		*/
	USART_Cmd(UART4, DISABLE);
	USART_DeInit(UART4);
	
	/* ��ʼ�������ʵ� */
	USART_Init(UART4, Serialx_InitStruct);
	
	/* �����ж� 			*/
// 	USART_ITConfig(UART4, USART_IT_TXE , ENABLE);
	USART_ITConfig(UART4, USART_IT_RXNE , ENABLE);	
	
	/* ʹ������ 			*/
  USART_Cmd(UART4, ENABLE);		
	
	return 0;
}

/**
  * @����
  * @����
  * @����ֵ
  */
bool UART4_putchar (uint8_t c) 
{
  struct buf_st *p = &UART4_tbuf;

  /* дһ���ֽڵ�����1 */
  if ((uint8_t)(p->in + 1) == p->out) {
    /* ���ڷ��ͻ������� */
    return (false);
  }
  NVIC_DisableIRQ(UART4_IRQn);			/*��ֹ����1�ж�*/
  __nop ();
  if (UART4_tx_active == false) {
    /* �������δ���ͣ���ֱ��д�봮�����ݼĴ��� */
    UART4->DR   = (uint8_t)c;
    UART4->CR1 |= 0x0080;			/*ʹ�ܷ����ж�*/
    UART4_tx_active = true;
  }
  else {
    /* ���һ���ֽڵ����ͻ��� */
    p->buf [p->in++] = c;
  }
  NVIC_EnableIRQ(UART4_IRQn);			/*ʹ�ܴ���1�ж�*/
  return (true);	
}

/**
  * @����
  * @����
  * @����ֵ
  */
int UART4_getchar (void) {
  /* �Ӵ��ڶ�ȡһ���ֽ� */
  struct buf_st *p = &UART4_rbuf;

  if (p->in == p->out) {
    /* ���ڻ���Ϊ�� */
    return (-1); /* EOF */
  }
  return (p->buf[p->out++]);
}

/**
  * @����
  * @����
  * @����ֵ
  */
bool UART4_Get_tx_active (void) {
  /* ��鴮�ڷ����Ƿ���Ч         */
  /* �����ͻ���Ϊ��ʱ��tx_active = false */
  return (UART4_tx_active);
}

/**
  * @����
  * @����
  * @����ֵ
  */
void UART4_IRQHandler(void)
{
  struct buf_st *p;

	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET){
		p = &UART4_rbuf;
    if ((uint8_t)(p->in + 1) != p->out) {
      p->buf [p->in++] = (uint8_t)UART4->DR;
    }		
	}
	
	if(USART_GetITStatus(UART4, USART_IT_TXE) != RESET){
    p = &UART4_tbuf;
    if (p->in != p->out) {
      UART4->DR = p->buf [p->out++];
    } else {
			USART_ITConfig(UART4, USART_IT_TXE, DISABLE);
      UART4_tx_active = false;
    }
	}
}

#endif

/**
  * @����
  * @����
  * @����ֵ
  */
#ifdef  _UART5_ENABLE

static int32_t UART5_HalInit(Seril_InitTypeDef* Serialx_InitStruct)
{
  GPIO_InitTypeDef 		GPIO_InitStructure;

  /* ʹ������ʱ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);

	/* ���� GPIO ���� */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;/* ���츴����� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	  
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; /* ���ջ��������� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* USART5 ��λ 	*/
	USART_Cmd(UART5, DISABLE);
	USART_DeInit(UART5);
	
	/* ��ʼ�������ʵ� 	*/
	USART_Init(UART5, Serialx_InitStruct);
	
	/* �����ж� 				*/
// 	USART_ITConfig(UART5, USART_IT_TXE , ENABLE);
	USART_ITConfig(UART5, USART_IT_RXNE , ENABLE);	
	
	/* ʹ������ 				*/
  USART_Cmd(UART5, ENABLE);		
	
	return 0;
}

/**
  * @����
  * @����
  * @����ֵ
  */
bool UART5_putchar (uint8_t c) 
{
  struct buf_st *p = &UART5_tbuf;

  /* дһ���ֽڵ�����1 */
  if ((uint8_t)(p->in + 1) == p->out) {
    /* ���ڷ��ͻ������� */
    return (false);
  }
  NVIC_DisableIRQ(UART5_IRQn);			/*��ֹ����1�ж�*/
  __nop ();
  if (UART5_tx_active == false) {
    /* �������δ���ͣ���ֱ��д�봮�����ݼĴ��� */
    UART5->DR   = (uint8_t)c;
    UART5->CR1 |= 0x0080;			/*ʹ�ܷ����ж�*/
    UART5_tx_active = true;
  }
  else {
    /* ���һ���ֽڵ����ͻ��� */
    p->buf [p->in++] = c;
  }
  NVIC_EnableIRQ(UART5_IRQn);			/*ʹ�ܴ���1�ж�*/
  return (true);	
}

/**
  * @����
  * @����
  * @����ֵ
  */
int UART5_getchar (void) {
  /* �Ӵ��ڶ�ȡһ���ֽ� */
  struct buf_st *p = &UART5_rbuf;

  if (p->in == p->out) {
    /* ���ڻ���Ϊ�� */
    return (-1); /* EOF */
  }
  return (p->buf[p->out++]);
}

/**
  * @����
  * @����
  * @����ֵ
  */
bool UART5_Get_tx_active (void) {
  /* ��鴮�ڷ����Ƿ���Ч         */
  /* �����ͻ���Ϊ��ʱ��tx_active = false */
  return (UART5_tx_active);
}

/**
  * @����
  * @����
  * @����ֵ
  */
void UART5_IRQHandler(void)
{
  struct buf_st *p;

	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET){
		p = &UART5_rbuf;
    if ((uint8_t)(p->in + 1) != p->out) {
      p->buf [p->in++] = (uint8_t)UART5->DR;
    }		
	}
	
	if(USART_GetITStatus(UART5, USART_IT_TXE) != RESET){
    p = &UART5_tbuf;
    if (p->in != p->out) {
      UART5->DR = p->buf [p->out++];
    } else {
			USART_ITConfig(UART5, USART_IT_TXE, DISABLE);
      UART5_tx_active = false;
    }
	}	
}

#endif

/**
  * @����
  * @����
  * @����ֵ
  */
int32_t USART_NVICConfig(Serial_TypeDef *Serialx, uint8_t PrePriority, uint8_t SubPriority, FunctionalState state)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
#ifdef  _USART1_ENABLE
	if(Serialx == USART1){
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	}
#endif
	
#ifdef  _USART2_ENABLE
	else if(Serialx == USART2){
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	}
#endif
	
#ifdef  _USART3_ENABLE
	else if(Serialx == USART3){
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	}
#endif	
	
#ifdef  _UART4_ENABLE
	else if(Serialx == UART4){
		NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	}
#endif
	
#ifdef  _UART5_ENABLE
	else if(Serialx == UART5){
		NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	}
#endif
	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PrePriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = state;  			/* ENABLE / DIASBLE*/
	NVIC_Init(&NVIC_InitStructure);		
	
	return 0;
}

/**
  * @����
  * @����
  * @����ֵ
  */
int32_t SerialInit(Serial_TypeDef *Serialx, Seril_InitTypeDef *Serialx_InitStruct)
{
#ifdef  _USART1_ENABLE
	if(Serialx == USART1){
		USART1_rbuf.in   = 0;
		USART1_rbuf.out  = 0;
		USART1_tbuf.in   = 0;
		USART1_tbuf.out  = 0;
		USART1_tx_active = false;		
		
		USART1_HalInit(Serialx_InitStruct);
	}
#endif	
	
#ifdef _USART2_ENABLE
	else if(Serialx == USART2){
		USART2_rbuf.in   = 0;
		USART2_rbuf.out  = 0;
		USART2_tbuf.in   = 0;
		USART2_tbuf.out  = 0;
		USART2_tx_active = false;		
		
		USART2_HalInit(Serialx_InitStruct);		
	}
#endif	
	
#ifdef _USART3_ENABLE
	else if(Serialx == USART3){
		USART3_rbuf.in   = 0;
		USART3_rbuf.out  = 0;
		USART3_tbuf.in   = 0;
		USART3_tbuf.out  = 0;
		USART3_tx_active = false;		
		
		USART3_HalInit(Serialx_InitStruct);		
	}
#endif	
	
#ifdef _UART4_ENABLE	
	else if(Serialx == UART4){
		UART4_rbuf.in   = 0;
		UART4_rbuf.out  = 0;
		UART4_tbuf.in   = 0;
		UART4_tbuf.out  = 0;
		UART4_tx_active = false;		
		
		UART4_HalInit(Serialx_InitStruct);	
	}
#endif	
	
#ifdef _UART5_ENABLE
	else if(Serialx == UART5){
		UART5_rbuf.in   = 0;
		UART5_rbuf.out  = 0;
		UART5_tbuf.in   = 0;
		UART5_tbuf.out  = 0;
		UART5_tx_active = false;		
		
		UART5_HalInit(Serialx_InitStruct);		
	}
#endif	
	else{
		return -1;
	}
	
	return 0;
}

/**
  * @}
  */

/**
 ******************************************************************************
 * �ļ�ĩβ
 ******************************************************************************
 */
