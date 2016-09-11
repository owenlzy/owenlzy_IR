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

#ifndef _CLI_H_
#define _CLI_H_

/* 头文件				--------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "BSP.h"

/* 数据类型     --------------------------------------------------------------*/
typedef struct _xCLI_buf{
	uint32_t index;			/*当前写入指针*/
	uint8_t data[80];
}xCLI_BufTypedef;

typedef struct _xCLI_Cmd{
	int32_t (*cmd)(uint8_t *pdata, uint32_t len);
	const char *name;
	const char *helpstr;
}xCLI_CmdTypedef;


/* 宏定义 	    --------------------------------------------------------------*/
#define xCLI_printf				Printf

#define xCLI_getchar() 		USART1_getchar()	
#define xCLI_putchar(c) 	USART1_putchar(c)	

/* 变量 	    	--------------------------------------------------------------*/
extern xCLI_CmdTypedef CLI_CmdTable[];

/* 函数声明     --------------------------------------------------------------*/

/* xCLI */
int32_t xCLI_GetLine(void);
int32_t xCLI_process_cmd(uint8_t *pdata, uint32_t len);
int32_t xCLI(void);

/* xCLI_cmd */
int32_t xCLI_wellcom(uint8_t *pdata, uint32_t len);
int32_t xCLI_picoc(uint8_t *pdata, uint32_t len);

/**
  * @功能
  * @参数
  * @返回值
  */

/**
  * @}
  */

#endif /* _CLI_H_ */

/**
 ******************************************************************************
 * 文件末尾
 ******************************************************************************
 */
