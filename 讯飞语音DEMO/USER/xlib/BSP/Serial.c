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
#include "Serial.h"

/* 数据类型     --------------------------------------------------------------*/
struct buf_st  {
  uint8_t in;
  uint8_t out;
  uint8_t buf [256];
};

/* 宏定义 			--------------------------------------------------------------*/
/* 变量 				--------------------------------------------------------------*/

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

/* 函数声明     --------------------------------------------------------------*/
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
  * @功能
  * @参数
  * @返回值
 
PUTCHAR_PROTOTYPE
{
	while(USART1_putchar(ch) == false)  __nop (); 	
  return ch;
}
*/ 

/**
  * @功能
  * @参数
  * @返回值
  */
GETCHAR_PROTOTYPE
{
  /* 从串口读取一个字节 */
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
  * @功能
  * @参数
  * @返回值
  */
#ifdef  _USART1_ENABLE

static int32_t USART1_HalInit(Seril_InitTypeDef* Serialx_InitStruct)
{
   GPIO_InitTypeDef 		GPIO_InitStructure;

  /* 使能外设时钟 			*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

	/* 配置 GPIO 引脚功能 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;/* 推挽复用输出 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	  
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; /* 浮空或上拉输人 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* USART1 复位	*/
	USART_Cmd(USART1, DISABLE);
	USART_DeInit(USART1);
	
	/* 配置波特率等 		*/
	USART_Init(USART1, Serialx_InitStruct);
	
	/* 配置中断 				*/	
// 	USART_ITConfig(USART1, USART_IT_TXE , ENABLE); /* 发送缓冲空中断 */
	USART_ITConfig(USART1, USART_IT_RXNE , ENABLE);
	
	/* 使能外设 				*/
  USART_Cmd(USART1, ENABLE);
	
	return 0;
}

/**
  * @功能
  * @参数
  * @返回值
  */
bool USART1_putchar (uint8_t c) 
{
  struct buf_st *p = &USART1_tbuf;

  /* 写一个字节到串口1 */
  if ((uint8_t)(p->in + 1) == p->out) {
    /* 串口发送缓存已满 */
    return (false);
  }
	
  NVIC_DisableIRQ(USART1_IRQn);			/*禁止串口1中断*/
  __nop ();
  if (USART1_tx_active == false) {
    /* 如果串口未发送，则直接写入串口数据寄存器 */
    USART1->DR   = (uint8_t)c;
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
    USART1_tx_active = true;
  }
  else {
    /* 添加一个字节到发送缓存 */
    p->buf [p->in++] = c;
  }
  NVIC_EnableIRQ(USART1_IRQn);			/*使能串口1中断*/
	
  return (true);	
}

/**
  * @功能
  * @参数
  * @返回值
  */
int USART1_getchar (void) {
  /* 从串口读取一个字节 */
  struct buf_st *p = &USART1_rbuf;

  if (p->in == p->out) {
    /* 串口缓存为空 */
    return (-1); /* EOF */
  }
		
  return (p->buf[p->out++]);
}

/**
  * @功能
  * @参数
  * @返回值
  */
bool USART1_Get_tx_active (void) {
  /* 检查串口发送是否有效         */
  /* 当发送缓存为空时，tx_active = false */
  return (USART1_tx_active);
}

/**
  * @功能			串口发送接收中断接口 
  * @参数
  * @返回值
  
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
  * @功能
  * @参数
  * @返回值
  */
#ifdef  _USART2_ENABLE

static int32_t USART2_HalInit(Seril_InitTypeDef* Serialx_InitStruct)
{
  GPIO_InitTypeDef 		GPIO_InitStructure;

  /* 使能外设时钟 			*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* 配置 GPIO 引脚功能 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;/* 推挽复用输出 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	  
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; /* 浮空或上拉输人 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* USART2 复位 		*/
	USART_Cmd(USART2, DISABLE);
	USART_DeInit(USART2);
	
	/* 初始化波特率等 		*/
	USART_Init(USART2, Serialx_InitStruct);
	
	/* 配置中断 					*/
// 	USART_ITConfig(USART2, USART_IT_TXE , ENABLE);
	USART_ITConfig(USART2, USART_IT_RXNE , ENABLE);	
	
	/* 使能外设 					*/
  USART_Cmd(USART2, ENABLE);	
	
	return 0;
}


/**
  * @功能
  * @参数
  * @返回值
  */
bool USART2_putchar (uint8_t c) 
{
  struct buf_st *p = &USART2_tbuf;

  /* 写一个字节到串口1 */
  if ((uint8_t)(p->in + 1) == p->out) {
    /* 串口发送缓存已满 */
    return (false);
  }
  NVIC_DisableIRQ(USART2_IRQn);			/*禁止串口1中断*/
  __nop ();
  if (USART2_tx_active == false) {
    /* 如果串口未发送，则直接写入串口数据寄存器 */
    USART2->DR   = (uint8_t)c;
    USART2->CR1 |= 0x0080;			/*使能发送中断*/
    USART2_tx_active = true;
  }
  else {
    /* 添加一个字节到发送缓存 */
    p->buf [p->in++] = c;
  }
  NVIC_EnableIRQ(USART2_IRQn);			/*使能串口1中断*/
  return (true);	
}

/**
  * @功能
  * @参数
  * @返回值
  */
int USART2_getchar (void) 
{
  /* 从串口读取一个字节 */
  struct buf_st *p = &USART2_rbuf;

  if (p->in == p->out) {
    /* 串口缓存为空 */
    return (-1); /* EOF */
  }
  return (p->buf[p->out++]);
}

/**
  * @功能
  * @参数
  * @返回值
  */
bool USART2_Get_tx_active (void) 
{
  /* 检查串口发送是否有效         */
  /* 当发送缓存为空时，tx_active = false */
  return (USART2_tx_active);
}

/**
  * @功能
  * @参数
  * @返回值
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
  * @功能
  * @参数
  * @返回值
  */
#ifdef  _USART3_ENABLE

static int32_t USART3_HalInit(Seril_InitTypeDef* Serialx_InitStruct)
{
  GPIO_InitTypeDef 		GPIO_InitStructure;

  /* 使能外设时钟 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	/* 配置GPIO引脚 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;/* 推挽复用输出 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	  
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; /* 浮空或上拉输人 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* USART3 复位 */
	USART_Cmd(USART3, DISABLE);
	USART_DeInit(USART3);
	
	/* 初始化波特率等 		*/
	USART_Init(USART3, Serialx_InitStruct);
	
	/* 配置中断 					*/
// 	USART_ITConfig(USART3, USART_IT_TXE , ENABLE);
	USART_ITConfig(USART3, USART_IT_RXNE , ENABLE);	
	
	/* 使能外设 */
  USART_Cmd(USART3, ENABLE);	
		
	return 0;
}

/**
  * @功能
  * @参数
  * @返回值
  */
bool USART3_putchar (uint8_t c) 
{
  struct buf_st *p = &USART3_tbuf;

  /* 写一个字节到串口1 */
  if ((uint8_t)(p->in + 1) == p->out) {
    /* 串口发送缓存已满 */
    return (false);
  }
  NVIC_DisableIRQ(USART3_IRQn);			/*禁止串口1中断*/
  __nop ();
  if (USART3_tx_active == false) {
    /* 如果串口未发送，则直接写入串口数据寄存器 */
    USART3->DR   = (uint8_t)c;
    USART3->CR1 |= 0x0080;			/*使能发送中断*/
    USART3_tx_active = true;
  }
  else {
    /* 添加一个字节到发送缓存 */
    p->buf [p->in++] = c;
  }
  NVIC_EnableIRQ(USART3_IRQn);			/*使能串口1中断*/
  return (true);	
}

/**
  * @功能
  * @参数
  * @返回值
  */
int USART3_getchar (void) {
  /* 从串口读取一个字节 */
  struct buf_st *p = &USART3_rbuf;

  if (p->in == p->out) {
    /* 串口缓存为空 */
    return (-1); /* EOF */
  }
  return (p->buf[p->out++]);
}

/**
  * @功能
  * @参数
  * @返回值
  */
bool USART3_Get_tx_active (void) {
  /* 检查串口发送是否有效         */
  /* 当发送缓存为空时，tx_active = false */
  return (USART3_tx_active);
}

/**
  * @功能
  * @参数
  * @返回值
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
  * @功能
  * @参数
  * @返回值
  */
#ifdef  _UART4_ENABLE

static int32_t UART4_HalInit(Seril_InitTypeDef* Serialx_InitStruct)
{
  GPIO_InitTypeDef 		GPIO_InitStructure;

  /* 使能外设时钟 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

	/* 配置 GPIO 引脚 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;/* 推挽复用输出 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	  
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; /* 浮空或上拉输人 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* UART4 复位 		*/
	USART_Cmd(UART4, DISABLE);
	USART_DeInit(UART4);
	
	/* 初始化波特率等 */
	USART_Init(UART4, Serialx_InitStruct);
	
	/* 配置中断 			*/
// 	USART_ITConfig(UART4, USART_IT_TXE , ENABLE);
	USART_ITConfig(UART4, USART_IT_RXNE , ENABLE);	
	
	/* 使能外设 			*/
  USART_Cmd(UART4, ENABLE);		
	
	return 0;
}

/**
  * @功能
  * @参数
  * @返回值
  */
bool UART4_putchar (uint8_t c) 
{
  struct buf_st *p = &UART4_tbuf;

  /* 写一个字节到串口1 */
  if ((uint8_t)(p->in + 1) == p->out) {
    /* 串口发送缓存已满 */
    return (false);
  }
  NVIC_DisableIRQ(UART4_IRQn);			/*禁止串口1中断*/
  __nop ();
  if (UART4_tx_active == false) {
    /* 如果串口未发送，则直接写入串口数据寄存器 */
    UART4->DR   = (uint8_t)c;
    UART4->CR1 |= 0x0080;			/*使能发送中断*/
    UART4_tx_active = true;
  }
  else {
    /* 添加一个字节到发送缓存 */
    p->buf [p->in++] = c;
  }
  NVIC_EnableIRQ(UART4_IRQn);			/*使能串口1中断*/
  return (true);	
}

/**
  * @功能
  * @参数
  * @返回值
  */
int UART4_getchar (void) {
  /* 从串口读取一个字节 */
  struct buf_st *p = &UART4_rbuf;

  if (p->in == p->out) {
    /* 串口缓存为空 */
    return (-1); /* EOF */
  }
  return (p->buf[p->out++]);
}

/**
  * @功能
  * @参数
  * @返回值
  */
bool UART4_Get_tx_active (void) {
  /* 检查串口发送是否有效         */
  /* 当发送缓存为空时，tx_active = false */
  return (UART4_tx_active);
}

/**
  * @功能
  * @参数
  * @返回值
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
  * @功能
  * @参数
  * @返回值
  */
#ifdef  _UART5_ENABLE

static int32_t UART5_HalInit(Seril_InitTypeDef* Serialx_InitStruct)
{
  GPIO_InitTypeDef 		GPIO_InitStructure;

  /* 使能外设时钟 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);

	/* 配置 GPIO 引脚 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;/* 推挽复用输出 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	  
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; /* 浮空或上拉输人 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* USART5 复位 	*/
	USART_Cmd(UART5, DISABLE);
	USART_DeInit(UART5);
	
	/* 初始化波特率等 	*/
	USART_Init(UART5, Serialx_InitStruct);
	
	/* 配置中断 				*/
// 	USART_ITConfig(UART5, USART_IT_TXE , ENABLE);
	USART_ITConfig(UART5, USART_IT_RXNE , ENABLE);	
	
	/* 使能外设 				*/
  USART_Cmd(UART5, ENABLE);		
	
	return 0;
}

/**
  * @功能
  * @参数
  * @返回值
  */
bool UART5_putchar (uint8_t c) 
{
  struct buf_st *p = &UART5_tbuf;

  /* 写一个字节到串口1 */
  if ((uint8_t)(p->in + 1) == p->out) {
    /* 串口发送缓存已满 */
    return (false);
  }
  NVIC_DisableIRQ(UART5_IRQn);			/*禁止串口1中断*/
  __nop ();
  if (UART5_tx_active == false) {
    /* 如果串口未发送，则直接写入串口数据寄存器 */
    UART5->DR   = (uint8_t)c;
    UART5->CR1 |= 0x0080;			/*使能发送中断*/
    UART5_tx_active = true;
  }
  else {
    /* 添加一个字节到发送缓存 */
    p->buf [p->in++] = c;
  }
  NVIC_EnableIRQ(UART5_IRQn);			/*使能串口1中断*/
  return (true);	
}

/**
  * @功能
  * @参数
  * @返回值
  */
int UART5_getchar (void) {
  /* 从串口读取一个字节 */
  struct buf_st *p = &UART5_rbuf;

  if (p->in == p->out) {
    /* 串口缓存为空 */
    return (-1); /* EOF */
  }
  return (p->buf[p->out++]);
}

/**
  * @功能
  * @参数
  * @返回值
  */
bool UART5_Get_tx_active (void) {
  /* 检查串口发送是否有效         */
  /* 当发送缓存为空时，tx_active = false */
  return (UART5_tx_active);
}

/**
  * @功能
  * @参数
  * @返回值
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
  * @功能
  * @参数
  * @返回值
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
  * @功能
  * @参数
  * @返回值
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
 * 文件末尾
 ******************************************************************************
 */
