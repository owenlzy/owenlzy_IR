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
#include "xCLI.h"

/* 数据类型     --------------------------------------------------------------*/
/* 宏定义 	    --------------------------------------------------------------*/
#define CLS     "\x1B[2J"
#define TBLUE   "\033[37;44m"
#define TNORM   "\033[0m"

/* 变量 	    	--------------------------------------------------------------*/

static  const char *xClI_header[9] = {
  CLS"\r\n",
  "        " TBLUE,
  "*=============================================================*\r\n" TNORM,
  "        " TBLUE,
  "*                        xCLI  v0.00                          *\r\n", TNORM,
  "        " TBLUE,
  "*=============================================================*\r\n" TNORM,
	NULL
  };

xCLI_CmdTypedef CLI_CmdTable[] = {
	{xCLI_wellcom, 				"wellcom", 		"Wellcom Header"},
	{xCLI_picoc, 					"picoc", 			"picoc is a very small C interpreter for scripting."},
	{NULL, NULL, NULL}
};

/* 函数声明     --------------------------------------------------------------*/

/**
  * @功能
  * @参数
  * @返回值
  */
int32_t xCLI_wellcom(uint8_t *pdata, uint32_t len)
{
	uint32_t i;
	
	for(i = 0; i<9 ; i++){
		Printf("%s", xClI_header[i]);
	}
	
	return 0;
}

/**
  * @功能
  * @参数
  * @返回值
  */
int picoc(char *SourceStr);

extern char code[]="void main(){}";

// int picoc(int argc, char **argv);

int32_t xCLI_picoc(uint8_t *pdata, uint32_t len)
{

	xCLI_printf("\r\nHello  World . I come in.....\n >");
	picoc((char *)code);

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
