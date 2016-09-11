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

#ifndef _CLI_H_
#define _CLI_H_

/* ͷ�ļ�				--------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "BSP.h"

/* ��������     --------------------------------------------------------------*/
typedef struct _xCLI_buf{
	uint32_t index;			/*��ǰд��ָ��*/
	uint8_t data[80];
}xCLI_BufTypedef;

typedef struct _xCLI_Cmd{
	int32_t (*cmd)(uint8_t *pdata, uint32_t len);
	const char *name;
	const char *helpstr;
}xCLI_CmdTypedef;


/* �궨�� 	    --------------------------------------------------------------*/
#define xCLI_printf				Printf

#define xCLI_getchar() 		USART1_getchar()	
#define xCLI_putchar(c) 	USART1_putchar(c)	

/* ���� 	    	--------------------------------------------------------------*/
extern xCLI_CmdTypedef CLI_CmdTable[];

/* ��������     --------------------------------------------------------------*/

/* xCLI */
int32_t xCLI_GetLine(void);
int32_t xCLI_process_cmd(uint8_t *pdata, uint32_t len);
int32_t xCLI(void);

/* xCLI_cmd */
int32_t xCLI_wellcom(uint8_t *pdata, uint32_t len);
int32_t xCLI_picoc(uint8_t *pdata, uint32_t len);

/**
  * @����
  * @����
  * @����ֵ
  */

/**
  * @}
  */

#endif /* _CLI_H_ */

/**
 ******************************************************************************
 * �ļ�ĩβ
 ******************************************************************************
 */
